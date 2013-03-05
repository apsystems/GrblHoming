/****************************************************************
 * gcode.cpp
 * GrblHoming - zapmaker fork on github
 *
 * 15 Nov 2012
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#include "gcode.h"

GCode::GCode()
    : waitTime(LONG_WAIT_SEC), zJogRate(DEFAULT_Z_JOG_RATE),
      errorCount(0), doubleDollarFormat(false),
      incorrectMeasurementUnits(false), incorrectLcdDisplayUnits(false),
      userSetMmMode(true), zRateLimit(false), zRateLimitAmount(DEFAULT_Z_LIMIT_RATE),
      xyRateAmount(DEFAULT_XY_RATE),
      maxZ(0), useAggressivePreload(false), motionOccurred(false), filterFileCommands(false)
{
    // use base class's timer - use it to capture random text from the controller
    startTimer(1000);
}

void GCode::openPort(QString commPortStr)
{
    clearToHome();

    currComPort = commPortStr;

    if (port.OpenComport(commPortStr))
    {
        emit portIsOpen(true);
    }
    else
    {
        emit portIsClosed(false);
        QString msg = "Can't open COM port " + commPortStr;
        sendMsg(msg);
        addList(msg);
        warn("%s", msg.toLocal8Bit().constData());

        addList("-Is hardware connected to USB?");
        addList("-Is correct port chosen?");
        addList("-Does current user have sufficient permissions?");
#if defined(Q_WS_X11)
        addList("-Is current user in sudoers group?");
#endif
        //QMessageBox(QMessageBox::Critical,"Error","Could not open port.",QMessageBox::Ok).exec();
    }
}

void GCode::closePort(bool reopen)
{
    port.CloseComport();
    emit portIsClosed(reopen);
}

bool GCode::isPortOpen()
{
    return port.isPortOpen();
}

// Abort means stop file send after the end of this line
void GCode::setAbort()
{
    // cross-thread operation, only set one atomic variable in this method (bool in this case) or add critsec
    abortState.set(true);
}

// Reset means immediately stop waiting for a response
void GCode::setReset()
{
    // cross-thread operation, only set one atomic variable in this method (bool in this case) or add critsec
    resetState.set(true);
}

// Shutdown means app is shutting down - we give thread about .3 sec to exit what it is doing
void GCode::setShutdown()
{
    // cross-thread operation, only set one atomic variable in this method (bool in this case) or add critsec
    shutdownState.set(true);
}

// Slot for interrupting current operation or doing a clean reset of grbl without changing position values
void GCode::sendGrblReset()
{
    clearToHome();

    QString x(CTRL_X);
    sendGcodeLocal(x, true, SHORT_WAIT_SEC);
}

void GCode::sendGrblUnlock()
{
    sendGcodeLocal(SET_UNLOCK_STATE_V08c);
}

// Slot for gcode-based 'zero out the current position values without motion'
void GCode::grblSetHome()
{
    clearToHome();

    gotoXYZ("G92 x0 y0 z0");
}

void GCode::goToHome()
{
    if (!motionOccurred)
        return;

    double maxZOver = maxZ;

    if (doubleDollarFormat)
    {
        maxZOver += (userSetMmMode ? PRE_HOME_Z_ADJ_MM : (PRE_HOME_Z_ADJ_MM / MM_IN_AN_INCH));
    }
    else
    {
        // all reporting is in mm
        maxZOver += PRE_HOME_Z_ADJ_MM;
    }

    QString zpos = QString::number(maxZOver);

    gotoXYZ(QString("G0 z").append(zpos));
    gotoXYZ(QString("G1 x0 y0"));

    maxZ -= maxZOver;

    motionOccurred = false;
}

// Slot called from other threads (i.e. main window, grbl dialog, etc.)
void GCode::sendGcode(QString line)
{
    bool checkMeasurementUnits = false;

    if (line.length() == 0)
    {
        // empty line means we want to see what the device responds with. Pass params to not fail if nothing happens.
        // normally get here after opening connection to com port
        sendGcodeLocal(line, true, SHORT_WAIT_SEC);
        checkMeasurementUnits = true;
    }
    else
    {
        pollPosWaitForIdle(false);

        // normal send of actual commands
        sendGcodeLocal(line, false);
    }

    pollPosWaitForIdle(checkMeasurementUnits);
}

// keep polling our position and state until we are done running
void GCode::pollPosWaitForIdle(bool checkMeasurementUnits)
{
    bool immediateQuit = false;
    for (int i = 0; i < 10000; i++)
    {
        bool ret = sendGcodeLocal(REQUEST_CURRENT_POS);
        if (!ret)
        {
            immediateQuit = true;
            break;
        }

        if (doubleDollarFormat)
        {
            if (lastState.compare("Run") != 0)
                break;
        }
        else
        {
            if (machineCoordLastIdlePos == machineCoord
                && workCoordLastIdlePos == workCoord)
            {
                break;
            }

            machineCoordLastIdlePos = machineCoord;
            workCoordLastIdlePos = workCoord;
        }

        if (shutdownState.get())
            return;
    }

    if (immediateQuit)
        return;

    if (checkMeasurementUnits)
    {
        if (doubleDollarFormat)
            checkAndSetCorrectMeasurementUnits();
        else
            setOldFormatMeasurementUnitControl();
    }
}

// Slot called from other thread that returns whatever text comes back from the controller
void GCode::sendGcodeAndGetResult(int id, QString line)
{
    QString result;

    emit sendMsg("");
    resetState.set(false);
    if (!sendGcodeInternal(line, result, false, SHORT_WAIT_SEC, false))
        result.clear();

    emit gcodeResult(id, result);
}

// To be called only from this class, not from other threads. Use above two methods for that.
// Wraps sendGcodeInternal() to allow proper handling of failure cases, etc.
bool GCode::sendGcodeLocal(QString line, bool recordResponseOnFail /* = false */, int waitSec /* = -1 */, bool aggressive /* = false */)
{
    QString result;
    sendMsg("");
    resetState.set(false);

    bool ret = sendGcodeInternal(line, result, recordResponseOnFail, waitSec, aggressive);
    if (shutdownState.get())
        return false;

    if (!ret && (!recordResponseOnFail || resetState.get()))
    {
        if (!resetState.get())
            emit stopSending();

        if (!ret && resetState.get())
        {
            resetState.set(false);
            port.Reset();
        }
    }
    else
    {
        if (result.contains("Grbl"))
        {
            QRegExp rx("Grbl (\\d+)\\.(\\d+)(\\w*)");
            if (rx.indexIn(result) != -1 && rx.captureCount() > 0)
            {
                doubleDollarFormat = false;

                QStringList list = rx.capturedTexts();
                if (list.size() >= 3)
                {
                    int majorVer = list.at(1).toInt();
                    int minorVer = list.at(2).toInt();
                    char letter = 'a';
                    if (list.size() == 4 && list.at(3).size() > 0)
                    {
                        letter = list.at(3).toAscii().at(0);
                    }

                    if (majorVer > 0 || (minorVer > 8 && minorVer < 51) || letter > 'a')
                    {
                        doubleDollarFormat = true;
                    }

                    diag("Got Grbl Version (Parsed:) %d.%d%c ($$=%d)\n",
                                majorVer, minorVer, letter, doubleDollarFormat);
                }

                if (!doubleDollarFormat)
                    setUnitsTypeDisplay(true);
            }

            emit enableGrblDialogButton();
        }
    }
    resetState.set(false);
    return ret;
}

