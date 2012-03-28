#include "mainwindow.h"
#include "ui_mainwindow.h"

//#define DEBUG

QStringList lista;
//Options opt();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    portOpen=false;
    ui->setupUi(this);
    readSettings();
    //ui->btnGoChange->setEnabled(toolChange);
    //ui->rbutManual->setVisible(false);
    //ui->groupBoxFav->setVisible(false);
    ui->comboFav->setVisible(false);
    //buttons
    connect(ui->btnOpenPort,SIGNAL(clicked()),this,SLOT(openPort()));
    connect(ui->btnGRBL,SIGNAL(clicked()),this,SLOT(setGRBL()));
        //Adjust
    connect(ui->DecXBtn,SIGNAL(clicked()),this,SLOT(decX()));
    connect(ui->DecYBtn,SIGNAL(clicked()),this,SLOT(decY()));
    connect(ui->DecZBtn,SIGNAL(clicked()),this,SLOT(decZ()));
    connect(ui->IncXBtn,SIGNAL(clicked()),this,SLOT(incX()));
    connect(ui->IncYBtn,SIGNAL(clicked()),this,SLOT(incY()));
    connect(ui->IncZBtn,SIGNAL(clicked()),this,SLOT(incZ()));
    connect(ui->ResetButton,SIGNAL(clicked()),this,SLOT(reset()));
        //Manual
    connect(ui->btnGoChange,SIGNAL(clicked()),this,SLOT(gotoToolChange()));
    connect(ui->btnGo,SIGNAL(clicked()),this,SLOT(gotoXYZ()));
    connect(ui->btnHome,SIGNAL(clicked()),this,SLOT(gotoHome()));
        //Send Gcode
    connect(ui->Begin,SIGNAL(clicked()),this,SLOT(begin()));
    connect(ui->openFile,SIGNAL(clicked()),this,SLOT(openFile()));
    connect(ui->Stop,SIGNAL(clicked()),this,SLOT(stop()));
    connect(ui->Stop,SIGNAL(clicked()),&readthread,SLOT(stopsig()));
    //radio buttons
    connect(ui->rbutAdj,SIGNAL(toggled(bool)),this,SLOT(adjustRBtn()));
    connect(ui->rbutManual,SIGNAL(toggled(bool)),this,SLOT(manualRBtn()));
    connect(ui->rbutSend,SIGNAL(toggled(bool)),this,SLOT(sendRBtn()));
    //check box
    connect(ui->SpindleOn,SIGNAL(toggled(bool)),this,SLOT(toggleSpindle()));
    //communications
        //options
    connect(&opt,SIGNAL(sendSettings(int)),this,SLOT(setSettings(int)));
    connect(&opt,SIGNAL(toolCoord(float[])),this,SLOT(setTCCoord(float[])));
        //thread
    connect(&readthread,SIGNAL(addList(QString)),this,SLOT(receiveList(QString)));
    connect(&readthread,SIGNAL(sendMsg(QString)),this,SLOT(receiveMsg(QString)));
    connect(&readthread,SIGNAL(sendAxis(QString)),this,SLOT(receiveAxis(QString)));
    connect(this,SIGNAL(Stop()),&readthread,SLOT(stopsig()));
    // menu bar
    connect(ui->actionOptions,SIGNAL(triggered()),this,SLOT(getOptions()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(showAbout()));

    fillFavList();
    ui->comboStep->addItem("0.01");
    ui->comboStep->addItem("0.1");
    ui->comboStep->addItem("1");
    ui->comboStep->addItem("10");
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
    styleSheet = ui->btnOpenPort->styleSheet();
}

MainWindow::~MainWindow()
{
    if(this->portOpen)
        port.CloseComport(port_nr);
    delete ui;
}

void MainWindow::adjustRBtn()
{
    if(ui->rbutAdj->isChecked())
    {
        ui->groupBoxAxis->setEnabled(true);
        ui->groupBoxSend->setEnabled(false);
        ui->groupBoxFav->setEnabled(false);
    }
}

void MainWindow::begin()
{
    ui->statusList->clear();
    readthread.goHome=this->goHome;
    readthread.path=ui->filePath->text();
    readthread.port=this->port;
    readthread.port_nr=port_nr;
    readthread.toolChange=this->toolChange;
    for(int i=0;i<3;i++)
        readthread.toolChangeXYZ[i]=this->toolChangeXYZ[i];
    readthread.start();
}

