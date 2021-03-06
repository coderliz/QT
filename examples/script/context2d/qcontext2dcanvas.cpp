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

#include "qcontext2dcanvas.h"

#include "context2d.h"
#include "environment.h"
#include "domimage.h"

#include <QPainter>
#include <QPaintEvent>

//! [3]
QContext2DCanvas::QContext2DCanvas(Context2D *context, Environment *env, QWidget *parent)
    : QWidget(parent), m_context(context), m_env(env)
{
    QObject::connect(context, SIGNAL(changed(QImage)), this, SLOT(contentsChanged(QImage)));
    setMouseTracking(true);
}
//! [3]

QContext2DCanvas::~QContext2DCanvas()
{
}

Context2D *QContext2DCanvas::context() const
{
    return m_context;
}

//! [0]
QScriptValue QContext2DCanvas::getContext(const QString &str)
{
    if (str != "2d")
        return QScriptValue();
    return m_env->toWrapper(m_context);
}
//! [0]

//! [1]
void QContext2DCanvas::contentsChanged(const QImage &image)
{
    m_image = image;
    update();
}

void QContext2DCanvas::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setClipRect(e->rect());
    p.drawImage(0, 0, m_image);
}
//! [1]

//! [2]
void QContext2DCanvas::mouseMoveEvent(QMouseEvent *e)
{
    m_env->handleEvent(this, e);
}

void QContext2DCanvas::mousePressEvent(QMouseEvent *e)
{
    m_env->handleEvent(this, e);
}

void QContext2DCanvas::mouseReleaseEvent(QMouseEvent *e)
{
    m_env->handleEvent(this, e);
}

void QContext2DCanvas::keyPressEvent(QKeyEvent *e)
{
    m_env->handleEvent(this, e);
}

void QContext2DCanvas::keyReleaseEvent(QKeyEvent *e)
{
    m_env->handleEvent(this, e);
}
//! [2]

void QContext2DCanvas::resizeEvent(QResizeEvent *e)
{
    m_context->setSize(e->size().width(), e->size().height());
}

void QContext2DCanvas::resize(int width, int height)
{
    QWidget::resize(width, height);
}

void QContext2DCanvas::reset()
{
    m_context->reset();
}

void QContext2DCanvas::addEventListener(const QString &type, const QScriptValue &listener,
                                        bool useCapture)
{
    Q_UNUSED(useCapture);
    if (listener.isFunction()) {
        QScriptValue self = m_env->toWrapper(this);
        self.setProperty("on" + type, listener);
    }
}
