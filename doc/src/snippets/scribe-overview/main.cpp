/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the documentation of the Qt Toolkit.
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

#include <QtGui>

void mergeFormat(QTextEdit *edit)
{
//! [0]
    QTextDocument *document = edit->document(); 
    QTextCursor cursor(document);

    cursor.movePosition(QTextCursor::Start); 
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);

    QTextCharFormat format;
    format.setFontWeight(QFont::Bold);

    cursor.mergeCharFormat(format);
//! [0]
}

int main(int argc, char *argv[])
{
    QWidget *parent = 0;
    QString aStringContainingHTMLtext("<h1>Scribe Overview</h1>");

    QApplication app(argc, argv);

//! [1]
    QTextEdit *editor = new QTextEdit(parent);
    editor->setHtml(aStringContainingHTMLtext);
    editor->show();
//! [1]

    return app.exec();
}
