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

#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <coreplugin/icontext.h>

#include <qglobal.h>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

namespace Help {
namespace Internal {

class HelpViewer;

class HelpWidget : public QWidget
{
    Q_OBJECT
public:
    enum WidgetStyle {
        SideBarWidget,
        ExternalWindow
    };

    HelpWidget(const Core::Context &context, WidgetStyle style, QWidget *parent = 0);
    ~HelpWidget();

    HelpViewer *currentViewer() const;

protected:
    void closeEvent(QCloseEvent *);

signals:
    void openHelpMode(const QUrl &url);
    void closeButtonClicked();
    void aboutToClose();

private slots:
    void updateBackMenu();
    void updateForwardMenu();
    void updateWindowTitle();
    void helpModeButtonClicked();
    void updateHelpModeButtonToolTip();

private:
    Core::IContext *m_context;
    QAction *m_switchToHelp;
    QMenu *m_backMenu;
    QMenu *m_forwardMenu;
    QAction *m_openHelpMode;
    QAction *m_scaleUp;
    QAction *m_scaleDown;
    QAction *m_resetScale;
    QAction *m_copy;

    HelpViewer *m_viewer;
    WidgetStyle m_style;
};

} // Internal
} // Help

#endif // HELPWIDGET_H
