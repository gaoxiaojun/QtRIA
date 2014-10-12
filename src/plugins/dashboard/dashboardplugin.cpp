#include "dashboardplugin.h"
#include "dashboardconstants.h"

#include <extensionsystem/pluginmanager.h>

#include <coreplugin/coreconstants.h>
#include <coreplugin/icore.h>
#include <coreplugin/imode.h>
#include <coreplugin/modemanager.h>
#include <coreplugin/iwizardfactory.h>

#include <utils/algorithm.h>
#include <utils/fileutils.h>
#include <utils/hostosinfo.h>
#include <utils/styledbar.h>
#include <utils/iwelcomepage.h>

#include <QVBoxLayout>
#include <QMessageBox>

#include <QDir>

#ifdef USE_QUICK_WIDGET
    #include <QtQuickWidgets/QQuickWidget>
    typedef QQuickWidget QuickContainer;
#else
    #include <QtQuick/QQuickView>
    typedef QQuickView QuickContainer;
#endif
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>

enum { debug = 0 };

using namespace ExtensionSystem;
using namespace Utils;

static const char currentPageSettingsKeyC[] = "DashBoardTab";

namespace DashBoard {
namespace Internal {

class DashBoardMode : public Core::IMode
{
    Q_OBJECT
    Q_PROPERTY(int activePlugin READ activePlugin WRITE setActivePlugin NOTIFY activePluginChanged)
public:
    DashBoardMode();
    ~DashBoardMode();

    void activated();
    void initPlugins();
    int activePlugin() const { return m_activePlugin; }

    Q_SCRIPTABLE QString platform() const;

//    bool eventFilter(QObject *, QEvent *);

public slots:
    void setActivePlugin(int pos)
    {
        if (m_activePlugin != pos) {
            m_activePlugin = pos;
            emit activePluginChanged(pos);
        }
    }

signals:
    void activePluginChanged(int pos);

private slots:
    void dashboardPluginAdded(QObject*);
#if QT_VERSION >= 0x050300
    void sceneGraphError(QQuickWindow::SceneGraphError, const QString &message);
#endif

private:
    void facilitateQml(QQmlEngine *engine);

