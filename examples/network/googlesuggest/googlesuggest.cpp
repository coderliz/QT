/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
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
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtCore>
#include <QtGui>
#include <QtNetwork>

#include "googlesuggest.h"

#define GSUGGEST_URL "http://google.com/complete/search?output=toolbar&q=%1"

GSuggestCompletion::GSuggestCompletion(QLineEdit *parent): QObject(parent), editor(parent)
{
    popup = new QTreeWidget;
    popup->setColumnCount(2);
    popup->setUniformRowHeights(true);
    popup->setRootIsDecorated(false);
    popup->setEditTriggers(QTreeWidget::NoEditTriggers);
    popup->setSelectionBehavior(QTreeWidget::SelectRows);
    popup->setFrameStyle(QFrame::Box | QFrame::Plain);
    popup->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    popup->header()->hide();
    popup->installEventFilter(this);
    popup->setMouseTracking(true);

    connect(popup, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            SLOT(doneCompletion()));

    popup->setWindowFlags(Qt::Popup);
    popup->setFocusPolicy(Qt::NoFocus);
    popup->setFocusProxy(parent);

    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(500);
    connect(timer, SIGNAL(timeout()), SLOT(autoSuggest()));
    connect(editor, SIGNAL(textEdited(QString)), timer, SLOT(start()));

    connect(&networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handleNetworkData(QNetworkReply*)));

}

GSuggestCompletion::~GSuggestCompletion()
{
    delete popup;
}

bool GSuggestCompletion::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj != popup)
        return false;

    if (ev->type() == QEvent::MouseButtonPress) {
        popup->hide();
        editor->setFocus();
        return true;
    }

    if (ev->type() == QEvent::KeyPress) {

        bool consumed = false;
        int key = static_cast<QKeyEvent*>(ev)->key();
        switch (key) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            doneCompletion();
            consumed = true;

        case Qt::Key_Escape:
            editor->setFocus();
            popup->hide();
            consumed = true;

        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Home:
        case Qt::Key_End:
        case Qt::Key_PageUp:
        case Qt::Key_PageDown:
            break;

        default:
            editor->setFocus();
            editor->event(ev);
            popup->hide();
            break;
        }

        return consumed;
    }

    return false;
}

void GSuggestCompletion::showCompletion(const QStringList &choices, const QStringList &hits)
{

    if (choices.isEmpty() || choices.count() != hits.count())
        return;

    const QPalette &pal = editor->palette();
    QColor color = pal.color(QPalette::Disabled, QPalette::WindowText);

    popup->setUpdatesEnabled(false);
    popup->clear();
    for (int i = 0; i < choices.count(); ++i) {
        QTreeWidgetItem * item;
        item = new QTreeWidgetItem(popup);
        item->setText(0, choices[i]);
        item->setText(1, hits[i]);
        item->setTextAlignment(1, Qt::AlignRight);
        item->setTextColor(1, color);
    }
    popup->setCurrentItem(popup->topLevelItem(0));
    popup->resizeColumnToContents(0);
    popup->resizeColumnToContents(1);
    popup->adjustSize();
    popup->setUpdatesEnabled(true);

    int h = popup->sizeHintForRow(0) * qMin(7, choices.count()) + 3;
    popup->resize(popup->width(), h);

    popup->move(editor->mapToGlobal(QPoint(0, editor->height())));
    popup->setFocus();
    popup->show();
}

void GSuggestCompletion::doneCompletion()
{
    timer->stop();
    popup->hide();
    editor->setFocus();
    QTreeWidgetItem *item = popup->currentItem();
    if (item) {
        editor->setText(item->text(0));
        QKeyEvent *e;
        e = new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
        QApplication::postEvent(editor, e);
        e = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Enter, Qt::NoModifier);
        QApplication::postEvent(editor, e);
    }
}

void GSuggestCompletion::preventSuggest()
{
    timer->stop();
}

void GSuggestCompletion::autoSuggest()
{
    QString str = editor->text();
    QString url = QString(GSUGGEST_URL).arg(str);
    networkManager.get(QNetworkRequest(QString(url)));
}

void GSuggestCompletion::handleNetworkData(QNetworkReply *networkReply)
{
    QUrl url = networkReply->url();
    if (!networkReply->error()) {
        QStringList choices;
        QStringList hits;

        QString response(networkReply->readAll());
        QXmlStreamReader xml(response);
        while (!xml.atEnd()) {
            xml.readNext();
            if (xml.tokenType() == QXmlStreamReader::StartElement)
                if (xml.name() == "suggestion") {
                    QStringRef str = xml.attributes().value("data");
                    choices << str.toString();
                }
            if (xml.tokenType() == QXmlStreamReader::StartElement)
                if (xml.name() == "num_queries") {
                    QStringRef str = xml.attributes().value("int");
                    hits << str.toString();
                }
        }

        showCompletion(choices, hits);
    }

    networkReply->deleteLater();
}
