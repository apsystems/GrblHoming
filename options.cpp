/****************************************************************
 * options.cpp
 * GrblHoming - zapmaker fork on github
 *
 * 15 Nov 2012
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#include "options.h"
#include "ui_options.h"

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);

    QSettings settings;

    QString invX = settings.value(SETTINGS_INVERSE_X, "false").value<QString>();
    QString invY = settings.value(SETTINGS_INVERSE_Y, "false").value<QString>();
    QString invZ = settings.value(SETTINGS_INVERSE_Z, "false").value<QString>();

    ui->chkInvX->setChecked(invX == "true");
    ui->chkInvY->setChecked(invY == "true");
    ui->chkInvZ->setChecked(invZ == "true");

    int waitTime = settings.value(SETTINGS_RESPONSE_WAIT_TIME, 100).value<int>();
    ui->spinResponseWaitSec->setValue(waitTime);

    int zJogRate = settings.value(SETTINGS_Z_JOG_RATE, DEFAULT_Z_JOG_RATE).value<int>();
    ui->spinZJogRate->setValue(zJogRate);
}

Options::~Options()
{
    delete ui;
}

void Options::accept()
{
    QSettings settings;

    settings.setValue(SETTINGS_INVERSE_X, ui->chkInvX->isChecked());
    settings.setValue(SETTINGS_INVERSE_Y, ui->chkInvY->isChecked());
    settings.setValue(SETTINGS_INVERSE_Z, ui->chkInvZ->isChecked());

    settings.setValue(SETTINGS_RESPONSE_WAIT_TIME, ui->spinResponseWaitSec->value());
    settings.setValue(SETTINGS_Z_JOG_RATE, ui->spinZJogRate->value());

    connect(this, SIGNAL(setSettings()), parentWidget(), SLOT(setSettings()));

    emit setSettings();
    this->close();
}
