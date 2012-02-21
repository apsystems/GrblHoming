#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rs232.h"

#define DEBUG

bool interrupt=false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::setFixedSize(670,400);
    connect(ui->DecXBtn,SIGNAL(clicked()),this,SLOT(decX()));
    connect(ui->DecYBtn,SIGNAL(clicked()),this,SLOT(decY()));
    connect(ui->DecZBtn,SIGNAL(clicked()),this,SLOT(decZ()));
    connect(ui->IncXBtn,SIGNAL(clicked()),this,SLOT(incX()));
    connect(ui->IncYBtn,SIGNAL(clicked()),this,SLOT(incY()));
    connect(ui->IncZBtn,SIGNAL(clicked()),this,SLOT(incZ()));
    connect(ui->rbutSend,SIGNAL(toggled(bool)),this,SLOT(send()));
    connect(ui->rbutAdj,SIGNAL(toggled(bool)),this,SLOT(adjust()));
    connect(ui->openFile,SIGNAL(clicked()),this,SLOT(open()));
    connect(ui->Begin,SIGNAL(clicked()),this,SLOT(begin()));
    connect(ui->Stop,SIGNAL(clicked()),this,SLOT(stop()));
    ui->comboStep->addItem("0.0001");
    ui->comboStep->addItem("0.001");
    ui->comboStep->addItem("0.01");
    ui->comboStep->addItem("0.1");
    ui->comboStep->addItem("1");
    ui->Begin->setEnabled(false);
    ui->Stop->setEnabled(false);
#ifdef Q_WS_X11
    ui->cmbPort->addItem("USB0");
    ui->cmbPort->addItem("USB1");
    ui->cmbPort->addItem("USB2");
    ui->cmbPort->addItem("USB3");
    ui->cmbPort->addItem("USB4");
    ui->cmbPort->addItem("USB5");
    ui->cmbPort->addItem("ACM0");
    ui->cmbPort->addItem("ACM1");
    ui->cmbPort->addItem("ACM2");
    ui->cmbPort->addItem("ACM3");
    ui->cmbPort->addItem("ACM4");
    ui->cmbPort->addItem("ACM5");
#ifdef DEBUG
    ui->cmbPort->setCurrentIndex(6);
#endif
#else
    ui->cmbPort->addItem("COM1");
    ui->cmbPort->addItem("COM2");
    ui->cmbPort->addItem("COM3");
    ui->cmbPort->addItem("COM4");
    ui->cmbPort->addItem("COM5");
    ui->cmbPort->addItem("COM6");
    ui->cmbPort->addItem("COM7");
    ui->cmbPort->addItem("COM8");
    ui->cmbPort->addItem("COM9");
    ui->cmbPort->addItem("COM10");
    ui->cmbPort->addItem("COM11");
    ui->cmbPort->addItem("COM12");
    ui->cmbPort->addItem("COM13");
    ui->cmbPort->addItem("COM14");
    ui->cmbPort->addItem("COM15");
    ui->cmbPort->addItem("COM16");
#ifdef DEBUG
    ui->cmbPort->setCurrentIndex(11);
#endif
#endif

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::incX()
{
    if(SendJog(ui->lcdNumberX->value(),ui->lcdNumberY->value(),ui->lcdNumberZ->value()))
    {
        ui->lcdNumberX->display(ui->lcdNumberX->value()+ui->comboStep->currentText().toFloat());
        ui->centralWidget->setStatusTip("");
    }
    else
        ui->centralWidget->setStatusTip("Connection error.");
}

void MainWindow::incY()
{
    if(SendJog(ui->lcdNumberX->value(),ui->lcdNumberY->value(),ui->lcdNumberZ->value()))
    {
        ui->lcdNumberY->display(ui->lcdNumberY->value()+ui->comboStep->currentText().toFloat());
        ui->centralWidget->setStatusTip("");
    }
    else
        ui->centralWidget->setStatusTip("Connection error.");
}

void MainWindow::incZ()
{
    if(SendJog(ui->lcdNumberX->value(),ui->lcdNumberY->value(),ui->lcdNumberZ->value()))
    {
        ui->lcdNumberZ->display(ui->lcdNumberZ->value()+ui->comboStep->currentText().toFloat());
        ui->centralWidget->setStatusTip("");
    }
    else
        ui->centralWidget->setStatusTip("Connection error.");
}

