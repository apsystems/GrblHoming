#include "grbldialog.h"
#include "ui_grbldialog.h"

float values[]={0,0,0,0,0,0,0,0,0,0};
bool change[10];

GrblDialog::GrblDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GrblDialog)
{
    ui->setupUi(this);
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(Cancel()));
    connect(ui->btnOk,SIGNAL(clicked()),this,SLOT(Ok()));
}

GrblDialog::~GrblDialog()
{
    delete ui;
}

//methods
void GrblDialog::setSettings()
{
    int i;
    for(i=0;i<10;i++)
    {
        ui->tableWidget->item(i,1)->setFlags(0);
        change[i]=false;
    }
    //ui->tableWidget->item(0,1)->setFlags(Qt::NoItemFlags);
    QTableWidgetItem *param0 = new QTableWidgetItem;
    QTableWidgetItem *param1 = new QTableWidgetItem;
    QTableWidgetItem *param2 = new QTableWidgetItem;
    QTableWidgetItem *param3 = new QTableWidgetItem;
    QTableWidgetItem *param4 = new QTableWidgetItem;
    QTableWidgetItem *param5 = new QTableWidgetItem;
    QTableWidgetItem *param6 = new QTableWidgetItem;
    QTableWidgetItem *param7 = new QTableWidgetItem;
    QTableWidgetItem *param8 = new QTableWidgetItem;
    QTableWidgetItem *param9 = new QTableWidgetItem;
    ui->tableWidget->setItem(0,0,param0);
    ui->tableWidget->setItem(1,0,param1);
    ui->tableWidget->setItem(2,0,param2);
    ui->tableWidget->setItem(3,0,param3);
    ui->tableWidget->setItem(4,0,param4);
    ui->tableWidget->setItem(5,0,param5);
    ui->tableWidget->setItem(6,0,param6);
    ui->tableWidget->setItem(7,0,param7);
    ui->tableWidget->setItem(8,0,param8);
    ui->tableWidget->setItem(9,0,param9);

    i=0;
#ifndef DISCONNECTED
#ifdef Q_WS_X11
        usleep(100000);
#else
        Sleep(100);
#endif
        QString linea="";
        char buf[]="$\r";
        char read[223];
        int n=0;
        //while(n==0)
          //  n=RS232().PollComport(port_nr,read,50);
            //n=port.PollComport(port_nr,read,50);
        port.SendBuf(port_nr,buf,2);
        while(i<10)
        {
            n=port.PollComport(port_nr,read,223);
            linea.append(read);
            while(linea.contains(QRegExp("\\$\\d = ")))
            {
                ui->tableWidget->item(i,0)->setFont(QFont("Tahoma",10,87,false));
                linea = linea.mid(linea.indexOf("$")+5,-1);
                values[i++]=linea.mid(0,linea.indexOf(" ")).toFloat();
            }
            if(i>=10)
                break;
        }
#endif
    i=0;
    param0->setText(QString::number(values[i++]));
    param1->setText(QString::number(values[i++]));
    param2->setText(QString::number(values[i++]));
    param3->setText(QString::number(values[i++]));
    param4->setText(QString::number(values[i++]));
    param5->setText(QString::number(values[i++]));
    param6->setText(QString::number(values[i++]));
    param7->setText(QString::number(values[i++]));
    param8->setText(QString::number(values[i++]));
    param9->setText(QString::number(values[i++]));
    ui->tableWidget->setColumnWidth(0,60);
    //for(i=0;i<10;i++)

    connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(changeValues(int,int)));
}

//slots

void GrblDialog::Cancel()
{
    this->close();
}

void GrblDialog::changeValues(int row, int col)
{
    if(ui->tableWidget->item(row,0)->text()!=QString::number(values[row]))
    {
        change[row]=true;
    }
}

void GrblDialog::Ok()
{
    int i=0;
    char line[20];
    int j;
    QString strline;
        for(i=0;i<10;i++)
        {
            if(change[i])
            {
                strline="$";
                strline.append(QString::number(i)).append("=").append(ui->tableWidget->item(i,0)->text()).append('\r');
                for(j=0;j<strline.length();j++)
                    line[j]=strline.at(j).toAscii();
#ifndef DISCONNECTED
                port.SendBuf(this->port_nr,line,j);
#endif
            }
        }
    this->close();
}
