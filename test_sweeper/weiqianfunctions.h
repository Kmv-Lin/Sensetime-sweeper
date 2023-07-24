#ifndef WEIQIANFUNCTIONS_H
#define WEIQIANFUNCTIONS_H

#include <QLibrary>

class WeiqianFunctions {

public:
    WeiqianFunctions();

    static void InitCan(int baudrate);
    static int OpenCan();
    static void CloseCan();
    static int CanWrite(int canId, const char *sendData,int dateLen);
    static int CanRead(int *datalenght, char *data);

    static int OpenSerialPort(char *path, int baud, int databits, char *parity, int stopbits);
    static void CloseSerialPort(int fd);
    static int ReadSerialPort(int fd, char *buff, int count);
    static int WriteSerialPort(int fd, char *buff, int count);

    static void Beep();
    static void SetBackLight(int flag);

    static void StarWatchDog();
    static void StopWatchDog();
    static void SetWatchDog(int timeout);
    static int GetWatchDog();
    static void FeedWatchDog();
    static void WatchDogEnable(int flag);

    static int SetMacAddress(const char *macAddress);


    static QLibrary *m_pWeiqianLib;

private:
    static void load();

};

#endif // WEIQIANFUNCTIONS_H