// Wrapped method. Should only be called from above method.
bool GCode::sendGcodeInternal(QString line, QString& result, bool recordResponseOnFail, int waitSec, bool aggressive)
{
    if (!port.isPortOpen())
    {
        QString msg = "Port not available yet";
        err("%s", msg.toLocal8Bit().constData());
        emit addList(msg);
        emit sendMsg(msg);
        return false;
    }

    bool ctrlX = line.size() > 0 ? (line.at(0).toAscii() == CTRL_X) : false;

    bool sentReqForLocation = false;
    bool sentReqForSettings = false;
    bool sentReqForParserState = false;

    if (!line.compare(REQUEST_CURRENT_POS))
    {
        sentReqForLocation = true;
    }
    else if (!line.compare(REQUEST_PARSER_STATE_V08c))
    {
        sentReqForParserState = true;
    }
    else if (!line.compare(SETTINGS_COMMAND_V08a))
    {
        if (doubleDollarFormat)
            line = SETTINGS_COMMAND_V08c;

        sentReqForSettings = true;
    }
    else
        motionOccurred = true;

    // adds to UI list, but prepends a > indicating a sent command
    if (ctrlX)
    {
        emit addListOut("(CTRL-X)");
    }
    else if (!sentReqForLocation)// if requesting location, don't add that "noise" to the output view
    {
        emit addListOut(line);
    }

    if (line.size() == 0 || (!line.endsWith('\r') && !ctrlX))
        line.append('\r');

    char buf[BUF_SIZE + 1] = {0};
    if (line.length() >= BUF_SIZE)
    {
        QString msg = "Buffer size too small";
        err("%s", msg.toLocal8Bit().constData());
        emit addList(msg);
        emit sendMsg(msg);
        return false;
    }
    for (int i = 0; i < line.length(); i++)
        buf[i] = line.at(i).toAscii();

    if (ctrlX)
        diag("SENDING: 0x%02X (CTRL-X)\n", buf[0]);
    else
        diag("SENDING: %s\n", buf);

    int waitSecActual = waitSec == -1 ? waitTime : waitSec;

    if (aggressive)
    {
        sendCount.append(line.length());

        waitForOk(result, waitSecActual, false, false, aggressive);

        if (shutdownState.get())
            return false;
    }

    if (!port.SendBuf(buf, line.length()))
    {
        QString msg = "Sending to port failed";
        err("%s", msg.toLocal8Bit().constData());
        emit addList(msg);
        emit sendMsg(msg);
        return false;
    }
    else
    {
        sentI++;
        if (!waitForOk(result, waitSecActual, sentReqForLocation, sentReqForParserState, aggressive))
        {
            diag("WAITFOROK FAILED\n");
            if (shutdownState.get())
                return false;

            if (!recordResponseOnFail && !(resetState.get() || abortState.get()))
            {
                QString msg = "Wait for ok failed";
                emit addList(msg);
                emit sendMsg(msg);
            }

            return false;
        }
        else
        {
            if (sentReqForSettings)
            {
                QStringList list = result.split("$");
                for (int i = 0; i < list.size(); i++)
                {
                    QString item = list.at(i);
                    const QRegExp rx(REGEXP_SETTINGS_LINE);

                    if (rx.indexIn(item, 0) != -1 && rx.captureCount() == 3)
                    {
                        QStringList capList = rx.capturedTexts();
                        if (!capList.at(1).compare("13"))
                        {
                            if (!capList.at(2).compare("0"))
                            {
                                if (!userSetMmMode)
                                    incorrectLcdDisplayUnits = true;
                            }
                            else
                            {
                                if (userSetMmMode)
                                    incorrectLcdDisplayUnits = true;
                            }
                            break;
                        }
                    }
                }

                settingsItemCount.set(list.size());
            }
        }
    }
    return true;
}

