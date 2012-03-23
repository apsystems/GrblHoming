#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include "definitions.h"

namespace Ui {
class Options;
}

class Options : public QDialog
{
    Q_OBJECT
    
public:
    explicit Options(QWidget *parent = 0);
    ~Options();
    void accept();
    void readFile();

signals:
    void toolCoord(float coords[]);
    void sendSettings(int settings);

private slots:
    void enableToolsAddr(bool status);
    
private:
    Ui::Options *ui;
    //variables
    int settings;
    //methods
    void writeFile();

};

#endif // OPTIONS_H
