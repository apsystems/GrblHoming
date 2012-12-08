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
    : abortState(false), resetState(false), shutdownState(false), waitTime(LONG_WAIT_SEC), zJogRate(DEFAULT_Z_JOG_RATE),
      errorCount(0)
{
    // use base class's timer - use it to capture random text from the controller
    startTimer(1000);
}

bool GCode::resetPort()
{
    port.CloseComport();
    emit addList("Resetting port to restart controller");
    emit resetLcds();

    if (port.OpenComport(currComPort))
    {
        QString result;
        // We wait for OK, but some versions may not send it, so timeout without error message if that happens
        waitForOk(result, SHORT_WAIT_SEC);
        return true;
    }
    else
    {
        return false;
    }
}

void GCode::openPort(QString commPortStr)
{
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
    this->abortState = true;
}

// Reset means immediately stop waiting for a response
void GCode::setReset()
{
    // cross-thread operation, only set one atomic variable in this method (bool in this case) or add critsec
    resetState = true;
}

// Shutdown means app is shutting down - we give thread about .3 sec to exit what it is doing
void GCode::setShutdown()
{
    // cross-thread operation, only set one atomic variable in this method (bool in this case) or add critsec
    shutdownState = true;
}

// Slot called from other threads (i.e. main window, grbl dialog, etc.)
void GCode::sendGcode(QString line)
{
    if (line.length() == 0)
    {
        // empty line means we want to see what the device responds with. Pass params to not fail if nothing happens.
        sendGcodeLocal(line, true, SHORT_WAIT_SEC);
    }
    else
    {
        // normal send of actual commands
        sendGcodeLocal(line, false);
    }
}

// Slot called from other thread that returns whatever text comes back from the controller
void GCode::sendGcodeAndGetResult(int id, QString line)
{
    QString result;

    emit sendMsg("");
    resetState = false;
    if (!sendGcodeInternal(line, result, false, SHORT_WAIT_SEC))
        result.clear();

    emit gcodeResult(id, result);
}

// To be called only from this class, not from other threads. Use above two methods for that.
// Wraps sendGcodeInternal() to allow proper handling of failure cases, etc.
bool GCode::sendGcodeLocal(QString line, bool recordResponseOnFail /* = false */, int waitSec /* = -1 */)
{
    QString result;
    sendMsg("");
    resetState = false;
    bool ret = sendGcodeInternal(line, result, recordResponseOnFail, waitSec);
    if (shutdownState)
        return false;

    if (!ret && (!recordResponseOnFail || resetState))
    {
        if (!resetState)
            emit stopSending(false);

        if (!ret && resetState)
        {
            resetState = false;
            port.Reset();
        }
    }
    resetState = false;
    return ret;
}

// Wrapped method. Should only be called from above method.
bool GCode::sendGcodeInternal(QString line, QString& result, bool recordResponseOnFail, int waitSec)
{
    if (!port.isPortOpen())
    {
        QString msg = "Port not available yet";
        emit addList(msg);
        emit sendMsg(msg);
        return false;
    }

    // adds to UI list, but prepends a > indicating a sent command
    emit addListOut(line);

    if (line.size() == 0 && !line.endsWith('\r'))
        line.append('\r');

    char buf[BUF_SIZE + 1] = {0};
    if (line.length() >= BUF_SIZE)
    {
        QString msg = "Buffer size too small";
        emit addList(msg);
        emit sendMsg(msg);
        return false;
    }
    for (int i = 0; i < line.length(); i++)
        buf[i] = line.at(i).toAscii();

    diag("SENDING: %s\n", buf);
    if (!port.SendBuf(buf, line.length()))
    {
        diag("SENDBUF FAILED\n");
        QString msg = "Sending to port failed";
        emit addList(msg);
        emit sendMsg(msg);
        return false;
    }
    else
    {
        int waitSecActual = waitSec == -1 ? waitTime : waitSec;

        if (!waitForOk(result, waitSecActual))
        {
            diag("WAITFOROK FAILED\n");
            if (shutdownState)
                return false;

            if (!recordResponseOnFail && !(resetState || abortState))
            {
                QString msg = "Wait for ok failed";
                emit addList(msg);
                emit sendMsg(msg);
            }

            return false;
        }
    }
    return true;
}