bool GCode::waitForOk(QString& result, int waitSec, bool sentReqForLocation, bool sentReqForParserState, bool aggressive)
{
    if (aggressive)
    {
        if (!port.bytesAvailable())
        {
            int total = 0;
            foreach (int count, sendCount)
            {
                total += count;
            }

            //printf("Total out (a): %d (%d)\n", total, sendCount.size());

            if (total < (GRBL_RX_BUFFER_SIZE - 1))
            {
                return true;
            }
        }
    }

    char tmp[BUF_SIZE + 1] = {0};
    int count = 0;
    int waitCount = waitSec * 10;// multiplier depends on sleep values below
    bool status = true;
    result.clear();
    while (!result.contains(RESPONSE_OK) && !result.contains(RESPONSE_ERROR) && !resetState.get())
    {
        int n = port.PollComportLine(tmp, BUF_SIZE);
        if (n == 0)
        {
            count++;
            SLEEP(100);
        }
        else if (n < 0)
        {
            err("Error reading data from COM port\n");
        }
        else
        {
            tmp[n] = 0;
            result.append(tmp);

            QString tmpTrim(tmp);
            int pos = tmpTrim.indexOf(port.getDetectedLineFeed());
            if (pos != -1)
                tmpTrim.remove(pos, port.getDetectedLineFeed().size());
            diag("GOT:%s\n", tmpTrim.toLocal8Bit().constData());
            QString received(tmp);

            if (aggressive)
            {
                if (received.contains(RESPONSE_OK))
                {
                    sendCount.removeFirst();
                    rcvdI++;
                }
                else if (received.contains(RESPONSE_ERROR))
                {
                    sendCount.removeFirst();
                    errorCount++;
                    emit addList(received);
                    rcvdI++;
                }
                else
                {
                    parseCoordinates(received, aggressive);
                }

                int total = 0;
                foreach (int count, sendCount)
                {
                    total += count;
                }

                //printf("Total out (b): %d (%d)\n", total, sendCount.size());
                //printf("SENT:%d RCVD:%d\n", sentI, rcvdI);

                if (port.bytesAvailable() || total >= (GRBL_RX_BUFFER_SIZE - 1))
                {
                    result.clear();
                    continue;
                }
                else
                {
                    return true;
                }
            }

            if (!received.contains(RESPONSE_OK) && !received.contains(RESPONSE_ERROR))
            {
                if (sentReqForParserState)
                {
                    const QRegExp rx("\\[([\\s\\w\\.\\d]+)\\]");

                    if (rx.indexIn(received, 0) != -1 && rx.captureCount() == 1)
                    {
                        QStringList list = rx.capturedTexts();
                        if (list.size() == 2)
                        {
                            QStringList items = list.at(1).split(" ");
                            if (items.contains("G20"))// inches
                            {
                                if (userSetMmMode)
                                    incorrectMeasurementUnits = true;
                                else
                                    incorrectMeasurementUnits = false;
                            }
                            else if (items.contains("G21"))// millimeters
                            {
                                if (userSetMmMode)
                                    incorrectMeasurementUnits = false;
                                else
                                    incorrectMeasurementUnits = true;
                            }
                            else
                            {
                                // not in list!
                                incorrectMeasurementUnits = true;
                            }
                        }
                    }
                }
                else
                {
                    parseCoordinates(received, aggressive);
                }
            }
            count = 0;
        }

        SLEEP(100);

        if (count > waitCount)
        {
            // waited too long for a response, fail
            status = false;
            break;
        }
    }

    if (shutdownState.get())
    {
        return false;
    }

    if (status)
    {
        if (!aggressive)
            SLEEP(100);

        if (resetState.get())
        {
            QString msg("Wait interrupted by user");
            err("%s", msg.toLocal8Bit().constData());
            emit addList(msg);
        }
    }

    if (result.contains(RESPONSE_ERROR))
    {
        errorCount++;
        // skip over errors
        //status = false;
    }

    QStringList list = QString(result).split(port.getDetectedLineFeed());
    QStringList listToSend;
    for (int i = 0; i < list.size(); i++)
    {
        if (list.at(i).length() > 0 && list.at(i) != RESPONSE_OK && !sentReqForLocation)
            listToSend.append(list.at(i));
    }

    sendStatusList(listToSend);

    if (resetState.get())
    {
        // we have been told by the user to stop.
        status = false;
    }

    return status;
}

