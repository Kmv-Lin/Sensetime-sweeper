#include "httpthread.h"
#include <QDebug>

HttpReadThread::HttpReadThread(int flag1,QObject *parent) :
    QThread(parent),flag(flag1),
    stopped(false)
{
    m_pAutoTimer      =new QTimer;

    selfCheckManger = new QNetworkAccessManager();
    vehicleStateManger = new QNetworkAccessManager();

    connect(vehicleStateManger, SIGNAL(finished(QNetworkReply *)), this, SLOT(vehicleStateFinished(QNetworkReply *)));
    connect(selfCheckManger, SIGNAL(finished(QNetworkReply *)), this, SLOT(selfCheckFinished(QNetworkReply *)));

    QString fullRequest = requestHeader+requestState;//QString("/vehicle_state");
    vehicleStateManger->get(QNetworkRequest(QUrl(fullRequest)));
    selfCheckManger->get(QNetworkRequest(QUrl(fullRequest)));

    if(this->flag == Start_Window){
        times = 2000;
    }else if(this->flag == Main_Window){
        times = 10000;
    }
    connect(m_pAutoTimer, SIGNAL(timeout()), this, SLOT(timeAuto()));
    //getDiskSpaceManger = new QNetworkAccessManager();
    //aboutDeviceManger = new QNetworkAccessManager();


}

void HttpReadThread::stop()
{
    stopped = true;
    m_pAutoTimer->stop();
    selfCheckManger->deleteLater();
    //vehicleStateManger->deleteLater();
}

unsigned char HttpReadThread::asciiToHex(unsigned char aHex)
{
    if( (aHex>=0) && (aHex<=9) ){
        aHex+=0x30;

    }
    else if( (aHex>=10) && (aHex<=15) )
        aHex+=0x37;
    else {
        aHex=0xFF;
    }
    return aHex;
}

void HttpReadThread::timeAuto()
{
    stopped = false;
    QString fullRequest = requestHeader+requestState;//QString("/vehicle_state");
    vehicleStateManger->get(QNetworkRequest(QUrl(fullRequest)));
    selfCheckManger->get(QNetworkRequest(QUrl(fullRequest)));
}

void HttpReadThread::run()
{
    m_pAutoTimer->start(times);
    qDebug() << "********HttpReadThread start ********";

    while(!stopped)
    {
        //connect(selfCheckManger, SIGNAL(finished(QNetworkReply *)), this, SLOT(selfCheckFinished(QNetworkReply *)));
        //selfCheckManger->get(QNetworkRequest(QUrl("http://www.baidu.com")));//get(QNetworkRequest(QUrl("http://192.168.8.100:10058/api/vehicle_state")));
        //qDebug() << "********get http://192.168.8.100:10058/api/vehicle_state ********";
        //QString str="{\"result\": {\"mission_info\": {\"finished_missions_num\": 0,\"mission_all_num\": 0}}, \"self_check_info\": {\"brake\": 0, \"close_brake_light\": 0, \"close_dust\": 0, \"close_headlight\": 0, \"close_spray\": 0, \"close_turn_left_light\": 0, \"close_turn_right_light\": 0, \"close_vacuum\": 0, \"forward\": 0, \"open_brake_light\": 0, \"open_dust\": 0, \"open_headlight\": 0, \"open_spray\": 0,\"open_turn_left_light\": 0,\"open_turn_right_light\": 0, \"open_vacuum\": 0,\"reverse\": 0, \"turn_left\": 0, \"turn_right\": 0}, \"status\": \"success\", \"vehicle_state\": \"FaultState\"}";
        //emit selfCheckSignal(str);

//        connect(vehicleStateManger, SIGNAL(finished(QNetworkReply *)), this, SLOT(vehicleStateFinished(QNetworkReply *)));
//        vehicleStateManger->get(QNetworkRequest(QUrl("http://192.168.1.100:10058/vehicle_state")));

//        connect(getDiskSpaceManger, SIGNAL(finished(QNetworkReply *)), this, SLOT(getDiskSpaceFinished(QNetworkReply *)));
//        getDiskSpaceManger->get(QNetworkRequest(QUrl("http://127.0.0.1/json.txt")));

//        connect(aboutDeviceManger, SIGNAL(finished(QNetworkReply *)), this, SLOT(aboutDeviceFinished(QNetworkReply *)));
//        aboutDeviceManger->get(QNetworkRequest(QUrl("http://127.0.0.1/json.txt")));


        //stopped =true;
    }
    qDebug() << "********HttpReadThread stop ********";

}

void HttpReadThread::selfCheckFinished(QNetworkReply *reply)
{
    QString str = reply->readAll();
    //str="{\"result\": {\"mission_info\": {\"finished_missions_num\": 0,\"mission_all_num\": 0}}, \"self_check_info\": {\"brake\": 0, \"close_brake_light\": 0, \"close_dust\": 0, \"close_headlight\": 0, \"close_spray\": 0, \"close_turn_left_light\": 0, \"close_turn_right_light\": 0, \"close_vacuum\": 0, \"forward\": 0, \"open_brake_light\": 0, \"open_dust\": 0, \"open_headlight\": 0, \"open_spray\": 0,\"open_turn_left_light\": 0,\"open_turn_right_light\": 0, \"open_vacuum\": 0,\"reverse\": 0, \"turn_left\": 0, \"turn_right\": 0}, \"status\": \"success\", \"vehicle_state\": \"FaultState\"}";
     //qDebug() <<str;
    emit selfCheckSignal(str);
    reply->deleteLater();
}

void HttpReadThread::vehicleStateFinished(QNetworkReply *reply)
{
    QString str = reply->readAll();
    emit vehicleStateSignal(str);
    qDebug() << "mainwindow:"<<str;
    reply->deleteLater();
}

void HttpReadThread::getDiskSpaceFinished(QNetworkReply *reply)
{
    QString str = reply->readAll();
    emit getDiskSpaceSignal(str);
    reply->deleteLater();
}

void HttpReadThread::aboutDeviceFinished(QNetworkReply *reply)
{
    QString str = reply->readAll();
    emit aboutDeviceSignal(str);
    reply->deleteLater();
}

