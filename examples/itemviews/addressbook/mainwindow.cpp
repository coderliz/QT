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

#include <QtGui>
#include "mainwindow.h"

//! [0]
MainWindow::MainWindow()
{
    addressWidget = new AddressWidget;
    setCentralWidget(addressWidget);
    createMenus();
    setWindowTitle(tr("Address Book"));
}
//! [0]

//! [1a]
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    
    openAct = new QAction(tr("&Open..."), this);
    fileMenu->addAction(openAct);
    connect(openAct, SIGNAL(triggered()),
        this, SLOT(openFile()));
//! [1a]

    saveAct = new QAction(tr("&Save As..."), this);
    fileMenu->addAction(saveAct);
    connect(saveAct, SIGNAL(triggered()),
        this, SLOT(saveFile()));

    fileMenu->addSeparator();

    exitAct = new QAction(tr("E&xit"), this);
    fileMenu->addAction(exitAct);
    connect(exitAct, SIGNAL(triggered()),
        this, SLOT(close()));

    toolMenu = menuBar()->addMenu(tr("&Tools"));

    addAct = new QAction(tr("&Add Entry..."), this);
    toolMenu->addAction(addAct);
    connect(addAct, SIGNAL(triggered()),
        addressWidget, SLOT(addEntry()));
    
//! [1b]
    editAct = new QAction(tr("&Edit Entry..."), this);
    editAct->setEnabled(false);
    toolMenu->addAction(editAct);
    connect(editAct, SIGNAL(triggered()),
        addressWidget, SLOT(editEntry()));

    toolMenu->addSeparator();

    removeAct = new QAction(tr("&Remove Entry"), this);
    removeAct->setEnabled(false);
    toolMenu->addAction(removeAct);
    connect(removeAct, SIGNAL(triggered()),
        addressWidget, SLOT(removeEntry()));

    connect(addressWidget, SIGNAL(selectionChanged(const QItemSelection &)),
        this, SLOT(updateActions(const QItemSelection &)));
}
//! [1b]

//! [2]
void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        addressWidget->readFromFile(fileName);
    }
}
//! [2]

//! [3]
void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (!fileName.isEmpty()) {
        addressWidget->writeToFile(fileName);
    }
}
//! [3]

//! [4]
void MainWindow::updateActions(const QItemSelection &selection)
{
    QModelIndexList indexes = selection.indexes();
    
    if (!indexes.isEmpty()) {
        removeAct->setEnabled(true);
        editAct->setEnabled(true);
    } else {
        removeAct->setEnabled(false);
        editAct->setEnabled(false);
    }
}
//! [4]