void GCode::parseCoordinates(const QString& received, bool aggressive)
{
    if (aggressive)
    {
        int ms = parseCoordTimer.elapsed();
        if (ms < 500)
            return;

        parseCoordTimer.restart();
    }

    QString state;
    QString prepend;
    QString append;
    QString preamble = "([a-zA-Z]+),MPos:";
    int captureCount = 4;

    if (!doubleDollarFormat)
    {
        prepend = "\\[";
        append = "\\]";
        preamble = "MPos:";
        captureCount = 3;
    }

    const QString coordRegExp(prepend + "(-*\\d+\\.\\d+),(-*\\d+\\.\\d+),(-*\\d+\\.\\d+)" + append);
    const QRegExp rxStateMPos(preamble + coordRegExp);
    const QRegExp rxWPos("WPos:" + coordRegExp);

    if (rxStateMPos.indexIn(received, 0) != -1 && rxStateMPos.captureCount() == captureCount
        && rxWPos.indexIn(received, 0) != -1 && rxWPos.captureCount() == 3)
    {
        QStringList list = rxStateMPos.capturedTexts();
        int index = 1;

        if (doubleDollarFormat)
            state = list.at(index++);

        machineCoord.x = list.at(index++).toFloat();
        machineCoord.y = list.at(index++).toFloat();
        machineCoord.z = list.at(index++).toFloat();

        list = rxWPos.capturedTexts();
        workCoord.x = list.at(1).toFloat();
        workCoord.y = list.at(2).toFloat();
        workCoord.z = list.at(3).toFloat();

        diag("Decoded: State:%s MPos: %f,%f,%f WPos: %f,%f,%f\n",
             state.toLocal8Bit().constData(),
             machineCoord.x, machineCoord.y, machineCoord.z,
             workCoord.x, workCoord.y, workCoord.z);

        if (workCoord.z > maxZ)
            maxZ = workCoord.z;

        emit updateCoordinates(machineCoord, workCoord);
        emit setLivePoint(workCoord.x, workCoord.y, userSetMmMode);
        emit setLastState(state);

        lastState = state;
        return;
    }
    else if (received.indexOf("MPos:") != -1)
    {
        err("Error decoding position data!\n");
    }
    lastState = "";
}

