#include "options.h"
#include "ui_options.h"

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
    readFile();
    connect(ui->chkToolChange,SIGNAL(toggled(bool)),this,SLOT(enableToolsAddr(bool)));
    ui->XYZframe->setEnabled(ui->chkToolChange->checkState());
}

Options::~Options()
{
    delete ui;
}

void Options::enableToolsAddr(bool status)
{
        ui->XYZframe->setEnabled(status);
}

void Options::accept()
{
    float coords[3];
    settings=0;
    if(ui->chkGoHome->checkState())
        settings+=1;
    if(ui->chkToolChange->checkState())
        settings+=2;
    if(ui->chkInvX->checkState())
        settings+=4;
    if(ui->chkInvY->checkState())
        settings+=8;
    if(ui->chkInvZ->checkState())
        settings+=16;
#ifdef DEBUG
    coords[0]=1.23;
    coords[1]=2.4;
    coords[2]=-2.1;
    writeFile();
    emit sendSettings(this->settings);
    emit toolCoord(coords);
    this->close();
#else
    bool ok1,ok2,ok3;
    coords[0]=ui->lineEditTCX->text().toFloat(&ok1);
    coords[1]=ui->lineEditTCY->text().toFloat(&ok2);
    coords[2]=ui->lineEditTCZ->text().toFloat(&ok3);

    if(ok1&&ok2&&ok3)
    {
        writeFile();
        emit sendSettings(this->settings);
        emit toolCoord(coords);
        this->close();
    }
    else
    {
        if(!ok1)
        {
            QMessageBox(QMessageBox::Information,"Error","Invalid X value",QMessageBox::Ok).exec();
            ui->lineEditTCX->setFocus();
        }
        else if(!ok2)
        {
            QMessageBox(QMessageBox::Information,"Error","Invalid Y value",QMessageBox::Ok).exec();
            ui->lineEditTCY->setFocus();
        }
        else
        {
            QMessageBox(QMessageBox::Information,"Error","Invalid Z value",QMessageBox::Ok).exec();
            ui->lineEditTCZ->setFocus();
        }
    }
#endif
}

void Options::readFile()
{
    QFile file(SETTINGS);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream in(&file);
    this->settings =in.readLine().toInt();
    int settings = this->settings;
    QStringList coord = in.readLine().split(QRegExp("\\t"),QString::SkipEmptyParts);
    ui->chkGoHome->setChecked(settings&1);
    ui->chkToolChange->setChecked(settings&2);
    ui->chkInvX->setChecked(settings&4);
    ui->chkInvY->setChecked(settings&8);
    ui->chkInvZ->setChecked(settings&16);
    ui->lineEditTCX->setText(coord.at(0));
    ui->lineEditTCY->setText(coord.at(1));
    ui->lineEditTCZ->setText(coord.at(2));
    file.close();
}

void Options::writeFile()
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
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream out(&file);
    out<<QString(QString::number(settings)).append("\n");
    QString coords = ui->lineEditTCX->text();
    coords.append("\t").append(ui->lineEditTCY->text());
    coords.append("\t").append(ui->lineEditTCZ->text());
    out<<coords;
    file.close();
}
