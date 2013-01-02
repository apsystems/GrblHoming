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

    connect(ui->checkBoxUseMmManualCmds,SIGNAL(toggled(bool)),this,SLOT(toggleUseMm(bool)));
    connect(ui->chkLimitZRate,SIGNAL(toggled(bool)),this,SLOT(toggleLimitZRate(bool)));

    QSettings settings;

    QString invX = settings.value(SETTINGS_INVERSE_X, "false").value<QString>();
    QString invY = settings.value(SETTINGS_INVERSE_Y, "false").value<QString>();
    QString invZ = settings.value(SETTINGS_INVERSE_Z, "false").value<QString>();

    ui->chkInvX->setChecked(invX == "true");
    ui->chkInvY->setChecked(invY == "true");
    ui->chkInvZ->setChecked(invZ == "true");

    QString enDebugLog = settings.value(SETTINGS_ENABLE_DEBUG_LOG, "false").value<QString>();
    QString useMmManualCmds = settings.value(SETTINGS_USE_MM_FOR_MANUAL_CMDS, "true").value<QString>();

    ui->checkBoxEnableDebugLog->setChecked(enDebugLog == "true");
    ui->checkBoxUseMmManualCmds->setChecked(useMmManualCmds == "true");

    int waitTime = settings.value(SETTINGS_RESPONSE_WAIT_TIME, DEFAULT_WAIT_TIME_SEC).value<int>();
    ui->spinResponseWaitSec->setValue(waitTime);

    double zJogRate = settings.value(SETTINGS_Z_JOG_RATE, DEFAULT_Z_JOG_RATE).value<double>();
    ui->doubleSpinZJogRate->setValue(zJogRate);

    QString zRateLimit = settings.value(SETTINGS_Z_RATE_LIMIT, "false").value<QString>();
    ui->chkLimitZRate->setChecked(zRateLimit == "true");

    double zRateLimitAmount = settings.value(SETTINGS_Z_RATE_LIMIT_AMOUNT, DEFAULT_Z_LIMT_RATE).value<double>();
    ui->doubleSpinZRateLimit->setValue(zRateLimitAmount);

    if (!ui->chkLimitZRate->isChecked())
    {
        ui->doubleSpinZRateLimit->setEnabled(false);
    }
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

    settings.setValue(SETTINGS_ENABLE_DEBUG_LOG, ui->checkBoxEnableDebugLog->isChecked());
    settings.setValue(SETTINGS_USE_MM_FOR_MANUAL_CMDS, ui->checkBoxUseMmManualCmds->isChecked());

    settings.setValue(SETTINGS_RESPONSE_WAIT_TIME, ui->spinResponseWaitSec->value());
    settings.setValue(SETTINGS_Z_JOG_RATE, ui->doubleSpinZJogRate->value());

    settings.setValue(SETTINGS_Z_RATE_LIMIT, ui->chkLimitZRate->isChecked());
    settings.setValue(SETTINGS_Z_RATE_LIMIT_AMOUNT, ui->doubleSpinZRateLimit->value());

    connect(this, SIGNAL(setSettings()), parentWidget(), SLOT(setSettings()));

    emit setSettings();
    this->close();
}

void Options::toggleUseMm(bool useMm)
{
    double zJogRate = ui->doubleSpinZJogRate->value();
    double zRateLimit = ui->doubleSpinZRateLimit->value();

    if (useMm)
    {
        ui->doubleSpinZJogRate->setValue(zJogRate * MM_IN_AN_INCH);
        ui->doubleSpinZRateLimit->setValue(zRateLimit * MM_IN_AN_INCH);
    }
    else
    {
        ui->doubleSpinZJogRate->setValue(zJogRate / MM_IN_AN_INCH);
        ui->doubleSpinZRateLimit->setValue(zRateLimit / MM_IN_AN_INCH);
    }
}

void Options::toggleLimitZRate(bool limitZ)
{
    ui->doubleSpinZRateLimit->setEnabled(limitZ);
}
