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

extern Log4Qt::FileAppender *p_fappender;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    absoluteAfterAxisAdj(false),
    checkLogWrite(false)
{
    // Setup our application information to be used by QSettings
    QCoreApplication::setOrganizationName(COMPANY_NAME);
    QCoreApplication::setOrganizationDomain(DOMAIN_NAME);
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    // required if passing the object by reference into signals/slots
    qRegisterMetaType<Coord3D>("Coord3D");

    ui->setupUi(this);

    readSettings();

    info("%s has started", GRBL_CONTROLLER_NAME_AND_VERSION);

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
    connect(ui->chkRestoreAbsolute,SIGNAL(toggled(bool)),this,SLOT(toggleRestoreAbsolute()));
    connect(ui->actionOptions,SIGNAL(triggered()),this,SLOT(getOptions()));
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(showAbout()));
    connect(ui->btnResetGrbl,SIGNAL(clicked()),this,SLOT(grblReset()));
    connect(ui->btnUnlockGrbl,SIGNAL(clicked()),this,SLOT(grblUnlock()));
    connect(ui->btnGoHomeSafe,SIGNAL(clicked()),this,SLOT(goHomeSafe()));

    connect(this, SIGNAL(sendFile(QString)), &gcode, SLOT(sendFile(QString)));
    connect(this, SIGNAL(openPort(QString)), &gcode, SLOT(openPort(QString)));
    connect(this, SIGNAL(closePort(bool)), &gcode, SLOT(closePort(bool)));
    connect(this, SIGNAL(sendGcode(QString)), &gcode, SLOT(sendGcode(QString)));
    connect(this, SIGNAL(gotoXYZ(QString)), &gcode, SLOT(gotoXYZ(QString)));
    connect(this, SIGNAL(axisAdj(char, float, bool, bool)), &gcode, SLOT(axisAdj(char, float, bool, bool)));
    connect(this, SIGNAL(setResponseWait(int, double, bool, bool, double, double, bool)), &gcode, SLOT(setResponseWait(int, double, bool, bool, double, double, bool)));
    connect(this, SIGNAL(shutdown()), &gcodeThread, SLOT(quit()));
    connect(this, SIGNAL(shutdown()), &timerThread, SLOT(quit()));
    connect(this, SIGNAL(setProgress(int)), ui->progressFileSend, SLOT(setValue(int)));
    connect(this, SIGNAL(setRuntime(QString)), ui->outputRuntime, SLOT(setText(QString)));
    connect(this, SIGNAL(sendSetHome()), &gcode, SLOT(grblSetHome()));
    connect(this, SIGNAL(sendGrblReset()), &gcode, SLOT(sendGrblReset()));
    connect(this, SIGNAL(sendGrblUnlock()), &gcode, SLOT(sendGrblUnlock()));
    connect(this, SIGNAL(goToHome()), &gcode, SLOT(goToHome()));

    connect(&gcode, SIGNAL(sendMsg(QString)),this,SLOT(receiveMsg(QString)));
    connect(&gcode, SIGNAL(portIsClosed(bool)), this, SLOT(portIsClosed(bool)));
    connect(&gcode, SIGNAL(portIsOpen(bool)), this, SLOT(portIsOpen(bool)));
    connect(&gcode, SIGNAL(addList(QString)),this,SLOT(receiveList(QString)));
    connect(&gcode, SIGNAL(addListFull(QStringList)),this,SLOT(receiveListFull(QStringList)));
    connect(&gcode, SIGNAL(addListOut(QString)),this,SLOT(receiveListOut(QString)));
    connect(&gcode, SIGNAL(stopSending()), this, SLOT(stopSending()));
    connect(&gcode, SIGNAL(setCommandText(QString)), ui->Command, SLOT(setText(QString)));
    connect(&gcode, SIGNAL(setProgress(int)), ui->progressFileSend, SLOT(setValue(int)));
    connect(&gcode, SIGNAL(adjustedAxis()), this, SLOT(adjustedAxis()));
    connect(&gcode, SIGNAL(resetTimer(bool)), &timer, SLOT(resetTimer(bool)));
    connect(&gcode, SIGNAL(enableGrblDialogButton()), this, SLOT(enableGrblDialogButton()));
    connect(&gcode, SIGNAL(updateCoordinates(Coord3D,Coord3D)), this, SLOT(updateCoordinates(Coord3D,Coord3D)));
    connect(&gcode, SIGNAL(setLastState(QString)), ui->outputLastState, SLOT(setText(QString)));
    connect(&gcode, SIGNAL(setUnitsWork(QString)), ui->outputUnitsWork, SLOT(setText(QString)));
    connect(&gcode, SIGNAL(setUnitsMachine(QString)), ui->outputUnitsMachine, SLOT(setText(QString)));

    connect(&timer, SIGNAL(setRuntime(QString)), ui->outputRuntime, SLOT(setText(QString)));

    timerThread.start();
    gcodeThread.start();

    ui->comboStep->addItem("0.01");
    ui->comboStep->addItem("0.1");
    ui->comboStep->addItem("1");
    ui->comboStep->addItem("10");
    ui->comboStep->setCurrentIndex(2);

    ui->statusList->setUniformItemSizes(true);
	// Does not work correctly for horizontal scrollbar:
    //MyItemDelegate *scrollDelegate = new MyItemDelegate(ui->statusList);
    //scrollDelegate->setWidth(600);
    //ui->statusList->setItemDelegate(scrollDelegate);

    scrollStatusTimer.start();

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

    ui->groupBoxAxisControl->setEnabled(false);
    ui->groupBoxSendFile->setEnabled(true);
    ui->groupBoxManualGCode->setEnabled(false);
    ui->Begin->setEnabled(false);
    ui->Stop->setEnabled(false);
    ui->progressFileSend->setEnabled(false);
    ui->outputRuntime->setEnabled(false);
    ui->labelRuntime->setEnabled(false);
    ui->btnGRBL->setEnabled(false);
    ui->btnSetHome->setEnabled(false);
    ui->btnResetGrbl->setEnabled(false);
    ui->btnUnlockGrbl->setEnabled(false);
    ui->btnGoHomeSafe->setEnabled(false);
    styleSheet = ui->btnOpenPort->styleSheet();
    ui->statusList->setEnabled(true);
    ui->openFile->setEnabled(true);

    this->setWindowTitle(GRBL_CONTROLLER_NAME_AND_VERSION);

    emit setResponseWait(waitTime, zJogRate, useMm, zRateLimiting, zRateLimitAmount, xyRateAmount, useAggressivePreload);
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

    info("%s has stopped", GRBL_CONTROLLER_NAME_AND_VERSION);

    SLEEP(300);

    emit shutdown();

    event->accept();
}

