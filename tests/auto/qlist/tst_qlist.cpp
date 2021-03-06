/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the test suite of the Qt Toolkit.
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


#include <QtTest/QtTest>
#include <QList>

//TESTED_CLASS=QList
//TESTED_FILES=corelib/tools/qlist.h corelib/tools/qlist.cpp

/*!
  \class tst_QVector
  \internal
  \since 4.5
  \brief Test Qt's class QList.
 */
class tst_QList : public QObject
{
    Q_OBJECT

private slots:
    void length() const;
    void lengthSignature() const;
    void append() const;
};

void tst_QList::length() const
{
    /* Empty list. */
    {
        const QList<int> list;
        QCOMPARE(list.length(), 0);
    }

    /* One entry. */
    {
        QList<int> list;
        list.append(0);
        QCOMPARE(list.length(), 1);
    }

    /* Two entries. */
    {
        QList<int> list;
        list.append(0);
        list.append(1);
        QCOMPARE(list.length(), 2);
    }

    /* Three entries. */
    {
        QList<int> list;
        list.append(0);
        list.append(0);
        list.append(0);
        QCOMPARE(list.length(), 3);
    }
}

void tst_QList::lengthSignature() const
{
    /* Constness. */
    {
        const QList<int> list;
        /* The function should be const. */
        list.length();
    }
}

void tst_QList::append() const
{
    /* test append(const QList<T> &) function */
    QString one("one");
    QString two("two");
    QString three("three");
    QString four("four");
    QList<QString> list1;
    QList<QString> list2;
    QList<QString> listTotal;
    list1.append(one);
    list1.append(two);
    list2.append(three);
    list2.append(four);
    list1.append(list2);
    qDebug() << list1;
    listTotal.append(one);
    listTotal.append(two);
    listTotal.append(three);
    listTotal.append(four);
    QCOMPARE(list1, listTotal);

}

QTEST_APPLESS_MAIN(tst_QList)
#include "tst_qlist.moc"
