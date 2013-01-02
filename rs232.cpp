/****************************************************************
 * rs232.cpp
 * GrblHoming - zapmaker fork on github
 *
 * 15 Nov 2012
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#include "rs232.h"

RS232::RS232()
    : port(NULL)
{
}

bool RS232::OpenComport(QString commPortStr)
{
    if (port != NULL)
        CloseComport();

    PortSettings settings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};

    port = new QextSerialPort(commPortStr, settings, QextSerialPort::Polling);

    port->open(QIODevice::ReadWrite);

    return port->isOpen();
}


int RS232::PollComport(char *buf, int size)
{
    if (port == NULL || !port->isOpen())
        return 0;

    int n = port->bytesAvailable();
    if (!n)
        return 0;

    n = port->read(buf, size);
    return(n);
}


int RS232::SendBuf(const char *buf, int size)
{
    if (port == NULL || !port->isOpen())
        return 0;

    char b[300] = {0};
    memcpy(b, buf, size);
#ifdef DIAG
    printf("Sending to port %s [%s]:", port->portName().toLocal8Bit().constData(), b);
    for (int x= 0; x < size; x++)
    {
        printf("%02X ", buf[x]);
    }
    printf("\n");
    fflush(stdout);
#endif
    return(port->write(buf, size));
}


void RS232::CloseComport()
{
    if (port != NULL)
    {
        port->close();
        delete port;
        port = NULL;
    }
}

void RS232::Reset() //still to test
{
    if (port != NULL)
        port->reset();
}

void RS232::flush()
{
    int n=1;
    char buf[255];

    while (n > 0)
        n = PollComport(buf,255);
}

bool RS232::isPortOpen()
{
    if (port == NULL)
        return false;

    return port->isOpen();
}

