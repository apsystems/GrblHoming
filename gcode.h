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

#include "log4qtdef.h"

#include <QString>
#include <QFile>
#include <QThread>
#include <QTextStream>
#include "definitions.h"
#include "rs232.h"
#include "coord3d.h"

#define SHORT_WAIT_SEC 2
#define LONG_WAIT_SEC  100

#define BUF_SIZE 300

#define RESPONSE_OK "ok"
#define RESPONSE_ERROR "error"

// as defined in the grbl project on github...
#define GRBL_RX_BUFFER_SIZE     128

#define CTRL_X '\x18'

class GCode : public QObject
{
    Q_OBJECT

public:
    GCode();
    void setAbort();
    void setReset();
    void setShutdown();
    int getSettingsItemCount();

    static void trimToEnd(QString& strline, QChar);

signals:
    void addList(QString line);
    void addListFull(QStringList list);
    void addListOut(QString line);
    void sendMsg(QString msg);
    void stopSending();
    void portIsClosed(bool reopen);
    void portIsOpen(bool sendCode);
    void setCommandText(QString value);
    void adjustedAxis();
    void gcodeResult(int id, QString result);
    void setProgress(int);
    void resetTimer(bool timeIt);
    void enableGrblDialogButton();
    void updateCoordinates(Coord3D machineCoord, Coord3D workCoord);
    void setLastState(QString state);
    void setUnitsWork(QString value);
    void setUnitsMachine(QString value);
    void setLivePoint(double x, double y, bool isMM);
    void setVisCurrLine(int currLine);

public slots:
    void openPort(QString commPortStr);
    void closePort(bool reopen);
    void sendGcode(QString line);
    void sendGcodeAndGetResult(int id, QString line);
    void sendFile(QString path);
    void gotoXYZ(QString line);
    void axisAdj(char axis, float coord, bool inv, bool absoluteAfterAxisAdj);
    void setResponseWait(int waitTime, double zJogRate, bool useMm, bool zRateLimit, double zRateLimitAmount, double xyRateLimitAmount,
                         bool useAggressivePreload, bool filterFileCommands);
    void grblSetHome();
    void sendGrblReset();
    void sendGrblUnlock();
    void goToHome();

protected:
    void timerEvent(QTimerEvent *event);

private:
    bool sendGcodeLocal(QString line, bool recordResponseOnFail = false, int waitSec = -1, bool aggressive = false);
    bool waitForOk(QString& result, int waitCount, bool sentReqForLocation, bool sentReqForParserState, bool aggressive);
    bool waitForStartupBanner(QString& result, int waitSec, bool failOnNoFound);
    bool sendGcodeInternal(QString line, QString& result, bool recordResponseOnFail, int waitSec, bool aggressive);
    QString removeUnsupportedCommands(QString line);
    bool isGCommandValid(float value);
    bool isMCommandValid(float value);
    bool isPortOpen();
    QString getMoveAmountFromString(QString prefix, QString item);
    bool SendJog(QString strline, bool absoluteAfterAxisAdj);
    void parseCoordinates(const QString& received, bool aggressive);
    void pollPosWaitForIdle(bool checkMeasurementUnits);
    void checkAndSetCorrectMeasurementUnits();
    void setOldFormatMeasurementUnitControl();
    void setUnitsTypeDisplay(bool millimeters);
    void setConfigureMmMode(bool setGrblUnits);
    void setConfigureInchesMode(bool setGrblUnits);
    QStringList doZRateLimit(QString strline, QString& msg, bool& xyRateSet);
    void sendStatusList(QStringList& listToSend);
    void clearToHome();
    bool checkGrbl(const QString& result);

private:
    RS232 port;
    AtomicIntBool abortState;
    AtomicIntBool resetState;
    AtomicIntBool shutdownState;
    int waitTime;
    double zJogRate;
    int errorCount;
    QString currComPort;
    bool doubleDollarFormat;
    AtomicIntBool settingsItemCount;
    QString lastState;
    bool incorrectMeasurementUnits;
    bool incorrectLcdDisplayUnits;
    bool userSetMmMode;
    Coord3D machineCoord, workCoord;
    Coord3D machineCoordLastIdlePos, workCoordLastIdlePos;
    bool zRateLimit;
    double zRateLimitAmount;
    double xyRateAmount;
    double maxZ;
    QList<int> sendCount;
    QTime parseCoordTimer;
    bool useAggressivePreload;
    bool motionOccurred;
    bool filterFileCommands;

    int sentI;
    int rcvdI;
};

#endif // GCODE_H
