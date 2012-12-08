/****************************************************************
 * mainwindow.cpp
 * GrblHoming - zapmaker fork on github
 *
 * 15 Nov 2012
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

//#define DEBUG

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Setup our application information to be used by QSettings
    QCoreApplication::setOrganizationName(COMPANY_NAME);
    QCoreApplication::setOrganizationDomain(DOMAIN_NAME);
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    ui->setupUi(this);

    readSettings();

    // see http://blog.qt.digia.com/2010/06/17/youre-doing-it-wrong/
    // The thread points out that the documentation for QThread is wrong :) and
    // you should NOT subclass from QThread and override run(), rather,
    // attach your QOBJECT to a thread and use events (signals/slots) to communicate.
    gcode.moveToThread(&gcodeThread);
    timer.moveToThread(&timerThread);

    //buttons
    connect(ui->btnOpenPort,SIGNAL(clicked()),this,SLOT(openPort()));
    connect(ui->btnGRBL,SIGNAL(clicked()),this,SLOT(setGRBL()));
    connect(ui->DecXBtn,SIGNAL(clicked()),this,SLOT(decX()));
    connect(ui->DecYBtn,SIGNAL(clicked()),this,SLOT(decY()));
    connect(ui->DecZBtn,SIGNAL(clicked()),this,SLOT(decZ()));
    connect(ui->IncXBtn,SIGNAL(clicked()),this,SLOT(incX()));
    connect(ui->IncYBtn,SIGNAL(clicked()),this,SLOT(incY()));
    connect(ui->IncZBtn,SIGNAL(clicked()),this,SLOT(incZ()));
    connect(ui->btnSetHome,SIGNAL(clicked()),this,SLOT(setHome()));
    connect(ui->Command,SIGNAL(editingFinished()),this,SLOT(gotoXYZ()));
    connect(ui->Begin,SIGNAL(clicked()),this,SLOT(begin()));
    connect(ui->openFile,SIGNAL(clicked()),this,SLOT(openFile()));
    connect(ui->Stop,SIGNAL(clicked()),this,SLOT(stop()));
    connect(ui->SpindleOn,SIGNAL(toggled(bool)),this,SLOT(toggleSpindle()));
    connect(ui->actionOptions,SIGNAL(triggered()),this,SLOT(getOptions()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(showAbout()));

    connect(this, SIGNAL(sendFile(QString)), &gcode, SLOT(sendFile(QString)));
    connect(this, SIGNAL(openPort(QString)), &gcode, SLOT(openPort(QString)));
    connect(this, SIGNAL(closePort(bool)), &gcode, SLOT(closePort(bool)));
    connect(this, SIGNAL(sendGcode(QString)), &gcode, SLOT(sendGcode(QString)));
    connect(this, SIGNAL(gotoXYZ(QString)), &gcode, SLOT(gotoXYZ(QString)));
    connect(this, SIGNAL(axisAdj(char, float, bool, float)), &gcode, SLOT(axisAdj(char, float, bool, float)));
    connect(this, SIGNAL(setResponseWait(int, int)), &gcode, SLOT(setResponseWait(int, int)));
    connect(this, SIGNAL(shutdown()), &gcodeThread, SLOT(quit()));
    connect(this, SIGNAL(shutdown()), &timerThread, SLOT(quit()));
    connect(this, SIGNAL(setProgress(int)), ui->progressFileSend, SLOT(setValue(int)));
    connect(this, SIGNAL(setRuntime(QString)), ui->outputRuntime, SLOT(setText(QString)));

    connect(&gcode, SIGNAL(sendMsg(QString)),this,SLOT(receiveMsg(QString)));
    connect(&gcode, SIGNAL(sendAxis(QString)),this,SLOT(receiveAxis(QString)));
    connect(&gcode, SIGNAL(portIsClosed(bool)), this, SLOT(portIsClosed(bool)));
    connect(&gcode, SIGNAL(portIsOpen(bool)), this, SLOT(portIsOpen(bool)));
    connect(&gcode, SIGNAL(addList(QString)),this,SLOT(receiveList(QString)));
    connect(&gcode, SIGNAL(addListOut(QString)),this,SLOT(receiveListOut(QString)));
    connect(&gcode, SIGNAL(stopSending(bool)), this, SLOT(stopSending(bool)));
    connect(&gcode, SIGNAL(lcdDisplay(char, float)), this, SLOT(lcdDisplay(char, float)));
    connect(&gcode, SIGNAL(setCommandText(QString)), ui->Command, SLOT(setText(QString)));
    connect(&gcode, SIGNAL(setProgress(int)), ui->progressFileSend, SLOT(setValue(int)));
    connect(&gcode, SIGNAL(adjustedAxis()), this, SLOT(adjustedAxis()));
    connect(&gcode, SIGNAL(resetTimer(bool)), &timer, SLOT(resetTimer(bool)));
    connect(&gcode, SIGNAL(resetLcds()), this, SLOT(resetLcds()));

    connect(&timer, SIGNAL(setRuntime(QString)), ui->outputRuntime, SLOT(setText(QString)));

    timerThread.start();
    gcodeThread.start();

    ui->comboStep->addItem("0.01");
    ui->comboStep->addItem("0.1");
    ui->comboStep->addItem("1");
    ui->comboStep->addItem("10");
    ui->comboStep->setCurrentIndex(2);

    // Cool utility class off Google code that enumerates COM ports in platform-independent manner
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();

    int portIndex = 0;
    for (int i = 0; i < ports.size(); i++)
    {
        ui->cmbPort->addItem(ports.at(i).portName.toLocal8Bit().constData());

        if (ports.at(i).portName == lastOpenPort)
            portIndex = i;

        //diag("port name: %s\n", ports.at(i).portName.toLocal8Bit().constData());
        //diag("friendly name: %s\n", ports.at(i).friendName.toLocal8Bit().constData());
        //diag("physical name: %s\n", ports.at(i).physName.toLocal8Bit().constData());
        //diag("enumerator name: %s\n", ports.at(i).enumName.toLocal8Bit().constData());
        //diag("===================================\n\n");
    }

    if (ports.size() > 0)
        ui->cmbPort->setCurrentIndex(portIndex);

    ui->groupBoxAxis->setEnabled(false);
    ui->groupBoxSend->setEnabled(false);
    ui->groupBoxFav->setEnabled(false);
    ui->Begin->setEnabled(false);
    ui->Stop->setEnabled(false);
    ui->progressFileSend->setEnabled(false);
    ui->outputRuntime->setEnabled(false);
    ui->labelRuntime->setEnabled(false);
    ui->btnGRBL->setEnabled(false);
    ui->btnSetHome->setEnabled(false);
    styleSheet = ui->btnOpenPort->styleSheet();
    ui->statusList->setEnabled(true);

    emit setResponseWait(waitTime, zJogRate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// called when user has clicked the close application button
void MainWindow::closeEvent(QCloseEvent *event)
{
    gcode.setShutdown();
    gcode.setAbort();
    gcode.setReset();

    writeSettings();

    SLEEP(300);

    emit shutdown();

    event->accept();
}

void MainWindow::begin()
{
    //receiveList("Starting File Send.");
    resetProgress();

    ui->groupBoxAxis->setEnabled(false);
    ui->groupBoxFav->setEnabled(false);

    ui->Begin->setEnabled(false);
    ui->Stop->setEnabled(true);
    ui->progressFileSend->setEnabled(true);
    ui->outputRuntime->setEnabled(true);
    ui->labelRuntime->setEnabled(true);
    ui->openFile->setEnabled(false);
    ui->btnGRBL->setEnabled(false);
    emit sendFile(ui->filePath->text());
}

void MainWindow::stop()
{
    // Calling openPortCtl will reset the COM port which resets the controller back to 0
    openPortCtl(true);
    /*
    gcode.setAbort();

    ui->groupBoxAxis->setEnabled(false);
    ui->groupBoxFav->setEnabled(false);
    ui->Begin->setEnabled(false);
    ui->Stop->setEnabled(false);
    ui->progressFileSend->setEnabled(false);
    ui->outputRuntime->setEnabled(false);
    ui->labelRuntime->setEnabled(false);
    ui->openFile->setEnabled(false);
    */
}

