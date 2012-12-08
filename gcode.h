/****************************************************************
 * gcode.h
 * GrblHoming - zapmaker fork on github
 *
 * 15 Nov 2012
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#ifndef GCODE_H
#define GCODE_H

#include <QString>
#include <QFile>
#include <QThread>
#include <QTextStream>
#include "definitions.h"
#include "rs232.h"

#define SHORT_WAIT_SEC 1
#define LONG_WAIT_SEC  100

#define BUF_SIZE 300

#define RESPONSE_OK "ok"
#define RESPONSE_OK_CRLF "ok\r\n"
#define RESPONSE_ERROR "error"


class GCode : public QObject
{
    Q_OBJECT

public:
    GCode();
    void setAbort();
    void setReset();
    void setShutdown();

signals:
    void addList(QString line);
    void addListOut(QString line);
    void sendMsg(QString msg);
    void sendAxis(QString axis);
    void stopSending(bool resetPort);
    void portIsClosed(bool reopen);
    void portIsOpen(bool sendCode);
    void lcdDisplay(char axis, float value);
    void setCommandText(QString value);
    void adjustedAxis();
    void gcodeResult(int id, QString result);
    void setProgress(int);
    void resetTimer(bool timeIt);
    void resetLcds();

public slots:
    void openPort(QString commPortStr);
    void closePort(bool reopen);
    void sendGcode(QString line);
    void sendGcodeAndGetResult(int id, QString line);
    void sendFile(QString path);
    void gotoXYZ(QString line);
    void axisAdj(char axis, float coord, bool inv, float prevValue);
    void setResponseWait(int waitTime, int zJogRate);

protected:
    void timerEvent(QTimerEvent *event);

private:
    bool resetPort();
    bool sendGcodeLocal(QString line, bool recordResponseOnFail = false, int waitSec = -1);
    bool waitForOk(QString& result, int waitCount);
    bool sendGcodeInternal(QString line, QString& result, bool recordResponseOnFail, int waitSec);
    QString removeInvalidMultipleGCommands(QString line);
    bool isGCommandValid(int value);
    bool isPortOpen();
    QString getMoveAmountFromString(QString prefix, QString item);
    bool SendJog(QString strline, bool relative);

private:
    RS232 port;
    bool abortState;
    bool resetState;
    bool shutdownState;
    int waitTime;
    int zJogRate;
    int errorCount;
    QString currComPort;

};

#endif // GCODE_H
