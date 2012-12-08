/****************************************************************
 * grbldialog.h
 * GrblHoming - zapmaker fork on github
 *
 * 15 Nov 2012
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#ifndef GRBLDIALOG_H
#define GRBLDIALOG_H

#include <QDialog>
#include <QFont>
#include <QTableWidgetItem>
#include <QMessageBox>
#include "definitions.h"
#include "mainwindow.h"
#include "gcode.h"

namespace Ui {
class GrblDialog;
}

#define GDLG_CMD_ID_GET 1
#define GDLG_CMD_ID_SET 2

#define GRBL_SETTINGS_ITEMS_COUNT 10

class GrblDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GrblDialog(QWidget *parent, GCode *gcode);
    ~GrblDialog();

    //variables
    GCode *gcode;

    //methods
    void getSettings();

signals:
    void sendGcodeAndGetResult(int id, QString cmd);

public slots:
    //buttons
    void Ok();
    void Cancel();
    //Column Widget
    void changeValues(int row, int col);
    void gcodeResult(int id, QString result);
    
private:
    Ui::GrblDialog *ui;
    float *values;
    bool *change;


};

#endif // GRBLDIALOG_H