void GCode::sendStatusList(QStringList& listToSend)
{
    if (listToSend.size() > 1)
    {
        emit addListFull(listToSend);
    }
    else if (listToSend.size() == 1)
    {
        emit addList(listToSend.at(0));
    }
}

// called once a second to capture any random strings that come from the controller
#pragma GCC diagnostic ignored "-Wunused-parameter" push
void GCode::timerEvent(QTimerEvent *event)
{
    if (port.isPortOpen())
    {
        char tmp[BUF_SIZE + 1] = {0};
        QString result;

        for (int i = 0; i < 10 && !shutdownState.get() && !resetState.get(); i++)
        {
            int n = port.PollComport(tmp, BUF_SIZE);
            if (n == 0)
                break;

            tmp[n] = 0;
            result.append(tmp);
            diag("GOT-TE:%s\n", tmp);
        }

        if (shutdownState.get())
        {
            return;
        }

        QStringList list = QString(result).split(port.getDetectedLineFeed());
        QStringList listToSend;
        for (int i = 0; i < list.size(); i++)
        {
            if (list.at(i).length() > 0 && (list.at(i) != "ok" || (list.at(i) == "ok" && abortState.get())))
                listToSend.append(list.at(i));
        }

        sendStatusList(listToSend);
    }
}
#pragma GCC diagnostic ignored "-Wunused-parameter" pop

void GCode::sendFile(QString path)
{
    addList(QString("Sending file '%1'").arg(path));

    // send something to be sure the controller is ready
    //sendGcodeLocal("", true, SHORT_WAIT_SEC);

    setProgress(0);

    errorCount = 0;
    abortState.set(false);
    QFile file(path);
    if (file.open(QFile::ReadOnly))
    {
        float totalLineCount = 0;
        QTextStream code(&file);
        while ((code.atEnd() == false))
        {
            totalLineCount++;
            code.readLine();
        }
        if (totalLineCount == 0)
            totalLineCount = 1;

        code.seek(0);

        // set here once so that it doesn't change in the middle of a file send
        bool aggressive = useAggressivePreload;
        if (aggressive)
            sendCount.clear();

        sentI = 0;
        rcvdI = 0;
        emit resetTimer(true);

        parseCoordTimer.restart();

        QTime pollPosTimer;
        pollPosTimer.start();

        int currLine = 0;
        bool xyRateSet = false;

        do
        {
            QString strline = code.readLine();

            emit setVisCurrLine(currLine + 1);

            if (filterFileCommands)
            {
                trimToEnd(strline, '(');
                trimToEnd(strline, ';');
                trimToEnd(strline, '%');
            }

            strline = strline.trimmed();

            if (strline.size() == 0)
            {}//ignore comments
            else
            {
                if (filterFileCommands)
                {
                    strline = strline.toUpper();
                    strline.replace(QRegExp("([A-Z])"), " \\1");
                    strline = removeUnsupportedCommands(strline);
                }

                if (strline.size() != 0)
                {
                    QString rateLimitMsg;
                    QStringList outputList;
                    if (zRateLimit)
                    {
                        outputList = doZRateLimit(strline, rateLimitMsg, xyRateSet);
                    }
                    else
                    {
                        outputList.append(strline);
                    }

                    bool ret = false;
                    if (outputList.size() == 1)
                    {
                        ret = sendGcodeLocal(outputList.at(0), false, -1, aggressive);
                    }
                    else
                    {
                        foreach (QString outputLine, outputList)
                        {
                            ret = sendGcodeLocal(outputLine, false, -1, aggressive);

                            if (!ret)
                                break;
                        }
                    }

                    if (rateLimitMsg.size() > 0)
                        addList(rateLimitMsg);

                    if (!ret)
                    {
                        abortState.set(true);
                        break;
                    }
                }
            }

            float percentComplete = (currLine * 100.0) / totalLineCount;
            setProgress((int)percentComplete);

            if (!aggressive)
            {
                sendGcodeLocal(REQUEST_CURRENT_POS);
            }
            else
            {
                int ms = pollPosTimer.elapsed();
                if (ms >= 1000)
                {
                    pollPosTimer.restart();
                    sendGcodeLocal(REQUEST_CURRENT_POS, false, -1, aggressive);
                }
            }
            currLine++;
        } while ((code.atEnd() == false) && (!abortState.get()));
        file.close();

        if (aggressive)
        {
            int limitCount = 0;
            while (sendCount.size() > 0 && limitCount < 5000)
            {
                QString result;
                waitForOk(result, waitTime, false, false, aggressive);
                SLEEP(100);

                if (shutdownState.get())
                    return;

                limitCount++;
            }
        }

        sendGcodeLocal(REQUEST_CURRENT_POS);

        emit resetTimer(false);

        if (shutdownState.get())
        {
            return;
        }

        QString msg;
        if (!abortState.get())
        {
            setProgress(100);
            if (errorCount > 0)
                msg = QString("Code sent successfully with %1 error(s)").arg(QString::number(errorCount));
            else
                msg = "Code sent successfully with no errors.";
        }
        else
        {
            msg = "Process interrupted.";
        }

        emit sendMsg(msg);
        emit addList(msg);
    }

    pollPosWaitForIdle(true);

    if (!resetState.get())
    {
        emit stopSending();
    }
}

