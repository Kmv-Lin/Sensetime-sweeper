#include "serialport.h"
#include "weiqianfunctions.h"

SerialPort::SerialPort()
{
    m_fd = -1;
}

void SerialPort::open(char *path, int baud, int databits, char *parity, int stopbits)
{
    m_fd = WeiqianFunctions::OpenSerialPort(path, baud, databits, parity, stopbits);
}

void SerialPort::close()
{
    WeiqianFunctions::CloseSerialPort(m_fd);
}

int SerialPort::read(char *buff, int count)
{
    return WeiqianFunctions::ReadSerialPort(m_fd, buff, count);
}

int SerialPort::write(char *buff, int count)
{
    return WeiqianFunctions::WriteSerialPort(m_fd, buff, count);
}


