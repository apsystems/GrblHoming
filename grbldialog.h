#ifndef GRBLDIALOG_H
#define GRBLDIALOG_H

#include <QDialog>
#include <QFont>
#include <QTableWidgetItem>
#include <QMessageBox>
#include "definitions.h"
#include "mainwindow.h"
#include "rs232.h"

namespace Ui {
class GrblDialog;
}

class GrblDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GrblDialog(QWidget *parent = 0);
    ~GrblDialog();
    //objects
    RS232 port;
    //vatiables
    int port_nr;
    //methods
    void setSettings();

public slots:
    //buttons
    void Ok();
    void Cancel();
    //Column Widget
    void changeValues(int row, int col);
    
private:
    Ui::GrblDialog *ui;

};

#endif // GRBLDIALOG_H
