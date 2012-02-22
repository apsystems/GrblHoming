#include "readthread.h"


ReadThread::ReadThread(QObject *parent) :
    QThread(parent)
{
    abort=false;
    path="";
}

void ReadThread::stopsig()
{
    if(isRunning())
    {
        abort=true;
        emit sendMsg("Process interrupted.");
    }
}

void ReadThread::run()
{
    abort=false;
    RS232 port = RS232();
    QFile file(this->path);
    int cport_nr = this->cport_nr;
    int i=0, n=0,sent=0;
    char buf[50];
    for(i=0;i<50;i++)
        buf[i]=0;
    if(file.open(QFile::ReadOnly))
    {
        QTextStream code(&file);
        QString strline = code.readLine();
#ifndef DISCONNECTED
        if(port.OpenComport(cport_nr))
            emit sendMsg("Error opening port");
        else
#endif
        {
            while((strline!=NULL)&&(!abort))
            {
                char line[75];
                if(strline.at(0)=='(')
                {}//ignore comments
                else
                {
                    strline.append("\n");
                    for(i=0;i<strline.length();i++)
                        line[i]=strline.at(i).toAscii();
#ifndef DISCONNECTED
#ifdef Q_WS_WIN32
                    buf[0]='\n';
                    port.SendBuf(cport_nr,buf,1);
                    buf[0]=0;
#endif
                    while(n==0)
                    {
                        n=port.PollComport(cport_nr,buf,50);
#ifdef Q_WS_X11
                        usleep(100000);  // sleep for 100 milliSeconds
#else
                        Sleep(100);
#endif
                    }
                    sent=port.SendBuf(cport_nr,line,i);
                    n=port.PollComport(cport_nr,buf,50);
#endif
/*#ifdef Q_WS_X11
            usleep(100000);  // sleep for 100 milliSeconds
#else
            Sleep(100);
#endif*/
#ifdef DEBUG
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
                    //while(n==0)
#ifndef DISCONNECTED
                    //while(((n==0)||(port.find_txt(buf)==0))&&(!abort))
                    while((n==0)&&(!abort))
                    {
                        n = port.PollComport(cport_nr, buf, 50);
                    }
#else
#ifdef Q_WS_X11
            usleep(100000);  // sleep for 100 milliSeconds
#else
            Sleep(100);
#endif
#endif
                }
                //addList(strline);
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
                char line[] = "G00 X0 Y0 Z0\n";
                port.SendBuf(cport_nr,line,13);
                sendAxis("G00 X0 Y0 Z0\n");
                addList("Gone Home.");
            }
        }
#ifndef DISCONNECTED
        port.CloseComport(cport_nr);
#endif
    }
}
