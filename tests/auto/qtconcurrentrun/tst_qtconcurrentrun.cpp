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
#include <qtconcurrentrun.h>
// #include <qtconcurrentcreatefunctor.h>
#include <qfuture.h>
#include <QString>
#include <QtTest/QtTest>
#include "../qfuture/versioncheck.h"

#ifndef QT_NO_CONCURRENT_TEST

using namespace QtConcurrent;

class TestRunFunction: public QObject
{
    Q_OBJECT
private slots:
    void runLightFunction();
    void runHeavyFunction();
    void returnValue();
    void functionObject();
    void memberFunctions();
    void implicitConvertibleTypes();
    void runWaitLoop();
    void recursive();
#if 0
    void createFunctor();
#endif
};

#if 0
# define F(X) createFunctor(X)
#else
# define F(X) X
#endif


QTEST_MAIN(TestRunFunction)

void light()
{
    qDebug("in function");
    qDebug("done function");
}

void heavy()
{
    qDebug("in function");
    QString str;
    for (int i = 0; i < 1000000; ++i)
        str.append("a");
    qDebug("done function");
}


void TestRunFunction::runLightFunction()
{
    qDebug("starting function");
    QFuture<void> future = run(F(light));
    qDebug("waiting");
    future.waitForFinished();
    qDebug("done");
}

void TestRunFunction::runHeavyFunction()
{
    qDebug("starting function");
    QFuture<void> future = run(F(heavy));
    qDebug("waiting");
    future.waitForFinished();
    qDebug("done");
}

int returnInt0()
{
    return 10;
}

int returnInt1(int i)
{
    return i;
}

class A
{
public:
    int member0() { return 10; }
    int member1(int in) { return in; }

    typedef int result_type;
    int operator()() { return 10; }
    int operator()(int in) { return in; }
};

class AConst
{
public:
    int member0() const { return 10; }
    int member1(int in) const { return in; }

    typedef int result_type;
    int operator()() const { return 10; }
    int operator()(int in) const { return in; }
};

void TestRunFunction::returnValue()
{
    QFuture<int> f;
    
    f = run(F(returnInt0));
    QCOMPARE(f.result(), 10);
    
    A a;
    f = run(&a, F(&A::member0));
    QCOMPARE(f.result(), 10);

    f = run(&a, F(&A::member1), 20);
    QCOMPARE(f.result(), 20);

    f = run(a, F(&A::member0));
    QCOMPARE(f.result(), 10);

    f = run(a, F(&A::member1), 20);
    QCOMPARE(f.result(), 20);

    f = run(a);
    QCOMPARE(f.result(), 10);

    f = run(&a);
    QCOMPARE(f.result(), 10);

    f = run(a, 20);
    QCOMPARE(f.result(), 20);

    f = run(&a, 20);
    QCOMPARE(f.result(), 20);

    const AConst aConst = AConst();
    f = run(&aConst, &AConst::member0);
    QCOMPARE(f.result(), 10);

    f = run(&aConst, F(&AConst::member1), 20);
    QCOMPARE(f.result(), 20);

    f = run(aConst, F(&AConst::member0));
    QCOMPARE(f.result(), 10);

    f = run(aConst, F(&AConst::member1), 20);
    QCOMPARE(f.result(), 20);

    f = run(aConst);
    QCOMPARE(f.result(), 10);

    f = run(&aConst);
    QCOMPARE(f.result(), 10);

    f = run(aConst, 20);
    QCOMPARE(f.result(), 20);

    f = run(&aConst, 20);
    QCOMPARE(f.result(), 20);
}

struct TestClass
{
    void foo() { }
    typedef void result_type;
    void operator()() { }
    void operator()(int) { }
    void fooInt(int){ };
};

struct TestConstClass
{
    void foo() const { }
    typedef void result_type;
    void operator()() const { }
    void operator()(int) const { }
    void fooInt(int) const { };
};

void TestRunFunction::functionObject()
{
    QFuture<void> f;
    TestClass c;
    
    f = run(c);
    f = run(F(&c));
    f = run(c, 10);
    f = run(F(&c), 10);

    const TestConstClass cc = TestConstClass();
    f = run(cc);
    f = run(F(&cc));
    f = run(cc, 10);
    f = run(F(&cc), 10);
}


