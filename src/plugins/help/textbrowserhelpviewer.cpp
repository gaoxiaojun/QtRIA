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

#include "textbrowserhelpviewer.h"

#include "helpconstants.h"
#include "localhelpmanager.h"

#include <coreplugin/find/findplugin.h>
#include <utils/hostosinfo.h>
#include <utils/qtcassert.h>

#include <QApplication>
#include <QClipboard>
#include <QContextMenuEvent>
#include <QHelpEngine>
#include <QKeyEvent>
#include <QMenu>
#include <QVBoxLayout>

using namespace Help;
using namespace Help::Internal;

// -- HelpViewer

TextBrowserHelpViewer::TextBrowserHelpViewer(qreal zoom, QWidget *parent)
    : HelpViewer(parent)
    , m_textBrowser(new TextBrowserHelpWidget(zoom, this))
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_textBrowser, 10);
    QPalette p = palette();
    p.setColor(QPalette::Inactive, QPalette::Highlight,
        p.color(QPalette::Active, QPalette::Highlight));
    p.setColor(QPalette::Inactive, QPalette::HighlightedText,
        p.color(QPalette::Active, QPalette::HighlightedText));
    setPalette(p);

    connect(m_textBrowser, SIGNAL(sourceChanged(QUrl)), this, SIGNAL(titleChanged()));
    connect(m_textBrowser, SIGNAL(forwardAvailable(bool)), this, SIGNAL(forwardAvailable(bool)));
    connect(m_textBrowser, SIGNAL(backwardAvailable(bool)), this, SIGNAL(backwardAvailable(bool)));
}

TextBrowserHelpViewer::~TextBrowserHelpViewer()
{
}

QFont TextBrowserHelpViewer::viewerFont() const
{
    return m_textBrowser->font();
}

void TextBrowserHelpViewer::setViewerFont(const QFont &newFont)
{
    m_textBrowser->forceFont = true;
    m_textBrowser->setFont(newFont);
    m_textBrowser->forceFont = false;
}

void TextBrowserHelpViewer::scaleUp()
{
    m_textBrowser->scaleUp();
}

void TextBrowserHelpViewer::scaleDown()
{
    m_textBrowser->scaleDown();
}

void TextBrowserHelpViewer::resetScale()
{
    if (m_textBrowser->zoomCount != 0) {
        m_textBrowser->forceFont = true;
        m_textBrowser->zoomOut(m_textBrowser->zoomCount);
        m_textBrowser->forceFont = false;
    }
    m_textBrowser->zoomCount = 0;
}

qreal TextBrowserHelpViewer::scale() const
{
    return m_textBrowser->zoomCount;
}

QString TextBrowserHelpViewer::title() const
{
    return m_textBrowser->documentTitle();
}

QUrl TextBrowserHelpViewer::source() const
{
    return m_textBrowser->source();
}

void TextBrowserHelpViewer::setSource(const QUrl &url)
{
    if (launchWithExternalApp(url))
        return;

    slotLoadStarted();
    m_textBrowser->setSource(url);
    slotLoadFinished();
}

void TextBrowserHelpViewer::scrollToAnchor(const QString &anchor)
{
    m_textBrowser->scrollToAnchor(anchor);
}

void TextBrowserHelpViewer::setHtml(const QString &html)
{
    m_textBrowser->setHtml(html);
}

QString TextBrowserHelpViewer::selectedText() const
{
    return m_textBrowser->textCursor().selectedText();
}

bool TextBrowserHelpViewer::isForwardAvailable() const
{
    return m_textBrowser->isForwardAvailable();
}

bool TextBrowserHelpViewer::isBackwardAvailable() const
{
    return m_textBrowser->isBackwardAvailable();
}

