#ifndef RS232_H
#define RS232_H

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

class RS232
{
public:
    RS232();
    int OpenComport(int comport_number);
    int PollComport(int comport_number, char *buf, int size);
    int SendBuf(int comport_number, char *buf, int size);
    void CloseComport(int comport_number);
    int find_txt(char *buf);

};

#endif // RS232_H