void MainWindow::decX()
{
    float coord=-ui->comboStep->currentText().toFloat();
    if(invX)
        coord =-coord;
    if(SendJog(QString("G01 X")
               .append(QString::number(coord))))
    {
        ui->lcdNumberX->display(ui->lcdNumberX->value()+coord);
        ui->centralWidget->setStatusTip("");
    }
    else
        ui->centralWidget->setStatusTip("Connection error.");
}

void MainWindow::decY()
{
    float coord=-ui->comboStep->currentText().toFloat();
    if(invY)
        coord=-coord;
    if(SendJog(QString("G01 Y")
               .append(QString::number(coord))))
    {
        ui->lcdNumberY->display(ui->lcdNumberY->value()+coord);
        ui->centralWidget->setStatusTip("");
    }
    else
        ui->centralWidget->setStatusTip("Connection error.");
}

void MainWindow::decZ()
{
    float coord=-ui->comboStep->currentText().toFloat();
    if(invZ)
        coord=-coord;
    if(SendJog(QString("G01 Z")
               .append(QString::number(coord)).append(" F260")))
    {
        ui->lcdNumberZ->display(ui->lcdNumberZ->value()+coord);
        ui->centralWidget->setStatusTip("");
    }
    else
        ui->centralWidget->setStatusTip("Connection error.");
}

void MainWindow::fillFavList()
{
    QFile file("favs.txt");
    file.open(QIODevice::ReadWrite|QIODevice::Text);
    QTextStream in(&file);
    QString line = in.readLine();
    while(line!=NULL)
    {
        lista.append(line);
        QString X=line.mid(0,line.indexOf('\t'));
        line=line.mid(line.indexOf('\t')+1,-1);
        QString Y=line.mid(0,line.indexOf('\t'));
        line=line.mid(line.indexOf('\t')+1,-1);
        QString Z=line.mid(0,line.indexOf('\t'));
        line=line.mid(line.indexOf('\t')+1,-1).trimmed();
        line.replace(QRegExp("\\t+")," ");
        line.append(" (");
        line.append(X).append(", ");
        line.append(Y).append(", ");
        line.append(Z).append(")");
        ui->comboFav->addItem(line);
        line=in.readLine();
    }
    file.close();
    connect(ui->comboFav,SIGNAL(currentIndexChanged(int)),this,SLOT(selectFav(int)));
}

void MainWindow::getOptions()
{
    opt.readFile();
    opt.exec();

}

void MainWindow::gotoToolChange()
{
#ifndef DISCONNECTED
    QString line = "G00 X";
    line.append(QString::number(toolChangeXYZ[0])).append(" Y");
    line.append(QString::number(toolChangeXYZ[1])).append(" Z");
    line.append(QString::number(toolChangeXYZ[2])).append(" F250").append("\r");
    SendGcode(line);
#endif
}

void MainWindow::gotoHome()
{
#ifndef DISCONNECTED
    SendGcode("G28\r");
    //SendGcode("G00 X0 Y0 Z0 F250\n\r");
#endif
}

void MainWindow::gotoXYZ()
{
    QString line = ui->Command->text().append("\r");
#ifndef DISCONNECTED
    if(SendGcode(line))
    {
        ui->statusList->addItem(line.mid(0,line.length()-2));
        line=line.mid(line.indexOf("X",Qt::CaseInsensitive)+1,-1);
        ui->lcdNumberX->display((line.mid(0,line.indexOf(" ")).toFloat()));
    }
    else
    {
        ui->statusList->addItem("Bad command.");
    }
#else
    ui->statusList->addItem(line.mid(0,line.length()-2));
#endif
    if(ui->statusList->count()>LINE_COUNT)
        delete ui->statusList->item(0);
    ui->Command->setText("");
}

void MainWindow::incX()
{
    float coord=ui->comboStep->currentText().toFloat();
    if(invX)
        coord=-coord;
    if(SendJog(QString("G01 X")
               .append(QString::number(coord))))
    {
        ui->lcdNumberX->display(ui->lcdNumberX->value()+coord);
        ui->centralWidget->setStatusTip("");
    }
    else
        ui->centralWidget->setStatusTip("Connection error.");
}

void MainWindow::incY()
{
    float coord=ui->comboStep->currentText().toFloat();
    if(invY)
        coord=-coord;
    if(SendJog(QString("G01 Y")
               .append(QString::number(coord))))
    {
        ui->lcdNumberY->display(ui->lcdNumberY->value()+coord);
        ui->centralWidget->setStatusTip("");
    }
    else
        ui->centralWidget->setStatusTip("Connection error.");
}

