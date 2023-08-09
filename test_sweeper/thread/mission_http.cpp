#include "mission_http.h"
//static int  cnt = 0;                //test
//static QString token = "eyJhbGciOiJIUzI1NiJ9.eyJkYXRhIjp7InN5bXBob255X2RldmljZV9pZCI6ImJjNGJlNGM3MWZlMzdiZWZjMjgzZWMyMzM0ZDgzZGEyNWQzMGM4OGFhNGYxMTBhM2RiZDU0NzE5MTFiNDc2MzkiLCJzZWNyZXQiOiI0OWY4MTk4MDJkZjc0YjBlOTViYTZkN2JjZjQ5OTQyMSIsImxvZ2luX3R5cGUiOiJkZXZpY2UifSwiZXhwIjoxNjg5MDM5OTUwfQ.joS1K_OXQrFD1wtxlvHNaXpK_ZbVuf6DkfkMLpQDoIg";
static QString token;
static QString mis_Header;
Mission_http::Mission_http(int flag,QString ID,QObject *parent) :
    QThread(parent),flag1(flag),MissionID(ID),
    stopped(false)
{
    //m_pAutoTimer  =  new QTimer;
    //times = 2000;
    //connect(m_pAutoTimer, SIGNAL(timeout()), this, SLOT(timeAuto()));


    switch(this->flag1){
        case GET_TOKEN:
            tokenManager = new QNetworkAccessManager(this);
            connect(tokenManager,SIGNAL(finished(QNetworkReply*)),this, SLOT(tokenFinished(QNetworkReply*)));
            //fullRequest = "http://192.168.1.13:9958/api";                                                           //http://10.8.96.6:8087/v1/token 需要内网
            fullRequest = mis_requestHeader+mis_requestState;
            qDebug() << "mission_token_req" <<fullRequest;
            reqToken.setUrl(QUrl(fullRequest));
            tokenManager->get(reqToken);

            missionManager = new QNetworkAccessManager(this);
            connect(missionManager,SIGNAL(finished(QNetworkReply*)),this, SLOT(missionFinished(QNetworkReply*)));
            break;
        case GET_MISSION_LIST:
            //++cnt;
            tokenManager = new QNetworkAccessManager(this);
            missionManager = new QNetworkAccessManager(this);
            connect(missionManager,SIGNAL(finished(QNetworkReply*)),this, SLOT(missionFinished(QNetworkReply*)));
            fullRequest = mis_Missionlist;                                                                        //http://127.0.0.1:10058/api/missions   需要在车端
//#if 1
//            if(cnt == 1){
//                    fullRequest =requestHeader;
//            }else{
//                    fullRequest =requestHeader + requestState;
//                    cnt = 0;
//            }
//#else
//            fullRequest =requestHeader + requestState;
//#endif
            qDebug() << "mission_list_req" <<fullRequest;
            reqMission.setUrl(QUrl(fullRequest));
            missionManager->get(reqMission);
            break;
        case POST_MISSION_ISSUE:
            tokenManager = new QNetworkAccessManager(this);
            missionManager = new QNetworkAccessManager(this);
            connect(missionManager,SIGNAL(finished(QNetworkReply*)),this, SLOT(missionFinished(QNetworkReply*)));

            conf = reqToken.sslConfiguration();
            conf.setPeerVerifyMode(QSslSocket::VerifyNone);
            conf.setProtocol(QSsl::AnyProtocol);
            reqToken.setSslConfiguration(conf);

            reqToken.setRawHeader("authorization",token.toLocal8Bit());
            reqToken.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
            fullRequest =mis_Header+mis_Headername+MissionID+mis_Issuetask;                                        //https://robosweeper.senseauto.com:32461/api/missions/{id}/perform_now
            qDebug() << "mission_issue" <<fullRequest;
            reqToken.setUrl(QUrl(fullRequest));
            missionManager->post(reqToken,QByteArray(""));
            break;

        case POST_MISSION_RESUME:
            tokenManager = new QNetworkAccessManager(this);
            missionManager = new QNetworkAccessManager(this);
            connect(missionManager,SIGNAL(finished(QNetworkReply*)),this, SLOT(missionFinished(QNetworkReply*)));

            conf = reqToken.sslConfiguration();
            conf.setPeerVerifyMode(QSslSocket::VerifyNone);
            conf.setProtocol(QSsl::AnyProtocol);
            reqToken.setSslConfiguration(conf);

            reqToken.setRawHeader("authorization",token.toLocal8Bit());
            reqToken.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
            fullRequest =mis_Header+mis_Headername+MissionID+mis_Resumetask;                                        //https://robosweeper.senseauto.com:32461/api/missions/{id}/resume_mission
            qDebug() << "mission_resume" <<fullRequest;
            reqToken.setUrl(QUrl(fullRequest));
            missionManager->post(reqToken,QByteArray(""));
            break;

    case POST_MISSION_CANCEL:
        tokenManager = new QNetworkAccessManager(this);
        missionManager = new QNetworkAccessManager(this);
        connect(missionManager,SIGNAL(finished(QNetworkReply*)),this, SLOT(missionFinished(QNetworkReply*)));

        conf = reqToken.sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        conf.setProtocol(QSsl::AnyProtocol);
        reqToken.setSslConfiguration(conf);

        reqToken.setRawHeader("authorization",token.toLocal8Bit());
        reqToken.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
        fullRequest =mis_Header+mis_Headername+MissionID+mis_Canceltask;                                        //https://robosweeper.senseauto.com:32461/api/missions/{id}/cancel_mission
        qDebug() << "mission_cancel" <<fullRequest;
        reqToken.setUrl(QUrl(fullRequest));
        missionManager->post(reqToken,QByteArray(""));
        break;
    }

//    if(this->flag1 == GET_TOKEN){                                                                                               //GET_TOKEN
//        tokenManager = new QNetworkAccessManager(this);
//        connect(tokenManager,SIGNAL(finished(QNetworkReply*)),this, SLOT(tokenFinished(QNetworkReply*)));
//        QString fullRequest = requestHeader+requestState;                                                   //http://10.8.96.6:8087/v1/token 需要内网
//        //QString fullRequest = mis_requestHeader+mis_requestState;
//        qDebug() << "mission_token_req" <<fullRequest;
//        reqToken.setUrl(QUrl(fullRequest));
//        tokenManager->get(reqToken);

//        missionManager = new QNetworkAccessManager(this);
//        connect(missionManager,SIGNAL(finished(QNetworkReply*)),this, SLOT(missionFinished(QNetworkReply*)));
//    }else if(this->flag1 == GET_MISSION_LIST){                                                                          //GET_MISSION_LIST
//        tokenManager = new QNetworkAccessManager(this);
//        missionManager = new QNetworkAccessManager(this);
//        connect(missionManager,SIGNAL(finished(QNetworkReply*)),this, SLOT(missionFinished(QNetworkReply*)));
//        //QString fullRequest = mis_Missionlist;                                                                        //http://127.0.0.1:10058/api/missions   需要在车端
//        QString fullRequest =requestHeader+requestState;
//        qDebug() << "mission_list_req" <<fullRequest;
//        reqToken.setUrl(QUrl(fullRequest));
//        missionManager->get(reqToken);
//    }
}

