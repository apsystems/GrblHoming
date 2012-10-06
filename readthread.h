#ifndef READTHREAD_H
#define READTHREAD_H

#include <QFile>
#include <QThread>
#include <QTextStream>
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
    QString removeInvalid(QString line);
    bool SendGcode(QString line);
    bool toolChangeRoutine();
    bool waitForPrompt(const char *expectStr);
    //variables
    bool abort;


};


#define OK_PROMPT "ok\r\n"

#endif // READTHREAD_H