void TestRunFunction::memberFunctions()
{
    TestClass c;

    run(c, F(&TestClass::foo)).waitForFinished();
    run(&c, F(&TestClass::foo)).waitForFinished();
    run(c, F(&TestClass::fooInt), 10).waitForFinished();
    run(&c, F(&TestClass::fooInt), 10).waitForFinished();

    const TestConstClass cc = TestConstClass();
    run(cc, F(&TestConstClass::foo)).waitForFinished();
    run(&cc, F(&TestConstClass::foo)).waitForFinished();
    run(cc, F(&TestConstClass::fooInt), 10).waitForFinished();
    run(&cc, F(&TestConstClass::fooInt), 10).waitForFinished();
}


void doubleFunction(double)
{

}

void stringConstRefFunction(const QString &)
{

}

void stringRefFunction(QString &)
{

}

void stringFunction(QString)
{

}

void stringIntFunction(QString)
{

}


void TestRunFunction::implicitConvertibleTypes()
{
    double d;
    run(F(doubleFunction), d).waitForFinished();
    int i;
    run(F(doubleFunction), d).waitForFinished();
    run(F(doubleFunction), i).waitForFinished();
    run(F(doubleFunction), 10).waitForFinished();
    run(F(stringFunction), QLatin1String("Foo")).waitForFinished();
    run(F(stringConstRefFunction), QLatin1String("Foo")).waitForFinished();
    QString string;
    run(F(stringRefFunction), string).waitForFinished();
}

void fn() { }

void TestRunFunction::runWaitLoop()
{
    for (int i = 0; i < 1000; ++i)
        run(fn).waitForFinished();
}

QAtomicInt count;

void recursiveRun(int level)
{
    count.ref();
    if (--level > 0) {
        QFuture<void> f1 = run(recursiveRun, level);
        QFuture<void> f2 = run(recursiveRun, level);
        f1.waitForFinished();
        f2.waitForFinished();
    }
}

int recursiveResult(int level)
{
    count.ref();
    if (--level > 0) {
        QFuture<int> f1 = run(recursiveResult, level);
        QFuture<int> f2 = run(recursiveResult, level);
        return f1.result() + f2.result();
    }
    return 1;
}

void TestRunFunction::recursive()
{
    int levels = 15;

    for (int i = 0; i < QThread::idealThreadCount(); ++i) {
        count = 0;
        QThreadPool::globalInstance()->setMaxThreadCount(i);
        recursiveRun(levels);
        QCOMPARE((int)count, (int)pow(2.0, levels) - 1);
    }

    for (int i = 0; i < QThread::idealThreadCount(); ++i) {
        count = 0;
        QThreadPool::globalInstance()->setMaxThreadCount(i);
        recursiveResult(levels);
        QCOMPARE((int)count, (int)pow(2.0, levels) - 1);
    }
}

int e;
void vfn0()
{
    ++e;
}

int fn0()
{
    return 1;
}

void vfn1(double)
{
    ++e;
}

int fn1(int)
{
    return 1;
}

void vfn2(double, int *)
{
    ++e;
}

int fn2(double, int *)
{
    return 1;
}

#if 0
void TestRunFunction::createFunctor()
{
    e = 0;
    ::QtConcurrent::createFunctor(vfn0)();
    e += QtConcurrent::createFunctor(fn0)();
    ::QtConcurrent::createFunctor(vfn1)(1); // implicit conversions should work
    e += QtConcurrent::createFunctor(fn1)(2);
    ::QtConcurrent::createFunctor(vfn2)(1.0, &e);
    e += QtConcurrent::createFunctor(fn2)(2, &e);
    QCOMPARE(e, 6);


    e = 0;
    TestClass c;

//    ::QtConcurrent::createFunctor(c, &TestClass::foo)();
    ::QtConcurrent::createFunctor(&c, &TestClass::foo)();
//    ::QtConcurrent::createFunctor(c, &TestClass::fooInt)(10);
    ::QtConcurrent::createFunctor(&c, &TestClass::fooInt)(10);

    const TestConstClass cc = TestConstClass();
/*
    ::QtConcurrent::createFunctor(cc, &TestConstClass::foo)();
    ::QtConcurrent::createFunctor(&cc, &TestConstClass::foo)();
    ::QtConcurrent::createFunctor(cc, &TestConstClass::fooInt(10);
    ::QtConcurrent::createFunctor(&cc, &TestConstClass::fooInt)(10);
*/
    qDebug() << e;
}
#endif

#include "tst_qtconcurrentrun.moc"

#else
QTEST_NOOP_MAIN
#endif
