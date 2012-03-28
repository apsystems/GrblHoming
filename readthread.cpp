#include "readthread.h"

//#define DEBUG

ReadThread::ReadThread(QObject *parent) :
    QThread(parent)
{
    abort=false;
    path="";
}

void ReadThread::run()
{
    port.flush(port_nr);
    abort=false;
    QFile file(this->path);
    QString received="";
    //int port_nr = this->port_nr;
    int i=0, n=0,sent=0;
    char buf[20];
    if(file.open(QFile::ReadOnly))
    {
        QTextStream code(&file);
        QString strline = code.readLine();
        while((strline!=NULL)&&(!abort))
        {
            for(i=0;i<20;i++)
                buf[i]=0;
            strline = strline.trimmed();
            if((strline.at(0)=='(')||(strline.at(0)=='%'))
            {}//ignore comments
            else
            {
                if((strline.contains("M06",Qt::CaseInsensitive))&&(toolChange))
                {
                    sendAxis(strline);
                    toolChangeRoutine();
                    while(!strline.contains("M03",Qt::CaseInsensitive))
                        strline=code.readLine();
                    strline=strline.trimmed().append("\r");
#ifndef DISCONNECTED
                    SendGcode(strline);
#else
                    //qDebug<<strline;
                    //qDebug(strline);
                    //printf((char *)strline);
                    printf(strline.toLocal8Bit().data());
#endif
                }
                else if((strline.contains("M06",Qt::CaseInsensitive))&&(!toolChange))
                {
                    while(!strline.contains("M03",Qt::CaseInsensitive))
                        strline=code.readLine();
                    strline=strline.trimmed().append("\r");
#ifndef DISCONNECTED
                    SendGcode(strline);
#else
                    //printf((char *)strline);
                    printf(strline.toLocal8Bit().data());
#endif
                }
                else
                {
                    strline.append("\r");
                    char line[70];
                    for(i=0;i<strline.length();i++)
                        line[i]=strline.at(i).toAscii();
#ifndef DISCONNECTED
                    sent=port.SendBuf(port_nr,line,i);
#else
                    //printf(strline);
                    printf(strline.toLocal8Bit().data());
#endif
                    //n=port.PollComport(port_nr,buf,4);

#ifdef Q_WS_X11
                    usleep(100000);  // sleep for 100 milliSeconds
#else
                    Sleep(100);
#endif
#ifdef DEBUGER
                    if(n > 0)
                    {
                        buf[n] = 0;   // always put a "null" at the end of a string!
                        for(i=0; i < n; i++)
                        {
                            if(buf[i] < 32)  // replace unreadable control-codes by dots
                            {
                                buf[i] = '.';
                            }
                        }
                        printf("received %i bytes: %s\n", n, (char *)buf);
                        printf("%s\n",(char *)buf);
                    }
                    else
                    {
                        printf("received %i bytes.\n", n);
                    }
#endif
                    n=1;
                    while((!received.contains("ok",Qt::CaseInsensitive))&&(!abort)&&(n>0))
                    {
#ifndef DISCONNECTED
                        n = port.PollComport(port_nr, buf, 4);
                        received=QString(buf);
#else
                        received="\r\nok";
#endif
#ifdef Q_WS_X11
                        usleep(100000);  // sleep for 100 milliSeconds
#else
                        Sleep(100);
#endif
                    }
                    if(n==0)
                    {
                        addList(QString("Line error in ").append(strline.mid(0,strline.length()-1)));
                        abort=true;
                        break;
                    }
                }
            }
#ifdef Q_WS_X11
                        usleep(100000);  // sleep for 100 milliSeconds
#else
                        Sleep(100);
#endif
            sendAxis(strline);
            strline=code.readLine();
        }
        file.close();
        if(!abort)
        {
            emit sendMsg("Code sent successfully.");
            addList("Code sent successfully.");
        }
        else
            addList("Process interrupted.");
        if(this->goHome)
        {
#ifndef DISCONNECTED
            //char line[] = "G00 X0 Y0 Z0\r";
            //port.SendBuf(port_nr,line,13);
            port.SendBuf(port_nr,"G00 X0 Y0 Z0\r",13);
#endif
            sendAxis("G00 X0 Y0 Z0\r");
            addList("Gone Home.");
        }
    }
}

int ReadThread::SendGcode(QString line)
{
    QString received="";
    port.flush(port_nr);
    int i, n=0;
    line.append('\r');
    char buf[50];
    for(i=0;i<50;i++)
        buf[i]=0;
    for(i=0;i<line.length();i++)
        buf[i]=line.at(i).toAscii();

    if(!port.SendBuf(port_nr,buf,line.length()))
        return 0;
    else
    {
        while((!received.contains("ok",Qt::CaseInsensitive))&&(n>0))
        {
            n = port.PollComport(port_nr, buf, 4);
            received=QString(buf);
#ifdef Q_WS_X11
            usleep(100000);  // sleep for 100 milliSeconds
#else
            Sleep(100);
#endif
        }
        if(n==0)
            return(0);
    }
    return(1);
}

void ReadThread::stopsig()
{
    if(isRunning())
    {
        abort=true;
        emit sendMsg("Process interrupted.");
    }
}

void ReadThread::toolChangeRoutine()
{
    QString line = "G01 X";
    line.append(QString::number(toolChangeXYZ[0])).append(" Y").append(QString::number(toolChangeXYZ[1]))
            .append(" Z").append(QString::number(toolChangeXYZ[2])).append(" F260\r");
    SendGcode(line);
    QMessageBox(QMessageBox::Information,"Change tool","Change the bit. Press OK to continue.",QMessageBox::Ok).exec();
}