void TextBrowserHelpViewer::addBackHistoryItems(QMenu *backMenu)
{
    for (int i = 1; i <= m_textBrowser->backwardHistoryCount(); ++i) {
        QAction *action = new QAction(backMenu);
        action->setText(m_textBrowser->historyTitle(-i));
        action->setData(-i);
        connect(action, SIGNAL(triggered()), this, SLOT(goToHistoryItem()));
        backMenu->addAction(action);
    }
}

void TextBrowserHelpViewer::addForwardHistoryItems(QMenu *forwardMenu)
{
    for (int i = 1; i <= m_textBrowser->forwardHistoryCount(); ++i) {
        QAction *action = new QAction(forwardMenu);
        action->setText(m_textBrowser->historyTitle(i));
        action->setData(i);
        connect(action, SIGNAL(triggered()), this, SLOT(goToHistoryItem()));
        forwardMenu->addAction(action);
    }
}

void TextBrowserHelpViewer::setOpenInNewWindowActionVisible(bool visible)
{
    m_textBrowser->showOpenInNewWindowAction = visible;
}

bool TextBrowserHelpViewer::findText(const QString &text, Core::FindFlags flags,
    bool incremental, bool fromSearch, bool *wrapped)
{
    if (wrapped)
        *wrapped = false;
    QTextDocument *doc = m_textBrowser->document();
    QTextCursor cursor = m_textBrowser->textCursor();
    if (!doc || cursor.isNull())
        return false;

    const int position = cursor.selectionStart();
    if (incremental)
        cursor.setPosition(position);

    QTextDocument::FindFlags f = Core::textDocumentFlagsForFindFlags(flags);
    QTextCursor found = doc->find(text, cursor, f);
    if (found.isNull()) {
        if ((flags & Core::FindBackward) == 0)
            cursor.movePosition(QTextCursor::Start);
        else
            cursor.movePosition(QTextCursor::End);
        found = doc->find(text, cursor, f);
        if (!found.isNull() && wrapped)
            *wrapped = true;
    }

    if (fromSearch) {
        cursor.beginEditBlock();
        m_textBrowser->viewport()->setUpdatesEnabled(false);

        QTextCharFormat marker;
        marker.setForeground(Qt::red);
        cursor.movePosition(QTextCursor::Start);
        m_textBrowser->setTextCursor(cursor);

        while (m_textBrowser->find(text)) {
            QTextCursor hit = m_textBrowser->textCursor();
            hit.mergeCharFormat(marker);
        }

        m_textBrowser->viewport()->setUpdatesEnabled(true);
        cursor.endEditBlock();
    }

    bool cursorIsNull = found.isNull();
    if (cursorIsNull) {
        found = m_textBrowser->textCursor();
        found.setPosition(position);
    }
    m_textBrowser->setTextCursor(found);
    return !cursorIsNull;
}

// -- public slots

void TextBrowserHelpViewer::copy()
{
    m_textBrowser->copy();
}

void TextBrowserHelpViewer::stop()
{
}

void TextBrowserHelpViewer::forward()
{
    m_textBrowser->forward();
}

void TextBrowserHelpViewer::backward()
{
    m_textBrowser->backward();
}

void TextBrowserHelpViewer::print(QPrinter *printer)
{
    m_textBrowser->print(printer);
}

void TextBrowserHelpViewer::goToHistoryItem()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QTC_ASSERT(action, return);
    bool ok = false;
    int index = action->data().toInt(&ok);
    QTC_ASSERT(ok, return);
    // go back?
    while (index < 0) {
        m_textBrowser->backward();
        ++index;
    }
    // go forward?
    while (index > 0) {
        m_textBrowser->forward();
        --index;
    }
}

// -- private

TextBrowserHelpWidget::TextBrowserHelpWidget(int zoom, TextBrowserHelpViewer *parent)
    : QTextBrowser(parent)
    , zoomCount(zoom)
    , forceFont(false)
    , lastAnchor(QString())
    , showOpenInNewWindowAction(true)
    , m_parent(parent)
{
    installEventFilter(this);
    document()->setDocumentMargin(8);
}

