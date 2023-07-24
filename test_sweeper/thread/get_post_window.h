#ifndef GET_POST_WINDOW_H
#define GET_POST_WINDOW_H

#include"com.h"
#include "serialport.h"
#include "httpthread.h"

#include <QMainWindow>

#include <QWidget>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>


namespace Ui {
class get_post_Window;
}

class get_post_Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit get_post_Window(QWidget *parent = 0);
    ~get_post_Window();

private slots:
    void receiveReplyGet(QNetworkReply *reply);
    void receiveReplyPost(QNetworkReply *reply);

    void on_postButton_clicked();

    void on_getButton_clicked();

    void on_UartButton_232_clicked();

    void closeEvent(QCloseEvent* e);

private:
    Ui::get_post_Window *ui;

    QNetworkAccessManager *managerGet;
    QNetworkAccessManager *managerPost;

    QNetworkRequest reqGet;
    QNetworkRequest reqPost;

    QNetworkReply *replyGet;
    QNetworkReply *replyPost;

    SerialPort *m_pSerialPort;

    //QString requestHeader;

};

#endif // GET_POST_WINDOW_H
