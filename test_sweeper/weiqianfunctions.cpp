#include "weiqianfunctions.h"
#include <QLabel>
#include <QDateTime>

QLibrary *WeiqianFunctions::m_pWeiqianLib = new QLibrary();

WeiqianFunctions::WeiqianFunctions()
{
    load();
}

void WeiqianFunctions::InitCan(int baudrate)
{
    load();//
    typedef void (*Fun) (int);
     Fun fun = (Fun)m_pWeiqianLib->resolve("InitCan");
     if(fun) {
         fun(baudrate);
     }
}

int WeiqianFunctions::OpenCan()
{
    load();
    typedef int (*Fun) ();
     Fun fun = (Fun)m_pWeiqianLib->resolve("OpenCan");
     if(fun) {
         return fun();
     }
     return -1;
}

void WeiqianFunctions::CloseCan()
{
    load();
    typedef void (*Fun) ();
     Fun fun = (Fun)m_pWeiqianLib->resolve("CloseCan");
     if(fun) {
         fun();
     }
}

int WeiqianFunctions::CanWrite(int canId, const char *sendData,int dateLen)
{
    load();
    typedef int (*Fun) (int, const char *,int);
     Fun fun = (Fun)m_pWeiqianLib->resolve("CanWrite");
     if(fun) {
         return fun(canId, sendData,dateLen);
     }
     return -1;
}

int WeiqianFunctions::CanRead(int *datalenght, char *data)
{
    load();
    typedef int (*Fun) (int *, char *);
     Fun fun = (Fun)m_pWeiqianLib->resolve("CanRead");
     if(fun) {
         return fun(datalenght, data);
     }
     return -1;
}

int WeiqianFunctions::OpenSerialPort(char *path, int baud, int databits, char *parity, int stopbits)
{
    load();
    typedef int (*Fun) (char *, int, int, char *, int);
     Fun fun = (Fun)m_pWeiqianLib->resolve("OpenSerialPort");
     if(fun) {
         return fun(path, baud, databits, parity, stopbits);
     }
     return -1;
}

void WeiqianFunctions::CloseSerialPort(int fd)
{
    load();
    typedef void (*Fun) (int);
     Fun fun = (Fun)m_pWeiqianLib->resolve("CloseSerialPort");
     if(fun) {
         fun(fd);
     }
}

int WeiqianFunctions::ReadSerialPort(int fd, char *buff, int count)
{
    load();
    typedef int (*Fun) (int, char *, int);
     Fun fun = (Fun)m_pWeiqianLib->resolve("ReadSerialPort");
     if(fun) {
         return fun(fd, buff, count);
     }
     return -1;
}

int WeiqianFunctions::WriteSerialPort(int fd, char *buff, int count)
{
    load();
    typedef int (*Fun) (int, char *, int);
     Fun fun = (Fun)m_pWeiqianLib->resolve("WriteSerialPort");
     if(fun) {
         return fun(fd, buff, count);
     }
     return -1;
}

void WeiqianFunctions::Beep()
{
    load();
    typedef void (*Fun) ();
     Fun fun = (Fun)m_pWeiqianLib->resolve("Beep");
     if(fun) {
         fun();
     }
}

void WeiqianFunctions::SetBackLight(int flag)
{
    load();
    typedef void (*Fun) (int);
     Fun fun = (Fun)m_pWeiqianLib->resolve("SetBackLight");
     if(fun) {
         fun(flag);
     }
}

void WeiqianFunctions::StarWatchDog()
{
    load();
    typedef void (*Fun) ();
     Fun fun = (Fun)m_pWeiqianLib->resolve("StarWatchDog");
     if(fun) {
         fun();
     }
}

void WeiqianFunctions::StopWatchDog()
{
    load();
    typedef void (*Fun) ();
     Fun fun = (Fun)m_pWeiqianLib->resolve("StopWatchDog");
     if(fun) {
         fun();
     }
}

void WeiqianFunctions::SetWatchDog(int timeout)
{
    load();
    typedef void (*Fun) (int);
     Fun fun = (Fun)m_pWeiqianLib->resolve("SetWatchDog");
     if(fun) {
         fun(timeout);
     }
}

int WeiqianFunctions::GetWatchDog()
{
    load();
    typedef int (*Fun) ();
     Fun fun = (Fun)m_pWeiqianLib->resolve("GetWatchDog");
     if(fun) {
         return fun();
     }
     return -1;
}

void WeiqianFunctions::FeedWatchDog()
{
    load();
    typedef void (*Fun) ();
     Fun fun = (Fun)m_pWeiqianLib->resolve("FeedWatchDog");
     if(fun) {
         fun();
     }
}

void WeiqianFunctions::WatchDogEnable(int flag)
{
    load();
    typedef void (*Fun) (int);
     Fun fun = (Fun)m_pWeiqianLib->resolve("WatchDogEnable");
     if(fun) {
         fun(flag);
     }
}

int WeiqianFunctions::SetMacAddress(const char *macAddress)
{
    load();
    typedef int (*Fun) (const char *);
     Fun fun = (Fun)m_pWeiqianLib->resolve("SetMacAddress");
     if(fun) {
         return fun(macAddress);
     }
     return -1;
}

void WeiqianFunctions::load()
{
    if(!m_pWeiqianLib->isLoaded()) {
        m_pWeiqianLib = new QLibrary("/lib/libWeiqianHardware.so");
        m_pWeiqianLib->load();
    }
}

