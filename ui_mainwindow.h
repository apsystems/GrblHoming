/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Wed 15. Aug 12:44:49 2012
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
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
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
    QAction *actionAbout;
    QAction *actionOptions;
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
    QListWidget *statusList;
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
    QVBoxLayout *verticalLayout_5;
    QLabel *lblStep;
    QComboBox *comboStep;
    QPushButton *ResetButton;
    QCheckBox *SpindleOn;
    QGroupBox *groupBoxSend;
    QWidget *layoutWidget6;
    QGridLayout *gridLayout_2;
    QPushButton *openFile;
    QLineEdit *filePath;
    QPushButton *Begin;
    QPushButton *Stop;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnGRBL;
    QGroupBox *groupBoxFav;
    QPushButton *btnChangeTool;
    QPushButton *btnGo;
    QWidget *layoutWidget7;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *Command;
    QComboBox *comboFav;
    QPushButton *btnHome;
    QPushButton *btnOpenPort;
    QGroupBox *groupBoxOptions;
    QWidget *layoutWidget8;
    QVBoxLayout *verticalLayout_4;
    QRadioButton *rbutSend;
    QRadioButton *rbutAdj;
    QRadioButton *rbutManual;
    QMenuBar *menuBar;
    QMenu *menuHelp;
    QMenu *menuTools;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(620, 470);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMaximumSize(QSize(10000000, 10000000));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionOptions = new QAction(MainWindow);
        actionOptions->setObjectName(QString::fromUtf8("actionOptions"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(30, 10, 91, 48));
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
        layoutWidget1->setGeometry(QRect(420, 20, 170, 131));
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
        layoutWidget2->setGeometry(QRect(392, 18, 20, 131));
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

        statusList = new QListWidget(centralWidget);
        statusList->setObjectName(QString::fromUtf8("statusList"));
        statusList->setEnabled(false);
        statusList->setGeometry(QRect(20, 190, 341, 141));
        groupBoxAxis = new QGroupBox(centralWidget);
        groupBoxAxis->setObjectName(QString::fromUtf8("groupBoxAxis"));
        groupBoxAxis->setEnabled(false);
        groupBoxAxis->setGeometry(QRect(380, 170, 231, 201));
        layoutWidget3 = new QWidget(groupBoxAxis);
        layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(10, 20, 130, 91));
        gridLayout = new QGridLayout(layoutWidget3);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        IncYBtn = new QPushButton(layoutWidget3);
        IncYBtn->setObjectName(QString::fromUtf8("IncYBtn"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img/up.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        IncYBtn->setIcon(icon);
        IncYBtn->setIconSize(QSize(24, 24));

        gridLayout->addWidget(IncYBtn, 0, 1, 1, 1);

        verticalSpacer = new QSpacerItem(18, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 1, 1, 1);

        DecXBtn = new QPushButton(layoutWidget3);
        DecXBtn->setObjectName(QString::fromUtf8("DecXBtn"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/img/left.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        DecXBtn->setIcon(icon1);
        DecXBtn->setIconSize(QSize(24, 24));

        gridLayout->addWidget(DecXBtn, 2, 0, 1, 1);

        DecYBtn = new QPushButton(layoutWidget3);
        DecYBtn->setObjectName(QString::fromUtf8("DecYBtn"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/img/down.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        DecYBtn->setIcon(icon2);
        DecYBtn->setIconSize(QSize(24, 24));

        gridLayout->addWidget(DecYBtn, 2, 1, 1, 1);

        IncXBtn = new QPushButton(layoutWidget3);
        IncXBtn->setObjectName(QString::fromUtf8("IncXBtn"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/img/right.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        IncXBtn->setIcon(icon3);
        IncXBtn->setIconSize(QSize(24, 24));

        gridLayout->addWidget(IncXBtn, 2, 2, 1, 1);

        layoutWidget4 = new QWidget(groupBoxAxis);
        layoutWidget4->setObjectName(QString::fromUtf8("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(160, 20, 41, 97));
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
        layoutWidget5->setGeometry(QRect(120, 130, 102, 61));
        verticalLayout_5 = new QVBoxLayout(layoutWidget5);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        lblStep = new QLabel(layoutWidget5);
        lblStep->setObjectName(QString::fromUtf8("lblStep"));

        verticalLayout_5->addWidget(lblStep);

        comboStep = new QComboBox(layoutWidget5);
        comboStep->setObjectName(QString::fromUtf8("comboStep"));

        verticalLayout_5->addWidget(comboStep);

        ResetButton = new QPushButton(groupBoxAxis);
        ResetButton->setObjectName(QString::fromUtf8("ResetButton"));
        ResetButton->setGeometry(QRect(20, 160, 77, 23));
        QFont font3;
        font3.setPointSize(9);
        ResetButton->setFont(font3);
        ResetButton->setAutoFillBackground(false);
        SpindleOn = new QCheckBox(groupBoxAxis);
        SpindleOn->setObjectName(QString::fromUtf8("SpindleOn"));
        SpindleOn->setGeometry(QRect(20, 130, 100, 17));
        SpindleOn->setChecked(false);
        groupBoxSend = new QGroupBox(centralWidget);
        groupBoxSend->setObjectName(QString::fromUtf8("groupBoxSend"));
        groupBoxSend->setEnabled(false);
        groupBoxSend->setGeometry(QRect(20, 90, 341, 91));
        layoutWidget6 = new QWidget(groupBoxSend);
        layoutWidget6->setObjectName(QString::fromUtf8("layoutWidget6"));
        layoutWidget6->setGeometry(QRect(10, 13, 321, 71));
        gridLayout_2 = new QGridLayout(layoutWidget6);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        openFile = new QPushButton(layoutWidget6);
        openFile->setObjectName(QString::fromUtf8("openFile"));

        gridLayout_2->addWidget(openFile, 0, 0, 1, 1);

        filePath = new QLineEdit(layoutWidget6);
        filePath->setObjectName(QString::fromUtf8("filePath"));

        gridLayout_2->addWidget(filePath, 0, 1, 1, 3);

        Begin = new QPushButton(layoutWidget6);
        Begin->setObjectName(QString::fromUtf8("Begin"));

        gridLayout_2->addWidget(Begin, 1, 2, 1, 1);

        Stop = new QPushButton(layoutWidget6);
        Stop->setObjectName(QString::fromUtf8("Stop"));

        gridLayout_2->addWidget(Stop, 1, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 1, 1, 1);

        btnGRBL = new QPushButton(centralWidget);
        btnGRBL->setObjectName(QString::fromUtf8("btnGRBL"));
        btnGRBL->setEnabled(false);
        btnGRBL->setGeometry(QRect(400, 390, 121, 23));
        groupBoxFav = new QGroupBox(centralWidget);
        groupBoxFav->setObjectName(QString::fromUtf8("groupBoxFav"));
        groupBoxFav->setEnabled(false);
        groupBoxFav->setGeometry(QRect(20, 340, 341, 80));
        btnChangeTool = new QPushButton(groupBoxFav);
        btnChangeTool->setObjectName(QString::fromUtf8("btnChangeTool"));
        btnChangeTool->setGeometry(QRect(190, 50, 81, 23));
        btnGo = new QPushButton(groupBoxFav);
        btnGo->setObjectName(QString::fromUtf8("btnGo"));
        btnGo->setGeometry(QRect(280, 20, 51, 21));
        layoutWidget7 = new QWidget(groupBoxFav);
        layoutWidget7->setObjectName(QString::fromUtf8("layoutWidget7"));
        layoutWidget7->setGeometry(QRect(10, 20, 261, 27));
        horizontalLayout = new QHBoxLayout(layoutWidget7);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget7);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);

        Command = new QLineEdit(layoutWidget7);
        Command->setObjectName(QString::fromUtf8("Command"));

        horizontalLayout->addWidget(Command);

        comboFav = new QComboBox(groupBoxFav);
        comboFav->setObjectName(QString::fromUtf8("comboFav"));
        comboFav->setGeometry(QRect(10, 50, 171, 22));
        btnHome = new QPushButton(groupBoxFav);
        btnHome->setObjectName(QString::fromUtf8("btnHome"));
        btnHome->setGeometry(QRect(280, 50, 51, 23));
        btnOpenPort = new QPushButton(centralWidget);
        btnOpenPort->setObjectName(QString::fromUtf8("btnOpenPort"));
        btnOpenPort->setGeometry(QRect(30, 65, 75, 23));
        groupBoxOptions = new QGroupBox(centralWidget);
        groupBoxOptions->setObjectName(QString::fromUtf8("groupBoxOptions"));
        groupBoxOptions->setEnabled(false);
        groupBoxOptions->setGeometry(QRect(130, 10, 221, 81));
        layoutWidget8 = new QWidget(groupBoxOptions);
        layoutWidget8->setObjectName(QString::fromUtf8("layoutWidget8"));
        layoutWidget8->setGeometry(QRect(10, 0, 201, 77));
        verticalLayout_4 = new QVBoxLayout(layoutWidget8);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        rbutSend = new QRadioButton(layoutWidget8);
        rbutSend->setObjectName(QString::fromUtf8("rbutSend"));
        rbutSend->setChecked(true);

        verticalLayout_4->addWidget(rbutSend);

        rbutAdj = new QRadioButton(layoutWidget8);
        rbutAdj->setObjectName(QString::fromUtf8("rbutAdj"));

        verticalLayout_4->addWidget(rbutAdj);

        rbutManual = new QRadioButton(layoutWidget8);
        rbutManual->setObjectName(QString::fromUtf8("rbutManual"));

        verticalLayout_4->addWidget(rbutManual);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 620, 20));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuHelp->addAction(actionAbout);
        menuTools->addAction(actionOptions);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Grbl Controller", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        MainWindow->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
        actionOptions->setText(QApplication::translate("MainWindow", "Options", 0, QApplication::UnicodeUTF8));
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
        lblStep->setText(QApplication::translate("MainWindow", "Step Size           ", 0, QApplication::UnicodeUTF8));
        ResetButton->setText(QApplication::translate("MainWindow", "Set Home", 0, QApplication::UnicodeUTF8));
        SpindleOn->setText(QApplication::translate("MainWindow", "Spindle On", 0, QApplication::UnicodeUTF8));
        groupBoxSend->setTitle(QApplication::translate("MainWindow", "Send Gcode", 0, QApplication::UnicodeUTF8));
        openFile->setText(QApplication::translate("MainWindow", "Open file", 0, QApplication::UnicodeUTF8));
        Begin->setText(QApplication::translate("MainWindow", "Begin", 0, QApplication::UnicodeUTF8));
        Stop->setText(QApplication::translate("MainWindow", "Stop", 0, QApplication::UnicodeUTF8));
        btnGRBL->setText(QApplication::translate("MainWindow", "GRBL settings", 0, QApplication::UnicodeUTF8));
        groupBoxFav->setTitle(QApplication::translate("MainWindow", "Manual", 0, QApplication::UnicodeUTF8));
        btnChangeTool->setText(QApplication::translate("MainWindow", "Change tool", 0, QApplication::UnicodeUTF8));
        btnGo->setText(QApplication::translate("MainWindow", "Go", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Command", 0, QApplication::UnicodeUTF8));
        Command->setText(QString());
        comboFav->clear();
        comboFav->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Select...", 0, QApplication::UnicodeUTF8)
        );
        btnHome->setText(QApplication::translate("MainWindow", "Home", 0, QApplication::UnicodeUTF8));
        btnOpenPort->setText(QApplication::translate("MainWindow", "Open", 0, QApplication::UnicodeUTF8));
        groupBoxOptions->setTitle(QString());
        rbutSend->setText(QApplication::translate("MainWindow", "Send Gcode", 0, QApplication::UnicodeUTF8));
        rbutAdj->setText(QApplication::translate("MainWindow", "Adjust machine", 0, QApplication::UnicodeUTF8));
        rbutManual->setText(QApplication::translate("MainWindow", "Manual coordinates", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("MainWindow", "Tools", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