void Mission_http::stop()
{
    qDebug() << this->flag1 <<"Manager stop";
    stopped = true;
    tokenManager->deleteLater();
    missionManager->deleteLater();
}
/*void Mission_http::timeAuto()
{
    stopped = false;
#if 1
    //QString fullRequest = mis_requestHeader+mis_requestState;      //https://127.0.0.1/v1/token            //test: http://10.8.96.6:8087/v1/token
    QString fullRequest = requestHeader+requestState;
    qDebug() << "mission_token_req" <<fullRequest;
    reqToken.setUrl(QUrl(fullRequest));
    tokenManager->get(reqToken);
#else           //https://robosweeper.senseauto.com:32461/api/missions test
    QString fullRequest = mis_Headername;
    qDebug() << "mission_req" <<fullRequest;
    reqMission.setUrl(QUrl(fullRequest));
    //reqMission.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json;charset=utf-8"));
    //QString token = "eyJhbGciOiJIUzI1NiJ9.eyJkYXRhIjp7InN5bXBob255X2RldmljZV9pZCI6ImJjNGJlNGM3MWZlMzdiZWZjMjgzZWMyMzM0ZDgzZGEyNWQzMGM4OGFhNGYxMTBhM2RiZDU0NzE5MTFiNDc2MzkiLCJzZWNyZXQiOiI0OWY4MTk4MDJkZjc0YjBlOTViYTZkN2JjZjQ5OTQyMSIsImxvZ2luX3R5cGUiOiJkZXZpY2UifSwiZXhwIjoxNjg4MzQ4NDUzfQ.nzOYVWnpaSAef92jl85e8MuNxFA9T9XM_J9XMgZQf7U";
    QByteArray value("eyJhbGciOiJIUzI1NiJ9.eyJkYXRhIjp7InN5bXBob255X2RldmljZV9pZCI6ImJjNGJlNGM3MWZlMzdiZWZjMjgzZWMyMzM0ZDgzZGEyNWQzMGM4OGFhNGYxMTBhM2RiZDU0NzE5MTFiNDc2MzkiLCJzZWNyZXQiOiI0OWY4MTk4MDJkZjc0YjBlOTViYTZkN2JjZjQ5OTQyMSIsImxvZ2luX3R5cGUiOiJkZXZpY2UifSwiZXhwIjoxNjg4MzQ4NDUzfQ.nzOYVWnpaSAef92jl85e8MuNxFA9T9XM_J9XMgZQf7U");

     //reqMission.setRawHeader("authorization","eyJhbGciOiJIUzI1NiJ9.eyJkYXRhIjp7InN5bXBob255X2RldmljZV9pZCI6ImJjNGJlNGM3MWZlMzdiZWZjMjgzZWMyMzM0ZDgzZGEyNWQzMGM4OGFhNGYxMTBhM2RiZDU0NzE5MTFiNDc2MzkiLCJzZWNyZXQiOiI0OWY4MTk4MDJkZjc0YjBlOTViYTZkN2JjZjQ5OTQyMSIsImxvZ2luX3R5cGUiOiJkZXZpY2UifSwiZXhwIjoxNjg4MzQ4NDUzfQ.nzOYVWnpaSAef92jl85e8MuNxFA9T9XM_J9XMgZQf7U");
     //reqMission.setRawHeader("authorization",token.toLocal8Bit());
     reqMission.setRawHeader(QByteArray("authorization"),value);
     //reqMission.setRawHeader(QByteArray("authorization"),"eyJhbGciOiJIUzI1NiJ9.eyJkYXRhIjp7InN5bXBob255X2RldmljZV9pZCI6ImJjNGJlNGM3MWZlMzdiZWZjMjgzZWMyMzM0ZDgzZGEyNWQzMGM4OGFhNGYxMTBhM2RiZDU0NzE5MTFiNDc2MzkiLCJzZWNyZXQiOiI0OWY4MTk4MDJkZjc0YjBlOTViYTZkN2JjZjQ5OTQyMSIsImxvZ2luX3R5cGUiOiJkZXZpY2UifSwiZXhwIjoxNjg4MzQ4NDUzfQ.nzOYVWnpaSAef92jl85e8MuNxFA9T9XM_J9XMgZQf7U");

#endif

//    QEventLoop loop;
//    connect(missionManager,SIGNAL(finished(QNetworkReply*)),&loop, SLOT(quit()));
//    loop.exec();
}
*/