void MainWindow::begin()
{
    //receiveList("Starting File Send.");
    resetProgress();

    ui->groupBoxAxisControl->setEnabled(false);
    ui->groupBoxManualGCode->setEnabled(false);

    ui->Begin->setEnabled(false);
    ui->Stop->setEnabled(true);
    ui->progressFileSend->setEnabled(true);
    ui->outputRuntime->setEnabled(true);
    ui->labelRuntime->setEnabled(true);
    ui->openFile->setEnabled(false);
    ui->btnGRBL->setEnabled(false);
    ui->btnUnlockGrbl->setEnabled(false);
    ui->btnSetHome->setEnabled(false);
    ui->btnGoHomeSafe->setEnabled(false);
    emit sendFile(ui->filePath->text());
}

void MainWindow::stop()
{
    gcode.setAbort();

    // Reenable a bunch of UI
    ui->Begin->setEnabled(true);
    ui->Stop->setEnabled(false);
    ui->btnGRBL->setEnabled(true);
    ui->btnSetHome->setEnabled(true);
    ui->btnResetGrbl->setEnabled(true);
    ui->btnUnlockGrbl->setEnabled(true);
    ui->btnGoHomeSafe->setEnabled(true);
}

void MainWindow::grblReset()
{
    gcode.setAbort();
    gcode.setReset();
    emit sendGrblReset();
}

void MainWindow::grblUnlock()
{
    emit sendGrblUnlock();
}

void MainWindow::goHomeSafe()
{
    emit goToHome();
}

