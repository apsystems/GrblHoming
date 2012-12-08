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
#define APPLICATION_NAME "GCodeSender"
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
    void axisAdj(char axis, float coord, bool inv, float prevValue);
    void setResponseWait(int waitTime, int zJogRate);
    void setProgress(int percent);
    void setRuntime(QString runtime);

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
    void stopSending(bool resetPort);
    //
    void portIsOpen(bool sendCode);
    void portIsClosed(bool reopen);
    void lcdDisplay(char axis, float value);
    void adjustedAxis();

    //check boxes
    void toggleSpindle();
    //communications
        //options
    void setSettings();
        //thread
    void receiveList(QString msg);
    void receiveListOut(QString msg);
    void receiveMsg(QString msg);
    void receiveAxis(QString axis);
    //menu bar
    void getOptions();
    void showAbout();
    void resetLcds();

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
    int zJogRate;
    //methods
    int SendJog(QString strline);
    void UpdateAxis(QString code);
    void readSettings();
    void writeSettings();
    void addToStatusList(bool in, QString msg);
    void disableAllButtons();
    void openPortCtl(bool reopen);
    void resetProgress();
};



#endif // MAINWINDOW_H