// slot called from GCode class to update our state
void MainWindow::stopSending(bool resetPort)
{
    ui->groupBoxAxis->setEnabled(true);
    ui->groupBoxFav->setEnabled(true);
    ui->Begin->setEnabled(true);
    ui->Stop->setEnabled(false);
    ui->progressFileSend->setEnabled(false);
    ui->outputRuntime->setEnabled(false);
    ui->labelRuntime->setEnabled(false);
    ui->btnOpenPort->setEnabled(true);
    ui->btnGRBL->setEnabled(true);
    ui->openFile->setEnabled(true);

    if (resetPort)
    {
        openPortCtl(true);
    }
}

// User has asked to open the port
void MainWindow::openPort()
{
    openPortCtl(false);
}

// User has asked to set current position as 'home' = 0,0,0
// We reset the COM port which resets the controller to ensure it is zeroed out
void MainWindow::setHome()
{
    resetProgress();
    openPortCtl(true);
}

// Just resets the LCDs to 0
void MainWindow::resetLcds()
{
    ui->lcdNumberX->display(0);
    ui->lcdNumberY->display(0);
    ui->lcdNumberZ->display(0);
}

void MainWindow::resetProgress()
{
    setProgress(0);
    setRuntime("");
}

// If the port isn't open, we ask to open it
// If the port is open, we close it, but if 'reopen' is
// true, we call back to this thread to reopen it which
// is done mainly to toggle the COM port state to reset
// the controller.
void MainWindow::openPortCtl(bool reopen)
{
    if (ui->btnOpenPort->text() == "Open")
    {
        // Port is closed if the button says 'Open'
        QString portStr = ui->cmbPort->currentText();
        ui->btnOpenPort->setEnabled(false);
        emit openPort(portStr);
    }
    else
    {
        if (!reopen)
            resetProgress();

        // presume button says 'Close' currently, meaning port is open

        // Tell gcode port thread to stop what it is doing immediately (within 0.1 sec)
        gcode.setAbort();
        gcode.setReset();

        // Disable a bunch of UI
        ui->Begin->setEnabled(false);
        ui->Stop->setEnabled(false);
        ui->progressFileSend->setEnabled(false);
        ui->outputRuntime->setEnabled(false);
        ui->labelRuntime->setEnabled(false);
        ui->btnOpenPort->setEnabled(false);
        ui->openFile->setEnabled(false);

        ui->groupBoxAxis->setEnabled(false);
        ui->groupBoxSend->setEnabled(false);
        ui->groupBoxFav->setEnabled(false);
        ui->cmbPort->setEnabled(false);
        ui->btnOpenPort->setEnabled(false);
        ui->btnGRBL->setEnabled(false);

        // Send event to close the port
        emit closePort(reopen);
    }
}