// slot called from GCode class to update our state
void MainWindow::stopSending()
{
    ui->groupBoxAxisControl->setEnabled(true);
    ui->groupBoxManualGCode->setEnabled(true);
    ui->Begin->setEnabled(true);
    ui->Stop->setEnabled(false);
    ui->progressFileSend->setEnabled(false);
    ui->outputRuntime->setEnabled(false);
    ui->labelRuntime->setEnabled(false);
    ui->btnOpenPort->setEnabled(true);
    ui->btnGRBL->setEnabled(true);
    ui->btnSetHome->setEnabled(true);
    ui->btnResetGrbl->setEnabled(true);
    ui->btnUnlockGrbl->setEnabled(true);
    ui->btnGoHomeSafe->setEnabled(true);
    ui->openFile->setEnabled(true);
}

// User has asked to open the port
void MainWindow::openPort()
{
    openPortCtl(false);
}

// User has asked to set current position as 'home' = 0,0,0
void MainWindow::setHome()
{
    resetProgress();
    sendSetHome();
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

        ui->groupBoxAxisControl->setEnabled(false);
        ui->groupBoxSendFile->setEnabled(false);
        ui->groupBoxManualGCode->setEnabled(false);
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

    ui->groupBoxAxisControl->setEnabled(false);
    ui->groupBoxSendFile->setEnabled(false);
    ui->groupBoxManualGCode->setEnabled(false);
    ui->cmbPort->setEnabled(true);
    ui->btnOpenPort->setEnabled(true);
    ui->btnOpenPort->setText("Open");
    ui->btnOpenPort->setStyleSheet(styleSheet);
    ui->btnGRBL->setEnabled(false);
    ui->btnSetHome->setEnabled(false);
    ui->btnResetGrbl->setEnabled(false);
    ui->btnUnlockGrbl->setEnabled(false);
    ui->btnGoHomeSafe->setEnabled(false);

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
    if (sendCode)
        sendGcode("");
}

void MainWindow::adjustedAxis()
{
    ui->groupBoxAxisControl->setEnabled(true);
    ui->groupBoxManualGCode->setEnabled(true);

    if (ui->filePath->text().length() > 0)
        ui->Begin->setEnabled(true);

    ui->Stop->setEnabled(false);
    ui->progressFileSend->setEnabled(false);
    ui->outputRuntime->setEnabled(false);
    ui->labelRuntime->setEnabled(false);

    ui->btnOpenPort->setEnabled(true);
    ui->openFile->setEnabled(true);
    ui->btnGRBL->setEnabled(true);
    ui->btnSetHome->setEnabled(true);
    ui->btnResetGrbl->setEnabled(true);
    ui->btnUnlockGrbl->setEnabled(true);
    ui->btnGoHomeSafe->setEnabled(true);
}

void MainWindow::disableAllButtons()
{
    ui->groupBoxAxisControl->setEnabled(false);
    ui->groupBoxManualGCode->setEnabled(false);
    ui->Begin->setEnabled(false);
    ui->Stop->setEnabled(false);
    ui->progressFileSend->setEnabled(false);
    ui->outputRuntime->setEnabled(false);
    ui->labelRuntime->setEnabled(false);
    ui->openFile->setEnabled(false);
    ui->btnGRBL->setEnabled(false);
    ui->btnSetHome->setEnabled(false);
    ui->btnResetGrbl->setEnabled(false);
    ui->btnUnlockGrbl->setEnabled(false);
    ui->btnGoHomeSafe->setEnabled(false);
}

void MainWindow::enableGrblDialogButton()
{
    ui->openFile->setEnabled(true);
    ui->btnOpenPort->setEnabled(true);
    ui->btnOpenPort->setText(CLOSE_BUTTON_TEXT);
    ui->btnOpenPort->setStyleSheet("* { background-color: rgb(255,125,100) }");
    ui->cmbPort->setEnabled(false);
    ui->groupBoxAxisControl->setEnabled(true);
    ui->groupBoxSendFile->setEnabled(true);
    ui->groupBoxManualGCode->setEnabled(true);
    ui->btnSetHome->setEnabled(true);
    ui->btnResetGrbl->setEnabled(true);
    ui->btnUnlockGrbl->setEnabled(true);
    ui->btnGoHomeSafe->setEnabled(true);

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

    ui->btnGRBL->setEnabled(true);
}

void MainWindow::incX()
{
    float coord = ui->comboStep->currentText().toFloat();
    disableAllButtons();
    emit axisAdj('X', coord, invX, absoluteAfterAxisAdj);
}

