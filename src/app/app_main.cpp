/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "crashhandlersetup.h"

#include "app_version.h"

#include <extensionsystem/iplugin.h>
#include <extensionsystem/pluginerroroverview.h>
#include <extensionsystem/pluginmanager.h>
#include <extensionsystem/pluginspec.h>
#include <qtsingleapplication.h>
#include <utils/hostosinfo.h>
#include <utils/logging.h>

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QLibraryInfo>
#include <QSettings>
#include <QTextStream>
#include <QThreadPool>
#include <QTimer>
#include <QTranslator>
#include <QUrl>
#include <QVariant>

#include <QNetworkProxyFactory>

#include <QApplication>
#include <QStandardPaths>
#include <QMessageBox>

using namespace ExtensionSystem;

enum { OptionIndent = 4, DescriptionIndent = 34 };

const char corePluginNameC[] = "Core";

const char fixedOptionsC[] =
" [OPTION]... [FILE]...\n"
"Options:\n"
"    -help                         Display this help\n"
"    -version                      Display program version\n"
"    -client                       Attempt to connect to already running first instance\n"
"    -settingspath <path>          Override the default path where user settings are stored\n"
"    -pid <pid>                    Attempt to connect to instance given by pid\n"
"    -block                        Block until editor is closed\n"
"    -pluginpath <path>            Add a custom search path for plugins\n";

const char HELP_OPTION1[] = "-h";
const char HELP_OPTION2[] = "-help";
const char HELP_OPTION3[] = "/h";
const char HELP_OPTION4[] = "--help";
const char VERSION_OPTION[] = "-version";
const char CLIENT_OPTION[] = "-client";
const char SETTINGS_OPTION[] = "-settingspath";
const char TEST_OPTION[] = "-test";
const char PID_OPTION[] = "-pid";
const char BLOCK_OPTION[] = "-block";
const char PLUGINPATH_OPTION[] = "-pluginpath";

typedef QList<PluginSpec *> PluginSpecSet;

// Helpers for displaying messages. Note that there is no console on Windows.
#ifdef Q_OS_WIN
// Format as <pre> HTML
static inline void toHtml(QString &t)
{
    t.replace(QLatin1Char('&'), QLatin1String("&amp;"));
    t.replace(QLatin1Char('<'), QLatin1String("&lt;"));
    t.replace(QLatin1Char('>'), QLatin1String("&gt;"));
    t.insert(0, QLatin1String("<html><pre>"));
    t.append(QLatin1String("</pre></html>"));
}

static void displayHelpText(QString t) // No console on Windows.
{
    toHtml(t);
    QMessageBox::information(0, QCoreApplication::applicationName(), t);
}

static void displayError(const QString &t) // No console on Windows.
{
    QMessageBox::critical(0, QCoreApplication::applicationName(), t);
}

#else

static void displayHelpText(const QString &t)
{
    qWarning("%s", qPrintable(t));
}

static void displayError(const QString &t)
{
    qCritical("%s", qPrintable(t));
}

#endif

static void printVersion(const PluginSpec *coreplugin)
{
    QString version;
    QTextStream str(&version);
    str << '\n' << QCoreApplication::applicationName() << ' ' << coreplugin->version()<< " based on Qt " << qVersion() << "\n\n";
    PluginManager::formatPluginVersions(str);
    displayHelpText(version);
}

static void printHelp(const QString &a0)
{
    QString help;
    QTextStream str(&help);
    str << "Usage: " << a0 << fixedOptionsC;
    PluginManager::formatOptions(str, OptionIndent, DescriptionIndent);
    PluginManager::formatPluginOptions(str, OptionIndent, DescriptionIndent);
    displayHelpText(help);
}

static inline QString msgCoreLoadFailure(const QString &why)
{
    return QCoreApplication::translate("Application", "Failed to load core: %1").arg(why);
}

static inline int askMsgSendFailed()
{
    return QMessageBox::question(0, QApplication::translate("Application","Could not send message"),
                                 QCoreApplication::translate("Application", "Unable to send command line arguments to the already running instance. "
                                                             "It appears to be not responding. Do you want to start a new instance of Creator?"),
                                 QMessageBox::Yes | QMessageBox::No | QMessageBox::Retry,
                                 QMessageBox::Retry);
}