QVariant TextBrowserHelpWidget::loadResource(int type, const QUrl &name)
{
    if (type < 4)
        return LocalHelpManager::helpData(name).data;
    return QByteArray();
}

bool TextBrowserHelpWidget::hasAnchorAt(const QPoint &pos)
{
    lastAnchor = anchorAt(pos);
    if (lastAnchor.isEmpty())
        return false;

    lastAnchor = source().resolved(lastAnchor).toString();
    if (lastAnchor.at(0) == QLatin1Char('#')) {
        QString src = source().toString();
        int hsh = src.indexOf(QLatin1Char('#'));
        lastAnchor = (hsh >= 0 ? src.left(hsh) : src) + lastAnchor;
    }
    return true;
}

void TextBrowserHelpWidget::openLink(bool newPage)
{
    if (lastAnchor.isEmpty())
        return;
    if (newPage)
        OpenPagesManager::instance().createPage(lastAnchor);
    else
        CentralWidget::instance()->setSource(lastAnchor);
    lastAnchor.clear();
}

void TextBrowserHelpWidget::scaleUp()
{
    if (zoomCount < 10) {
        zoomCount++;
        forceFont = true;
        zoomIn();
        forceFont = false;
    }
}

void TextBrowserHelpWidget::scaleDown()
{
    if (zoomCount > -5) {
        zoomCount--;
        forceFont = true;
        zoomOut();
        forceFont = false;
    }
}

void TextBrowserHelpWidget::openLink()
{
    openLink(false);
}

void TextBrowserHelpWidget::openLinkInNewPage()
{
    openLink(true);
}

void TextBrowserHelpWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(QLatin1String(""), 0);

    QUrl link;
    QAction *copyAnchorAction = 0;
    if (hasAnchorAt(event->pos())) {
        link = anchorAt(event->pos());
        if (link.isRelative())
            link = source().resolved(link);
        menu.addAction(tr("Open Link"), this, SLOT(openLink()));
        if (showOpenInNewWindowAction)
            menu.addAction(tr("Open Link as New Page"), this, SLOT(openLinkInNewPage()));

        if (!link.isEmpty() && link.isValid())
            copyAnchorAction = menu.addAction(tr("Copy Link"));
    } else if (!textCursor().selectedText().isEmpty()) {
        menu.addAction(tr("Copy"), this, SLOT(copy()));
    } else {
        menu.addAction(tr("Reload"), this, SLOT(reload()));
    }

    if (copyAnchorAction == menu.exec(event->globalPos()))
        QApplication::clipboard()->setText(link.toString());
}

bool TextBrowserHelpWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == this) {
        if (event->type() == QEvent::FontChange) {
            if (!forceFont)
                return true;
        } else if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Slash) {
                keyEvent->accept();
                Core::FindPlugin::instance()->openFindToolBar(Core::FindPlugin::FindForwardDirection);
                return true;
            }
        }
    }
    return QTextBrowser::eventFilter(obj, event);
}

void TextBrowserHelpWidget::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() == Qt::ControlModifier) {
        e->accept();
        e->delta() > 0 ? scaleUp() : scaleDown();
    } else {
        QTextBrowser::wheelEvent(e);
    }
}

void TextBrowserHelpWidget::mousePressEvent(QMouseEvent *e)
{
    if (Utils::HostOsInfo::isLinuxHost() && m_parent->handleForwardBackwardMouseButtons(e))
        return;
    QTextBrowser::mousePressEvent(e);
}

void TextBrowserHelpWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (!Utils::HostOsInfo::isLinuxHost() && m_parent->handleForwardBackwardMouseButtons(e))
        return;

    bool controlPressed = e->modifiers() & Qt::ControlModifier;
    if ((controlPressed && hasAnchorAt(e->pos())) ||
        (e->button() == Qt::MidButton && hasAnchorAt(e->pos()))) {
        openLinkInNewPage();
        return;
    }

    QTextBrowser::mouseReleaseEvent(e);
}