void GCode::trimToEnd(QString& strline, QChar ch)
{
    int pos = strline.indexOf(ch);
    if (pos != -1)
        strline = strline.left(pos);
}

QString GCode::removeUnsupportedCommands(QString line)
{
    QStringList components = line.split(" ", QString::SkipEmptyParts);
    QString tmp;
    QString s;
    foreach (s, components)
    {
        if (s.at(0) == 'G')
        {
            float value = s.mid(1,-1).toFloat();
            if (isGCommandValid(value))
                tmp.append(s).append(" ");
            else
            {
                QString msg(QString("Removing unsupported G command '%1'").arg(s));
                warn("%s", msg.toLocal8Bit().constData());
                emit addList(msg);
            }
        }
        else if (s.at(0) == 'M')
        {
            float value = s.mid(1,-1).toFloat();
            if (isMCommandValid(value))
                tmp.append(s).append(" ");
            else
            {
                QString msg(QString("Removing unsupported M command '%1'").arg(s));
                warn("%s", msg.toLocal8Bit().constData());
                emit addList(msg);
            }
        }
        else if (s.at(0) == 'N')
        {
            // skip line numbers
        }
        else if (s.at(0) == 'X' || s.at(0) == 'Y' || s.at(0) == 'Z'
                 || s.at(0) == 'I' || s.at(0) == 'J' || s.at(0) == 'K'
                 || s.at(0) == 'F' || s.at(0) == 'L' || s.at(0) == 'S')
        {
            tmp.append(s).append(" ");
        }
        else
        {
            QString msg(QString("Removing unsupported command '%1'").arg(s));
            warn("%s", msg.toLocal8Bit().constData());
            emit addList(msg);
        }
    }

    return tmp.trimmed();
}

bool GCode::isGCommandValid(float value)
{
    // supported values obtained from https://github.com/grbl/grbl/wiki
    const static float supported[] =
    {
        0,    1,    2,    3,    4,
        10,    17,    18,    19,    20,    21,    28,    28.1,    30,    30.1,
        53,    54,    55,    56,    57,    58,    59,
        80,    90,    91,    92,    92.1,    93,    94
    };

    int len = sizeof(supported) / sizeof(float);
    for (int i = 0; i < len; i++)
    {
        if (value == supported[i])
            return true;
    }
    return false;
}

bool GCode::isMCommandValid(float value)
{
    // supported values obtained from https://github.com/grbl/grbl/wiki
    const static float supported[] =
    {
        0,    2,    3,    4,    5,    8,    9,    30
    };

    int len = sizeof(supported) / sizeof(float);
    for (int i = 0; i < len; i++)
    {
        if (value == supported[i])
            return true;
    }
    return false;
}

