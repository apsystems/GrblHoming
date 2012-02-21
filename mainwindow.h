#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTextStream>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void incX();
    void incY();
    void incZ();
    void decX();
    void decY();
    void decZ();
    void adjust();
    void send();
    void open();
    void begin();
    void stop();
    
private:
    Ui::MainWindow *ui;
    int SendJog(float X, float Y, float Z);
    int SendGcode(char* line, int length);
    void UpdateAxis(QString code);
};

#endif // MAINWINDOW_H
