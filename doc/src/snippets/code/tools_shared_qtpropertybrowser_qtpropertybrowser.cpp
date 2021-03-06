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
        QtSpinBoxFactory *factory;
        QSet<QtIntPropertyManager *> managers = factory->propertyManagers();
//! [0]


//! [1]
        QtBrowserItem *item;
        QList<QtBrowserItem *> childrenItems = item->children();

        QList<QtProperty *> childrenProperties = item->property()->subProperties();
//! [1]


//! [2]
        QtProperty *property1, *property2, *property3;

        property2->addSubProperty(property1);
        property3->addSubProperty(property2);

        QtAbstractPropertyBrowser *editor;

        editor->addProperty(property1);
        editor->addProperty(property2);
        editor->addProperty(property3);
//! [2]


//! [3]
        QtIntPropertyManager *intManager;
        QtDoublePropertyManager *doubleManager;

        QtProperty *myInteger = intManager->addProperty();
        QtProperty *myDouble = doubleManager->addProperty();

        QtSpinBoxFactory  *spinBoxFactory;
        QtDoubleSpinBoxFactory *doubleSpinBoxFactory;

        QtAbstractPropertyBrowser *editor;
        editor->setFactoryForManager(intManager, spinBoxFactory);
        editor->setFactoryForManager(doubleManager, doubleSpinBoxFactory);

        editor->addProperty(myInteger);
        editor->addProperty(myDouble);
//! [3]