void MainWindow::decX()
{
    if(SendJog(ui->lcdNumberX->value(),ui->lcdNumberY->value(),ui->lcdNumberZ->value()))
    {
        ui->lcdNumberX->display(ui->lcdNumberX->value()-ui->comboStep->currentText().toFloat());
        ui->centralWidget->setStatusTip("");
    }
    else
        ui->centralWidget->setStatusTip("Connection error.");
}

void MainWindow::decY()
{
    if(SendJog(ui->lcdNumberX->value(),ui->lcdNumberY->value(),ui->lcdNumberZ->value()))
    {
        ui->lcdNumberY->display(ui->lcdNumberY->value()-ui->comboStep->currentText().toFloat());
        ui->centralWidget->setStatusTip("");
    }
    else
        ui->centralWidget->setStatusTip("Connection error.");
}

void MainWindow::decZ()
{
    if(SendJog(ui->lcdNumberX->value(),ui->lcdNumberY->value(),ui->lcdNumberZ->value()))
    {
        ui->lcdNumberZ->display(ui->lcdNumberZ->value()-ui->comboStep->currentText().toFloat());
        ui->centralWidget->setStatusTip("");
    }
    else
        ui->centralWidget->setStatusTip("Connection error.");
}

void MainWindow::send()
{
    if(ui->rbutSend->isChecked())
    {
        ui->groupBoxAxis->setEnabled(false);
        ui->groupBoxSend->setEnabled(true);
        ui->rbutAdj->setChecked(false);
    }
}

void MainWindow::adjust()
{
    if(ui->rbutAdj->isChecked())
    {
        ui->groupBoxAxis->setEnabled(true);
        ui->groupBoxSend->setEnabled(false);
        ui->rbutSend->setChecked(false);
    }
}

void MainWindow::open()
{
    ui->filePath->setText(QFileDialog::getOpenFileName(this,"Open file","",".Nc files (*.nc)"));
    if(ui->filePath->text()!="")
    {
        ui->Begin->setEnabled(true);
        ui->Stop->setEnabled(true);
    }
    else
    {
        ui->Begin->setEnabled(false);
        ui->Stop->setEnabled(false);
    }
}

void MainWindow::begin()
{
    int i;
    QFile file(ui->filePath->text());
    if(file.open(QFile::ReadOnly))
    {
        QTextStream code(&file);
        QString strline = code.readLine();

        while((strline!=NULL)&&(!interrupt))
        {
            char line[50];
            if(strline.at(0)=='(')
            {}
            else
            {
                strline.append("\n");
                for(i=0;i<strline.length();i++)
                    line[i]=strline.at(i).toAscii();
#ifndef DEBUG
                if(!SendGcode(line,strline.length()))
                {
                    ui->centralWidget->setStatusTip("Connection error.");
                    break;
                }
                else
#endif
                {
                    UpdateAxis(strline);
                    ui->listWidget->addItem(strline.trimmed());
                    MainWindow::repaint();
                    if(ui->listWidget->count()>12)
                    {
                        delete ui->listWidget->item(0);
                    }
                }
            }
            strline=code.readLine();
        }
        file.close();
    }
    //QTextStream file = QTextStream(ui->filePath->text(),QIODevice::OpenMode=QIODevice::ReadWrite);
}

void MainWindow::stop()
{
    ui->centralWidget->setStatusTip("Process stopped.");
    interrupt=true;

}

int MainWindow::SendJog(float X, float Y, float Z)
{
    QString strline;
    char line[35];
    int i;
    strline="G00 X";
    strline.append(QString::number(X,'g',6));
    strline.append(" Y");
    strline.append(QString::number(Y,'g',6));
    strline.append(" Z");
    strline.append(QString::number(Z,'g',6));
    for(i=0;i<strline.length();i++)
    {
        line[i]=strline.at(i).toAscii();
    }
    line[i++]='\n';
    if(SendGcode(line,i))
        return(1);
    else
        return (0);
}

