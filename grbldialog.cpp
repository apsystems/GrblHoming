/****************************************************************
 * grbldialog.cpp
 * GrblHoming - zapmaker fork on github
 *
 * 15 Nov 2012
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#include "grbldialog.h"
#include "ui_grbldialog.h"

GrblDialog::GrblDialog(QWidget *parent, GCode *gc) :
    QDialog(parent),
    gcode(gc),
    ui(new Ui::GrblDialog)
{
    ui->setupUi(this);
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(Cancel()));
    connect(ui->btnOk,SIGNAL(clicked()),this,SLOT(Ok()));
    connect(this, SIGNAL(sendGcodeAndGetResult(int, QString)), gcode, SLOT(sendGcodeAndGetResult(int, QString)));
    connect(gcode, SIGNAL(gcodeResult(int, QString)), this, SLOT(gcodeResult(int, QString)));

    ui->btnCancel->setEnabled(false);
    ui->btnOk->setEnabled(false);

    values = new float[GRBL_SETTINGS_ITEMS_COUNT];
    change = new bool[GRBL_SETTINGS_ITEMS_COUNT];

}

GrblDialog::~GrblDialog()
{
    delete values;
    delete change;
    delete ui;
}

void GrblDialog::getSettings()
{
    int i;
    for (i = 0; i < GRBL_SETTINGS_ITEMS_COUNT; i++)
    {
        ui->tableWidget->item(i,1)->setFlags(0);
        change[i] = false;
    }
    //ui->tableWidget->item(0,1)->setFlags(Qt::NoItemFlags);
    QTableWidgetItem * params[GRBL_SETTINGS_ITEMS_COUNT];
    for (i = 0; i < GRBL_SETTINGS_ITEMS_COUNT; i++)
    {
        params[i] = new QTableWidgetItem;
        ui->tableWidget->setItem(i, 0, params[i]);
    }

    emit sendGcodeAndGetResult(GDLG_CMD_ID_GET, "$\r");
}

void GrblDialog::gcodeResult(int id, QString result)
{
    switch (id)
    {
    case GDLG_CMD_ID_GET:
        if (result.size() > 0)
        {
            int i;
            for (i = 0; i < GRBL_SETTINGS_ITEMS_COUNT; i++)
            {
                if (!result.contains(QRegExp("\\$\\d = ")))
                {
                    // only collect as many as provided up to 10 (older controllers only have 8 items)
                    break;
                }

                ui->tableWidget->item(i,0)->setFont(QFont("Tahoma",10,87,false));
                result = result.mid(result.indexOf(QRegExp("\\$\\d = "))+5,-1);
                values[i]=result.mid(0,result.indexOf(" ")).toFloat();
            }

            for (int j = 0; j < i; j++)
            {
                ui->tableWidget->item(j, 0)->setText(QString::number(values[j]));
            }

            ui->tableWidget->setColumnWidth(0,60);

            connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(changeValues(int,int)));
        }
        ui->btnCancel->setEnabled(true);
        ui->btnOk->setEnabled(true);
        break;
    case GDLG_CMD_ID_SET:
        break;
    }
}

void GrblDialog::Cancel()
{
    this->close();
}

void GrblDialog::changeValues(int row, int col)
{
    if ((ui->tableWidget->item(row,0)->text() != QString::number(values[row]))
        && ui->tableWidget->item(row,0)->text().length() > 0)
    {
        change[row]=true;
    }
}

void GrblDialog::Ok()
{
    for(int i = 0; i < GRBL_SETTINGS_ITEMS_COUNT; i++)
    {
        if (change[i])
        {
            QString strline = "$";
            strline.append(QString::number(i)).append("=").append(ui->tableWidget->item(i,0)->text()).append('\r');
            char line[20];
            for (int j = 0 ; j < strline.length() ;j++)
                line[j] = strline.at(j).toAscii();

            emit sendGcodeAndGetResult(GDLG_CMD_ID_SET, strline);
        }
    }
    this->close();
}

