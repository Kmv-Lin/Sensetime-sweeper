#ifndef HTTPTHREAD_H
#define HTTPTHREAD_H

#include"com.h"

#include <QThread>
#include <QTimer>
#include <QtNetwork>

#define Start_Window 0
#define Main_Window 1

class HttpReadThread : public QThread
{
    Q_OBJECT
public:
    explicit HttpReadThread(int flag1=0,QObject *parent = 0);

    void stop();


signals:
    void receiveHttpSignal();
    void selfCheckSignal(QString str);
    void vehicleStateSignal(QString str);
    void getDiskSpaceSignal(QString str);
    void aboutDeviceSignal(QString str);


public slots:
    void timeAuto();
    void selfCheckFinished(QNetworkReply *reply);
    void vehicleStateFinished(QNetworkReply *reply);
    void getDiskSpaceFinished(QNetworkReply *reply);
    void aboutDeviceFinished(QNetworkReply *reply);

protected:
    void run();
    unsigned char asciiToHex(unsigned char aHex);

private:
    int flag;
    volatile bool stopped;
    QTimer      *m_pAutoTimer;
    unsigned int times;
    QNetworkAccessManager *selfCheckManger;
    QNetworkAccessManager *vehicleStateManger;
    QNetworkAccessManager *getDiskSpaceManger;
    QNetworkAccessManager *aboutDeviceManger;
};

class QReplyTimeout : public QObject {

    Q_OBJECT
public:
    QReplyTimeout(QNetworkReply *reply, const int timeout) : QObject(reply) {
        Q_ASSERT(reply);
        if (reply && reply->isRunning()) { // 启动单次定时器
            QTimer::singleShot(timeout, this, SLOT(onTimeout()));
        }
    }
signals:
    void timeout(); // 超时信号 - 供进一步处理
private slots:
    void onTimeout() { // 处理超时
        QNetworkReply *reply = static_cast<QNetworkReply*>(parent());
        if (reply->isRunning()) {
            reply->abort();
            reply->deleteLater();
            emit timeout();
        }
    }
};


#endif // HTTPTHREAD_H
