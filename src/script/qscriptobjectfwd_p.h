/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtScript module of the Qt Toolkit.
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

#ifndef QSCRIPTOBJECTFWD_P_H
#define QSCRIPTOBJECTFWD_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qglobal.h>

#ifndef QT_NO_SCRIPT

#include "qscriptbuffer_p.h"
#include "qscriptmemberfwd_p.h"
#include "qscriptvalueimplfwd_p.h"

QT_BEGIN_NAMESPACE

class QScriptObjectData;

class QScriptObject
{
public:
    inline void reset();
    inline ~QScriptObject();
    inline void finalize();
    inline void finalizeData();

    inline bool findMember(QScriptNameIdImpl *nameId,
                           QScript::Member *m) const;

    inline bool findGetter(QScript::Member *m) const;

    inline bool findSetter(QScript::Member *m) const;

    inline int memberCount() const;

    inline void createMember(QScriptNameIdImpl *nameId,
                             QScript::Member *member, uint flags);

    inline void member(int index, QScript::Member *member);

    inline void put(const QScript::Member &m, const QScriptValueImpl &v);

    inline QScriptValueImpl &reference(const QScript::Member &m);

    inline void get(const QScript::Member &m, QScriptValueImpl *v);

    inline void removeMember(const QScript::Member &member);

    QScriptValueImpl m_prototype;
    QScriptValueImpl m_scope;
    QScriptValueImpl m_internalValue; // [[value]]
    QScriptObjectData *m_data;
    QScript::Buffer<QScript::Member> m_members;
    QScript::Buffer<QScriptValueImpl> m_values;
    qint64 m_id;
    QScriptClassInfo *m_class;
};

QT_END_NAMESPACE

#endif // QT_NO_SCRIPT

#endif
