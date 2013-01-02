/****************************************************************
 * main.cpp
 * GrblHoming - zapmaker fork on github
 *
 * 15 Nov 2012
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#include <QtGui/QApplication>
#include "mainwindow.h"

FILE *pDebugLogFile = NULL;
AtomicIntBool g_enableDebugLog;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    int result = a.exec();

    if (pDebugLogFile != NULL)
    {
        fclose(pDebugLogFile);
        pDebugLogFile = NULL;
    }
    return result;
}


//------------------------------

void diag(const char *str, ...)
{
#ifdef DEBUG
    va_list args;

    va_start(args, str );
    vfprintf(stdout, str, args);
    va_end(args);
    fflush(stdout);
#endif

    if (g_enableDebugLog.get())
    {
        if (pDebugLogFile == NULL)
        {
            QString path = QDir::homePath() + "/GrblControllerLog.txt";

            // TODO: may not work on non-english systems
            pDebugLogFile = fopen(path.toLocal8Bit().constData(), "w");
            if (pDebugLogFile == NULL)
            {
                fprintf(stderr, "Error opening debug log file\n");
            }
            else
            {
                fprintf(pDebugLogFile, "%s\n", QDateTime::currentDateTime().toLocalTime().toString(Qt::TextDate).toLocal8Bit().constData());
            }
        }

        if (pDebugLogFile != NULL)
        {
            va_list args;

            va_start(args, str );
            vfprintf(pDebugLogFile, str, args);
            va_end(args);
            fflush(pDebugLogFile);
        }
    }
}
