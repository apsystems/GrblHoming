/********************************************************************************
** Form generated from reading UI file 'grbldialog.ui'
**
** Created: Wed 15. Aug 12:44:49 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRBLDIALOG_H
#define UI_GRBLDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_GrblDialog
{
public:
    QPushButton *btnOk;
    QPushButton *btnCancel;
    QTableWidget *tableWidget;

    void setupUi(QDialog *GrblDialog)
    {
        if (GrblDialog->objectName().isEmpty())
            GrblDialog->setObjectName(QString::fromUtf8("GrblDialog"));
        GrblDialog->resize(322, 343);
        btnOk = new QPushButton(GrblDialog);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));
        btnOk->setGeometry(QRect(220, 40, 75, 23));
        btnCancel = new QPushButton(GrblDialog);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        btnCancel->setGeometry(QRect(220, 80, 75, 23));
        tableWidget = new QTableWidget(GrblDialog);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        if (tableWidget->rowCount() < 10)
            tableWidget->setRowCount(10);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setItem(0, 1, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setItem(1, 1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setItem(2, 1, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setItem(3, 1, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setItem(4, 1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setItem(5, 1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setItem(6, 1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setItem(7, 1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setItem(8, 1, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget->setItem(9, 1, __qtablewidgetitem9);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(40, 20, 160, 301));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy);
        tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget->setAutoScroll(true);
        tableWidget->setRowCount(10);
        tableWidget->setColumnCount(2);
        tableWidget->horizontalHeader()->setVisible(false);
        tableWidget->horizontalHeader()->setDefaultSectionSize(100);
        tableWidget->verticalHeader()->setVisible(false);

        retranslateUi(GrblDialog);

        QMetaObject::connectSlotsByName(GrblDialog);
    } // setupUi

    void retranslateUi(QDialog *GrblDialog)
    {
        GrblDialog->setWindowTitle(QApplication::translate("GrblDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        btnOk->setText(QApplication::translate("GrblDialog", "Apply", 0, QApplication::UnicodeUTF8));
        btnCancel->setText(QApplication::translate("GrblDialog", "Close", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = tableWidget->isSortingEnabled();
        tableWidget->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem = tableWidget->item(0, 1);
        ___qtablewidgetitem->setText(QApplication::translate("GrblDialog", "steps/mm X", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->item(1, 1);
        ___qtablewidgetitem1->setText(QApplication::translate("GrblDialog", "steps/mm Y", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->item(2, 1);
        ___qtablewidgetitem2->setText(QApplication::translate("GrblDialog", "steps/mm Z", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->item(3, 1);
        ___qtablewidgetitem3->setText(QApplication::translate("GrblDialog", "microseconds step pulse", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->item(4, 1);
        ___qtablewidgetitem4->setText(QApplication::translate("GrblDialog", "mm/min default feed rate", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->item(5, 1);
        ___qtablewidgetitem5->setText(QApplication::translate("GrblDialog", "mm/min default seek rate", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->item(6, 1);
        ___qtablewidgetitem6->setText(QApplication::translate("GrblDialog", "mm/arc segment", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->item(7, 1);
        ___qtablewidgetitem7->setText(QApplication::translate("GrblDialog", "step port invert mask", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->item(8, 1);
        ___qtablewidgetitem8->setText(QApplication::translate("GrblDialog", "acceleration in mm/sec^2", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget->item(9, 1);
        ___qtablewidgetitem9->setText(QApplication::translate("GrblDialog", "cornering junction deviation in mm", 0, QApplication::UnicodeUTF8));
        tableWidget->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class GrblDialog: public Ui_GrblDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRBLDIALOG_H
