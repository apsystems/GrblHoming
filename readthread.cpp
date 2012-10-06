#include "readthread.h"

#define BUF_SIZE    302

ReadThread::ReadThread(QObject *parent) :
    QThread(parent)
{
    abort=false;
    path="";
}

QString ReadThread::removeInvalid(QString line)
{
    QStringList components = line.split(" ",QString::SkipEmptyParts);
    line = "";
    QString s;
    foreach (s, components)
    {
        int value = s.mid(1,-1).toInt();
        if (value > 4 && value < 17)
        {}
        else if (value > 21 && value < 28)
        {}
        else if (value > 28 && value < 53)
        {}
        else if (value > 53 && value < 80)
        {}
        else if (value > 80 && value < 90)
        {}
        else if (value > 94)
        {}
        else
            line.append(s).append(" ");
    }
    return line.trimmed();
}

void ReadThread::run()
{
    port.Reset(port_nr);

    //printf("Flushing\n"); fflush(stdout);

    if (waitForPrompt("\r\n"))
    {
        //printf("Got OK\n"); fflush(stdout);
    }
    else
    {
        //printf("Didn't get OK\n"); fflush(stdout);
    }

    //printf("Ready to send\n"); fflush(stdout);

    abort = false;
    QFile file(this->path);
    if (file.open(QFile::ReadOnly))
    {
        QTextStream code(&file);
        QString strline = code.readLine();
        while ((code.atEnd() == false) && (!abort))
        {
            strline = strline.trimmed();
            if ((strline.at(0) == '(') || (strline.at(0) == '%'))
            {}//ignore comments
            else
            {
                strline = strline.toUpper();
                strline = strline.replace("M6", "M06");
                int times = strline.count("G");
                if (times > 1)
                {
                    strline = removeInvalid(strline);
                }
                strline = strline.replace(QRegExp("\\s+"), " ");
                int num = strline.mid(1, strline.indexOf(" ")).toInt();
                if (strline.contains("G", Qt::CaseInsensitive) && num > 4 && num < 17)
                {}
                else if (strline.contains("G", Qt::CaseInsensitive) && num > 21 && num < 28)
                {}
                else if (strline.contains("G", Qt::CaseInsensitive) && num > 28 && num < 53)
                {}
                else if (strline.contains("G", Qt::CaseInsensitive) && num > 53 && num < 80)
                {}
                else if (strline.contains("G", Qt::CaseInsensitive) && num > 80 && num < 90)
                {}
                else if (strline.contains("G", Qt::CaseInsensitive) && num > 94)
                {}
                else if (strline.contains("M", Qt::CaseInsensitive) && num == 1)
                {}
                else if (strline.contains("M", Qt::CaseInsensitive) && num > 7)
                {}
                else if ((strline.contains("M06", Qt::CaseInsensitive)) && (toolChange))
                {
                    sendAxis(strline);
                    if (!toolChangeRoutine())
                    {
                        abort = true;
                        break;
                    }
                    while (!strline.contains("M03", Qt::CaseInsensitive))
                    {
                        strline = code.readLine();
                        strline = strline.replace("M3", "M03");
                    }
                    strline = strline.replace(QRegExp("\\s+"), " ");
                    strline = strline.trimmed().append("\r");
#ifndef DISCONNECTED
                    if (!SendGcode(strline))
                    {
                        abort =true;
                        break;
                    }
                    //printf("Sending GCode1:%s", strline.toLocal8Bit().data()); fflush(stdout);
#else
                    printf(strline.append("\n").toLocal8Bit().data());
#endif
                }
                else if ((strline.contains("M06", Qt::CaseInsensitive)) && (!toolChange))
                {
                    while (!strline.contains("M03", Qt::CaseInsensitive))
                    {
                        strline = code.readLine();
                        strline = strline.replace("M3", "M03");
                    }
                    strline = strline.replace(QRegExp("\\s+"), " ");
                    strline = strline.trimmed().append("\r");
#ifndef DISCONNECTED
                    if (!SendGcode(strline))
                    {
                        abort = true;
                        break;
                    }
                    //printf("Sending GCode2:%s", strline.toLocal8Bit().data()); fflush(stdout);
#else
                    printf(strline.append("\n").toLocal8Bit().data());
#endif
                }
                else
                {
                    strline.append("\r");
                    if (strline.length() >= BUF_SIZE)
                    {
                        //printf("ERROR: buf size too small for line\n"); fflush(stdout);
                        addList(QString("Line too long: ").append(strline.mid(0, strline.length() - 1)));
                        abort = true;
                        break;
                    }
                    char line[BUF_SIZE + 1] = {0};
                    int i;
                    for (i = 0; i < strline.length(); i++)
                        line[i] = strline.at(i).toAscii();
#ifndef DISCONNECTED
                    port.SendBuf(port_nr, line, i);
                    //printf("Sending Buf:%s", strline.toLocal8Bit().data()); fflush(stdout);
#else
                    printf(strline.append("\n").toLocal8Bit().data());
#endif
#ifdef Q_WS_X11
                    usleep(100000);  // sleep for 100 milliSeconds
#else
                    Sleep(100);
#endif
                    if (!waitForPrompt(OK_PROMPT))
                    {
                        addList(QString("Line error in ").append(strline.mid(0, strline.length() - 1)));
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
        if (!abort)
        {
            emit sendMsg("Code sent successfully.");
            addList("Code sent successfully.");
        }
        else
            addList("Process interrupted.");
        /* usually mills a diagonal line through finished work, don't use this until
           a better solution can be found
        if (this->goHome)
        {
#ifndef DISCONNECTED
            port.SendBuf(port_nr, "G00 X0 Y0 Z0 F250\r", 18);
#endif
            sendAxis("G00 X0 Y0 Z0 F250\r");
            addList("Gone Home.");
        }
        */
    }
}

bool ReadThread::SendGcode(QString line)
{
    QString received="";

    line.append('\r');
    char buf[BUF_SIZE + 1] = {0};
    if (line.length() >= BUF_SIZE)
    {
        //printf("ERROR: buf size too small\n"); fflush(stdout);
        addList(QString("Buffer too small for line ").append(line.mid(0, line.length() - 1)));
        return false;
    }
    for (int i = 0; i < line.length(); i++)
        buf[i] = line.at(i).toAscii();

    if (!port.SendBuf(port_nr, buf, line.length()))
    {
        //printf("SENDBUF FAILED\n"); fflush(stdout);
        addList(QString("Failed sending line ").append(line.mid(0, line.length() - 1)));
        return false;
    }
    else
    {
        if (!waitForPrompt(OK_PROMPT))
        {
            //printf("WAITFOROK FAILED\n"); fflush(stdout);
            addList(QString("Failed waiting for OK for ").append(line.mid(0, line.length() - 1)));
            return false;
        }
    }
    return true;
}

bool ReadThread::waitForPrompt(const char *expectStr)
{
    char tmp[BUF_SIZE + 1] = {0};
    int count = 0;
    bool status = true;
    QString result;
    while (!result.contains(expectStr))
    {
#ifndef DISCONNECTED
        int n = port.PollComport(port_nr, tmp, BUF_SIZE);
        if (n == 0)
        {
            count++;
#ifdef Q_WS_X11
            usleep(500000);  // sleep for 500 milliSeconds
#else
            Sleep(500);
#endif
        }
        else
        {
            tmp[n] = 0;
            result.append(tmp);
            printf("GOT:[%s]\n", tmp);
            for (int x= 0; x < n; x++)
            {
                printf("%02X ", tmp[x]);
            }
            printf("\n");
            fflush(stdout);
            count = 0;
        }
#else
        result = RESPONSE_EXPECT;
#endif
#ifdef Q_WS_X11
        usleep(100000);  // sleep for 100 milliSeconds
#else
        Sleep(100);
#endif
        if (count > 1000)
        {\
            status = false;
            break;
        }
    }

    if (status)
    {/*
#ifdef Q_WS_X11
        usleep(200000);  // sleep for 200 millseconds
#else
        Sleep(200);
#endif
        */
    }

    return status;
}

void ReadThread::stopsig()
{
    if (isRunning())
    {
        abort=true;
        emit sendMsg("Process interrupted.");
    }
}

bool ReadThread::toolChangeRoutine()
{
    QString line = "G01 X";
    line.append(QString::number(toolChangeXYZ[0])).append(" Y").append(QString::number(toolChangeXYZ[1]))
            .append(" Z").append(QString::number(toolChangeXYZ[2])).append(" F260\r");
    if (!SendGcode(line))
        return false;
    QMessageBox(QMessageBox::Information,"Change tool","Change the bit. Press OK to continue.",QMessageBox::Ok).exec();
    return true;
}
