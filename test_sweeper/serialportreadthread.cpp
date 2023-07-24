#include "serialportreadthread.h"
#include <QDebug>

SerialPortReadThread::SerialPortReadThread(QObject *parent) :
    QThread(parent),
    stopped(false)
{
    char *pBuff = new char[64];
}

void SerialPortReadThread::setWeiQianFunctions(SerialPort *serialPort)
{
    m_pSerialPort = serialPort;
}

void SerialPortReadThread::stop()
{
    stopped = true;
}

void SerialPortReadThread::run()
{
    int lenth=0;
    while(!stopped) {
        QString str ;
        //char *pBuff = new char[64];
        int size = m_pSerialPort->read(pBuff, 64);
        if(size > 0) {
            emit receiveSignal(pBuff);
        }

    }
     delete pBuff;
    stopped = false;
}