// taken from utils/fileutils.cpp. We can not use utils here since that depends app_version.h.
static bool copyRecursively(const QString &srcFilePath,
                            const QString &tgtFilePath)
{
    QFileInfo srcFileInfo(srcFilePath);
    if (srcFileInfo.isDir()) {
        QDir targetDir(tgtFilePath);
        targetDir.cdUp();
        if (!targetDir.mkdir(QFileInfo(tgtFilePath).fileName()))
            return false;
        QDir sourceDir(srcFilePath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        foreach (const QString &fileName, fileNames) {
            const QString newSrcFilePath
                    = srcFilePath + QLatin1Char('/') + fileName;
            const QString newTgtFilePath
                    = tgtFilePath + QLatin1Char('/') + fileName;
            if (!copyRecursively(newSrcFilePath, newTgtFilePath))
                return false;
        }
    } else {
        if (!QFile::copy(srcFilePath, tgtFilePath))
            return false;
    }
    return true;
}

static inline QStringList getPluginPaths()
{
    QStringList rc;
    // Figure out root:  Up one from 'bin'
    QDir rootDir = QApplication::applicationDirPath();
    rootDir.cdUp();
    const QString rootDirPath = rootDir.canonicalPath();
    //qDebug() << "rootDirPath:" << rootDirPath;

    QString pluginPath = rootDirPath;
#if !defined(Q_OS_MAC)
    pluginPath += QLatin1String("/plugins");
#else
    pluginPath += QLatin1String("/PlugIns");
#endif
    rc.push_back(pluginPath);

    //   <localappdata>/APP_NAME/APP_VERSION/plugins/
    //    where <localappdata> is e.g.
    //    "%LOCALAPPDATA%\APP_NAME" on Windows Vista and later
    //    "$XDG_DATA_HOME/data/APP_NAME/" or "~/.local/share/data/APP_NAME" on Linux
    //    "~/Library/Application Support/APP_NAME" on Mac
    QStringList paths = QStandardPaths::standardLocations(QStandardPaths::DataLocation);

    foreach(QString path, paths) {
        if (!path.endsWith('/'))
            path += QLatin1Char('/');
        path += QLatin1String(Application::Constants::APP_COMPACT_VERSION_STR);
        path += QLatin1String("/plugins/");
        rc.push_back(path);
    }
    return rc;
}

static QString getResourcePaths()
{
    QDir rootDir = QApplication::applicationDirPath();
    rootDir.cdUp();
    const QString rootDirPath = rootDir.canonicalPath();

    QString resourcePath = rootDirPath;
#if !defined(Q_OS_MAC)
    resourcePath += QLatin1String("/share");
#else
    resourcePath += QLatin1String("/Resources");
#endif

    return resourcePath;
}

int main(int argc, char **argv)
{
    QLoggingCategory::setFilterRules(QLatin1String("qtc.*.debug=false"));

#ifdef Q_OS_MAC
    // increase the number of file that can be opened in QForex.
    struct rlimit rl;
    getrlimit(RLIMIT_NOFILE, &rl);

    rl.rlim_cur = qMin((rlim_t)OPEN_MAX, rl.rlim_max);
    setrlimit(RLIMIT_NOFILE, &rl);
#endif
    // Set Application Info
    SharedTools::QtSingleApplication app(QCoreApplication::applicationName(), argc, argv);

    const int threadCount = QThreadPool::globalInstance()->maxThreadCount();
    QThreadPool::globalInstance()->setMaxThreadCount(qMax(8, 2 * threadCount));

    setupCrashHandler(); // Display a backtrace once a serious signal is delivered.

    app.setAttribute(Qt::AA_UseHighDpiPixmaps);

    QCoreApplication::setApplicationName(QLatin1String(Application::Constants::APP_NAME_STR));
    QCoreApplication::setApplicationVersion(QLatin1String(Application::Constants::APP_VERSION_STR));
    QCoreApplication::setOrganizationName(QLatin1String(Application::Constants::APP_ORGNAME_STR));
    QCoreApplication::setOrganizationDomain(QLatin1String(Application::Constants::APP_DOMAIN_STR));
    qApp->setProperty("APPLICATION_AUTHOR", QLatin1String(Application::Constants::APP_AUTHOR_STR));
    qApp->setProperty("APPLICATION_COMPACT_VERSION", QLatin1String(Application::Constants::APP_COMPACT_VERSION_STR));
    qApp->setProperty("APPLICATION_VERSION_MAJOR", QLatin1String(Application::Constants::APP_VERSION_MAJOR));
    qApp->setProperty("APPLICATION_VERSION_MINOR", QLatin1String(Application::Constants::APP_VERSION_MINOR));
    qApp->setProperty("APPLICATION_VERSION_MICRO", QLatin1String(Application::Constants::APP_VERSION_MICRO));
    qApp->setProperty("APPLICATION_VERSION_PATCH", QLatin1String(Application::Constants::APP_VERSION_PATCH));


    // Manually determine -settingspath command line option
    // We can't use the regular way of the plugin manager, because that needs to parse pluginspecs
    // but the settings path can influence which plugins are enabled
    QString settingsPath;
    QStringList customPluginPaths;
    QStringList arguments = app.arguments(); // adapted arguments list is passed to plugin manager later
    QMutableStringListIterator it(arguments);
    bool testOptionProvided = false;
    while (it.hasNext()) {
        const QString &arg = it.next();
        if (arg == QLatin1String(SETTINGS_OPTION)) {
            it.remove();
            if (it.hasNext()) {
                settingsPath = QDir::fromNativeSeparators(it.next());
                it.remove();
            }
        } else if (arg == QLatin1String(PLUGINPATH_OPTION)) {
            it.remove();
            if (it.hasNext()) {
                customPluginPaths << QDir::fromNativeSeparators(it.next());
                it.remove();
            }
        } else if (arg == QLatin1String(TEST_OPTION)) {
            testOptionProvided = true;
        }
    }
    if (settingsPath.isEmpty() && testOptionProvided) {
        settingsPath = QDir::tempPath() + QString::fromLatin1("/qtc-%1-test-settings")
                .arg(QLatin1String(Application::Constants::APP_VERSION_STR));
        settingsPath = QDir::cleanPath(settingsPath);
    }
    if (!settingsPath.isEmpty())
        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, settingsPath);

    qDebug() << "settingsPath: " << settingsPath;

    // Must be done before any QSettings class is created
    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, getResourcePaths());
    QSettings::setDefaultFormat(QSettings::IniFormat);
    // plugin manager takes control of this settings object
    // format: ~/.config/APP_ORG_NAME/APP_NAME.ini
    QSettings *settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                    QCoreApplication::organizationName(),
                                    QCoreApplication::applicationName());

    QSettings *globalSettings = new QSettings(QSettings::IniFormat, QSettings::SystemScope,
                                              QCoreApplication::organizationName(),
                                              QCoreApplication::applicationName());

    PluginManager pluginManager;
    PluginManager::setFileExtension(QLatin1String("pluginspec"));
    PluginManager::setGlobalSettings(globalSettings);
    PluginManager::setSettings(settings);

    QTranslator translator;
    QTranslator qtTranslator;
    QStringList uiLanguages;

    uiLanguages = QLocale::system().uiLanguages();

    QString overrideLanguage = settings->value(QLatin1String("General/OverrideLanguage")).toString();
    if (!overrideLanguage.isEmpty())
        uiLanguages.prepend(overrideLanguage);
    const QString &appTrPath = getResourcePaths() + QLatin1String("/translations");
    qDebug() << "TrPath:" << appTrPath;
    foreach (QString locale, uiLanguages) {

        locale = QLocale(locale).name();

        if (translator.load(QLatin1String(Application::Constants::APP_NAME_STR) + QLatin1Char('_') + locale, appTrPath)) {
            const QString &qtTrPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
            const QString &qtTrFile = QLatin1String("qt_") + locale;
            // Binary installer puts Qt tr files into appTrPath
            if (qtTranslator.load(qtTrFile, qtTrPath) || qtTranslator.load(qtTrFile, appTrPath)) {
                app.installTranslator(&translator);
                app.installTranslator(&qtTranslator);
                app.setProperty("qtc_locale", locale);
                break;
            }
            translator.load(QString()); // unload()
        } else if (locale == QLatin1String("C") /* overrideLanguage == "English" */) {
            // use built-in
            break;
        } else if (locale.startsWith(QLatin1String("en")) /* "English" is built-in */) {
            // use built-in
            break;
        }
    }

    // Make sure we honor the system's proxy settings