int MainWindow::SendGcode(char* line, int length)
{
    RS232 port = RS232();
    int cport_nr = ui->cmbPort->currentIndex();
    //ui->lineEdit->setText(QString::number(cport_nr,10));
    int i, n=0;
    char buf[50],buff[]="\n";
    //ui->lineEdit->setText(strline);
    for(i=0;i<50;i++)
        buf[i]=0;
    if(port.OpenComport(cport_nr))
    {
        return(0);
    }
    else
    {
        port.SendBuf(cport_nr,buff,1);
        while(n==0)
        {
            //usleep(100000);  /* sleep for 100 milliSeconds */
            n = port.PollComport(cport_nr, buf, 50);
#ifdef Q_WS_X11
            usleep(100000);  /* sleep for 100 milliSeconds */
#else
            Sleep(100);
#endif
            //perror("esperando\n");
            /*if(n > 0)
            {
                buf[n] = 0;   // always put a "null" at the end of a string!
                for(i=0; i < n; i++)
                {
                    if(buf[i] < 32)  // replace unreadable control-codes by dots
                    {
                        buf[i] = '.';
                    }
                }
                printf("received %i bytes: %s\n", n, (char *)buf);
                printf("%s\n",(char *)buf);
            }
            else
            {
                printf("received %i bytes.\n", n);
            }*/
        }
        //perror("Success\n");
        //while(!exit)
        {
            //usleep(150000);
            port.SendBuf(cport_nr,line,length);
            //->usleep(50000);
            n = port.PollComport(cport_nr, buf, 50);
#ifdef DEBUG
            if(n > 0)
            {
                buf[n] = 0;   // always put a "null" at the end of a string!
                for(i=0; i < n; i++)
                {
                    if(buf[i] < 32)  // replace unreadable control-codes by dots
                    {
                        buf[i] = '.';
                    }
                }
                printf("received %i bytes: %s\n", n, (char *)buf);
                printf("%s\n",(char *)buf);
            }
            else
            {
                printf("received %i bytes.\n", n);
            }
#endif
            //->usleep(50000);  /* sleep for 100 milliSeconds */
            while((n==0)||(port.find_txt(buf)==0))
            //while (n==0)
            {
                n = port.PollComport(cport_nr, buf, 50);
                //usleep(50000);
                /*if(n > 0)
                {
                    buf[n] = 0;   // always put a "null" at the end of a string!
                    for(i=0; i < n; i++)
                    {
                        if(buf[i] < 32)  // replace unreadable control-codes by dots
                        {
                            buf[i] = '.';
                        }
                    }
                    printf("Last received %i bytes: %s\n", n, (char *)buf);
                    printf("%s\n",(char *)buf);
                }
                else
                {
                    printf("Last received %i bytes.\n", n);
                }*/
            }
        }
        port.CloseComport(cport_nr);
    }
    return(1);
}

void MainWindow::UpdateAxis(QString code)
{
    QStringList list;
    list = code.split(QRegExp("\\s+"),QString::SkipEmptyParts);
    if((list.at(0)=="G00")||(list.at(0)=="G01"))
    {
        if(list.count()==2)
        {
            if(list.at(1).at(0)=='Z')
                ui->lcdNumberZ->display(list.at(1).mid(1,-1).toFloat(0));
        }
        else if(list.count()==3)
        {
            if(list.at(1).at(0)=='X')
            {
                ui->lcdNumberX->display(list.at(1).mid(1,-1).toFloat(0));
                ui->lcdNumberY->display(list.at(2).mid(1,-1).toFloat(0));
            }
            //if(list.at(1).at(0)=='Z')
            else
                ui->lcdNumberZ->display(list.at(1).mid(1,-1).toFloat(0));
            /*if(list.at(2).length()>0)
                if(list.at(2).at(0)=='Y')
                    ui->lcdNumberY->display(list.at(2).mid(1,-1).toFloat(0));*/
        }
        else if(list.count()==4)
        {
            if(list.at(1).at(0)=='X')
            {
                ui->lcdNumberX->display(list.at(1).mid(1,-1).toFloat(0));
            //if(list.at(2).at(0)=='Y')
                ui->lcdNumberY->display(list.at(2).mid(1,-1).toFloat(0));
            }
            if(list.at(3).at(0)=='Z')
                    ui->lcdNumberZ->display(list.at(3).mid(1,-1).toFloat(0));
        }
        else{}
    }
}

