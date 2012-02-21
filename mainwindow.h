#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "readthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void Stop();

private slots:
    void incX();
    void incY();
    void incZ();
    void decX();
    void decY();
    void decZ();
    void adjustBtn();
    void sendBtn();
    void open();
    void begin();
    void stop();
    void receiveList(QString msg);
    void receiveMsg(QString msg);
    void receiveAxis(QString axis);
    
private:
    Ui::MainWindow *ui;
    int SendJog(float X, float Y, float Z);
    int SendGcode(char* line, int length);
    void UpdateAxis(QString code);
    ReadThread readthread;
};

#endif // MAINWINDOW_H
