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
    QTableWidgetItem * params[10];
    for (i = 0; i < 10; i++)
    {
        params[i] = new QTableWidgetItem;
        ui->tableWidget->setItem(i,0,params[i]);
    }

    char buf[]="$\r";
    port.SendBuf(port_nr,buf,2);
#define BUF_SIZE 300
#define RESPONSE_EXPECT "ok\r\n"
    char tmp[BUF_SIZE + 1] = {0};
    int count = 0;
    bool status = true;
    QString result;
    while (!result.contains(RESPONSE_EXPECT))
    {
        int n = port.PollComport(port_nr, tmp, BUF_SIZE);
        if (n == 0)
        {
            count++;
#ifdef Q_WS_X11
            usleep(100000);  // sleep for 100 milliSeconds
#else
            Sleep(100);
#endif
        }
        else
        {
            tmp[n] = 0;
            result.append(tmp);
            //printf("GOT:%s\n", tmp); fflush(stdout);
            count = 0;
        }
#ifdef Q_WS_X11
        usleep(100000);  // sleep for 100 milliSeconds
#else
        Sleep(100);
#endif
        if (count > 50)
        {\
            status = false;
            break;
        }
    }

    if (status)
    {
        for (i = 0; i < 10; i++)
        {
            if (!result.contains(QRegExp("\\$\\d = ")))
            {
                // only collect as many as provided up to 10
                break;
            }

            ui->tableWidget->item(i,0)->setFont(QFont("Tahoma",10,87,false));
            result = result.mid(result.indexOf(QRegExp("\\$\\d = "))+5,-1);
            values[i]=result.mid(0,result.indexOf(" ")).toFloat();
        }

        for (int j = 0; j < i; j++)
        {
            params[j]->setText(QString::number(values[j]));
        }

        ui->tableWidget->setColumnWidth(0,60);
    }

    connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(changeValues(int,int)));
}

//slots

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
            if (!waitForOk())
            {
                break;
            }
#endif
        }
    }
    this->close();
}

bool GrblDialog::waitForOk()
{
#define RESPONSE_EXPECT "ok\r\n"
    char tmp[BUF_SIZE + 1] = {0};
    int count = 0;
    bool status = true;
    QString result;
    while (!result.contains(RESPONSE_EXPECT))
    {
#ifndef DISCONNECTED
        int n = port.PollComport(port_nr, tmp, BUF_SIZE);
        if (n == 0)
        {
            count++;
#ifdef Q_WS_X11
            usleep(500000);  // sleep for 100 milliSeconds
#else
            Sleep(500);
#endif
        }
        else
        {
            tmp[n] = 0;
            result.append(tmp);
            //printf("GOT:%s\n", tmp); fflush(stdout);
            count = 0;
        }
#else
        result = RESPONSE_EXPECT;
#endif
#ifdef Q_WS_X11
        usleep(100000);  // sleep for 100 milliSeconds
#else
        Sleep(100);
#endif
        if (count > 50)
        {\
            status = false;
            break;
        }
    }

    if (status)
    {
#ifdef Q_WS_X11
        usleep(200000);  // sleep for 200 millseconds
#else
        Sleep(200);
#endif
    }

    return status;
}
