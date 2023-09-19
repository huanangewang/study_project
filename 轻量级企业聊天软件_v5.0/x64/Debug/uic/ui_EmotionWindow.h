/********************************************************************************
** Form generated from reading UI file 'EmotionWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMOTIONWINDOW_H
#define UI_EMOTIONWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EmotionWindowClass
{
public:
    QVBoxLayout *verticalLayout_3;
    QWidget *topWidget;
    QWidget *bottomWidget;
    QVBoxLayout *verticalLayout_2;
    QScrollArea *scrollArea;
    QWidget *emotionWidget;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;

    void setupUi(QWidget *EmotionWindowClass)
    {
        if (EmotionWindowClass->objectName().isEmpty())
            EmotionWindowClass->setObjectName(QString::fromUtf8("EmotionWindowClass"));
        EmotionWindowClass->resize(480, 358);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(EmotionWindowClass->sizePolicy().hasHeightForWidth());
        EmotionWindowClass->setSizePolicy(sizePolicy);
        EmotionWindowClass->setMinimumSize(QSize(480, 358));
        EmotionWindowClass->setMaximumSize(QSize(480, 358));
        verticalLayout_3 = new QVBoxLayout(EmotionWindowClass);
        verticalLayout_3->setSpacing(12);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(12, 12, 12, 12);
        topWidget = new QWidget(EmotionWindowClass);
        topWidget->setObjectName(QString::fromUtf8("topWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(topWidget->sizePolicy().hasHeightForWidth());
        topWidget->setSizePolicy(sizePolicy1);
        topWidget->setMinimumSize(QSize(456, 62));
        topWidget->setMaximumSize(QSize(456, 62));

        verticalLayout_3->addWidget(topWidget);

        bottomWidget = new QWidget(EmotionWindowClass);
        bottomWidget->setObjectName(QString::fromUtf8("bottomWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(bottomWidget->sizePolicy().hasHeightForWidth());
        bottomWidget->setSizePolicy(sizePolicy2);
        verticalLayout_2 = new QVBoxLayout(bottomWidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(bottomWidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setWidgetResizable(true);
        emotionWidget = new QWidget();
        emotionWidget->setObjectName(QString::fromUtf8("emotionWidget"));
        emotionWidget->setGeometry(QRect(0, 0, 456, 260));
        sizePolicy2.setHeightForWidth(emotionWidget->sizePolicy().hasHeightForWidth());
        emotionWidget->setSizePolicy(sizePolicy2);
        verticalLayout = new QVBoxLayout(emotionWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

        verticalLayout->addLayout(gridLayout);

        scrollArea->setWidget(emotionWidget);

        verticalLayout_2->addWidget(scrollArea);


        verticalLayout_3->addWidget(bottomWidget);


        retranslateUi(EmotionWindowClass);

        QMetaObject::connectSlotsByName(EmotionWindowClass);
    } // setupUi

    void retranslateUi(QWidget *EmotionWindowClass)
    {
        EmotionWindowClass->setWindowTitle(QCoreApplication::translate("EmotionWindowClass", "EmotionWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EmotionWindowClass: public Ui_EmotionWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMOTIONWINDOW_H