#if defined(Q_OS_UNIX)
    QUrl proxyUrl(QString::fromLatin1(qgetenv("http_proxy")));
    if (proxyUrl.isValid()) {
        QNetworkProxy proxy(QNetworkProxy::HttpProxy, proxyUrl.host(),
                            proxyUrl.port(), proxyUrl.userName(), proxyUrl.password());
        QNetworkProxy::setApplicationProxy(proxy);
# if defined(Q_OS_MAC) // unix and mac
    } else {
        QNetworkProxyFactory::setUseSystemConfiguration(true);
# endif
    }
#else // windows
    QNetworkProxyFactory::setUseSystemConfiguration(true);
#endif

    // Load
    const QStringList pluginPaths = getPluginPaths() + customPluginPaths;
    PluginManager::setPluginPaths(pluginPaths);
    QMap<QString, QString> foundAppOptions;
    if (arguments.size() > 1) {
        QMap<QString, bool> appOptions;
        appOptions.insert(QLatin1String(HELP_OPTION1), false);
        appOptions.insert(QLatin1String(HELP_OPTION2), false);
        appOptions.insert(QLatin1String(HELP_OPTION3), false);
        appOptions.insert(QLatin1String(HELP_OPTION4), false);
        appOptions.insert(QLatin1String(VERSION_OPTION), false);
        appOptions.insert(QLatin1String(CLIENT_OPTION), false);
        appOptions.insert(QLatin1String(PID_OPTION), true);
        appOptions.insert(QLatin1String(BLOCK_OPTION), false);
        QString errorMessage;
        if (!PluginManager::parseOptions(arguments, appOptions, &foundAppOptions, &errorMessage)) {
            displayError(errorMessage);
            printHelp(QFileInfo(app.applicationFilePath()).baseName());
            return -1;
        }
    }

    const PluginSpecSet plugins = PluginManager::plugins();
    PluginSpec *coreplugin = 0;
    foreach (PluginSpec *spec, plugins) {
        if (spec->name() == QLatin1String(corePluginNameC)) {
            coreplugin = spec;
            break;
        }
    }
    if (!coreplugin) {
        QString nativePaths = QDir::toNativeSeparators(pluginPaths.join(QLatin1String(",")));
        const QString reason = QCoreApplication::translate("Application", "Could not find 'Core.pluginspec' in %1").arg(nativePaths);
        displayError(msgCoreLoadFailure(reason));
        return 1;
    }
    if (coreplugin->hasError()) {
        displayError(msgCoreLoadFailure(coreplugin->errorString()));
        return 1;
    }
    if (foundAppOptions.contains(QLatin1String(VERSION_OPTION))) {
        printVersion(coreplugin);
        return 0;
    }
    if (foundAppOptions.contains(QLatin1String(HELP_OPTION1))
            || foundAppOptions.contains(QLatin1String(HELP_OPTION2))
            || foundAppOptions.contains(QLatin1String(HELP_OPTION3))
            || foundAppOptions.contains(QLatin1String(HELP_OPTION4))) {
        printHelp(QFileInfo(app.applicationFilePath()).baseName());
        return 0;
    }

    qint64 pid = -1;
    if (foundAppOptions.contains(QLatin1String(PID_OPTION))) {
        QString pidString = foundAppOptions.value(QLatin1String(PID_OPTION));
        bool pidOk;
        qint64 tmpPid = pidString.toInt(&pidOk);
        if (pidOk)
            pid = tmpPid;
    }

    bool isBlock = foundAppOptions.contains(QLatin1String(BLOCK_OPTION));
    if (app.isRunning() && (pid != -1 || isBlock
                            || foundAppOptions.contains(QLatin1String(CLIENT_OPTION)))) {
        app.setBlock(isBlock);
        if (app.sendMessage(PluginManager::serializedArguments(), 5000 /*timeout*/, pid))
            return 0;

        // Message could not be send, maybe it was in the process of quitting
        if (app.isRunning(pid)) {
            // Nah app is still running, ask the user
            int button = askMsgSendFailed();
            while (button == QMessageBox::Retry) {
                if (app.sendMessage(PluginManager::serializedArguments(), 5000 /*timeout*/, pid))
                    return 0;
                if (!app.isRunning(pid)) // App quit while we were trying so start a new creator
                    button = QMessageBox::Yes;
                else
                    button = askMsgSendFailed();
            }
            if (button == QMessageBox::No)
                return -1;
        }
    }

    PluginManager::loadPlugins();
    if (coreplugin->hasError()) {
        displayError(msgCoreLoadFailure(coreplugin->errorString()));
        return 1;
    }
    if (PluginManager::hasError()) {
        PluginErrorOverview *errorOverview = new PluginErrorOverview(QApplication::activeWindow());
        errorOverview->setAttribute(Qt::WA_DeleteOnClose);
        errorOverview->setModal(true);
        errorOverview->show();
    }

    // Set up remote arguments.
    QObject::connect(&app, SIGNAL(messageReceived(QString,QObject*)),
                     &pluginManager, SLOT(remoteArguments(QString,QObject*)));

    QObject::connect(&app, SIGNAL(fileOpenRequest(QString)), coreplugin->plugin(),
                     SLOT(fileOpenRequest(QString)));

    // quit when last window (relevant window, see WA_QuitOnClose) is closed
    // this should actually be the default, but doesn't work in Qt 5
    // QTBUG-31569
    QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    // shutdown plugin manager on the exit
    QObject::connect(&app, SIGNAL(aboutToQuit()), &pluginManager, SLOT(shutdown()));

    const int r = app.exec();
    cleanupCrashHandler();
    return r;
}
