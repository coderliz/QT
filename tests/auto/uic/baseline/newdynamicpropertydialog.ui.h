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

/********************************************************************************
** Form generated from reading ui file 'newdynamicpropertydialog.ui'
**
** Created: Thu Jul 10 09:47:35 2008
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef NEWDYNAMICPROPERTYDIALOG_H
#define NEWDYNAMICPROPERTYDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class Ui_NewDynamicPropertyDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLineEdit *m_lineEdit;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QComboBox *m_comboBox;
    QSpacerItem *horizontalSpacer;
    QLabel *label_2;
    QSpacerItem *spacerItem;
    QDialogButtonBox *m_buttonBox;

    void setupUi(QDialog *qdesigner_internal__NewDynamicPropertyDialog)
    {
        if (qdesigner_internal__NewDynamicPropertyDialog->objectName().isEmpty())
            qdesigner_internal__NewDynamicPropertyDialog->setObjectName(QString::fromUtf8("qdesigner_internal__NewDynamicPropertyDialog"));
        qdesigner_internal__NewDynamicPropertyDialog->resize(340, 118);
        verticalLayout = new QVBoxLayout(qdesigner_internal__NewDynamicPropertyDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        m_lineEdit = new QLineEdit(qdesigner_internal__NewDynamicPropertyDialog);
        m_lineEdit->setObjectName(QString::fromUtf8("m_lineEdit"));
        m_lineEdit->setMinimumSize(QSize(220, 0));

        formLayout->setWidget(0, QFormLayout::FieldRole, m_lineEdit);

        label = new QLabel(qdesigner_internal__NewDynamicPropertyDialog);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        m_comboBox = new QComboBox(qdesigner_internal__NewDynamicPropertyDialog);
        m_comboBox->setObjectName(QString::fromUtf8("m_comboBox"));

        horizontalLayout->addWidget(m_comboBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        formLayout->setLayout(1, QFormLayout::FieldRole, horizontalLayout);

        label_2 = new QLabel(qdesigner_internal__NewDynamicPropertyDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);


        verticalLayout->addLayout(formLayout);

        spacerItem = new QSpacerItem(QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(spacerItem);

        m_buttonBox = new QDialogButtonBox(qdesigner_internal__NewDynamicPropertyDialog);
        m_buttonBox->setObjectName(QString::fromUtf8("m_buttonBox"));
        m_buttonBox->setOrientation(Qt::Horizontal);
        m_buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        m_buttonBox->setCenterButtons(false);

        verticalLayout->addWidget(m_buttonBox);


        retranslateUi(qdesigner_internal__NewDynamicPropertyDialog);

        QMetaObject::connectSlotsByName(qdesigner_internal__NewDynamicPropertyDialog);
    } // setupUi

    void retranslateUi(QDialog *qdesigner_internal__NewDynamicPropertyDialog)
    {
        qdesigner_internal__NewDynamicPropertyDialog->setWindowTitle(QApplication::translate("qdesigner_internal::NewDynamicPropertyDialog", "Create Dynamic Property", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("qdesigner_internal::NewDynamicPropertyDialog", "Property Name", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("qdesigner_internal::NewDynamicPropertyDialog", "Property Type", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(qdesigner_internal__NewDynamicPropertyDialog);
    } // retranslateUi

};

} // namespace qdesigner_internal

namespace qdesigner_internal {
namespace Ui {
    class NewDynamicPropertyDialog: public Ui_NewDynamicPropertyDialog {};
} // namespace Ui
} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // NEWDYNAMICPROPERTYDIALOG_H
