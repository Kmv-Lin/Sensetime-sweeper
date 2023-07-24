#ifndef SERIALPORTREADTHREAD_H
#define SERIALPORTREADTHREAD_H

#include <QThread>
#include "serialport.h"
#include <QString>

class SerialPortReadThread : public QThread
{
    Q_OBJECT
public:
    explicit SerialPortReadThread(QObject *parent = 0);

    void setWeiQianFunctions(SerialPort *serialPort);

    void stop();

signals:
    void receiveSignal(const char *str);

public slots:

protected:
    void run();

private:
    SerialPort *m_pSerialPort;
    volatile bool stopped;
    QString str_test ;
    char pBuff[64];
};

#endif // SERIALPORTREADTHREAD_H
