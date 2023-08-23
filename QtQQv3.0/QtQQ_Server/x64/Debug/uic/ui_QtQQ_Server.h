/********************************************************************************
** Form generated from reading UI file 'QtQQ_Server.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTQQ_SERVER_H
#define UI_QTQQ_SERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtQQ_ServerClass
{
public:
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QTableWidget *tableWidget;
    QWidget *widget;
    QLabel *label;
    QGroupBox *groupBox;
    QFrame *line;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QComboBox *employeeDepBox;
    QLineEdit *nameLineEdit;
    QPushButton *selectPictureBtn;
    QPushButton *addBtn;
    QLabel *headLabel;
    QComboBox *departentBox;
    QPushButton *queryDepartmentBtn;
    QPushButton *queryIdBtn;
    QLineEdit *queryIdLineEdit;
    QPushButton *logoutBtn;
    QLineEdit *logoutIdLineEdit;

    void setupUi(QDialog *QtQQ_ServerClass)
    {
        if (QtQQ_ServerClass->objectName().isEmpty())
            QtQQ_ServerClass->setObjectName(QString::fromUtf8("QtQQ_ServerClass"));
        QtQQ_ServerClass->resize(999, 502);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QtQQ_ServerClass->sizePolicy().hasHeightForWidth());
        QtQQ_ServerClass->setSizePolicy(sizePolicy);
        QtQQ_ServerClass->setMinimumSize(QSize(999, 502));
        QtQQ_ServerClass->setMaximumSize(QSize(999, 502));
        scrollArea = new QScrollArea(QtQQ_ServerClass);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(1, 1, 700, 500));
        sizePolicy.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy);
        scrollArea->setMinimumSize(QSize(700, 500));
        scrollArea->setMaximumSize(QSize(700, 500));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 698, 498));
        tableWidget = new QTableWidget(scrollAreaWidgetContents);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(1, 1, 700, 500));
        sizePolicy.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy);
        tableWidget->setMinimumSize(QSize(700, 500));
        tableWidget->setMaximumSize(QSize(700, 500));
        scrollArea->setWidget(scrollAreaWidgetContents);
        widget = new QWidget(QtQQ_ServerClass);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(702, 1, 296, 500));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(13, 0, 271, 201));
        label->setPixmap(QPixmap(QString::fromUtf8("tiger.png")));
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(0, 201, 291, 290));
        line = new QFrame(groupBox);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(0, 170, 291, 3));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 10, 72, 15));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 40, 72, 15));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 70, 72, 15));
        employeeDepBox = new QComboBox(groupBox);
        employeeDepBox->addItem(QString());
        employeeDepBox->addItem(QString());
        employeeDepBox->addItem(QString());
        employeeDepBox->setObjectName(QString::fromUtf8("employeeDepBox"));
        employeeDepBox->setGeometry(QRect(170, 6, 113, 25));
        nameLineEdit = new QLineEdit(groupBox);
        nameLineEdit->setObjectName(QString::fromUtf8("nameLineEdit"));
        nameLineEdit->setGeometry(QRect(170, 36, 113, 25));
        selectPictureBtn = new QPushButton(groupBox);
        selectPictureBtn->setObjectName(QString::fromUtf8("selectPictureBtn"));
        selectPictureBtn->setGeometry(QRect(247, 66, 31, 25));
        addBtn = new QPushButton(groupBox);
        addBtn->setObjectName(QString::fromUtf8("addBtn"));
        addBtn->setGeometry(QRect(170, 140, 110, 25));
        headLabel = new QLabel(groupBox);
        headLabel->setObjectName(QString::fromUtf8("headLabel"));
        headLabel->setGeometry(QRect(110, 70, 101, 61));
        departentBox = new QComboBox(groupBox);
        departentBox->addItem(QString());
        departentBox->addItem(QString());
        departentBox->addItem(QString());
        departentBox->addItem(QString());
        departentBox->setObjectName(QString::fromUtf8("departentBox"));
        departentBox->setGeometry(QRect(20, 191, 113, 25));
        queryDepartmentBtn = new QPushButton(groupBox);
        queryDepartmentBtn->setObjectName(QString::fromUtf8("queryDepartmentBtn"));
        queryDepartmentBtn->setGeometry(QRect(170, 189, 110, 25));
        queryIdBtn = new QPushButton(groupBox);
        queryIdBtn->setObjectName(QString::fromUtf8("queryIdBtn"));
        queryIdBtn->setGeometry(QRect(170, 227, 110, 25));
        queryIdLineEdit = new QLineEdit(groupBox);
        queryIdLineEdit->setObjectName(QString::fromUtf8("queryIdLineEdit"));
        queryIdLineEdit->setGeometry(QRect(20, 227, 113, 25));
        logoutBtn = new QPushButton(groupBox);
        logoutBtn->setObjectName(QString::fromUtf8("logoutBtn"));
        logoutBtn->setGeometry(QRect(170, 261, 110, 25));
        logoutIdLineEdit = new QLineEdit(groupBox);
        logoutIdLineEdit->setObjectName(QString::fromUtf8("logoutIdLineEdit"));
        logoutIdLineEdit->setGeometry(QRect(20, 260, 113, 25));

        retranslateUi(QtQQ_ServerClass);

        QMetaObject::connectSlotsByName(QtQQ_ServerClass);
    } // setupUi

    void retranslateUi(QDialog *QtQQ_ServerClass)
    {
        QtQQ_ServerClass->setWindowTitle(QCoreApplication::translate("QtQQ_ServerClass", "QtQQ_Server", nullptr));
        label->setText(QString());
        groupBox->setTitle(QString());
        label_2->setText(QCoreApplication::translate("QtQQ_ServerClass", "\345\221\230\345\267\245\351\203\250\351\227\250\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("QtQQ_ServerClass", "\345\221\230\345\267\245\345\247\223\345\220\215\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("QtQQ_ServerClass", "\345\221\230\345\267\245\345\257\270\347\205\247\357\274\232", nullptr));
        employeeDepBox->setItemText(0, QCoreApplication::translate("QtQQ_ServerClass", "\344\272\272\344\272\213\351\203\250", nullptr));
        employeeDepBox->setItemText(1, QCoreApplication::translate("QtQQ_ServerClass", "\347\240\224\345\217\221\351\203\250", nullptr));
        employeeDepBox->setItemText(2, QCoreApplication::translate("QtQQ_ServerClass", "\345\270\202\345\234\272\351\203\250", nullptr));

        nameLineEdit->setPlaceholderText(QCoreApplication::translate("QtQQ_ServerClass", "\350\257\267\350\276\223\345\205\245\345\221\230\345\267\245\345\247\223\345\220\215", nullptr));
        selectPictureBtn->setText(QCoreApplication::translate("QtQQ_ServerClass", "...", nullptr));
        addBtn->setText(QCoreApplication::translate("QtQQ_ServerClass", "\346\226\260\345\242\236\345\221\230\345\267\245", nullptr));
        headLabel->setText(QCoreApplication::translate("QtQQ_ServerClass", "  \345\221\230\345\267\245\345\257\270\347\205\247", nullptr));
        departentBox->setItemText(0, QCoreApplication::translate("QtQQ_ServerClass", "\345\205\254\345\217\270\347\276\244", nullptr));
        departentBox->setItemText(1, QCoreApplication::translate("QtQQ_ServerClass", "\344\272\272\344\272\213\351\203\250", nullptr));
        departentBox->setItemText(2, QCoreApplication::translate("QtQQ_ServerClass", "\347\240\224\345\217\221\351\203\250", nullptr));
        departentBox->setItemText(3, QCoreApplication::translate("QtQQ_ServerClass", "\345\270\202\345\234\272\351\203\250", nullptr));

        queryDepartmentBtn->setText(QCoreApplication::translate("QtQQ_ServerClass", "\346\237\245\350\257\242\345\221\230\345\267\245", nullptr));
        queryIdBtn->setText(QCoreApplication::translate("QtQQ_ServerClass", "\346\237\245\350\257\242\345\221\230\345\267\245", nullptr));
        queryIdLineEdit->setPlaceholderText(QCoreApplication::translate("QtQQ_ServerClass", "\350\257\267\350\276\223\345\205\245\345\221\230\345\267\245id\345\217\267", nullptr));
        logoutBtn->setText(QCoreApplication::translate("QtQQ_ServerClass", "\346\263\250\351\224\200\345\221\230\345\267\245", nullptr));
        logoutIdLineEdit->setPlaceholderText(QCoreApplication::translate("QtQQ_ServerClass", "\350\257\267\350\276\223\345\205\245\345\221\230\345\267\245id\345\217\267", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtQQ_ServerClass: public Ui_QtQQ_ServerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTQQ_SERVER_H