// slot telling us that port was closed successfully
// if 'reopen' is true, reopen our port to toggle
// so we reset the controller
void MainWindow::portIsClosed(bool reopen)
{
    SLEEP(100);

    ui->groupBoxAxis->setEnabled(false);
    ui->groupBoxSend->setEnabled(false);
    ui->groupBoxFav->setEnabled(false);
    ui->cmbPort->setEnabled(true);
    ui->btnOpenPort->setEnabled(true);
    ui->btnOpenPort->setText("Open");
    ui->btnOpenPort->setStyleSheet(styleSheet);
    ui->btnGRBL->setEnabled(false);
    ui->btnSetHome->setEnabled(false);

    if (reopen)
    {
        receiveList("Resetting port to restart controller");
        openPortCtl(false);
    }
}

// slot that tells us the gcode thread successfully opened the port
void MainWindow::portIsOpen(bool sendCode)
{
    // Comm port successfully opened

    ui->btnGRBL->setEnabled(true);
    ui->btnOpenPort->setEnabled(true);
    ui->btnOpenPort->setText("Close");
    ui->btnOpenPort->setStyleSheet("* { background-color: rgb(255,125,100) }");
    ui->cmbPort->setEnabled(false);
    ui->groupBoxAxis->setEnabled(true);
    ui->groupBoxSend->setEnabled(true);
    ui->groupBoxFav->setEnabled(true);
    ui->openFile->setEnabled(true);
    ui->btnSetHome->setEnabled(true);

    if (ui->filePath->text().length() > 0)
    {
        ui->Begin->setEnabled(true);
        ui->Stop->setEnabled(false);
        ui->progressFileSend->setEnabled(false);
        ui->outputRuntime->setEnabled(false);
        ui->labelRuntime->setEnabled(false);
    }
    else
    {
        ui->Begin->setEnabled(false);
        ui->Stop->setEnabled(false);
        ui->progressFileSend->setEnabled(false);
        ui->outputRuntime->setEnabled(false);
        ui->labelRuntime->setEnabled(false);
    }

    resetLcds();

    if (sendCode)
        sendGcode("");
}

