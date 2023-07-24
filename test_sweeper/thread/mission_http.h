#ifndef MISSION_HTTP_H
#define MISSION_HTTP_H

#include<QThread>
#include <QtNetwork>
#include <QString>
#include <QDebug>
#include <QByteArray>
#include <QJson/Parser>
#include <QJson/Serializer>
#include "com_mission.h"
#include "com.h"
#include <QSsl>
#include <QSslConfiguration>
#include<QSslCertificate>
#include "qsslsocket.h"


#define GET_TOKEN 0
#define GET_MISSION_LIST 1
#define POST_MISSION_ISSUE 2

//extern QString mis_Header;

class Mission_http : public QThread
{
    Q_OBJECT
public:
    explicit Mission_http(int flag = 0,QString ID="",QObject *parent = 0);
    ~Mission_http();
    void stop();
signals:
    void receiveHttpSignal();
    void Mission_data(QString);
public slots:
    //void timeAuto();
    void tokenFinished(QNetworkReply *reply);
    void missionFinished(QNetworkReply *reply);
protected:
    void run() override;
private:
    int flag1;
    QString MissionID;
    volatile bool stopped;
    //QTimer      *m_pAutoTimer;
    unsigned int times;
    QNetworkAccessManager *tokenManager;
    QNetworkAccessManager *missionManager;
    QNetworkRequest reqToken;
    QNetworkRequest reqMission;
    QString fullRequest;
    QTimer *ptm;
};

#endif // MISSION_HTTP_H