QStringList GCode::doZRateLimit(QString inputLine, QString& msg, bool& xyRateSet)
{
    // i.e.
    //G00 Z1 => G01 Z1 F100
    //G01 Z1 F260 => G01 Z1 F100
    //G01 Z1 F30 => G01 Z1 F30
    //G01 X1 Y1 Z1 F200 -> G01 X1 Y1 & G01 Z1 F100
    QStringList list;
    QString line = inputLine.toUpper();

    if (line.count("Z") > 0)
    {
        QStringList components = line.split(" ", QString::SkipEmptyParts);
        QString s;
        bool foundFeed = false;
        bool didLimit = false;

        // We need to build one or two command strings depending on input.
        QString x, y, g, f;

        // First get all component parts
        bool inLimit = false;
        foreach (s, components)
        {
            if (s.at(0) == 'G')
            {
                g = s;
            }
            else if (s.at(0) == 'F')
            {
                f = s;

                double value = s.mid(1,-1).toDouble();
                if (value > zRateLimitAmount)
                    inLimit = true;
            }
            else if (s.at(0) == 'X')
            {
                x = s;
            }
            else if (s.at(0) == 'Y')
            {
                y = s;
            }
        }

        // Determine whether we want to have one or two command lins
        // 1 string: Have !G0 and F but not in limit
        // 1 string: Have Z only
        // 2 strings: All other conditions
        QString line1;
        QString line2;
        if ((g != "G0" && f.size() > 0 && !inLimit)
                || (x.size() == 0 && y.size() == 0))
        {
            foreach (s, components)
            {
                if (s.at(0) == 'G')
                {
                    int value = s.mid(1,-1).toInt();
                    if (value == 0)
                        line1.append("G1");
                    else
                        line1.append(s);
                }
                else if (s.at(0) == 'F')
                {
                    double value = s.mid(1,-1).toDouble();
                    if (value > zRateLimitAmount)
                    {
                        line1.append("F").append(QString::number(zRateLimitAmount));
                        didLimit = true;
                    }
                    else
                        line1.append(s);

                    foundFeed = true;
                }
                else
                {
                    line1.append(s);
                }
                line1.append(" ");
            }
        }
        else
        {
            // two lines
            foreach (s, components)
            {
                if (s.at(0) == 'G')
                {
                    int value = s.mid(1,-1).toInt();
                    if (value == 0)
                        line1.append("G1").append(" ");
                    else
                        line1.append(s).append(" ");

                    line2.append(s).append(" ");
                }
                else if (s.at(0) == 'F')
                {
                    double value = s.mid(1,-1).toDouble();
                    if (value > zRateLimitAmount)
                    {
                        line1.append("F").append(QString::number(zRateLimitAmount));
                        didLimit = true;
                    }
                    else
                        line1.append(s).append(" ");

                    line2.append(s).append(" ");

                    foundFeed = true;
                }
                else if (s.at(0) == 'Z')
                {
                    line1.append(s).append(" ");
                }
                else
                {
                    line2.append(s).append(" ");
                }
            }
        }

        if (!foundFeed)
        {
            line1.append("F").append(QString::number(zRateLimitAmount));
            didLimit = true;
        }

        line1 = line1.trimmed();
        line2 = line2.trimmed();

        if (didLimit)
        {
            if (line2.size() == 0)
            {
                msg = QString("Z-Rate Limit: [%1]=>[%2]").arg(inputLine).arg(line1);
                xyRateSet = true;
            }
            else
            {
                msg = QString("Z-Rate Limit: [%1]=>[%2,%3]").arg(inputLine).arg(line1).arg(line2);
                line2.append(QString(" F").append(QString::number(xyRateAmount)));
            }
        }

        list.append(line1);
        if (line2.size() > 0)
            list.append(line2);
        return list;
    }
    else if (xyRateSet)
    {
        QStringList components = line.split(" ", QString::SkipEmptyParts);
        QString s;

        bool addRateG = false;
        bool addRateXY = false;
        bool gotF = false;
        foreach (s, components)
        {
            if (s.at(0) == 'G')
            {
                int value = s.mid(1,-1).toInt();
                if (value != 0)
                {
                    addRateG = true;
                }
            }
            else if (s.at(0) == 'F')
            {
                gotF = true;
            }
            else if (s.at(0) == 'X' || s.at(0) == 'Y')
            {
                addRateXY = true;
            }
        }

        if (addRateG && addRateXY)
        {
            if (!gotF)
            {
                QString line = inputLine;
                line.append(QString(" F").append(QString::number(xyRateAmount)));
                msg = QString("XY-Rate Limit FIX: [%1]=>[%2]").arg(inputLine).arg(line);
                list.append(line);
            }
            else
            {
                list.append(inputLine);
            }

            xyRateSet = false;

            return list;
        }
    }

    list.append(inputLine);
    return list;

}

void GCode::gotoXYZ(QString line)
{
    pollPosWaitForIdle(false);

    if (sendGcodeLocal(line))
    {
        line = line.toUpper();

        bool moveDetected = false;

        QStringList list = line.split(QRegExp("[\\s]+"));
        for (int i = 0; i < list.size(); i++)
        {
            QString item = getMoveAmountFromString("X", list.at(i));
            if (item.length() > 0)
            {
                moveDetected = true;
            }
            item = getMoveAmountFromString("Y", list.at(i));
            if (item.length() > 0)
            {
                moveDetected = true;
            }
            item = getMoveAmountFromString("Z", list.at(i));
            if (item.length() > 0)
            {
                moveDetected = true;
            }
        }

        if (!moveDetected)
        {
            //emit addList("No movement expected for command.");
        }

        pollPosWaitForIdle(false);
    }
    else
    {
        QString msg(QString("Bad command: %1").arg(line));
        warn("%s", msg.toLocal8Bit().constData());
        emit addList(msg);
    }

    emit setCommandText("");
}