void MainWindow::adjustedAxis()
{
    ui->groupBoxAxis->setEnabled(true);
    ui->groupBoxFav->setEnabled(true);

    if (ui->filePath->text().length() > 0)
        ui->Begin->setEnabled(true);

    ui->Stop->setEnabled(false);
    ui->progressFileSend->setEnabled(false);
    ui->outputRuntime->setEnabled(false);
    ui->labelRuntime->setEnabled(false);

    ui->btnOpenPort->setEnabled(true);
    ui->openFile->setEnabled(true);
}

void MainWindow::disableAllButtons()
{
    ui->groupBoxAxis->setEnabled(false);
    ui->groupBoxFav->setEnabled(false);
    ui->Begin->setEnabled(false);
    ui->Stop->setEnabled(false);
    ui->progressFileSend->setEnabled(false);
    ui->outputRuntime->setEnabled(false);
    ui->labelRuntime->setEnabled(false);
    ui->openFile->setEnabled(false);
    ui->btnGRBL->setEnabled(false);
}

void MainWindow::incX()
{
    float coord = ui->comboStep->currentText().toFloat();

    disableAllButtons();

    emit axisAdj('X', coord, invX, ui->lcdNumberX->value());
}

void MainWindow::incY()
{
    float coord = ui->comboStep->currentText().toFloat();

    disableAllButtons();

    emit axisAdj('Y', coord, invY, ui->lcdNumberY->value());
}

void MainWindow::incZ()
{
    float coord = ui->comboStep->currentText().toFloat();

    disableAllButtons();

    emit axisAdj('Z', coord, invZ, ui->lcdNumberZ->value());
}

void MainWindow::decX()
{
    float coord = -ui->comboStep->currentText().toFloat();

    disableAllButtons();

    emit axisAdj('X', coord, invX, ui->lcdNumberX->value());
}

void MainWindow::decY()
{
    float coord = -ui->comboStep->currentText().toFloat();

    disableAllButtons();

    emit axisAdj('Y', coord, invY, ui->lcdNumberY->value());
}

void MainWindow::decZ()
{
    float coord = -ui->comboStep->currentText().toFloat();

    disableAllButtons();

    emit axisAdj('Z', coord, invZ, ui->lcdNumberZ->value());
}

void MainWindow::getOptions()
{
    Options opt(this);
    opt.exec();
}

void MainWindow::gotoXYZ()
{
    if (ui->Command->text().length() == 0)
        return;

    QString line = ui->Command->text().append("\r");

    emit gotoXYZ(line);
}

void MainWindow::openFile()
{
    QFileDialog dialog(this, tr("Open File"),
                       directory,
                       tr("NC (*.nc);;All Files (*.*)"));

    dialog.setFileMode(QFileDialog::ExistingFile);

    if (nameFilter.size() > 0)
        dialog.selectNameFilter(nameFilter);

    if (fileOpenDialogState.size() > 0)
        dialog.restoreState(fileOpenDialogState);

    QString fileName;
    QStringList fileNames;
    if (dialog.exec())
    {
        fileOpenDialogState = dialog.saveState();

        fileNames = dialog.selectedFiles();
        if (fileNames.length() > 0)
            fileName = fileNames.at(0);

        nameFilter = dialog.selectedNameFilter();

        resetProgress();
    }

    int slash = fileName.lastIndexOf('/');
    if (slash == -1)
    {
        slash = fileName.lastIndexOf('\\');
    }

    directory = "";
    if (slash != -1)
    {
        directory = fileName.left(slash);
    }

    ui->filePath->setText(fileName);
    if(ui->filePath->text()!="")
    {
        ui->Begin->setEnabled(true);
        ui->Stop->setEnabled(false);
        ui->progressFileSend->setEnabled(false);
        ui->outputRuntime->setEnabled(false);
        ui->labelRuntime->setEnabled(false);
    }
    else
    {
        ui->Begin->setEnabled(false);
        ui->Stop->setEnabled(false);
        ui->progressFileSend->setEnabled(false);
        ui->outputRuntime->setEnabled(false);
        ui->labelRuntime->setEnabled(false);
    }
}

