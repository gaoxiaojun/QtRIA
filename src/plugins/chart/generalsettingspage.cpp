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

#include "generalsettingspage.h"

#include "chartconstants.h"

#include <coreplugin/coreconstants.h>
#include <coreplugin/icore.h>

#include <utils/fileutils.h>

#include <QCoreApplication>
#include <QSettings>
#include <QTextStream>

#include <QApplication>
#include <QFileDialog>

using namespace Core;
using namespace Chart::Internal;

GeneralSettingsPage::GeneralSettingsPage()
    : m_ui(0)
{
    setId("C.General settings");
    setDisplayName(tr("General"));
    setCategory(Chart::Constants::CHART_CATEGORY);
    setDisplayCategory(QCoreApplication::translate("Chart", Chart::Constants::CHART_TR_CATEGORY));
    setCategoryIcon(QLatin1String(Chart::Constants::CHART_CATEGORY_ICON));
}

QWidget *GeneralSettingsPage::widget()
{
    if (!m_widget) {
        m_widget = new QWidget;
        m_ui = new Ui::GeneralSettingsPage;
        m_ui->setupUi(m_widget);
    }
    return m_widget;
}

void GeneralSettingsPage::apply()
{
    if (!m_ui) // page was never shown
        return;
}

void GeneralSettingsPage::finish()
{
    delete m_widget;
    if (!m_ui) // page was never shown
        return;
    delete m_ui;
    m_ui = 0;
}
