#ifndef READTHREAD_H
#define READTHREAD_H

#include <QThread>
#include <QTextStream>
#include <QFile>
#include "rs232.h"
#include "definitions.h"

class ReadThread : public QThread
{
    Q_OBJECT
public:
    explicit ReadThread(QObject *parent = 0);
    //variables
    QString path;
    int port_nr;
    bool toolChange;
    float toolChangeXYZ[3];
    //objects
    RS232 port;
    //methods
    bool goHome;

protected:
    void run();
    
signals:
    void addList(QString line);
    void sendMsg(QString msg);
    void sendAxis(QString axis);
    
public slots:
    void stopsig();

private:
    //methods
    int SendGcode(QString line);
    void toolChangeRoutine();
    //variables
    bool abort;


};

#endif // READTHREAD_H