void MainWindow::incZ()
{
    float coord=ui->comboStep->currentText().toFloat();
    if(invZ)
        coord=-coord;
    if(SendJog(QString("G01 Z")
               .append(QString::number(coord).append(" F260"))))
    {
        ui->lcdNumberZ->display(ui->lcdNumberZ->value()+coord);
        ui->centralWidget->setStatusTip("");
    }
    else
        ui->centralWidget->setStatusTip("Connection error.");
}

void MainWindow::manualRBtn()
{
    if(ui->rbutManual->isChecked())
    {
        ui->groupBoxAxis->setEnabled(false);
        ui->groupBoxSend->setEnabled(false);
        ui->groupBoxFav->setEnabled(true);
    }
}

void MainWindow::openFile()
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

void MainWindow::openPort()
{
    port_nr=ui->cmbPort->currentIndex();
    if(!portOpen)
    {
#ifndef DISCONNECTED
        port.Reset(port_nr);
        if(!port.OpenComport(port_nr))
        {
            ui->groupBoxOptions->setEnabled(true);
            ui->btnGRBL->setEnabled(true);
            ui->statusList->setEnabled(true);
            portOpen=true;
            ui->btnOpenPort->setText("Close");
            ui->btnOpenPort->setStyleSheet("* { background-color: rgb(255,125,100) }");
            ui->cmbPort->setEnabled(false);
            ui->groupBoxSend->setEnabled(true);
        }
        else
            QMessageBox(QMessageBox::Critical,"Error","Could not open port.",QMessageBox::Ok).exec();
#else
        ui->groupBoxOptions->setEnabled(true);
        ui->btnGRBL->setEnabled(true);
        ui->statusList->setEnabled(true);
        portOpen=true;
        ui->btnOpenPort->setText("Close");
        ui->btnOpenPort->setStyleSheet("* { background-color: rgb(255,125,100) }");
        ui->cmbPort->setEnabled(false);
        ui->groupBoxSend->setEnabled(true);
#endif
    }
    else
    {
#ifndef DISCONNECTED
        port.CloseComport(port_nr);
#endif
        ui->rbutSend->toggle();
        ui->cmbPort->setEnabled(true);
        ui->btnOpenPort->setText("Open");
        ui->btnOpenPort->setStyleSheet(styleSheet);
        ui->groupBoxOptions->setEnabled(false);
        ui->groupBoxSend->setEnabled(false);
        ui->btnGRBL->setEnabled(false);
        ui->statusList->setEnabled(false);
        portOpen=false;
    }
}

void MainWindow::readSettings()//carga de archivo
{
    QFile file(SETTINGS);
    if(!file.exists())
    {
        file.open(QIODevice::WriteOnly|QIODevice::Text);
        QTextStream out(&file);
        out<<"1\n";
        out<<"0\t0\t0\n";
        file.close();
    }
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream in(&file);
    int settings = in.readLine().toInt();
    QStringList coord = in.readLine().split(QRegExp("\\t"),QString::SkipEmptyParts);
    this->goHome=settings&1;
    this->toolChange=settings&2;
    this->invX=settings&4;
    this->invY=settings&8;
    this->invZ=settings&16;
    this->toolChangeXYZ[0]=coord.at(0).toFloat();
    this->toolChangeXYZ[1]=coord.at(1).toFloat();
    this->toolChangeXYZ[2]=coord.at(2).toFloat();
    file.close();
    ui->btnGoChange->setEnabled(toolChange);
}

void MainWindow::receiveAxis(QString axis)
{
    axis.replace(QRegExp("\\s+")," ");
    UpdateAxis(axis.trimmed());
    ui->statusList->addItem(axis.trimmed());
    if(ui->statusList->count()>LINE_COUNT)
    {
        delete ui->statusList->item(0);
    }
}

void MainWindow::receiveList(QString msg)
{
    ui->statusList->addItem(msg.trimmed());
    if(ui->statusList->count()>LINE_COUNT)
    {
        delete ui->statusList->item(0);
    }
    MainWindow::repaint();
}

void MainWindow::receiveMsg(QString msg)
{
    ui->centralWidget->setStatusTip(msg);
}

void MainWindow::reset()
{
    ui->lcdNumberX->display(0);
    ui->lcdNumberY->display(0);
    ui->lcdNumberZ->display(0);
#ifndef DISCONNECTED
    RS232().Reset(ui->cmbPort->currentIndex());
    //ui->btnOpenPort->click();
#endif
}