void Mission_http::run(){
    //m_pAutoTimer->start(times);
    qDebug() << "********Mission_http start ********";
//    while(!stopped){}
//    qDebug() << "********Mission_http stop ********";
    ptm = new QTimer();
    ptm->start(500);
    exec();
}

void Mission_http::tokenFinished(QNetworkReply *reply){
    QString str = reply->readAll();
    QJson::Parser parser;
    bool ok;
    QVariantMap Tokenmap=parser.parse(str.toUtf8(),&ok).toMap();
    if(!ok){
        qDebug() << "********get token failed********";
        return;
    }else{
        token = Tokenmap["token"].toString();
        mis_Header = Tokenmap["url"].toString();
    }
    //token = reply->readAll();
    qDebug() << "mission_token:" << token;
    reply->deleteLater();

    /***************************token to mission_list**************************/
    QString fullRequest = mis_Header+mis_Headername;
    //QString fullRequest = requestHeader+requestState;
    qDebug() << "mission_req" <<fullRequest;
    QSslConfiguration conf1 = reqMission.sslConfiguration();
    conf1.setPeerVerifyMode(QSslSocket::VerifyNone);
    conf1.setProtocol(QSsl::AnyProtocol);
    reqMission.setSslConfiguration(conf1);

    reqMission.setUrl(QUrl(fullRequest));
    //QString token = "eyJhbGciOiJIUzI1NiJ9.eyJkYXRhIjp7InN5bXBob255X2RldmljZV9pZCI6ImJjNGJlNGM3MWZlMzdiZWZjMjgzZWMyMzM0ZDgzZGEyNWQzMGM4OGFhNGYxMTBhM2RiZDU0NzE5MTFiNDc2MzkiLCJzZWNyZXQiOiI0OWY4MTk4MDJkZjc0YjBlOTViYTZkN2JjZjQ5OTQyMSIsImxvZ2luX3R5cGUiOiJkZXZpY2UifSwiZXhwIjoxNjg5MDM5OTUwfQ.joS1K_OXQrFD1wtxlvHNaXpK_ZbVuf6DkfkMLpQDoIg";
    reqMission.setRawHeader("authorization",token.toLocal8Bit());
    missionManager->get(reqMission);

}

void Mission_http::missionFinished(QNetworkReply *reply){

    QString mission_list = reply->readAll();
    qDebug() << "mission_list:" << mission_list;
    //qDebug() << "emit";
    emit Mission_data(mission_list);
    reply->deleteLater();

}

Mission_http::~Mission_http()
{
    //while(!stopped);
    ptm->deleteLater();
    qDebug() << this->flag1 <<"********Mission_http close  ********";
}
