#include "starting_thread.h"

starting_thread::starting_thread(QObject *parent) : QThread(parent)
{
    stopped=false;
}

void starting_thread::run()
{
    qDebug() << "********start_thread running ********";
//    for(size_t i=0;i<100;i++){              //10s
//        for(size_t j=0; j<12;j++)
//        {
//            QThread::msleep(100);
//            emit start_progress(i);
//        }
//    }

    for(size_t i=0;i<50;i++){
            QThread::msleep(100);
            emit start_progress(i);
    }
    emit start_progress(100);
    qDebug() << "********start_thread stop ********";
    stopped=true;
}
void starting_thread::stop()
{
    stopped=true;
}
