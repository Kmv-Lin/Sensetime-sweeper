#include "setting_http.h"

#include "com_setting.h"
setting_http::setting_http(int api,int way,QString str, QObject *parent) : QThread(parent),API(api),WAY(way),data(str),stopped(false)
{
    setManager = new QNetworkAccessManager();

    connect(setManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(setFinished(QNetworkReply *)));
    set_num setnum = (set_num)api;
    way_choice waynum = (way_choice)way;
    switch(setnum){
        case network:
            set_requestState = set_network;break;
        case disk:
            set_requestState = set_disk;break;
        case version:
            set_requestState = set_version;break;
        case volume:
        //设置请求数据（我这里发送的是json格式数据）
            strdata = str;
            bytearray.append(strdata);
            qDebug() << " ================== " << bytearray.constData();
//        char cByte[1024] = "{\"volume\":\value\}";
//        QByteArray byte(cByte);
            set_requestState = set_volume;break;
    }

    QString fullRequest = set_requestHeader+set_requestState;
    qDebug() << "fullRequest" << fullRequest;
    setRequest.setUrl(QUrl(fullRequest));
    setRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    switch(waynum){
        case get:
            setManager->get(setRequest);break;
        case post:
            setManager->post(setRequest,bytearray);break;
        case put:
            setManager->put(setRequest,bytearray);break;
        case deleteResource:
            setManager->deleteResource(setRequest);break;
    }

}

void setting_http::stop()
{
    stopped = true;
    setManager->deleteLater();
}

void setting_http::run()
{

    qDebug() << "********setting_http start ********";

//    while(!stopped)
//    {
//    }
//    qDebug() << "********setting_http stop ********";

        ptm = new QTimer();
        ptm->start(500);
        exec();

}

void setting_http::setFinished(QNetworkReply *reply){
    QString set_data = reply->readAll();
    qDebug() << "set_data:" << set_data;
    emit Set_data(set_data);
    reply->deleteLater();
}

setting_http::~setting_http()
{
     //while(!stopped);
    ptm->deleteLater();
     qDebug() <<"********setting_http close  ********";
}