    QWidget *m_modeWidget;
    QuickContainer *m_dashboardPage;
    QList<QObject*> m_pluginList;
    int m_activePlugin;
};

// ---  DashBoardMode
DashBoardMode::DashBoardMode() :
    m_activePlugin(0)
{
    setPriority(DashBoard::Constants::P_MODE_DASHBOARD);
    setDisplayName(tr("DashBoard"));
    QIcon dashboardLogo;
    dashboardLogo.addFile(QLatin1String(":/dashboard/images/mode_dashboard.png"));
    dashboardLogo.addFile(QLatin1String(":/dashboard/images/mode_dashboard_2x.png"));
    dashboardLogo.addFile(QLatin1String(":/dashboard/images/mode_dashboard_4x.png"));
    setIcon(dashboardLogo);
    setPriority(Core::Constants::P_MODE_WELCOME);
    setId(Core::Constants::MODE_WELCOME);
    setContextHelpId(QLatin1String("Qt Creator Manual"));
    setContext(Core::Context(Core::Constants::C_WELCOME_MODE));

    m_modeWidget = new QWidget;
    m_modeWidget->setObjectName(QLatin1String("DashBoardPageModeWidget"));
    QVBoxLayout *layout = new QVBoxLayout(m_modeWidget);
    layout->setMargin(0);
    layout->setSpacing(0);

    m_dashboardPage = new QuickContainer();
    m_dashboardPage->setResizeMode(QuickContainer::SizeRootObjectToView);

    m_dashboardPage->setObjectName(QLatin1String("DashBoardPage"));

#if QT_VERSION >= 0x050300
    connect(m_dashboardPage, SIGNAL(sceneGraphError(QQuickWindow::SceneGraphError,QString)),
            this, SLOT(sceneGraphError(QQuickWindow::SceneGraphError,QString)));
#endif // Qt 5.3

    Utils::StyledBar* styledBar = new Utils::StyledBar(m_modeWidget);
    styledBar->setObjectName(QLatin1String("DashBoardPageStyledBar"));
    layout->addWidget(styledBar);

#ifdef USE_QUICK_WIDGET
    m_dashboardPage->setParent(m_modeWidget);
    layout->addWidget(m_dashboardPage);
#else
    QWidget *container = QWidget::createWindowContainer(m_dashboardPage, m_modeWidget);
    container->setProperty("nativeAncestors", true);
    m_modeWidget->setLayout(layout);
    layout->addWidget(container);
#endif // USE_QUICK_WIDGET

    connect(PluginManager::instance(), SIGNAL(objectAdded(QObject*)), SLOT(dashboardPluginAdded(QObject*)));

    setWidget(m_modeWidget);
}

DashBoardMode::~DashBoardMode()
{
    QSettings *settings = Core::ICore::settings();
    settings->setValue(QLatin1String(currentPageSettingsKeyC), activePlugin());
    delete m_modeWidget;
}

#if QT_VERSION >= 0x050300
void DashBoardMode::sceneGraphError(QQuickWindow::SceneGraphError, const QString &message)
{
    QMessageBox *messageBox =
        new QMessageBox(QMessageBox::Warning,
                        tr("DashBoard Mode Load Error"), message,
                        QMessageBox::Close, m_modeWidget);
    messageBox->setModal(false);
    messageBox->setAttribute(Qt::WA_DeleteOnClose);
    messageBox->show();
}
#endif // Qt 5.3

void DashBoardMode::facilitateQml(QQmlEngine * /*engine*/)
{
}

static QString applicationDirPath()
{
    // normalize paths so QML doesn't freak out if it's wrongly capitalized on Windows
    return Utils::FileUtils::normalizePathName(QCoreApplication::applicationDirPath());
}

static QString resourcePath()
{
    // normalize paths so QML doesn't freak out if it's wrongly capitalized on Windows
    return Utils::FileUtils::normalizePathName(Core::ICore::resourcePath());
}

void DashBoardMode::initPlugins()
{
    QSettings *settings = Core::ICore::settings();
    setActivePlugin(settings->value(QLatin1String(currentPageSettingsKeyC)).toInt());

    QQmlContext *ctx = m_dashboardPage->rootContext();
    ctx->setContextProperty(QLatin1String("dashboardMode"), this);

    QList<Utils::IWelcomePage*> duplicatePlugins = PluginManager::getObjects<Utils::IWelcomePage>();
    Utils::sort(duplicatePlugins, [](const Utils::IWelcomePage *l, const Utils::IWelcomePage *r) {
        return l->priority() < r->priority();
    });

    QList<Utils::IWelcomePage*> plugins;
    QHash<Utils::IWelcomePage::Id, Utils::IWelcomePage*> pluginHash;

    //avoid duplicate ids - choose by priority
    foreach (Utils::IWelcomePage* plugin, duplicatePlugins) {
        if (pluginHash.contains(plugin->id())) {
            Utils::IWelcomePage* pluginOther = pluginHash.value(plugin->id());

            if (pluginOther->priority() > plugin->priority()) {
                plugins.removeAll(pluginOther);
                pluginHash.remove(pluginOther->id());
                plugins << plugin;
                pluginHash.insert(plugin->id(), plugin);
            }

        } else {
            plugins << plugin;
            pluginHash.insert(plugin->id(), plugin);
        }
    }


    QQmlEngine *engine = m_dashboardPage->engine();
    QStringList importPathList = engine->importPathList();
    importPathList << resourcePath() + QLatin1String("/dashboardscreen");
    engine->setImportPathList(importPathList);
    if (!debug)
        engine->setOutputWarningsToStandardError(false);
    QString pluginPath = applicationDirPath();
    if (HostOsInfo::isMacHost())
        pluginPath += QLatin1String("/../PlugIns");
    else
        pluginPath += QLatin1String("/../lib/qtcreator");   //IDE_LIBRARY_BASENAME
    engine->addImportPath(QDir::cleanPath(pluginPath));
    facilitateQml(engine);
    foreach (Utils::IWelcomePage *plugin, plugins) {
        plugin->facilitateQml(engine);
        m_pluginList.append(plugin);
    }

    ctx->setContextProperty(QLatin1String("pagesModel"), QVariant::fromValue(m_pluginList));

    QString path = resourcePath() + QLatin1String("/dashboardscreen/dashboardscreen.qml");

    // finally, load the root page
    m_dashboardPage->setSource(
            QUrl::fromLocalFile(path));
}

QString DashBoardMode::platform() const
{
    switch (HostOsInfo::hostOs()) {
    case OsTypeWindows: return QLatin1String("windows");
    case OsTypeMac: return QLatin1String("mac");
    case OsTypeLinux: return QLatin1String("linux");
    case OsTypeOtherUnix: return QLatin1String("unix");
    default: return QLatin1String("other");
    }
}

void DashBoardMode::dashboardPluginAdded(QObject *obj)
{
    QHash<Utils::IWelcomePage::Id, Utils::IWelcomePage*> pluginHash;

    foreach (QObject *obj, m_pluginList) {
        Utils::IWelcomePage *plugin = qobject_cast<Utils::IWelcomePage*>(obj);
        pluginHash.insert(plugin->id(), plugin);
    }
    if (Utils::IWelcomePage *plugin = qobject_cast<Utils::IWelcomePage*>(obj)) {
        //check for duplicated id
        if (pluginHash.contains(plugin->id())) {
            Utils::IWelcomePage* pluginOther = pluginHash.value(plugin->id());

            if (pluginOther->priority() > plugin->priority())
                m_pluginList.removeAll(pluginOther);
            else
                return;
        }

        int insertPos = 0;
        foreach (Utils::IWelcomePage* p, PluginManager::getObjects<Utils::IWelcomePage>()) {
            if (plugin->priority() < p->priority())
                insertPos++;
            else
                break;
        }
        m_pluginList.insert(insertPos, plugin);
        // update model through reset
        QQmlContext *ctx = m_dashboardPage->rootContext();
        ctx->setContextProperty(QLatin1String("pagesModel"), QVariant::fromValue(m_pluginList));
    }
}

DashBoardPlugin::DashBoardPlugin()
  : m_dashboardMode(0)
{
}

/*! Initializes the plugin. Returns true on success.
    Plugins want to register objects with the plugin manager here.

    \a errorMessage can be used to pass an error message to the plugin system,
       if there was any.
*/
bool DashBoardPlugin::initialize(const QStringList & /* arguments */, QString * /* errorMessage */)
{
    m_dashboardMode = new DashBoardMode;
    addAutoReleasedObject(m_dashboardMode);

    return true;
}

/*! Notification that all extensions that this plugin depends on have been
    initialized. The dependencies are defined in the plugins .qwp file.

    Normally this function is used for things that rely on other plugins to have
    added objects to the plugin manager, that implement interfaces that we're
    interested in. These objects can now be requested through the
    PluginManagerInterface.

    The DashBoardPlugin doesn't need things from other plugins, so it does
    nothing here.
*/
void DashBoardPlugin::extensionsInitialized()
{
    m_dashboardMode->initPlugins();
    Core::ModeManager::activateMode(m_dashboardMode->id());
}

} // namespace Internal
} // namespace DashBoard


Q_EXPORT_PLUGIN(DashBoard::Internal::DashBoardPlugin)

#include "dashboardplugin.moc"
