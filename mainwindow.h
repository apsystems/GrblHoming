#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include "about.h"
#include "definitions.h"
#include "grbldialog.h"
#include "options.h"
#include "readthread.h"
#include "rs232.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //objects
    Options opt;
    RS232 port;
    //variables
    int delete_nr;
    int port_nr;

signals:
    //threads
    void Stop();
    //void sendPort(int port_nr);

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
    void reset();
        //manual
    void gotoHome();
    void gotoToolChange();
    void gotoXYZ();
        //send Gcode
    void begin();
    void openFile();
    void stop();
    //radio buttons
    void adjustRBtn();
    void manualRBtn();
    void sendRBtn();
    //combo boxes
    void selectFav(int selected);
    //check boxes
    void toggleSpindle();
    //communications
        //options
    void setSettings(int settings);
    void setTCCoord(float coords[]);
        //thread
    void receiveList(QString msg);
    void receiveMsg(QString msg);
    void receiveAxis(QString axis);
    //menu bar
    void getOptions();
    void showAbout();

    
private:
    //objects
    Ui::MainWindow *ui;
    ReadThread readthread;
    //variables
    bool goHome;
    bool toolChange;
    bool invX;
    bool invY;
    bool invZ;
    float toolChangeXYZ[3];
    bool portOpen;
    QString styleSheet;
    //methods
    int SendJog(QString strline);
    bool SendGcode(QString line);
    bool waitForOk();
    void UpdateAxis(QString code);
    void fillFavList();
    void readSettings();
};

#endif // MAINWINDOW_H