QString GCode::getMoveAmountFromString(QString prefix, QString item)
{
    int index = item.indexOf(prefix);
    if (index != -1)
    {
        return item.mid(index + 1);
    }
    return "";
}

void GCode::axisAdj(char axis, float coord, bool inv, bool absoluteAfterAxisAdj)
{
    if (inv)
    {
        coord =- coord;
    }

    QString cmd = QString("G01 ").append(axis)
            .append(QString::number(coord));

    if (axis == 'Z')
    {
        cmd.append(" F").append(QString::number(zJogRate));
    }

    SendJog(cmd, absoluteAfterAxisAdj);

    emit adjustedAxis();
}

bool GCode::SendJog(QString cmd, bool absoluteAfterAxisAdj)
{
    pollPosWaitForIdle(false);

    // G91 = distance relative to previous
    bool ret = sendGcodeLocal("G91\r");

    bool result = ret && sendGcodeLocal(cmd.append("\r"));

    if (result)
    {
        pollPosWaitForIdle(false);
    }

    if (absoluteAfterAxisAdj)
        sendGcodeLocal("G90\r");

    return result;
}

// settings change calls here
void GCode::setResponseWait(int wt, double zj, bool useMm, bool zRL, double zRLAmount, double xyRAmount, bool useAggr, bool ffCommands)
{
    waitTime = wt;
    zJogRate = zj;
    zRateLimit = zRL;
    zRateLimitAmount = zRLAmount;
    xyRateAmount = xyRAmount;
    useAggressivePreload = useAggr;
    filterFileCommands = ffCommands;

    if ((userSetMmMode != useMm) && isPortOpen() && doubleDollarFormat)
    {
        if (useMm)
        {
            setConfigureMmMode(true);
        }
        else
        {
            setConfigureInchesMode(true);
        }
    }

    userSetMmMode = useMm;

    setUnitsTypeDisplay(useMm);
}

int GCode::getSettingsItemCount()
{
    return settingsItemCount.get();
}

// 0.8c and above only!
void GCode::checkAndSetCorrectMeasurementUnits()
{
    sendGcodeLocal(REQUEST_PARSER_STATE_V08c, false);

    if (incorrectMeasurementUnits)
    {
        if (userSetMmMode)
        {
            emit addList("Options specify use mm but Grbl parser set for inches. Fixing.");
            setConfigureMmMode(true);
        }
        else
        {
            emit addList("Options specify use inches but Grbl parser set for mm. Fixing.");
            setConfigureInchesMode(true);
        }
        incorrectMeasurementUnits = false;// hope this is ok to do here
        sendGcodeLocal(REQUEST_CURRENT_POS);
    }
    else
    {
        sendGcodeLocal(SETTINGS_COMMAND_V08c);

        if (incorrectLcdDisplayUnits)
        {
            if (userSetMmMode)
            {
                emit addList("Options specify use mm but Grbl reporting set for inches. Fixing.");
                setConfigureMmMode(false);
            }
            else
            {
                emit addList("Options specify use inches but Grbl reporting set for mm. Fixing.");
                setConfigureInchesMode(false);
            }
        }
        incorrectLcdDisplayUnits = false;
    }
}

void GCode::setOldFormatMeasurementUnitControl()
{
    if (userSetMmMode)
        sendGcodeLocal("G21");
    else
        sendGcodeLocal("G20");
}

void GCode::setConfigureMmMode(bool setGrblUnits)
{
    sendGcodeLocal("$13=0");
    if (setGrblUnits)
        sendGcodeLocal("G21");
    sendGcodeLocal(REQUEST_CURRENT_POS);
}

void GCode::setConfigureInchesMode(bool setGrblUnits)
{
    sendGcodeLocal("$13=1");
    if (setGrblUnits)
        sendGcodeLocal("G20");
    sendGcodeLocal(REQUEST_CURRENT_POS);
}

void GCode::setUnitsTypeDisplay(bool millimeters)
{
    if (millimeters)
    {
        emit setUnitsWork("(mm)");
        emit setUnitsMachine("(mm)");
    }
    else
    {
        emit setUnitsWork("(in)");
        emit setUnitsMachine("(in)");
    }
}

void GCode::clearToHome()
{
    maxZ = 0;
    motionOccurred = false;
}
