#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

class SerialPort
{
public:
    SerialPort();
    void open(char *path, int baud, int databits, char *parity, int stopbits);
    void close();
    int write(char *buff, int count);
    int read(char *buff, int count);

private:
    int m_fd;
};

#endif // SERIALPORT_H
