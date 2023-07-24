#ifndef SETTING_HTTP_H
#define SETTING_HTTP_H

#include "com_setting.h"

#include <QDebug>
#include <QMainWindow>
#include <QThread>
#include <QWidget>
#include <QtNetwork>

typedef enum{
    network = 0,
    disk = 1,
    version,
    volume
}set_num;

typedef enum{
    get = 0,
    post = 1,
    put,
    deleteResource
}way_choice;

class setting_http : public QThread
{
    Q_OBJECT
public:
    explicit setting_http(int api = 0,int way = 0,QString str = "",QObject *parent = 0);
    ~setting_http();
    void stop();
signals:
    void Set_data(QString);
public slots:
    void setFinished(QNetworkReply *);
protected:
    void run() override;

private:
    int API;
    int WAY;
    QString data;
    volatile bool stopped;
    QString set_requestState;
    QString strdata;
    QByteArray bytearray;
    QNetworkAccessManager *setManager;
    QNetworkRequest setRequest;
    QTimer *ptm;
};

#endif // SETTING_HTTP_H
