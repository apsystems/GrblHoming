#include "readthread.h"

ReadThread::ReadThread(QObject *parent) :
    QThread(parent)
{
    abort=false;
    path="";
}

QString ReadThread::removeInvalid(QString line)
{
    QStringList components = line.split(" ",QString::SkipEmptyParts);
    line="";
    QString s;
    foreach(s,components)
    {
        int value = s.mid(1,-1).toInt();
        if(value>4&&value<17)
        {}
        else if(value>21&&value<28)
        {}
        else if(value>28&&value<53)
        {}
        else if(value>53&&value<80)
        {}
        else if(value>80&&value<90)
        {}
        else if(value>94)
        {}
        else
            line.append(s).append(" ");
    }
    return line.trimmed();
}

void ReadThread::run()
{
    port.Reset(port_nr);
    port.flush(port_nr);
    abort=false;
    QFile file(this->path);
    QString received="";
    int i=0, n=0;
    char buf[40];
    if(file.open(QFile::ReadOnly))
    {
        QTextStream code(&file);
        QString strline = code.readLine();
        for(i=0;i<40;i++)
            buf[i]=0;
        while((code.atEnd()==false)&&(!abort))
        {
            strline = strline.trimmed();
            if((strline.at(0)=='(')||(strline.at(0)=='%'))
            {}//ignore comments
            else
            {
                strline = strline.toUpper();
                strline = strline.replace("M6","M06");
                int times = strline.count("G");
                if (times>1)
                {
                    strline = removeInvalid(strline);
                }
                strline =strline.replace(QRegExp("\\s+")," ");
                int num = strline.mid(1,strline.indexOf(" ")).toInt();
                if(strline.contains("G",Qt::CaseInsensitive)&&num>4&&num<17)
                {}
                else if(strline.contains("G",Qt::CaseInsensitive)&&num>21&&num<28)
                {}
                else if(strline.contains("G",Qt::CaseInsensitive)&&num>28&&num<53)
                {}
                else if(strline.contains("G",Qt::CaseInsensitive)&&num>53&&num<80)
                {}
                else if(strline.contains("G",Qt::CaseInsensitive)&&num>80&&num<90)
                {}
                else if(strline.contains("G",Qt::CaseInsensitive)&&num>94)
                {}
                else if (strline.contains("M",Qt::CaseInsensitive)&&num==1)
                {}
                else if (strline.contains("M",Qt::CaseInsensitive)&&num>7)
                {}
                else if((strline.contains("M06",Qt::CaseInsensitive))&&(toolChange))
                {
                    sendAxis(strline);
                    toolChangeRoutine();
                    while(!strline.contains("M03",Qt::CaseInsensitive))
                    {
                        strline=code.readLine();
                        strline = strline.replace("M3","M03");
                    }
                    strline =strline.replace(QRegExp("\\s+")," ");
                    strline=strline.trimmed().append("\r");
#ifndef DISCONNECTED
                    SendGcode(strline);
#else
                    printf(strline.append("\n").toLocal8Bit().data());
#endif
                }
                else if((strline.contains("M06",Qt::CaseInsensitive))&&(!toolChange))
                {
                    while(!strline.contains("M03",Qt::CaseInsensitive))
                    {
                        strline=code.readLine();
                        strline = strline.replace("M3","M03");
                    }
                    strline =strline.replace(QRegExp("\\s+")," ");
                    strline=strline.trimmed().append("\r");
#ifndef DISCONNECTED
                    SendGcode(strline);
#else
                    printf(strline.append("\n").toLocal8Bit().data());
#endif
                }
                else
                {
                    strline.append("\r");
                    char line[80];
                    for(i=0;i<strline.length();i++)
                        line[i]=strline.at(i).toAscii();
#ifndef DISCONNECTED
                    port.SendBuf(port_nr,line,i);
#else
                    printf(strline.append("\n").toLocal8Bit().data());
#endif
#ifdef Q_WS_X11
                    usleep(100000);  // sleep for 100 milliSeconds
#else
                    Sleep(100);
#endif
                    n=1;
                    int cuenta=0;
                    received="";
                    //while((!received.contains("ok",Qt::CaseInsensitive))&&(!abort)&&(n>0))
                    while(1)
                    {
#ifndef DISCONNECTED
                        n = port.PollComport(port_nr, buf, 4);
                        if(n==0)
                        {
                            port.flush(port_nr);
                            received="";
                            port.SendBuf(port_nr,line,i);
                            cuenta++;
#ifdef Q_WS_X11
                        usleep(500000);  // sleep for 100 milliSeconds
#else
                        Sleep(500);
#endif
                        }
                        else
                        {
                            received=QString(buf);
                        }
#else
                        received="\r\nok";
#endif
#ifdef Q_WS_X11
                        usleep(100000);  // sleep for 100 milliSeconds
#else
                        Sleep(100);
#endif
                        if((received.contains("ok"))||(abort)||(cuenta>10))
                            break;
                    }
                    if(n==0)
                    {
                        addList(QString("Line error in ").append(strline.mid(0,strline.length()-1)));
                        abort=true;
                        break;
                    }
                }
            }
/*#ifdef Q_WS_X11
                        usleep(100000);  // sleep for 100 milliSeconds
#else
                        Sleep(100);
#endif*/
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
            port.SendBuf(port_nr,"G00 X0 Y0 Z0 F250\r",18);
#endif
            sendAxis("G00 X0 Y0 Z0 F250\r");
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

void ReadThread::stopSig()
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
