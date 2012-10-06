#include "rs232.h"

RS232::RS232()
{
}
#ifdef Q_WS_X11

int Cport[12],
    error;

struct termios new_port_settings,
       old_port_settings[22];

char comports[12][13]={"/dev/ttyUSB0","/dev/ttyUSB1","/dev/ttyUSB2","/dev/ttyUSB3","/dev/ttyUSB4",
                       "/dev/ttyUSB5","/dev/ttyACM0","/dev/ttyACM1","/dev/ttyACM2","/dev/ttyACM3",
                       "/dev/ttyACM4","/dev/ttyACM5"};


int RS232::OpenComport(int comport_number)
{
  int baudr= B9600;
  if((comport_number>27)||(comport_number<0))
  {
    printf("illegal comport number\n");
    return(1);
  }
  //baudr = B9600;

  Cport[comport_number] = open(comports[comport_number], O_RDWR | O_NOCTTY | O_NDELAY);
  if(Cport[comport_number]==-1)
  {
    perror("unable to open comport ");
    return(1);
  }

  error = tcgetattr(Cport[comport_number], old_port_settings + comport_number);
  if(error==-1)
  {
    close(Cport[comport_number]);
    perror("unable to read portsettings ");
    return(1);
  }
  memset(&new_port_settings, 0, sizeof(new_port_settings));  /* clear the new struct */

  new_port_settings.c_cflag = baudr | CS8 | CLOCAL | CREAD;
  new_port_settings.c_iflag = IGNPAR;
  new_port_settings.c_oflag = 0;
  new_port_settings.c_lflag = 0;
  new_port_settings.c_cc[VMIN] = 0;      /* block untill n bytes are received */
  new_port_settings.c_cc[VTIME] = 0;     /* block untill a timer expires (n * 100 mSec.) */
  error = tcsetattr(Cport[comport_number], TCSANOW, &new_port_settings);
  if(error==-1)
  {
    close(Cport[comport_number]);
    perror("unable to adjust portsettings ");
    return(1);
  }

  return(0);
}


int RS232::PollComport(int comport_number, char *buf, int size)
{
  int n;

#ifndef __STRICT_ANSI__                       /* __STRICT_ANSI__ is defined when the -ansi option is used for gcc */
  if(size>SSIZE_MAX)  size = (int)SSIZE_MAX;  /* SSIZE_MAX is defined in limits.h */
#else
  if(size>4096)  size = 4096;
#endif

  n = read(Cport[comport_number], buf, size);

  return(n);
}


int RS232::SendBuf(int comport_number, const char *buf, int size)
{
    char b[300] = {0};
    memcpy(b, buf, size);
    printf("Sending to port %d [%s]:", comport_number, b);
    for (int x= 0; x < size; x++)
    {
        printf("%02X ", buf[x]);
    }
    printf("\n");
    fflush(stdout);


  return(write(Cport[comport_number], buf, size));
}


void RS232::CloseComport(int comport_number)
{
  close(Cport[comport_number]);
  tcsetattr(Cport[comport_number], TCSANOW, old_port_settings + comport_number);
}

void RS232::Reset(int comport_number) //still to test
{
    //OpenComport(comport_number);
    int mcr=!TIOCM_DTR;
    ioctl(Cport[comport_number],TIOCMSET,&mcr);
    //CloseComport(comport_number);
}

#else

HANDLE Cport[16];

char comports[16][10]={"\\\\.\\COM1",  "\\\\.\\COM2",  "\\\\.\\COM3",  "\\\\.\\COM4",
                       "\\\\.\\COM5",  "\\\\.\\COM6",  "\\\\.\\COM7",  "\\\\.\\COM8",
                       "\\\\.\\COM9",  "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12",
                       "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15", "\\\\.\\COM16"};

char baudr[64];

int RS232::OpenComport(int comport_number)
{
  if((comport_number>15)||(comport_number<0))
  {
    printf("illegal comport number\n");
    return(1);
  }
  strcpy(baudr, "baud=9600 data=8 parity=N stop=1");

  Cport[comport_number] = CreateFileA(comports[comport_number],
                      GENERIC_READ|GENERIC_WRITE,
                      0,
                      0,
                      OPEN_EXISTING,
                      FILE_ATTRIBUTE_NORMAL,
                      0);

  if(Cport[comport_number]==INVALID_HANDLE_VALUE)
  {
    printf("unable to open comport\n");
    return(1);
  }

  DCB port_settings;
  memset(&port_settings, 0, sizeof(port_settings));  /* clear the new struct  */
  port_settings.DCBlength = sizeof(port_settings);

  if (!GetCommState(Cport[comport_number], &port_settings))

  {
      printf("Error getting state\n");
      CloseHandle(Cport[comport_number]);
      return(1);
  }

  /*if(!BuildCommDCBA(baudr, &port_settings))
  {
    printf("unable to set comport dcb settings\n");
    CloseHandle(Cport[comport_number]);
    return(1);
  }*/

  port_settings.BaudRate=CBR_9600;
  port_settings.ByteSize=8;
  port_settings.StopBits=ONESTOPBIT;
  port_settings.Parity=NOPARITY;
  port_settings.fDtrControl=DTR_CONTROL_ENABLE;

  if(!SetCommState(Cport[comport_number], &port_settings))
  {
    printf("unable to set comport cfg settings\n");
    CloseHandle(Cport[comport_number]);
    return(1);
  }

  COMMTIMEOUTS Cptimeouts={0};

  Cptimeouts.ReadIntervalTimeout         = 250;
  Cptimeouts.ReadTotalTimeoutMultiplier  = 0;
  Cptimeouts.ReadTotalTimeoutConstant    = 250;
  Cptimeouts.WriteTotalTimeoutMultiplier = 0;
  Cptimeouts.WriteTotalTimeoutConstant   = 200;

  if(!SetCommTimeouts(Cport[comport_number], &Cptimeouts))
  {
      QMessageBox(QMessageBox::Warning,"Error","Could not read port.",QMessageBox::Ok).exec();
      printf("unable to set comport time-out settings\n");
      CloseHandle(Cport[comport_number]);
      return(1);
  }

  return(0);
}

int RS232::PollComport(int comport_number, char *buf, int size)
{
  DWORD n;
  int b;

  if(size>4096)  size = 4096;

/* added the void pointer cast, otherwise gcc will complain about */
/* "warning: dereferencing type-punned pointer will break strict aliasing rules" */

  ReadFile(Cport[comport_number], buf, size, &n, NULL);
  //ReadFile(Cport[comport_number], buf, size, &n, 0);
  b=int(n);
  return(b);
}

int RS232::SendBuf(int comport_number, const char *buf, int size)
{
  int n;

  if(WriteFile(Cport[comport_number], buf, size, (LPDWORD)((void *)&n), NULL))
  {
    return(n);
  }

  return(-1);
}

void RS232::CloseComport(int comport_number)
{
  CloseHandle(Cport[comport_number]);
}

void RS232::Reset(int comport_number) //Tested 24/02/12
{
    //OpenComport(comport_number);
    DWORD dtr=5;
    EscapeCommFunction(Cport[comport_number],dtr);
    //CloseComport(comport_number);
}

#endif

void RS232::flush(int comport_number)
{
    int n=1;
    char buf[255];
    while(n>0)
        n=PollComport(comport_number,buf,255);
}
