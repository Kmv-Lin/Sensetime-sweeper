#include "my_thread.h"

#include <QThread>
#include <QDebug>

my_thread::my_thread(QObject *parent) : QThread(parent)
{
    stopped=false;

}

void my_thread::run()
{
    qDebug() << "********my_thread running ********";

    for(size_t i=1; i<100;i++)
    {
        QThread::msleep(100);
        emit progress(i,"running");
    }

    emit progress(100,"stoped");
    qDebug() << "********my_thread stop ********";
    stopped=true;
}

void my_thread::stop()
{
    stopped=true;
}
