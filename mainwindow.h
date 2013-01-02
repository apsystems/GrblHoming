/****************************************************************
 * mainwindow.h
 * GrblHoming - zapmaker fork on github
 *
 * 15 Nov 2012
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QSettings>
#include <QCloseEvent>
#include "about.h"
#include "definitions.h"
#include "grbldialog.h"
#include "options.h"
//#include "filesender.h"
#include "timer.h"
#include "gcode.h"

#define COMPANY_NAME "zapmaker"
#define APPLICATION_NAME "GrblController"
#define DOMAIN_NAME "org.zapmaker"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

    //variables
    int delete_nr;

signals:
    //threads
    void openPort(QString port);
    void closePort(bool reopen);
    void shutdown();
    void sendGcode(QString line, bool recordResponseOnFail = false, int waitCount = SHORT_WAIT_SEC);
    void sendFile(QString path);
    void gotoXYZ(QString line);
    void axisAdj(char axis, float coord, bool inv, bool absoluteAfterAxisAdj);
    void setResponseWait(int waitTime, double zJogRate, bool useMm, bool zRateLimit, double zRateLimitAmount);
    void setProgress(int percent);
    void setRuntime(QString runtime);
    void sendSetHome();
    void sendGrblReset();
    void sendGrblUnlock();
    void goToHome();

private slots:
    //buttons
    void openPort();
    void setGRBL();
        //Adjust
    void decX();
    void decY();
    void decZ();
    void incX();
    void incY();
    void incZ();
    void setHome();
        //manual
    void gotoXYZ();
        //send Gcode
    void begin();
    void openFile();
    void stop();
    void stopSending();
    //
    void portIsOpen(bool sendCode);
    void portIsClosed(bool reopen);
    void adjustedAxis();

    //check boxes
    void toggleSpindle();
    void toggleRestoreAbsolute();

    //communications
        //options
    void setSettings();
        //thread
    void receiveList(QString msg);
    void receiveListOut(QString msg);
    void receiveMsg(QString msg);
    //menu bar
    void getOptions();
    void showAbout();
    void enableGrblDialogButton();
    void grblReset();
    void grblUnlock();
    void updateCoordinates(Coord3D machineCoord, Coord3D workCoord);
    void goHomeSafe();

private:
    //objects
    Ui::MainWindow *ui;
    //FileSender fileSender;
    //QThread fileSenderThread;
    GCode gcode;
    QThread gcodeThread;
    Timer timer;
    QThread timerThread;

    //variables
    bool invX;
    bool invY;
    bool invZ;
    QString styleSheet;
    QString directory;
    QString nameFilter;
    QString lastOpenPort;
    QByteArray fileOpenDialogState;
    int waitTime;
    double zJogRate;
    Coord3D machineCoordinates;
    Coord3D workCoordinates;
    bool absoluteAfterAxisAdj;
    bool useMm;
    bool zRateLimiting;
    double zRateLimitAmount;

    //methods
    int SendJog(QString strline);
    void readSettings();
    void writeSettings();
    void addToStatusList(bool in, QString msg);
    void disableAllButtons();
    void openPortCtl(bool reopen);
    void resetProgress();
    void refreshLcd();
    void lcdDisplay(char axis, bool workCoord, float value);
    void updateSettingsFromOptionDlg(QSettings& settings);
};



#endif // MAINWINDOW_H
