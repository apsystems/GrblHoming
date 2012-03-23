#ifndef RS232_H
#define RS232_H

#include <QtGlobal>
#include <QMessageBox>

#include <stdio.h>
#include <string.h>
#ifdef Q_WS_X11
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#else
#include <Windows.h>
#endif

#include "definitions.h"

class RS232
{
public:
    RS232();
    //methods
    int OpenComport(int comport_number);
    int PollComport(int comport_number, char *buf, int size);
    int SendBuf(int comport_number, char *buf, int size);
    void CloseComport(int comport_number);
    void Reset(int comport_number);
    void flush(int comport_number);
};

#endif // RS232_H
