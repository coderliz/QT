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

//! [0]
Q3Url url( "http://qt.nokia.com" );
// or
Q3Url url( "file:///home/myself/Mail", "Inbox" );
//! [0]


//! [1]
Q3Url url( "http://qt.nokia.com" );
QString s = url;
// or
QString s( "http://qt.nokia.com" );
Q3Url url( s );
//! [1]


//! [2]
Q3Url url( "ftp://ftp.qt.nokia.com/qt/source", "qt-2.1.0.tar.gz" );
//! [2]


//! [3]
Q3Url url( "ftp://ftp.qt.nokia.com/qt/source", "/usr/local" );
//! [3]


//! [4]
Q3Url url( "ftp://ftp.qt.nokia.com/qt/source", "file:///usr/local" );
//! [4]


//! [5]
QString url = http://qt.nokia.com
Q3Url::encode( url );
// url is now "http%3A//qt%20nokia%20com"
//! [5]


//! [6]
QString url = "http%3A//qt%20nokia%20com"
Q3Url::decode( url );
// url is now "http://qt.nokia.com"
//! [6]
