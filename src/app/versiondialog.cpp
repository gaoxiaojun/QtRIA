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

#include "app_version.h"
#include "versiondialog.h"


#include <QDialogButtonBox>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QApplication>
#include <QtDebug>
static QString compilerString()
{
#if defined(Q_CC_CLANG) // must be before GNU, because clang claims to be GNU too
    QString isAppleString;
#if defined(__apple_build_version__) // Apple clang has other version numbers
    isAppleString = QLatin1String(" (Apple)");
#endif
    return QLatin1String("Clang " ) + QString::number(__clang_major__) + QLatin1Char('.')
            + QString::number(__clang_minor__) + isAppleString;
#elif defined(Q_CC_GNU)
    return QLatin1String("GCC " ) + QLatin1String(__VERSION__);
#elif defined(Q_CC_MSVC)
    if (_MSC_VER >= 1800) // 1800: MSVC 2013 (yearly release cycle)
        return QLatin1String("MSVC ") + QString::number(2008 + ((_MSC_VER / 100) - 13));
    if (_MSC_VER >= 1500) // 1500: MSVC 2008, 1600: MSVC 2010, ... (2-year release cycle)
        return QLatin1String("MSVC ") + QString::number(2008 + 2 * ((_MSC_VER / 100) - 15));
#endif
    return QLatin1String("<unknown compiler>");
}

VersionDialog::VersionDialog(QWidget *parent)
    : QDialog(parent)
{
    // We need to set the window icon explicitly here since for some reason the
    // application icon isn't used when the size of the dialog is fixed (at least not on X11/GNOME)
    setWindowIcon(QIcon(QLatin1String(":/application/images/about.png")));

    setWindowTitle(tr("About %1").arg(Application::Constants::APP_NAME_STR));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QGridLayout *layout = new QGridLayout(this);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    const QString versionString = tr("%1 %2").arg(Application::Constants::APP_NAME_STR,
                                            Application::Constants::APP_VERSION_STR);
    const QString buildCompatibilityString = tr("Based on Qt %1 (%2, %3 bit)").arg(QLatin1String(qVersion()),
                                                                      compilerString(),
                                                                      QString::number(QSysInfo::WordSize));
    const QString ideRev = tr("From revision %1<br/>").arg(Application::Constants::APP_VERSION_PATCH);
    const QString description = tr(
       "<h3>%1</h3>"
       "%2<br/>"
       "<br/>"
       "Built on %3 at %4<br />"
       "<br/>"
       "%5"
       "<br/>"
       "Copyright 2014 %6. All rights reserved.<br/>"
       "<br/>"
       "The program is provided AS IS with NO WARRANTY OF ANY KIND, "
       "INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A "
       "PARTICULAR PURPOSE.<br/>")
       .arg(versionString,
            buildCompatibilityString,
            QLatin1String(__DATE__), QLatin1String(__TIME__),
            ideRev,
            QLatin1String(Application::Constants::APP_ORGNAME_STR));

    QLabel *copyRightLabel = new QLabel(description);
    copyRightLabel->setWordWrap(true);
    copyRightLabel->setOpenExternalLinks(true);
    copyRightLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    QPushButton *closeButton = buttonBox->button(QDialogButtonBox::Close);
    //QTC_CHECK(closeButton);
    buttonBox->addButton(closeButton, QDialogButtonBox::ButtonRole(QDialogButtonBox::RejectRole | QDialogButtonBox::AcceptRole));
    connect(buttonBox , SIGNAL(rejected()), this, SLOT(reject()));

    QLabel *logoLabel = new QLabel;
    logoLabel->setPixmap(QPixmap(QLatin1String(":/application/images/about.png")));
    layout->addWidget(logoLabel , 0, 0, 1, 1);
    layout->addWidget(copyRightLabel, 0, 1, 4, 4);
    layout->addWidget(buttonBox, 4, 0, 1, 5);
}

bool VersionDialog::event(QEvent *event)
{
    if (event->type() == QEvent::ShortcutOverride) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Escape && !ke->modifiers()) {
            ke->accept();
            return true;
        }
    }
    return QDialog::event(event);
}