void MainWindow::selectFav(int selected)
{
    if(selected>0)
    {
        disconnect(ui->comboFav,SIGNAL(currentIndexChanged(int)),this,SLOT(selectFav(int)));
        QString strline = lista.at(selected-1);
        strline = QString("G00 X").append(strline.left(strline.lastIndexOf('\t')));
        strline.replace(QRegExp("\\t+")," Y");
        strline.replace(strline.lastIndexOf('Y'),1,"Z");
        strline.append('\r');
#ifndef DISCONNECTED
        SendGcode(strline);
#endif
        ui->comboFav->setCurrentIndex(0);
        connect(ui->comboFav,SIGNAL(currentIndexChanged(int)),this,SLOT(selectFav(int)));
    }
}

int MainWindow::SendGcode(QString line)
{
    QString received="";
    port.flush(port_nr);
    int i, n=0;
    //line.append('\r');
    char buf[50];
    for(i=0;i<50;i++)
        buf[i]=0;
    for(i=0;i<line.length();i++)
        buf[i]=line.at(i).toAscii();

    if(!port.SendBuf(port_nr,buf,line.length()))
        return 0;
    else
    {
#ifdef DEBUGER
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
        do
        {
            n = port.PollComport(port_nr, buf, 4);
            received=QString(buf);
#ifdef Q_WS_X11
            usleep(100000);  // sleep for 100 milliSeconds
#else
            Sleep(100);
#endif
        }while((!received.contains("ok",Qt::CaseInsensitive))&&(n>0));
        if(n==0)
            return(0);
    }
    return(1);
}

int MainWindow::SendJog(QString strline)
{
#ifndef DISCONNECTED

    if((SendGcode("G91\r"))&&(SendGcode(strline.append("\r"))))
        return(1);
    else
        return (0);
#else
    return(1);
#endif
}

void MainWindow::sendRBtn()
{
    if(ui->rbutSend->isChecked())
    {
        ui->groupBoxAxis->setEnabled(false);
        ui->groupBoxSend->setEnabled(true);
        ui->groupBoxFav->setEnabled(false);
    }
}

void MainWindow::setGRBL()
{
    GrblDialog dial(this);
    dial.port_nr = ui->cmbPort->currentIndex();
    dial.port = this->port;
    dial.setParent(this);
    port.flush(port_nr);
    //if(dial.setSettings())
    dial.setSettings();
        dial.exec();
}

void MainWindow::setSettings(int settings)//recibe cambios
{
    this->goHome=settings&1;
    this->toolChange=settings&2;
    this->invX=settings&4;
    this->invY=settings&8;
    this->invZ=settings&16;
    ui->btnGoChange->setEnabled(toolChange);
}

void MainWindow::setTCCoord(float coords[])//recibe cambios
{
    this->toolChangeXYZ[0]=coords[0];
    this->toolChangeXYZ[1]=coords[1];
    this->toolChangeXYZ[2]=coords[2];
}

void MainWindow::showAbout()
{
    About about(this);
    about.exec();
}

void MainWindow::stop()
{
    ui->centralWidget->setStatusTip("Process stopped.");
    emit Stop();
    RS232().Reset(port_nr);
}

void MainWindow::toggleSpindle()
{
    if(ui->SpindleOn->QAbstractButton::isChecked())
    {
#ifndef DISCONNECTED
        SendGcode("M03\r");
        //SendGcode(line,4);
#endif
        ui->statusList->addItem("Spindle On.");
        if(ui->statusList->count()>LINE_COUNT)
        {
            delete ui->statusList->item(0);
        }
    }
    else
    {
#ifndef DISCONNECTED
        //SendGcode(line,4);
        SendGcode("M05\r");
#endif
        ui->statusList->addItem("Spindle Off.");
        if(ui->statusList->count()>LINE_COUNT)
        {
            delete ui->statusList->item(0);
        }
    }
}

void MainWindow::UpdateAxis(QString code)
{
    QStringList list;
    code = code.trimmed();
    code.toUpper();
    if(code.indexOf(QRegExp("[XYZ]"))!=-1)
    {
        list = code.split(QRegExp("\\s+"),QString::SkipEmptyParts);
        QString s;
        foreach(s,list)
        {
            if(s.indexOf('X')!=-1)
            {
                ui->lcdNumberX->display(s.mid(1,-1).toFloat());
            }
            else if(s.indexOf('Y')!=-1)
            {
                ui->lcdNumberY->display(s.mid(1,-1).toFloat());
            }
            else if(s.indexOf('Z')!=-1)
            {
                ui->lcdNumberZ->display(s.mid(1,-1).toFloat());
            }
            else
            {}
        }
    }
}
