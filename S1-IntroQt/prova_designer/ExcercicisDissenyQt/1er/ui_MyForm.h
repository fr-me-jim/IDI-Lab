/********************************************************************************
** Form generated from reading UI file 'MyForm.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYFORM_H
#define UI_MYFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>
#include "MyLabel.h"

QT_BEGIN_NAMESPACE

class Ui_MyForm
{
public:
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    MyLabel *label;

    void setupUi(QWidget *MyForm)
    {
        if (MyForm->objectName().isEmpty())
            MyForm->setObjectName(QStringLiteral("MyForm"));
        MyForm->resize(714, 574);
        radioButton = new QRadioButton(MyForm);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setGeometry(QRect(60, 190, 111, 22));
        radioButton_2 = new QRadioButton(MyForm);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));
        radioButton_2->setGeometry(QRect(60, 230, 105, 22));
        label = new MyLabel(MyForm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(60, 80, 191, 51));
        label->setStyleSheet(QStringLiteral("background-color:rgb(255, 0, 0)"));

        retranslateUi(MyForm);
        QObject::connect(radioButton, SIGNAL(clicked()), label, SLOT(pintaVerd()));
        QObject::connect(radioButton_2, SIGNAL(clicked()), label, SLOT(pintaVermell()));

        QMetaObject::connectSlotsByName(MyForm);
    } // setupUi

    void retranslateUi(QWidget *MyForm)
    {
        MyForm->setWindowTitle(QApplication::translate("MyForm", "Form", 0));
        radioButton->setText(QApplication::translate("MyForm", "Obir e&l pas", 0));
        radioButton_2->setText(QApplication::translate("MyForm", "&Tancar el pas", 0));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MyForm: public Ui_MyForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYFORM_H
