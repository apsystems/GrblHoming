/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Mon 20. Feb 15:48:27 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *lblPort;
    QComboBox *cmbPort;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QLCDNumber *lcdNumberX;
    QLCDNumber *lcdNumberY;
    QLCDNumber *lcdNumberZ;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_3;
    QLabel *lblX;
    QLabel *lblY;
    QLabel *lblZ;
    QListWidget *listWidget;
    QGroupBox *groupBoxAxis;
    QWidget *layoutWidget3;
    QGridLayout *gridLayout;
    QPushButton *IncYBtn;
    QSpacerItem *verticalSpacer;
    QPushButton *DecXBtn;
    QPushButton *DecYBtn;
    QPushButton *IncXBtn;
    QWidget *layoutWidget4;
    QVBoxLayout *RadioButtons;
    QPushButton *IncZBtn;
    QLabel *lblZJog;
    QPushButton *DecZBtn;
    QWidget *layoutWidget5;
    QGridLayout *gridLayout_2;
    QLabel *lblStep;
    QComboBox *comboStep;
    QGroupBox *groupBoxSend;
    QWidget *layoutWidget6;
    QGridLayout *gridLayout_3;
    QPushButton *openFile;
    QLineEdit *filePath;
    QPushButton *Begin;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *Stop;
    QWidget *layoutWidget7;
    QVBoxLayout *verticalLayout_4;
    QRadioButton *rbutSend;
    QRadioButton *rbutAdj;
    QCheckBox *returnHome;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(670, 400);
        MainWindow->setMaximumSize(QSize(10000000, 10000000));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(40, 20, 91, 48));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lblPort = new QLabel(layoutWidget);
        lblPort->setObjectName(QString::fromUtf8("lblPort"));

        verticalLayout->addWidget(lblPort);

        cmbPort = new QComboBox(layoutWidget);
        cmbPort->setObjectName(QString::fromUtf8("cmbPort"));

        verticalLayout->addWidget(cmbPort);

        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(450, 20, 171, 161));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        lcdNumberX = new QLCDNumber(layoutWidget1);
        lcdNumberX->setObjectName(QString::fromUtf8("lcdNumberX"));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush);
        lcdNumberX->setPalette(palette);
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        font.setKerning(true);
        lcdNumberX->setFont(font);
        lcdNumberX->setSmallDecimalPoint(true);
        lcdNumberX->setNumDigits(7);
        lcdNumberX->setSegmentStyle(QLCDNumber::Filled);

        verticalLayout_2->addWidget(lcdNumberX);

        lcdNumberY = new QLCDNumber(layoutWidget1);
        lcdNumberY->setObjectName(QString::fromUtf8("lcdNumberY"));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::Light, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush);
        lcdNumberY->setPalette(palette1);
        lcdNumberY->setFont(font);
        lcdNumberY->setSmallDecimalPoint(true);
        lcdNumberY->setNumDigits(7);
        lcdNumberY->setSegmentStyle(QLCDNumber::Filled);

        verticalLayout_2->addWidget(lcdNumberY);

        lcdNumberZ = new QLCDNumber(layoutWidget1);
        lcdNumberZ->setObjectName(QString::fromUtf8("lcdNumberZ"));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::Light, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Light, brush);
        lcdNumberZ->setPalette(palette2);
        lcdNumberZ->setFont(font);
        lcdNumberZ->setSmallDecimalPoint(true);
        lcdNumberZ->setNumDigits(7);
        lcdNumberZ->setDigitCount(7);
        lcdNumberZ->setSegmentStyle(QLCDNumber::Filled);

        verticalLayout_2->addWidget(lcdNumberZ);

        layoutWidget2 = new QWidget(centralWidget);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(422, 18, 21, 161));
        verticalLayout_3 = new QVBoxLayout(layoutWidget2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        lblX = new QLabel(layoutWidget2);
        lblX->setObjectName(QString::fromUtf8("lblX"));
        QFont font1;
        font1.setPointSize(18);
        lblX->setFont(font1);
        lblX->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(lblX);

        lblY = new QLabel(layoutWidget2);
        lblY->setObjectName(QString::fromUtf8("lblY"));
        lblY->setFont(font1);
        lblY->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(lblY);

        lblZ = new QLabel(layoutWidget2);
        lblZ->setObjectName(QString::fromUtf8("lblZ"));
        lblZ->setFont(font1);
        lblZ->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(lblZ);

        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(20, 170, 371, 161));
        groupBoxAxis = new QGroupBox(centralWidget);
        groupBoxAxis->setObjectName(QString::fromUtf8("groupBoxAxis"));
        groupBoxAxis->setEnabled(false);
        groupBoxAxis->setGeometry(QRect(410, 190, 251, 191));
        layoutWidget3 = new QWidget(groupBoxAxis);
        layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(10, 20, 161, 111));
        gridLayout = new QGridLayout(layoutWidget3);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        IncYBtn = new QPushButton(layoutWidget3);
        IncYBtn->setObjectName(QString::fromUtf8("IncYBtn"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img/up.gif"), QSize(), QIcon::Normal, QIcon::Off);
        IncYBtn->setIcon(icon);
        IncYBtn->setIconSize(QSize(24, 24));

        gridLayout->addWidget(IncYBtn, 0, 1, 1, 1);

        verticalSpacer = new QSpacerItem(18, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 1, 1, 1);

        DecXBtn = new QPushButton(layoutWidget3);
        DecXBtn->setObjectName(QString::fromUtf8("DecXBtn"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/img/left.gif"), QSize(), QIcon::Normal, QIcon::Off);
        DecXBtn->setIcon(icon1);
        DecXBtn->setIconSize(QSize(24, 24));

        gridLayout->addWidget(DecXBtn, 2, 0, 1, 1);

        DecYBtn = new QPushButton(layoutWidget3);
        DecYBtn->setObjectName(QString::fromUtf8("DecYBtn"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/img/down.gif"), QSize(), QIcon::Normal, QIcon::Off);
        DecYBtn->setIcon(icon2);
        DecYBtn->setIconSize(QSize(24, 24));

        gridLayout->addWidget(DecYBtn, 2, 1, 1, 1);

        IncXBtn = new QPushButton(layoutWidget3);
        IncXBtn->setObjectName(QString::fromUtf8("IncXBtn"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/img/right.gif"), QSize(), QIcon::Normal, QIcon::Off);
        IncXBtn->setIcon(icon3);
        IncXBtn->setIconSize(QSize(24, 24));

        gridLayout->addWidget(IncXBtn, 2, 2, 1, 1);

        layoutWidget4 = new QWidget(groupBoxAxis);
        layoutWidget4->setObjectName(QString::fromUtf8("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(190, 20, 51, 111));
        RadioButtons = new QVBoxLayout(layoutWidget4);
        RadioButtons->setSpacing(6);
        RadioButtons->setContentsMargins(11, 11, 11, 11);
        RadioButtons->setObjectName(QString::fromUtf8("RadioButtons"));
        RadioButtons->setContentsMargins(0, 0, 0, 0);
        IncZBtn = new QPushButton(layoutWidget4);
        IncZBtn->setObjectName(QString::fromUtf8("IncZBtn"));
        IncZBtn->setIcon(icon);
        IncZBtn->setIconSize(QSize(24, 24));

        RadioButtons->addWidget(IncZBtn);

        lblZJog = new QLabel(layoutWidget4);
        lblZJog->setObjectName(QString::fromUtf8("lblZJog"));
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(true);
        font2.setWeight(75);
        lblZJog->setFont(font2);
        lblZJog->setAlignment(Qt::AlignCenter);

        RadioButtons->addWidget(lblZJog);

        DecZBtn = new QPushButton(layoutWidget4);
        DecZBtn->setObjectName(QString::fromUtf8("DecZBtn"));
        DecZBtn->setIcon(icon2);
        DecZBtn->setIconSize(QSize(24, 24));

        RadioButtons->addWidget(DecZBtn);

        layoutWidget5 = new QWidget(groupBoxAxis);
        layoutWidget5->setObjectName(QString::fromUtf8("layoutWidget5"));
        layoutWidget5->setGeometry(QRect(30, 150, 191, 31));
        gridLayout_2 = new QGridLayout(layoutWidget5);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        lblStep = new QLabel(layoutWidget5);
        lblStep->setObjectName(QString::fromUtf8("lblStep"));

        gridLayout_2->addWidget(lblStep, 0, 0, 1, 1);

        comboStep = new QComboBox(layoutWidget5);
        comboStep->setObjectName(QString::fromUtf8("comboStep"));

        gridLayout_2->addWidget(comboStep, 0, 1, 1, 1);

        groupBoxSend = new QGroupBox(centralWidget);
        groupBoxSend->setObjectName(QString::fromUtf8("groupBoxSend"));
        groupBoxSend->setGeometry(QRect(10, 70, 391, 80));
        layoutWidget6 = new QWidget(groupBoxSend);
        layoutWidget6->setObjectName(QString::fromUtf8("layoutWidget6"));
        layoutWidget6->setGeometry(QRect(10, 20, 371, 60));
        gridLayout_3 = new QGridLayout(layoutWidget6);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        openFile = new QPushButton(layoutWidget6);
        openFile->setObjectName(QString::fromUtf8("openFile"));

        gridLayout_3->addWidget(openFile, 0, 0, 1, 1);

        filePath = new QLineEdit(layoutWidget6);
        filePath->setObjectName(QString::fromUtf8("filePath"));

        gridLayout_3->addWidget(filePath, 0, 1, 1, 3);

        Begin = new QPushButton(layoutWidget6);
        Begin->setObjectName(QString::fromUtf8("Begin"));

        gridLayout_3->addWidget(Begin, 1, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 1, 3, 1, 1);

        Stop = new QPushButton(layoutWidget6);
        Stop->setObjectName(QString::fromUtf8("Stop"));

        gridLayout_3->addWidget(Stop, 1, 2, 1, 1);

        layoutWidget7 = new QWidget(centralWidget);
        layoutWidget7->setObjectName(QString::fromUtf8("layoutWidget7"));
        layoutWidget7->setGeometry(QRect(160, 20, 211, 50));
        verticalLayout_4 = new QVBoxLayout(layoutWidget7);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        rbutSend = new QRadioButton(layoutWidget7);
        rbutSend->setObjectName(QString::fromUtf8("rbutSend"));
        rbutSend->setChecked(true);

        verticalLayout_4->addWidget(rbutSend);

        rbutAdj = new QRadioButton(layoutWidget7);
        rbutAdj->setObjectName(QString::fromUtf8("rbutAdj"));

        verticalLayout_4->addWidget(rbutAdj);

        returnHome = new QCheckBox(centralWidget);
        returnHome->setObjectName(QString::fromUtf8("returnHome"));
        returnHome->setGeometry(QRect(20, 350, 371, 17));
        returnHome->setChecked(true);
        MainWindow->setCentralWidget(centralWidget);
        layoutWidget->raise();
        layoutWidget->raise();
        layoutWidget->raise();
        layoutWidget->raise();
        listWidget->raise();
        groupBoxAxis->raise();
        groupBoxSend->raise();
        returnHome->raise();
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Grbl Controller", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        MainWindow->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        lblPort->setText(QApplication::translate("MainWindow", "Port name", 0, QApplication::UnicodeUTF8));
        lblX->setText(QApplication::translate("MainWindow", "X", 0, QApplication::UnicodeUTF8));
        lblY->setText(QApplication::translate("MainWindow", "Y", 0, QApplication::UnicodeUTF8));
        lblZ->setText(QApplication::translate("MainWindow", "Z", 0, QApplication::UnicodeUTF8));
        groupBoxAxis->setTitle(QApplication::translate("MainWindow", "Axis Adjust", 0, QApplication::UnicodeUTF8));
        IncYBtn->setText(QString());
        DecXBtn->setText(QString());
        DecYBtn->setText(QString());
        IncXBtn->setText(QString());
        IncZBtn->setText(QString());
        lblZJog->setText(QApplication::translate("MainWindow", "Z Jog", 0, QApplication::UnicodeUTF8));
        DecZBtn->setText(QString());
        lblStep->setText(QApplication::translate("MainWindow", "Step Size", 0, QApplication::UnicodeUTF8));
        groupBoxSend->setTitle(QApplication::translate("MainWindow", "Send Gcode", 0, QApplication::UnicodeUTF8));
        openFile->setText(QApplication::translate("MainWindow", "Open file", 0, QApplication::UnicodeUTF8));
        Begin->setText(QApplication::translate("MainWindow", "Begin", 0, QApplication::UnicodeUTF8));
        Stop->setText(QApplication::translate("MainWindow", "Stop", 0, QApplication::UnicodeUTF8));
        rbutSend->setText(QApplication::translate("MainWindow", "Send Gcode", 0, QApplication::UnicodeUTF8));
        rbutAdj->setText(QApplication::translate("MainWindow", "Adjust machine", 0, QApplication::UnicodeUTF8));
        returnHome->setText(QApplication::translate("MainWindow", "Return Home after completion", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