void MainWindow::readSettings()//carga de archivo
{
    // use platform-independent settings storage, i.e. registry under Windows
    QSettings settings;

    fileOpenDialogState = settings.value(SETTINGS_FILE_OPEN_DIALOG_STATE).value<QByteArray>();
    directory = settings.value(SETTINGS_DIRECTORY).value<QString>();
    nameFilter = settings.value(SETTINGS_NAME_FILTER).value<QString>();
    lastOpenPort = settings.value(SETTINGS_PORT).value<QString>();

    QString sinvX = settings.value(SETTINGS_INVERSE_X, "false").value<QString>();
    QString sinvY = settings.value(SETTINGS_INVERSE_Y, "false").value<QString>();
    QString sinvZ = settings.value(SETTINGS_INVERSE_Z, "false").value<QString>();

    invX = sinvX == "true";
    invY = sinvY == "true";
    invZ = sinvZ == "true";

    waitTime = settings.value(SETTINGS_RESPONSE_WAIT_TIME, 100).value<int>();
    zJogRate = settings.value(SETTINGS_Z_JOG_RATE, DEFAULT_Z_JOG_RATE).value<int>();
}

// Slot called from settings dialog after user made a change. Reload settings from registry.
void MainWindow::setSettings()
{
    QSettings settings;

    QString sinvX = settings.value(SETTINGS_INVERSE_X, "false").value<QString>();
    QString sinvY = settings.value(SETTINGS_INVERSE_Y, "false").value<QString>();
    QString sinvZ = settings.value(SETTINGS_INVERSE_Z, "false").value<QString>();

    invX = sinvX == "true";
    invY = sinvY == "true";
    invZ = sinvZ == "true";

    waitTime = settings.value(SETTINGS_RESPONSE_WAIT_TIME, 100).value<int>();
    zJogRate = settings.value(SETTINGS_Z_JOG_RATE, DEFAULT_Z_JOG_RATE).value<int>();

    // update gcode thread with latest values
    emit setResponseWait(waitTime, zJogRate);
}

// save last state of settings
void MainWindow::writeSettings()
{
    QSettings settings;

    settings.setValue(SETTINGS_FILE_OPEN_DIALOG_STATE, fileOpenDialogState);
    settings.setValue(SETTINGS_NAME_FILTER, nameFilter);
    settings.setValue(SETTINGS_DIRECTORY, directory);
    settings.setValue(SETTINGS_PORT, ui->cmbPort->currentText());
}

void MainWindow::receiveAxis(QString axis)
{
    axis.replace(QRegExp("\\s+")," ");
    UpdateAxis(axis.trimmed());
}

void MainWindow::receiveList(QString msg)
{
    addToStatusList(true, msg);
}

void MainWindow::receiveListOut(QString msg)
{
    addToStatusList(false, msg);
}

void MainWindow::addToStatusList(bool in, QString msg)
{
    msg.trimmed();
    msg.remove('\r');
    msg.remove('\n');

    if (msg.length() == 0)
        return;

    if (in)
        ui->statusList->addItem(msg);
    else
        ui->statusList->addItem("> " + msg);
    ui->statusList->scrollToBottom();
    MainWindow::repaint();
}

void MainWindow::receiveMsg(QString msg)
{
    ui->centralWidget->setStatusTip(msg);
}

void MainWindow::setGRBL()
{
    GrblDialog dlg(this, &gcode);
    dlg.setParent(this);
    dlg.getSettings();
    dlg.exec();
}

void MainWindow::showAbout()
{
    About about(this);
    about.exec();
}

void MainWindow::toggleSpindle()
{
    if(ui->SpindleOn->QAbstractButton::isChecked())
    {
        sendGcode("M03\r");
        receiveList("Spindle On.");
    }
    else
    {
        sendGcode("M05\r");
        receiveList("Spindle Off.");
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

void MainWindow::lcdDisplay(char axis, float value)
{
    switch (axis)
    {
    case 'X':
        ui->lcdNumberX->display(value);
        break;
    case 'Y':
        ui->lcdNumberY->display(value);
        break;
    case 'Z':
        ui->lcdNumberZ->display(value);
        break;
    }
}
