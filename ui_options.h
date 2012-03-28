/********************************************************************************
** Form generated from reading UI file 'options.ui'
**
** Created: Tue 27. Mar 18:14:43 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONS_H
#define UI_OPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_Options
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBoxStream;
    QCheckBox *chkGoHome;
    QCheckBox *chkToolChange;
    QFrame *XYZframe;
    QLineEdit *lineEditTCX;
    QLineEdit *lineEditTCY;
    QLineEdit *lineEditTCZ;
    QGroupBox *groupBoxAxis;
    QCheckBox *chkInvX;
    QCheckBox *chkInvY;
    QCheckBox *chkInvZ;

    void setupUi(QDialog *Options)
    {
        if (Options->objectName().isEmpty())
            Options->setObjectName(QString::fromUtf8("Options"));
        Options->resize(400, 300);
        buttonBox = new QDialogButtonBox(Options);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 260, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        groupBoxStream = new QGroupBox(Options);
        groupBoxStream->setObjectName(QString::fromUtf8("groupBoxStream"));
        groupBoxStream->setGeometry(QRect(40, 130, 301, 121));
        chkGoHome = new QCheckBox(groupBoxStream);
        chkGoHome->setObjectName(QString::fromUtf8("chkGoHome"));
        chkGoHome->setGeometry(QRect(20, 20, 211, 21));
        chkToolChange = new QCheckBox(groupBoxStream);
        chkToolChange->setObjectName(QString::fromUtf8("chkToolChange"));
        chkToolChange->setGeometry(QRect(20, 50, 211, 17));
        XYZframe = new QFrame(groupBoxStream);
        XYZframe->setObjectName(QString::fromUtf8("XYZframe"));
        XYZframe->setGeometry(QRect(40, 70, 161, 41));
        XYZframe->setFrameShape(QFrame::StyledPanel);
        XYZframe->setFrameShadow(QFrame::Raised);
        lineEditTCX = new QLineEdit(XYZframe);
        lineEditTCX->setObjectName(QString::fromUtf8("lineEditTCX"));
        lineEditTCX->setGeometry(QRect(10, 10, 41, 20));
        lineEditTCY = new QLineEdit(XYZframe);
        lineEditTCY->setObjectName(QString::fromUtf8("lineEditTCY"));
        lineEditTCY->setGeometry(QRect(60, 10, 41, 20));
        lineEditTCZ = new QLineEdit(XYZframe);
        lineEditTCZ->setObjectName(QString::fromUtf8("lineEditTCZ"));
        lineEditTCZ->setGeometry(QRect(110, 10, 41, 20));
        groupBoxAxis = new QGroupBox(Options);
        groupBoxAxis->setObjectName(QString::fromUtf8("groupBoxAxis"));
        groupBoxAxis->setGeometry(QRect(40, 10, 151, 101));
        chkInvX = new QCheckBox(groupBoxAxis);
        chkInvX->setObjectName(QString::fromUtf8("chkInvX"));
        chkInvX->setGeometry(QRect(10, 20, 151, 17));
        chkInvY = new QCheckBox(groupBoxAxis);
        chkInvY->setObjectName(QString::fromUtf8("chkInvY"));
        chkInvY->setGeometry(QRect(10, 50, 141, 17));
        chkInvZ = new QCheckBox(groupBoxAxis);
        chkInvZ->setObjectName(QString::fromUtf8("chkInvZ"));
        chkInvZ->setGeometry(QRect(10, 80, 141, 17));

        retranslateUi(Options);
        QObject::connect(buttonBox, SIGNAL(accepted()), Options, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Options, SLOT(reject()));

        QMetaObject::connectSlotsByName(Options);
    } // setupUi

    void retranslateUi(QDialog *Options)
    {
        Options->setWindowTitle(QApplication::translate("Options", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBoxStream->setTitle(QApplication::translate("Options", "Send Gcode", 0, QApplication::UnicodeUTF8));
        chkGoHome->setText(QApplication::translate("Options", "Return home at completion", 0, QApplication::UnicodeUTF8));
        chkToolChange->setText(QApplication::translate("Options", "Enable Tool Change", 0, QApplication::UnicodeUTF8));
        groupBoxAxis->setTitle(QApplication::translate("Options", "Invert Axis", 0, QApplication::UnicodeUTF8));
        chkInvX->setText(QApplication::translate("Options", "X Axis", 0, QApplication::UnicodeUTF8));
        chkInvY->setText(QApplication::translate("Options", "Y Axis", 0, QApplication::UnicodeUTF8));
        chkInvZ->setText(QApplication::translate("Options", "Z Axis", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Options: public Ui_Options {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONS_H
