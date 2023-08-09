#include "setting_thread.h"

setting_thread::setting_thread(QObject *parent) : QThread(parent)
{
        stopped=false;
}
void setting_thread::run()
{
    qDebug() << "********setting_thread running ********";
    for(size_t i=0;i<=100;i++){                                             //used for system update
           if(!stopped){
                QThread::msleep(100);
                emit updating_progress(i,"running");
           }else{
               emit updating_progress(i,"stopped");
                break;
           }
    }
    //emit updating_progress(100);
    qDebug() << "********setting_thread stop ********";
    stopped=true;
}
void setting_thread::stop()
{
    stopped=true;
}