bool GCode::waitForOk(QString& result, int waitSec)
{
    char tmp[BUF_SIZE + 1] = {0};
    int count = 0;
    int waitCount = waitSec * 10;// multiplier depends on sleep values below
    bool status = true;
    result.clear();
    while (!result.contains(RESPONSE_OK_CRLF) && !result.contains(RESPONSE_ERROR) && !resetState)
    {
        int n = port.PollComport(tmp, BUF_SIZE);
        if (n == 0)
        {
            count++;
            SLEEP(100);
        }
        else
        {
            tmp[n] = 0;
            result.append(tmp);
            diag("GOT:%s\n", tmp);
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

    if (shutdownState)
    {
        return false;
    }

    if (status)
    {
        SLEEP(100);

        if (resetState)
        {
            emit addList("Wait interrupted via Reset");
        }
    }

    if (result.contains(RESPONSE_ERROR))
    {
        errorCount++;
        // skip over errors
        //status = false;
    }

    QStringList list = QString(result).split("\r\n");
    for (int i = 0; i < list.size(); i++)
    {
        if (list.at(i).length() > 0 && list.at(i) != RESPONSE_OK)
            emit addList(list.at(i));
    }

    if (resetState)
    {
        // we have been told by the user to stop.
        status = false;
    }

    return status;
}

// called once a second to capture any random strings that come from the controller
void GCode::timerEvent(QTimerEvent *event)
{
    if (port.isPortOpen())
    {
        char tmp[BUF_SIZE + 1] = {0};
        QString result;

        for (int i = 0; i < 10 && !shutdownState && !resetState; i++)
        {
            int n = port.PollComport(tmp, BUF_SIZE);
            if (n == 0)
                break;

            tmp[n] = 0;
            result.append(tmp);
            diag("GOT-TE:%s\n", tmp);
        }

        if (shutdownState)
        {
            return;
        }

        QStringList list = QString(result).split("\r\n");
        for (int i = 0; i < list.size(); i++)
        {
            if (list.at(i).length() > 0 && list.at(i) != "ok")
                emit addList(list.at(i));
        }
    }
}

void GCode::sendFile(QString path)
{
    addList(QString("Sending file '%1'").arg(path));

    if (!resetPort())
    {
        QString msg = "Unable to send file. Can't reopen com port.";

        emit sendMsg(msg);
        emit addList(msg);
        return;
    }

    // send something to be sure the controller is ready
    //sendGcodeLocal("", true, SHORT_WAIT_SEC);

    setProgress(0);

    errorCount = 0;
    abortState = false;
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

        emit resetTimer(true);

        float currLine = 0;
        QString strline = code.readLine();
        while ((code.atEnd() == false) && (!abortState))
        {
            strline = strline.trimmed();
            if ((strline.at(0) == '(') || (strline.at(0) == '%'))
            {}//ignore comments
            else
            {
                strline = strline.toUpper();
                strline = strline.replace("M6", "M06");
                int times = strline.count("G");
                if (times > 1)
                {
                    strline = removeInvalidMultipleGCommands(strline);
                }
                strline = strline.replace(QRegExp("\\s+"), " ");
                int num = strline.mid(1, strline.indexOf(" ")).toInt();
                if (strline.contains("G", Qt::CaseInsensitive)
                    && (!isGCommandValid(num)))
                {
                    emit addList(QString("Removing unsupported G command '%1'").arg(strline));
                }
                else if (strline.contains("M", Qt::CaseInsensitive)
                         && (num == 1 || num > 7))
                {
                    emit addList(QString("Removing unsupported M command '%1'").arg(strline));
                }
                else
                {
                    strline.append("\r");
                    if (!sendGcodeLocal(strline, false))
                    {
                        abortState = true;
                        break;
                    }
                }
            }
            emit sendAxis(strline);

            float percentComplete = (++currLine * 100.0) / totalLineCount;
            setProgress((int)percentComplete);

            strline = code.readLine();
        }
        file.close();

        emit resetTimer(false);

        if (shutdownState)
        {
            return;
        }

        QString msg;
        if (!abortState)
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

        // Removed code that would return back to home, because this could cause
        // piece to get cut unintentionally due to beeline back to home
    }

    if (!resetState)
    {
        emit stopSending(!abortState);
    }
}

QString GCode::removeInvalidMultipleGCommands(QString line)
{
    QStringList components = line.split(" ",QString::SkipEmptyParts);
    QString tmp;
    QString s;
    foreach (s, components)
    {
        int value = s.mid(1,-1).toInt();
        if (isGCommandValid(value))
            tmp.append(s).append(" ");
        else
            emit addList(QString("Removing unsupported G command '%1'").arg(s));
    }
    return tmp.trimmed();
}

bool GCode::isGCommandValid(int value)
{
    return !((value > 4 && value < 17)
            || (value > 21 && value < 28)
            || (value > 28 && value < 53)
            || (value > 53 && value < 80)
            || (value > 80 && value < 90)
            || (value > 94));
}

void GCode::gotoXYZ(QString line)
{
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
                lcdDisplay('X', item.toFloat());
                moveDetected = true;
            }
            item = getMoveAmountFromString("Y", list.at(i));
            if (item.length() > 0)
            {
                lcdDisplay('Y', item.toFloat());
                moveDetected = true;
            }
            item = getMoveAmountFromString("Z", list.at(i));
            if (item.length() > 0)
            {
                lcdDisplay('Z', item.toFloat());
                moveDetected = true;
            }
        }

        if (!moveDetected)
        {
            emit addList("No movement expected for command.");
        }
    }
    else
    {
        emit addList("Bad command.");
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

void GCode::axisAdj(char axis, float coord, bool inv, float prevValue)
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

    if (SendJog(cmd, true))
    {
        lcdDisplay(axis, prevValue + coord);
    }

    emit adjustedAxis();
}

bool GCode::SendJog(QString cmd, bool relative)
{
    bool ret = true;
    if (relative)
    {
        // G91 = distance relative to previous
        ret = sendGcodeLocal("G91\r");
    }
    return ret && sendGcodeLocal(cmd.append("\r"));
}

// settings change calls here
void GCode::setResponseWait(int wt, int zj)
{
    waitTime = wt;
    zJogRate = zj;
}
