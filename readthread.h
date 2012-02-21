#ifndef READTHREAD_H
#define READTHREAD_H

#include <QThread>
#include <QTextStream>
#include <QFile>
#include "rs232.h"

#define CONNECTED
#define DEBUG

class ReadThread : public QThread
{
    Q_OBJECT
public:
    explicit ReadThread(QObject *parent = 0);
    QString path;
    int cport_nr;
    bool goHome;

protected:
    void run();

    //void readFile();
    
signals:
    void addList(QString line);
    void sendMsg(QString msg);
    void sendAxis(QString axis);
    
public slots:
    void stopsig();

private:
    bool abort;

};

#endif // READTHREAD_H