void MainWindow::incY()
{
    float coord = ui->comboStep->currentText().toFloat();
    disableAllButtons();
    emit axisAdj('Y', coord, invY, absoluteAfterAxisAdj);
}

void MainWindow::incZ()
{
    float coord = ui->comboStep->currentText().toFloat();
    disableAllButtons();
    emit axisAdj('Z', coord, invZ, absoluteAfterAxisAdj);
}

void MainWindow::decX()
{
    float coord = -ui->comboStep->currentText().toFloat();
    disableAllButtons();
    emit axisAdj('X', coord, invX, absoluteAfterAxisAdj);
}

void MainWindow::decY()
{
    float coord = -ui->comboStep->currentText().toFloat();
    disableAllButtons();
    emit axisAdj('Y', coord, invY, absoluteAfterAxisAdj);
}

void MainWindow::decZ()
{
    float coord = -ui->comboStep->currentText().toFloat();
    disableAllButtons();
    emit axisAdj('Z', coord, invZ, absoluteAfterAxisAdj);
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
    if(ui->filePath->text() != "" && ui->btnOpenPort->text() == CLOSE_BUTTON_TEXT)
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

void MainWindow::readSettings()
{
    // use platform-independent settings storage, i.e. registry under Windows
    QSettings settings;

    fileOpenDialogState = settings.value(SETTINGS_FILE_OPEN_DIALOG_STATE).value<QByteArray>();
    directory = settings.value(SETTINGS_DIRECTORY).value<QString>();
    nameFilter = settings.value(SETTINGS_NAME_FILTER).value<QString>();
    lastOpenPort = settings.value(SETTINGS_PORT).value<QString>();

    updateSettingsFromOptionDlg(settings);
}

// Slot called from settings dialog after user made a change. Reload settings from registry.
void MainWindow::setSettings()
{
    QSettings settings;

    updateSettingsFromOptionDlg(settings);

    // update gcode thread with latest values
    emit setResponseWait(waitTime, zJogRate, useMm, zRateLimiting, zRateLimitAmount, xyRateAmount, useAggressivePreload);
}

void MainWindow::updateSettingsFromOptionDlg(QSettings& settings)
{
    QString sinvX = settings.value(SETTINGS_INVERSE_X, "false").value<QString>();
    QString sinvY = settings.value(SETTINGS_INVERSE_Y, "false").value<QString>();
    QString sinvZ = settings.value(SETTINGS_INVERSE_Z, "false").value<QString>();

    QString sdbgLog = settings.value(SETTINGS_ENABLE_DEBUG_LOG, "false").value<QString>();
    g_enableDebugLog.set(sdbgLog == "true");

    // only enable/not enable file logging at startup. There are some kind of
    // multithreaded issues turning on or off file logging at runtime causing
    // crashes.
    if (!checkLogWrite)
    {
        checkLogWrite = true;

        if (g_enableDebugLog.get())
        {
            p_fappender->activateOptions();
            Log4Qt::Logger::rootLogger()->addAppender(p_fappender);
        }
    }

    invX = sinvX == "true";
    invY = sinvY == "true";
    invZ = sinvZ == "true";

    waitTime = settings.value(SETTINGS_RESPONSE_WAIT_TIME, DEFAULT_WAIT_TIME_SEC).value<int>();
    zJogRate = settings.value(SETTINGS_Z_JOG_RATE, DEFAULT_Z_JOG_RATE).value<double>();
    QString useMmManualCmds = settings.value(SETTINGS_USE_MM_FOR_MANUAL_CMDS, "true").value<QString>();
    useMm = useMmManualCmds == "true";
    QString useAggrPreload = settings.value(SETTINGS_USE_AGGRESSIVE_PRELOAD, "false").value<QString>();
    useAggressivePreload = useAggrPreload == "true";

    QString absAfterAdj = settings.value(SETTINGS_ABSOLUTE_AFTER_AXIS_ADJ, "false").value<QString>();
    absoluteAfterAxisAdj = absAfterAdj == "true";
    ui->chkRestoreAbsolute->setChecked(absoluteAfterAxisAdj);

    QString zRateLimit = settings.value(SETTINGS_Z_RATE_LIMIT, "false").value<QString>();
    zRateLimiting = zRateLimit == "true";

    zRateLimitAmount = settings.value(SETTINGS_Z_RATE_LIMIT_AMOUNT, DEFAULT_Z_LIMIT_RATE).value<double>();
    xyRateAmount = settings.value(SETTINGS_XY_RATE_AMOUNT, DEFAULT_XY_RATE).value<double>();
}

// save last state of settings
void MainWindow::writeSettings()
{
    QSettings settings;

    settings.setValue(SETTINGS_FILE_OPEN_DIALOG_STATE, fileOpenDialogState);
    settings.setValue(SETTINGS_NAME_FILTER, nameFilter);
    settings.setValue(SETTINGS_DIRECTORY, directory);
    settings.setValue(SETTINGS_PORT, ui->cmbPort->currentText());

    settings.setValue(SETTINGS_ABSOLUTE_AFTER_AXIS_ADJ, ui->chkRestoreAbsolute->isChecked());
}

void MainWindow::receiveList(QString msg)
{
    addToStatusList(true, msg);
}

void MainWindow::receiveListFull(QStringList list)
{
    addToStatusList(list);
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

    QString nMsg(msg);
    if (!in)
        nMsg = "> " + msg;
    ui->statusList->addItem(nMsg);

    status("%s", nMsg.toLocal8Bit().constData());

    doScroll();
}

void MainWindow::addToStatusList(QStringList& list)
{
    QStringList cleanList;
    foreach (QString msg, list)
    {
        msg.trimmed();
        msg.remove('\r');
        msg.remove('\n');

        if (msg.length() == 0)
            continue;

        cleanList.append(msg);

        status("%s", msg.toLocal8Bit().constData());
    }

    if (cleanList.size() == 0)
        return;

    ui->statusList->addItems(cleanList);

    doScroll();
}

void MainWindow::doScroll()
{
    if (scrollStatusTimer.elapsed() > 1000)
    {
        ui->statusList->scrollToBottom();
        QApplication::processEvents();
        scrollStatusTimer.restart();
    }
}

/* testing optimizing scrollbar, doesn't work
int MainWindow::computeListViewMinimumWidth(QAbstractItemView* view)
{
    int minWidth = 0;
    QAbstractItemModel* model = view->model();

    QStyleOptionViewItem option;

    int rowCount = model->rowCount();
    for (int row = 0; row < rowCount; ++row)
    {
        QModelIndex index = model->index(row, 0);
        QSize size = view->itemDelegate()->sizeHint(option, index);
        scrollDelegate = new MyItemDelegate(view);
        view->setItemDelegate(scrollDelegate);

        minWidth = qMax(size.width(), minWidth);
    }

    if (rowCount > 0)
    {
        if (scrollDelegate == NULL)
        {
            scrollDelegate = new MyItemDelegate(view);
            QModelIndex index = model->index(0, 0);
            view->setItemDelegate(scrollDelegate);
        }

        scrollDelegate->setWidth(minWidth);
        info("Width is %d\n", minWidth);
    }
    return minWidth;
}
*/

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
    if (ui->SpindleOn->QAbstractButton::isChecked())
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

void MainWindow::toggleRestoreAbsolute()
{
    absoluteAfterAxisAdj = ui->chkRestoreAbsolute->QAbstractButton::isChecked();
}

void MainWindow::updateCoordinates(Coord3D machineCoord, Coord3D workCoord)
{
    machineCoordinates = machineCoord;
    workCoordinates = workCoord;

    refreshLcd();
}

void MainWindow::refreshLcd()
{
    lcdDisplay('X', true, workCoordinates.x);
    lcdDisplay('Y', true, workCoordinates.y);
    lcdDisplay('Z', true, workCoordinates.z);
    lcdDisplay('X', false, machineCoordinates.x);
    lcdDisplay('Y', false, machineCoordinates.y);
    lcdDisplay('Z', false, machineCoordinates.z);
}

void MainWindow::lcdDisplay(char axis, bool workCoord, float value)
{
    switch (axis)
    {
    case 'X':
        if (workCoord)
            ui->lcdWorkNumberX->display(value);
        else
            ui->lcdMachNumberX->display(value);
        break;
    case 'Y':
        if (workCoord)
            ui->lcdWorkNumberY->display(value);
        else
            ui->lcdMachNumberY->display(value);
        break;
    case 'Z':
        if (workCoord)
            ui->lcdWorkNumberZ->display(value);
        else
            ui->lcdMachNumberZ->display(value);
        break;
    }
}
