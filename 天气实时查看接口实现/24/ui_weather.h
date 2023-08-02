/********************************************************************************
** Form generated from reading UI file 'weather.ui'
**
** Created by: Qt User Interface Compiler version 5.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEATHER_H
#define UI_WEATHER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Weather
{
public:
    QGridLayout *gridLayout;
    QWidget *widget;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_2;
    QLabel *shiduTitleLb;
    QLabel *pm25TitleLb;
    QLabel *fxTitleLb;
    QLabel *flTitleLb;
    QLabel *fxLb;
    QLabel *shiduLb;
    QLabel *flLb;
    QLabel *pm25Lb;
    QWidget *layoutWidget_2;
    QGridLayout *gridLayout_3;
    QLabel *week1Lb;
    QLabel *date2Lb;
    QLabel *date3Lb;
    QLabel *date0Lb;
    QLabel *week3Lb;
    QLabel *date4Lb;
    QLabel *date1Lb;
    QLabel *week4Lb;
    QLabel *week5Lb;
    QLabel *quality2Lb;
    QLabel *quality1Lb;
    QLabel *quality0Lb;
    QLabel *week2Lb;
    QLabel *curveLb;
    QLabel *low4Lb;
    QLabel *low0Lb;
    QLabel *low1Lb;
    QFrame *hLine_8;
    QLabel *low5Lb;
    QFrame *hLine_4;
    QFrame *hLine_5;
    QFrame *hLine_2;
    QLabel *typeIco3Lb;
    QFrame *vLine_3;
    QLabel *typeIco5Lb;
    QLabel *typeIco4Lb;
    QFrame *hLine_6;
    QFrame *hLine_7;
    QLabel *week0Lb;
    QFrame *hLine_9;
    QFrame *vLine_2;
    QFrame *hLine_3;
    QLabel *date5Lb;
    QLabel *quality4Lb;
    QLabel *quality3Lb;
    QLabel *quality5Lb;
    QLabel *type0Lb;
    QLabel *type3Lb;
    QLabel *type1Lb;
    QLabel *type4Lb;
    QLabel *type2Lb;
    QLabel *type5Lb;
    QLabel *high0Lb;
    QLabel *typeIco2Lb;
    QLabel *typeIco1Lb;
    QLabel *high2Lb;
    QLabel *typeIco0Lb;
    QLabel *high5Lb;
    QLabel *high4Lb;
    QLabel *high3Lb;
    QLabel *low2Lb;
    QLabel *low3Lb;
    QLabel *high1Lb;
    QLabel *temLb;
    QFrame *hLine_1;
    QFrame *hLine_11;
    QFrame *vLine_1;
    QFrame *hLine_10;
    QWidget *layoutWidget_3;
    QGridLayout *gridLayout_4;
    QLabel *gmTitleLb;
    QTextBrowser *ganmaoBrowser;
    QLabel *typeIcoLb;
    QLabel *dateLb;
    QLabel *noticeLb;
    QLabel *sunRiseSetLb;
    QLineEdit *cityLineEdit;
    QLabel *typeLb;
    QToolButton *searchBt;
    QLabel *leafLb;
    QLabel *qualityLb;
    QToolButton *refreshBt;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_5;
    QLabel *label;
    QLabel *cityLb;
    QLabel *label_4;

    void setupUi(QWidget *Weather)
    {
        if (Weather->objectName().isEmpty())
            Weather->setObjectName(QStringLiteral("Weather"));
        Weather->resize(800, 450);
        Weather->setStyleSheet(QStringLiteral(""));
        gridLayout = new QGridLayout(Weather);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(Weather);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setStyleSheet(QLatin1String("QWidget#widget{\n"
"	border-image: url(:/weaUI/weaUI.png);\n"
"	color:rgb(255, 255, 255);\n"
"}\n"
"\n"
"QLabel{\n"
"	font: 25 10pt \"Microsoft YaHei\";\n"
"	border-radius: 4px;\n"
"	background-color: argb(60, 60, 60, 130);\n"
"	color: rgb(255, 255, 255);\n"
"}\n"
"\n"
"Line{\n"
"	background-color: rgb(0, 85, 0);\n"
"}\n"
"\n"
"\n"
"\n"
""));
        layoutWidget = new QWidget(widget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 190, 351, 141));
        gridLayout_2 = new QGridLayout(layoutWidget);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        shiduTitleLb = new QLabel(layoutWidget);
        shiduTitleLb->setObjectName(QStringLiteral("shiduTitleLb"));
        shiduTitleLb->setStyleSheet(QLatin1String("\n"
"color: rgb(255, 255, 255);"));
        shiduTitleLb->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(shiduTitleLb, 0, 0, 1, 1);

        pm25TitleLb = new QLabel(layoutWidget);
        pm25TitleLb->setObjectName(QStringLiteral("pm25TitleLb"));
        pm25TitleLb->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));
        pm25TitleLb->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(pm25TitleLb, 1, 0, 1, 1);

        fxTitleLb = new QLabel(layoutWidget);
        fxTitleLb->setObjectName(QStringLiteral("fxTitleLb"));
        fxTitleLb->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));
        fxTitleLb->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(fxTitleLb, 2, 0, 1, 1);

        flTitleLb = new QLabel(layoutWidget);
        flTitleLb->setObjectName(QStringLiteral("flTitleLb"));
        flTitleLb->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));
        flTitleLb->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(flTitleLb, 3, 0, 1, 1);

        fxLb = new QLabel(layoutWidget);
        fxLb->setObjectName(QStringLiteral("fxLb"));
        fxLb->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(fxLb, 2, 1, 1, 2);

        shiduLb = new QLabel(layoutWidget);
        shiduLb->setObjectName(QStringLiteral("shiduLb"));
        shiduLb->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(shiduLb, 0, 1, 1, 2);

        flLb = new QLabel(layoutWidget);
        flLb->setObjectName(QStringLiteral("flLb"));
        flLb->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(flLb, 3, 1, 1, 2);

        pm25Lb = new QLabel(layoutWidget);
        pm25Lb->setObjectName(QStringLiteral("pm25Lb"));
        pm25Lb->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(pm25Lb, 1, 1, 1, 2);

        layoutWidget_2 = new QWidget(widget);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(370, 50, 421, 391));
        gridLayout_3 = new QGridLayout(layoutWidget_2);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setHorizontalSpacing(2);
        gridLayout_3->setVerticalSpacing(0);
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        week1Lb = new QLabel(layoutWidget_2);
        week1Lb->setObjectName(QStringLiteral("week1Lb"));
        week1Lb->setStyleSheet(QStringLiteral(""));
        week1Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(week1Lb, 1, 2, 1, 1);

        date2Lb = new QLabel(layoutWidget_2);
        date2Lb->setObjectName(QStringLiteral("date2Lb"));
        date2Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(date2Lb, 2, 3, 1, 1);

        date3Lb = new QLabel(layoutWidget_2);
        date3Lb->setObjectName(QStringLiteral("date3Lb"));
        date3Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(date3Lb, 2, 4, 1, 1);

        date0Lb = new QLabel(layoutWidget_2);
        date0Lb->setObjectName(QStringLiteral("date0Lb"));
        date0Lb->setStyleSheet(QStringLiteral("\\"));
        date0Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(date0Lb, 2, 1, 1, 1);

        week3Lb = new QLabel(layoutWidget_2);
        week3Lb->setObjectName(QStringLiteral("week3Lb"));
        week3Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(week3Lb, 1, 4, 1, 1);

        date4Lb = new QLabel(layoutWidget_2);
        date4Lb->setObjectName(QStringLiteral("date4Lb"));
        date4Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(date4Lb, 2, 5, 1, 1);

        date1Lb = new QLabel(layoutWidget_2);
        date1Lb->setObjectName(QStringLiteral("date1Lb"));
        date1Lb->setStyleSheet(QStringLiteral(""));
        date1Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(date1Lb, 2, 2, 1, 1);

        week4Lb = new QLabel(layoutWidget_2);
        week4Lb->setObjectName(QStringLiteral("week4Lb"));
        week4Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(week4Lb, 1, 5, 1, 1);

        week5Lb = new QLabel(layoutWidget_2);
        week5Lb->setObjectName(QStringLiteral("week5Lb"));
        week5Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(week5Lb, 1, 6, 1, 1);

        quality2Lb = new QLabel(layoutWidget_2);
        quality2Lb->setObjectName(QStringLiteral("quality2Lb"));
        quality2Lb->setStyleSheet(QStringLiteral("color: rgb(255, 170, 0);"));
        quality2Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(quality2Lb, 4, 3, 1, 1);

        quality1Lb = new QLabel(layoutWidget_2);
        quality1Lb->setObjectName(QStringLiteral("quality1Lb"));
        quality1Lb->setStyleSheet(QStringLiteral("color: rgb(255, 255, 0);"));
        quality1Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(quality1Lb, 4, 2, 1, 1);

        quality0Lb = new QLabel(layoutWidget_2);
        quality0Lb->setObjectName(QStringLiteral("quality0Lb"));
        quality0Lb->setStyleSheet(QStringLiteral("color: rgb(0, 170, 0);"));
        quality0Lb->setFrameShape(QFrame::NoFrame);
        quality0Lb->setFrameShadow(QFrame::Sunken);
        quality0Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(quality0Lb, 4, 1, 1, 1);

        week2Lb = new QLabel(layoutWidget_2);
        week2Lb->setObjectName(QStringLiteral("week2Lb"));
        week2Lb->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
"font: 25 10pt \"Microsoft YaHei\";"));
        week2Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(week2Lb, 1, 3, 1, 1);

        curveLb = new QLabel(layoutWidget_2);
        curveLb->setObjectName(QStringLiteral("curveLb"));
        curveLb->setStyleSheet(QStringLiteral(""));
        curveLb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(curveLb, 9, 1, 3, 6);

        low4Lb = new QLabel(layoutWidget_2);
        low4Lb->setObjectName(QStringLiteral("low4Lb"));
        low4Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(low4Lb, 12, 5, 1, 1);

        low0Lb = new QLabel(layoutWidget_2);
        low0Lb->setObjectName(QStringLiteral("low0Lb"));
        low0Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(low0Lb, 12, 1, 1, 1);

        low1Lb = new QLabel(layoutWidget_2);
        low1Lb->setObjectName(QStringLiteral("low1Lb"));
        low1Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(low1Lb, 12, 2, 1, 1);

        hLine_8 = new QFrame(layoutWidget_2);
        hLine_8->setObjectName(QStringLiteral("hLine_8"));
        hLine_8->setMaximumSize(QSize(16777215, 1));
        hLine_8->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 0);"));
        hLine_8->setLineWidth(0);
        hLine_8->setFrameShape(QFrame::HLine);
        hLine_8->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(hLine_8, 3, 6, 1, 1);

        low5Lb = new QLabel(layoutWidget_2);
        low5Lb->setObjectName(QStringLiteral("low5Lb"));
        low5Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(low5Lb, 12, 6, 1, 1);

        hLine_4 = new QFrame(layoutWidget_2);
        hLine_4->setObjectName(QStringLiteral("hLine_4"));
        hLine_4->setMinimumSize(QSize(0, 0));
        hLine_4->setMaximumSize(QSize(16777215, 1));
        hLine_4->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 0);"));
        hLine_4->setLineWidth(0);
        hLine_4->setFrameShape(QFrame::HLine);
        hLine_4->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(hLine_4, 3, 2, 1, 1);

        hLine_5 = new QFrame(layoutWidget_2);
        hLine_5->setObjectName(QStringLiteral("hLine_5"));
        hLine_5->setMaximumSize(QSize(16777215, 1));
        hLine_5->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 0);"));
        hLine_5->setLineWidth(0);
        hLine_5->setFrameShape(QFrame::HLine);
        hLine_5->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(hLine_5, 3, 3, 1, 1);

        hLine_2 = new QFrame(layoutWidget_2);
        hLine_2->setObjectName(QStringLiteral("hLine_2"));
        hLine_2->setMaximumSize(QSize(16777215, 1));
        hLine_2->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 0);"));
        hLine_2->setLineWidth(0);
        hLine_2->setFrameShape(QFrame::HLine);
        hLine_2->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(hLine_2, 0, 0, 1, 8);

        typeIco3Lb = new QLabel(layoutWidget_2);
        typeIco3Lb->setObjectName(QStringLiteral("typeIco3Lb"));
        typeIco3Lb->setStyleSheet(QString::fromUtf8("image: url(:/night/night/\351\233\276.png);"));
        typeIco3Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(typeIco3Lb, 6, 4, 2, 1);

        vLine_3 = new QFrame(layoutWidget_2);
        vLine_3->setObjectName(QStringLiteral("vLine_3"));
        vLine_3->setMaximumSize(QSize(1, 16777215));
        vLine_3->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 0);"));
        vLine_3->setLineWidth(0);
        vLine_3->setFrameShape(QFrame::VLine);
        vLine_3->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(vLine_3, 1, 7, 13, 1);

        typeIco5Lb = new QLabel(layoutWidget_2);
        typeIco5Lb->setObjectName(QStringLiteral("typeIco5Lb"));
        typeIco5Lb->setStyleSheet(QString::fromUtf8("image: url(:/day/day/\345\274\272\346\262\231\345\260\230\346\232\264.png);"));
        typeIco5Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(typeIco5Lb, 6, 6, 2, 1);

        typeIco4Lb = new QLabel(layoutWidget_2);
        typeIco4Lb->setObjectName(QStringLiteral("typeIco4Lb"));
        typeIco4Lb->setStyleSheet(QString::fromUtf8("image: url(:/day/day/\351\230\265\351\233\250.png);"));
        typeIco4Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(typeIco4Lb, 6, 5, 2, 1);

        hLine_6 = new QFrame(layoutWidget_2);
        hLine_6->setObjectName(QStringLiteral("hLine_6"));
        hLine_6->setMaximumSize(QSize(16777215, 1));
        hLine_6->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 0);"));
        hLine_6->setLineWidth(0);
        hLine_6->setFrameShape(QFrame::HLine);
        hLine_6->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(hLine_6, 3, 4, 1, 1);

        hLine_7 = new QFrame(layoutWidget_2);
        hLine_7->setObjectName(QStringLiteral("hLine_7"));
        hLine_7->setMaximumSize(QSize(16777215, 1));
        hLine_7->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 0);"));
        hLine_7->setLineWidth(0);
        hLine_7->setFrameShape(QFrame::HLine);
        hLine_7->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(hLine_7, 3, 5, 1, 1);

        week0Lb = new QLabel(layoutWidget_2);
        week0Lb->setObjectName(QStringLiteral("week0Lb"));
        week0Lb->setStyleSheet(QStringLiteral(""));
        week0Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(week0Lb, 1, 1, 1, 1);

        hLine_9 = new QFrame(layoutWidget_2);
        hLine_9->setObjectName(QStringLiteral("hLine_9"));
        hLine_9->setMaximumSize(QSize(16777215, 1));
        hLine_9->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 0);"));
        hLine_9->setLineWidth(0);
        hLine_9->setFrameShape(QFrame::HLine);
        hLine_9->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(hLine_9, 13, 1, 1, 6);

        vLine_2 = new QFrame(layoutWidget_2);
        vLine_2->setObjectName(QStringLiteral("vLine_2"));
        vLine_2->setMaximumSize(QSize(1, 16777215));
        vLine_2->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 0);"));
        vLine_2->setLineWidth(0);
        vLine_2->setFrameShape(QFrame::VLine);
        vLine_2->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(vLine_2, 1, 0, 13, 1);

        hLine_3 = new QFrame(layoutWidget_2);
        hLine_3->setObjectName(QStringLiteral("hLine_3"));
        hLine_3->setMinimumSize(QSize(0, 0));
        hLine_3->setMaximumSize(QSize(16777215, 1));
        hLine_3->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 0);"));
        hLine_3->setLineWidth(0);
        hLine_3->setFrameShape(QFrame::HLine);
        hLine_3->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(hLine_3, 3, 1, 1, 1);

        date5Lb = new QLabel(layoutWidget_2);
        date5Lb->setObjectName(QStringLiteral("date5Lb"));
        date5Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(date5Lb, 2, 6, 1, 1);

        quality4Lb = new QLabel(layoutWidget_2);
        quality4Lb->setObjectName(QStringLiteral("quality4Lb"));
        quality4Lb->setStyleSheet(QStringLiteral("color: rgb(170, 0, 0);"));
        quality4Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(quality4Lb, 4, 5, 1, 1);

        quality3Lb = new QLabel(layoutWidget_2);
        quality3Lb->setObjectName(QStringLiteral("quality3Lb"));
        quality3Lb->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        quality3Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(quality3Lb, 4, 4, 1, 1);

        quality5Lb = new QLabel(layoutWidget_2);
        quality5Lb->setObjectName(QStringLiteral("quality5Lb"));
        quality5Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(quality5Lb, 4, 6, 1, 1);

        type0Lb = new QLabel(layoutWidget_2);
        type0Lb->setObjectName(QStringLiteral("type0Lb"));
        type0Lb->setStyleSheet(QStringLiteral(""));
        type0Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(type0Lb, 5, 1, 1, 1);

        type3Lb = new QLabel(layoutWidget_2);
        type3Lb->setObjectName(QStringLiteral("type3Lb"));
        type3Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(type3Lb, 5, 4, 1, 1);

        type1Lb = new QLabel(layoutWidget_2);
        type1Lb->setObjectName(QStringLiteral("type1Lb"));
        type1Lb->setStyleSheet(QStringLiteral(""));
        type1Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(type1Lb, 5, 2, 1, 1);

        type4Lb = new QLabel(layoutWidget_2);
        type4Lb->setObjectName(QStringLiteral("type4Lb"));
        type4Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(type4Lb, 5, 5, 1, 1);

        type2Lb = new QLabel(layoutWidget_2);
        type2Lb->setObjectName(QStringLiteral("type2Lb"));
        type2Lb->setStyleSheet(QStringLiteral(""));
        type2Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(type2Lb, 5, 3, 1, 1);

        type5Lb = new QLabel(layoutWidget_2);
        type5Lb->setObjectName(QStringLiteral("type5Lb"));
        type5Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(type5Lb, 5, 6, 1, 1);

        high0Lb = new QLabel(layoutWidget_2);
        high0Lb->setObjectName(QStringLiteral("high0Lb"));
        high0Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(high0Lb, 8, 1, 1, 1);

        typeIco2Lb = new QLabel(layoutWidget_2);
        typeIco2Lb->setObjectName(QStringLiteral("typeIco2Lb"));
        typeIco2Lb->setStyleSheet(QString::fromUtf8("image: url(:/day/day/\346\231\264.png);"));
        typeIco2Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(typeIco2Lb, 6, 3, 2, 1);

        typeIco1Lb = new QLabel(layoutWidget_2);
        typeIco1Lb->setObjectName(QStringLiteral("typeIco1Lb"));
        typeIco1Lb->setStyleSheet(QString::fromUtf8("image: url(:/day/day/\345\244\247\346\232\264\351\233\250\345\210\260\347\211\271\345\244\247\346\232\264\351\233\250.png);"));
        typeIco1Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(typeIco1Lb, 6, 2, 2, 1);

        high2Lb = new QLabel(layoutWidget_2);
        high2Lb->setObjectName(QStringLiteral("high2Lb"));
        high2Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(high2Lb, 8, 3, 1, 1);

        typeIco0Lb = new QLabel(layoutWidget_2);
        typeIco0Lb->setObjectName(QStringLiteral("typeIco0Lb"));
        typeIco0Lb->setStyleSheet(QString::fromUtf8("image: url(:/day/day/\345\260\217\351\233\250.png);\n"
""));
        typeIco0Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(typeIco0Lb, 6, 1, 2, 1);

        high5Lb = new QLabel(layoutWidget_2);
        high5Lb->setObjectName(QStringLiteral("high5Lb"));
        high5Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(high5Lb, 8, 6, 1, 1);

        high4Lb = new QLabel(layoutWidget_2);
        high4Lb->setObjectName(QStringLiteral("high4Lb"));
        high4Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(high4Lb, 8, 5, 1, 1);

        high3Lb = new QLabel(layoutWidget_2);
        high3Lb->setObjectName(QStringLiteral("high3Lb"));
        high3Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(high3Lb, 8, 4, 1, 1);

        low2Lb = new QLabel(layoutWidget_2);
        low2Lb->setObjectName(QStringLiteral("low2Lb"));
        low2Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(low2Lb, 12, 3, 1, 1);

        low3Lb = new QLabel(layoutWidget_2);
        low3Lb->setObjectName(QStringLiteral("low3Lb"));
        low3Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(low3Lb, 12, 4, 1, 1);

        high1Lb = new QLabel(layoutWidget_2);
        high1Lb->setObjectName(QStringLiteral("high1Lb"));
        high1Lb->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(high1Lb, 8, 2, 1, 1);

        temLb = new QLabel(widget);
        temLb->setObjectName(QStringLiteral("temLb"));
        temLb->setGeometry(QRect(10, 50, 241, 101));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(50);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        temLb->setFont(font);
        temLb->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
"font: 50pt \"Arial\";"));
        temLb->setAlignment(Qt::AlignCenter);
        hLine_1 = new QFrame(widget);
        hLine_1->setObjectName(QStringLiteral("hLine_1"));
        hLine_1->setGeometry(QRect(10, 45, 781, 1));
        hLine_1->setMinimumSize(QSize(0, 0));
        hLine_1->setMaximumSize(QSize(100000, 1));
        hLine_1->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 0);"));
        hLine_1->setLineWidth(0);
        hLine_1->setMidLineWidth(0);
        hLine_1->setFrameShape(QFrame::HLine);
        hLine_1->setFrameShadow(QFrame::Sunken);
        hLine_11 = new QFrame(widget);
        hLine_11->setObjectName(QStringLiteral("hLine_11"));
        hLine_11->setGeometry(QRect(10, 335, 351, 1));
        hLine_11->setMinimumSize(QSize(0, 0));
        hLine_11->setMaximumSize(QSize(16777215, 1));
        hLine_11->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 0);"));
        hLine_11->setLineWidth(0);
        hLine_11->setFrameShape(QFrame::HLine);
        hLine_11->setFrameShadow(QFrame::Sunken);
        vLine_1 = new QFrame(widget);
        vLine_1->setObjectName(QStringLiteral("vLine_1"));
        vLine_1->setGeometry(QRect(205, 340, 1, 101));
        vLine_1->setMinimumSize(QSize(0, 0));
        vLine_1->setMaximumSize(QSize(1, 16777215));
        vLine_1->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 0);"));
        vLine_1->setLineWidth(0);
        vLine_1->setFrameShape(QFrame::VLine);
        vLine_1->setFrameShadow(QFrame::Sunken);
        hLine_10 = new QFrame(widget);
        hLine_10->setObjectName(QStringLiteral("hLine_10"));
        hLine_10->setGeometry(QRect(10, 183, 351, 1));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(hLine_10->sizePolicy().hasHeightForWidth());
        hLine_10->setSizePolicy(sizePolicy);
        hLine_10->setMinimumSize(QSize(0, 0));
        hLine_10->setMaximumSize(QSize(16777215, 1));
        hLine_10->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 0);"));
        hLine_10->setLineWidth(0);
        hLine_10->setFrameShape(QFrame::HLine);
        hLine_10->setFrameShadow(QFrame::Sunken);
        layoutWidget_3 = new QWidget(widget);
        layoutWidget_3->setObjectName(QStringLiteral("layoutWidget_3"));
        layoutWidget_3->setGeometry(QRect(10, 340, 191, 101));
        gridLayout_4 = new QGridLayout(layoutWidget_3);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setHorizontalSpacing(6);
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        gmTitleLb = new QLabel(layoutWidget_3);
        gmTitleLb->setObjectName(QStringLiteral("gmTitleLb"));
        gmTitleLb->setMinimumSize(QSize(40, 0));
        gmTitleLb->setSizeIncrement(QSize(30, 0));
        QFont font1;
        font1.setFamily(QStringLiteral("Microsoft YaHei"));
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(3);
        gmTitleLb->setFont(font1);
        gmTitleLb->setStyleSheet(QLatin1String("background-color: argb(60, 60, 60, 130);\n"
"border-radius: 3px;\n"
"color: rgb(255, 255, 255);"));
        gmTitleLb->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(gmTitleLb, 0, 0, 1, 1);

        ganmaoBrowser = new QTextBrowser(layoutWidget_3);
        ganmaoBrowser->setObjectName(QStringLiteral("ganmaoBrowser"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(ganmaoBrowser->sizePolicy().hasHeightForWidth());
        ganmaoBrowser->setSizePolicy(sizePolicy1);
        ganmaoBrowser->setMinimumSize(QSize(22, 0));
        QFont font2;
        font2.setFamily(QStringLiteral("Arial"));
        font2.setPointSize(9);
        font2.setBold(false);
        font2.setItalic(false);
        font2.setWeight(50);
        ganmaoBrowser->setFont(font2);
        ganmaoBrowser->setAutoFillBackground(true);
        ganmaoBrowser->setStyleSheet(QLatin1String("border-radius: 5px;\n"
"color: rgb(255, 255, 255);\n"
"background-color: argb(255, 255, 255, 0);\n"
"font: 9pt \"Arial\";"));
        ganmaoBrowser->setFrameShape(QFrame::NoFrame);

        gridLayout_4->addWidget(ganmaoBrowser, 0, 1, 1, 1);

        typeIcoLb = new QLabel(widget);
        typeIcoLb->setObjectName(QStringLiteral("typeIcoLb"));
        typeIcoLb->setGeometry(QRect(260, 63, 101, 101));
        typeIcoLb->setStyleSheet(QString::fromUtf8("border-image: url(:/day/day/\346\231\264.png);\n"
"background-color: argb(60, 60, 60, 0);\n"
"\n"
""));
        dateLb = new QLabel(widget);
        dateLb->setObjectName(QStringLiteral("dateLb"));
        dateLb->setGeometry(QRect(190, 3, 421, 41));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Adobe \351\273\221\344\275\223 Std R"));
        font3.setPointSize(20);
        font3.setBold(false);
        font3.setItalic(false);
        font3.setWeight(50);
        dateLb->setFont(font3);
        dateLb->setStyleSheet(QString::fromUtf8("font: 20pt \"Adobe \351\273\221\344\275\223 Std R\";\n"
"color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255, 0);"));
        dateLb->setAlignment(Qt::AlignCenter);
        noticeLb = new QLabel(widget);
        noticeLb->setObjectName(QStringLiteral("noticeLb"));
        noticeLb->setGeometry(QRect(10, 160, 351, 21));
        noticeLb->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
"background-color: argb(60, 60, 60, 0);"));
        noticeLb->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        sunRiseSetLb = new QLabel(widget);
        sunRiseSetLb->setObjectName(QStringLiteral("sunRiseSetLb"));
        sunRiseSetLb->setGeometry(QRect(210, 340, 150, 100));
        sunRiseSetLb->setStyleSheet(QStringLiteral(""));
        cityLineEdit = new QLineEdit(widget);
        cityLineEdit->setObjectName(QStringLiteral("cityLineEdit"));
        cityLineEdit->setGeometry(QRect(575, 10, 171, 31));
        cityLineEdit->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));
        cityLineEdit->setFrame(true);
        typeLb = new QLabel(widget);
        typeLb->setObjectName(QStringLiteral("typeLb"));
        typeLb->setGeometry(QRect(140, 120, 101, 31));
        typeLb->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
"background-color: argb(60, 60, 60, 0);"));
        typeLb->setAlignment(Qt::AlignCenter);
        searchBt = new QToolButton(widget);
        searchBt->setObjectName(QStringLiteral("searchBt"));
        searchBt->setGeometry(QRect(722, 15, 20, 20));
        searchBt->setCursor(QCursor(Qt::PointingHandCursor));
        searchBt->setStyleSheet(QStringLiteral("border-image: url(:/weatherIco/search.ico);"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/weatherIco/search.ico"), QSize(), QIcon::Normal, QIcon::Off);
        searchBt->setIcon(icon);
        searchBt->setIconSize(QSize(20, 20));
        searchBt->setPopupMode(QToolButton::DelayedPopup);
        leafLb = new QLabel(widget);
        leafLb->setObjectName(QStringLiteral("leafLb"));
        leafLb->setGeometry(QRect(18, 58, 20, 20));
        leafLb->setStyleSheet(QLatin1String("image: url(:/weatherIco/leaf.ico);\n"
"background-color: argb(255, 255, 255, 0);"));
        qualityLb = new QLabel(widget);
        qualityLb->setObjectName(QStringLiteral("qualityLb"));
        qualityLb->setGeometry(QRect(38, 58, 56, 20));
        qualityLb->setFont(font1);
        qualityLb->setStyleSheet(QStringLiteral("background-color: argb(255, 255, 255, 0);"));
        refreshBt = new QToolButton(widget);
        refreshBt->setObjectName(QStringLiteral("refreshBt"));
        refreshBt->setGeometry(QRect(753, 10, 37, 31));
        refreshBt->setCursor(QCursor(Qt::PointingHandCursor));
        refreshBt->setToolTipDuration(-1);
        refreshBt->setStyleSheet(QLatin1String("image: url(:/weatherIco/refresh.ico);\n"
"background-color: argb(255, 255, 255, 0);"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/weatherIco/refresh.ico"), QSize(), QIcon::Normal, QIcon::Off);
        refreshBt->setIcon(icon1);
        refreshBt->setIconSize(QSize(31, 31));
        layoutWidget1 = new QWidget(widget);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(140, 50, 101, 30));
        gridLayout_5 = new QGridLayout(layoutWidget1);
        gridLayout_5->setSpacing(0);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget1);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);
        label->setMinimumSize(QSize(0, 25));
        label->setMaximumSize(QSize(16777215, 25));
        label->setStyleSheet(QLatin1String("background-color: argb(60, 60, 60, 0);\n"
"image: url(:/weatherIco/location.ico);"));

        gridLayout_5->addWidget(label, 0, 0, 1, 1);

        cityLb = new QLabel(layoutWidget1);
        cityLb->setObjectName(QStringLiteral("cityLb"));
        QFont font4;
        font4.setFamily(QStringLiteral("Microsoft YaHei UI"));
        font4.setPointSize(16);
        font4.setBold(false);
        font4.setItalic(false);
        font4.setWeight(50);
        cityLb->setFont(font4);
        cityLb->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
"font: 16pt \"Microsoft YaHei UI\";\n"
"background-color: rgba(60, 60, 60, 0);"));
        cityLb->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(cityLb, 0, 1, 1, 1);

        label_4 = new QLabel(widget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(80, 10, 171, 31));
        label_4->setStyleSheet(QString::fromUtf8("font: 20pt \"\345\215\216\346\226\207\347\220\245\347\217\200\";\n"
"background-color: rgba(255, 255, 255, 0);\n"
"color: rgb(85, 255, 0);"));
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(widget, 0, 0, 1, 1);


        retranslateUi(Weather);

        QMetaObject::connectSlotsByName(Weather);
    } // setupUi

    void retranslateUi(QWidget *Weather)
    {
        Weather->setWindowTitle(QApplication::translate("Weather", "Form", Q_NULLPTR));
        shiduTitleLb->setText(QApplication::translate("Weather", "\346\271\277\345\272\246", Q_NULLPTR));
        pm25TitleLb->setText(QApplication::translate("Weather", "PM2.5", Q_NULLPTR));
        fxTitleLb->setText(QApplication::translate("Weather", "\351\243\216\345\220\221", Q_NULLPTR));
        flTitleLb->setText(QApplication::translate("Weather", "\351\243\216\345\212\233", Q_NULLPTR));
        fxLb->setText(QApplication::translate("Weather", "\344\270\234\345\215\227\351\243\216", Q_NULLPTR));
        shiduLb->setText(QApplication::translate("Weather", "86%", Q_NULLPTR));
        flLb->setText(QApplication::translate("Weather", "<3\347\272\247", Q_NULLPTR));
        pm25Lb->setText(QApplication::translate("Weather", "74", Q_NULLPTR));
        week1Lb->setText(QApplication::translate("Weather", "\344\273\212\345\244\251", Q_NULLPTR));
        date2Lb->setText(QApplication::translate("Weather", "01/03", Q_NULLPTR));
        date3Lb->setText(QApplication::translate("Weather", "01/04", Q_NULLPTR));
        date0Lb->setText(QApplication::translate("Weather", "01/01", Q_NULLPTR));
        week3Lb->setText(QApplication::translate("Weather", "\345\221\250\345\233\233", Q_NULLPTR));
        date4Lb->setText(QApplication::translate("Weather", "01/05", Q_NULLPTR));
        date1Lb->setText(QApplication::translate("Weather", "01/02", Q_NULLPTR));
        week4Lb->setText(QApplication::translate("Weather", "\345\221\250\344\272\224", Q_NULLPTR));
        week5Lb->setText(QApplication::translate("Weather", "\345\221\250\345\205\255", Q_NULLPTR));
        quality2Lb->setText(QApplication::translate("Weather", "\350\275\273\345\272\246\346\261\241\346\237\223", Q_NULLPTR));
        quality1Lb->setText(QApplication::translate("Weather", "\350\211\257\345\245\275", Q_NULLPTR));
        quality0Lb->setText(QApplication::translate("Weather", "\344\274\230\350\264\250", Q_NULLPTR));
        week2Lb->setText(QApplication::translate("Weather", "\345\221\250\344\270\211", Q_NULLPTR));
        curveLb->setText(QString());
        low4Lb->setText(QApplication::translate("Weather", "6\302\260", Q_NULLPTR));
        low0Lb->setText(QApplication::translate("Weather", "6\302\260", Q_NULLPTR));
        low1Lb->setText(QApplication::translate("Weather", "6\302\260", Q_NULLPTR));
        low5Lb->setText(QApplication::translate("Weather", "6\302\260", Q_NULLPTR));
        typeIco3Lb->setText(QString());
        typeIco5Lb->setText(QString());
        typeIco4Lb->setText(QString());
        week0Lb->setText(QApplication::translate("Weather", "\346\230\250\345\244\251", Q_NULLPTR));
        date5Lb->setText(QApplication::translate("Weather", "01/06", Q_NULLPTR));
        quality4Lb->setText(QApplication::translate("Weather", "\344\270\245\351\207\215\346\261\241\346\237\223", Q_NULLPTR));
        quality3Lb->setText(QApplication::translate("Weather", "\344\270\255\345\272\246\346\261\241\346\237\223", Q_NULLPTR));
        quality5Lb->setText(QApplication::translate("Weather", "\350\211\257", Q_NULLPTR));
        type0Lb->setText(QApplication::translate("Weather", "\351\230\264", Q_NULLPTR));
        type3Lb->setText(QApplication::translate("Weather", "\351\230\264", Q_NULLPTR));
        type1Lb->setText(QApplication::translate("Weather", "\351\230\264", Q_NULLPTR));
        type4Lb->setText(QApplication::translate("Weather", "\351\230\264", Q_NULLPTR));
        type2Lb->setText(QApplication::translate("Weather", "\351\230\264", Q_NULLPTR));
        type5Lb->setText(QApplication::translate("Weather", "\351\230\264", Q_NULLPTR));
        high0Lb->setText(QApplication::translate("Weather", "9\302\260", Q_NULLPTR));
        typeIco2Lb->setText(QString());
        typeIco1Lb->setText(QString());
        high2Lb->setText(QApplication::translate("Weather", "9\302\260", Q_NULLPTR));
        typeIco0Lb->setText(QString());
        high5Lb->setText(QApplication::translate("Weather", "9\302\260", Q_NULLPTR));
        high4Lb->setText(QApplication::translate("Weather", "9\302\260", Q_NULLPTR));
        high3Lb->setText(QApplication::translate("Weather", "9\302\260", Q_NULLPTR));
        low2Lb->setText(QApplication::translate("Weather", "6\302\260", Q_NULLPTR));
        low3Lb->setText(QApplication::translate("Weather", "6\302\260", Q_NULLPTR));
        high1Lb->setText(QApplication::translate("Weather", "9\302\260", Q_NULLPTR));
        temLb->setText(QApplication::translate("Weather", "9\302\260", Q_NULLPTR));
        gmTitleLb->setText(QApplication::translate("Weather", "\346\204\237\n"
"\345\206\222\n"
"\346\214\207\n"
"\346\225\260", Q_NULLPTR));
        ganmaoBrowser->setHtml(QApplication::translate("Weather", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Arial'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun';\">\346\236\201\345\260\221\346\225\260\346\225\217\346\204\237\344\272\272\347\276\244\345\272\224\345\207\217\345\260\221\346\210\267\345\244\226\346\264\273\345\212\250</span></p></body></html>", Q_NULLPTR));
        typeIcoLb->setText(QString());
        dateLb->setText(QApplication::translate("Weather", "2017-01-02", Q_NULLPTR));
        noticeLb->setText(QApplication::translate("Weather", "\351\230\264\345\244\251\346\230\257\345\275\251\350\231\271\347\232\204\345\211\215\346\234\237\344\271\213\346\231\257", Q_NULLPTR));
        sunRiseSetLb->setText(QString());
        cityLineEdit->setPlaceholderText(QApplication::translate("Weather", "\345\237\216\345\270\202", Q_NULLPTR));
        typeLb->setText(QApplication::translate("Weather", "\346\231\264", Q_NULLPTR));
        searchBt->setText(QString());
#ifndef QT_NO_SHORTCUT
        searchBt->setShortcut(QApplication::translate("Weather", "Return", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        leafLb->setText(QString());
        qualityLb->setText(QApplication::translate("Weather", "\344\274\230", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        refreshBt->setToolTip(QApplication::translate("Weather", "\345\210\267\346\226\260", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        refreshBt->setText(QString());
        label->setText(QString());
        cityLb->setText(QApplication::translate("Weather", "\351\207\215\345\272\206", Q_NULLPTR));
        label_4->setText(QApplication::translate("Weather", "\345\244\251\346\260\224\351\242\204\346\212\245", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Weather: public Ui_Weather {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEATHER_H
