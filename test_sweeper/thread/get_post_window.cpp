#include "get_post_window.h"
#include "ui_get_post_window.h"

#include <QDebug>
#include <QSsl>
#include <QSslConfiguration>
#include<QSslCertificate>
#include "qsslsocket.h"

get_post_Window::get_post_Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::get_post_Window)
{
    ui->setupUi(this);

     m_pSerialPort = new SerialPort();
     QByteArray name = QString("COM3").toUtf8();
     char *pName = const_cast<char*>(name.data());
     QByteArray parity = QString("8").toUtf8();
     char *pParity = parity.data();
     m_pSerialPort->open(pName, 115200, 8, pParity, 1);
     qDebug() << "********m_pSerialPort COM3 open  ********";

    managerGet = new QNetworkAccessManager(this);
    managerPost = new QNetworkAccessManager(this);
    // requestHeader = QString("http://192.168.8.100:10058");

    connect(managerGet, SIGNAL(finished(QNetworkReply *)), this, SLOT(receiveReplyGet(QNetworkReply *)));
    connect(managerPost, SIGNAL(finished(QNetworkReply *)), this, SLOT(receiveReplyPost(QNetworkReply *)));
}

get_post_Window::~get_post_Window()
{
    delete ui;
    delete managerGet;
    delete managerPost;
    m_pSerialPort->close();
    delete m_pSerialPort;
     qDebug() << "********m_pMySerialPort COM3 close  ********";
}

void get_post_Window::closeEvent(QCloseEvent* e)
{
    delete ui;
    delete managerGet;
    delete managerPost;
    m_pSerialPort->close();
    delete m_pSerialPort;
     qDebug() << "********m_pSerialPort COM3 close  ********";
}

void get_post_Window::on_postButton_clicked()
{
    QString fullRequest = requestHeader+QString("/companies");
    // 设置url
    reqPost.setUrl(QUrl(fullRequest));//QUrl("http://www.baidu.com")

    // 设置请求头
//    reqPost.setRawHeader("accept", "*/*");
//    reqPost.setRawHeader("Accept-Language", "zh-CN");
    reqPost.setRawHeader("Content-Type", "application/json;charset=UTF-8");
//    reqPost.setRawHeader("tenantId", "1");

    //设置请求数据（我这里发送的是json格式数据）
    char cByte[1024] = "{\"test\":\"value\"}";
    //char cByte[1024] = "{\test\"}";
    QByteArray byte(cByte);

    qDebug() << fullRequest <<byte;

    // 发送post请求
    replyPost = managerPost->post(reqPost, byte);

}

void get_post_Window::on_getButton_clicked()
{
        QTimer timer;
        timer.setInterval(30000);  // 设置超时时间 30 秒
        timer.setSingleShot(true);  // 单次触发

        QSslConfiguration conf = reqGet.sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        conf.setProtocol(QSsl::AnyProtocol);
        reqGet.setSslConfiguration(conf);

       QString fullRequest = QString("https://robosweeper.senseauto.com/api/missions");
       //QString fullRequest = requestHeader+requestState;

        //设置url requestHeader+requestState;//QString("/vehicle_state");//
        QString token = "eyJhbGciOiJIUzI1NiJ9.eyJkYXRhIjp7InN5bXBob255X2RldmljZV9pZCI6ImZmNzllMTBiMzEzMWU3YWY1YzEyMDI5ZDNhNDY3MTg5MGMyOTQxOGU2NDgwNTMyYmQ5OGM0OTI1MTcyZTc1Y2UiLCJzZWNyZXQiOiIwNjhiYTliNjQ4ZTk0ZTgyODA4MzFlMTI0MDdlZWNlYyIsImxvZ2luX3R5cGUiOiJkZXZpY2UifSwiZXhwIjoxNjg5ODQ1NTUyfQ.a6Tw6vY0_XWiWmnMQY8FttXr1k5oWucShIy-OVaHjGQ";
        reqGet.setUrl(QUrl(fullRequest));
        reqGet.setRawHeader("authorization",token.toLocal8Bit());


        qDebug() <<fullRequest;

        // 设置请求头
//        reqGet.setRawHeader("accept", "*/*");
//        reqGet.setRawHeader("accept-language", "zh-CN");

        // 发送get请求
        replyGet = managerGet->get(reqGet);

        QEventLoop loop;
        connect(&timer, SIGNAL(timeout()), &loop,SLOT(quit()));
        connect(replyGet, SIGNAL(finished()), &loop,SLOT(quit()));
        timer.start();
        loop.exec(); // 启动事件循环
        if (timer.isActive()) { // 处理响应
            timer.stop();
//            if (replyGet->error() != QNetworkReply::NoError) { // 错误处理
//                qDebug() << "Error String : " << replyGet->errorString();
//            } else {
//                QVariant variant = replyGet->attribute(QNetworkRequest::HttpStatusCodeAttribute);
//                int nStatusCode = variant.toInt(); // 根据状态码做进一步数据处理 //QByteArray bytes = pReply->readAll();
//                qDebug() << "Status Code : " << nStatusCode;
//            }
        } else { // 处理超时
            disconnect(replyGet, SIGNAL(finished()), &loop,SLOT(quit()));
            replyGet->abort();
            replyGet->deleteLater();
            qDebug() << "Timeout";
        }


//        QObject::connect(replyGet, &QNetworkReply::finished, [=](){
//            if (replyGet->error() == QNetworkReply::NoError) {
//                QByteArray data =replyGet->readAll();
//            }   else {

//            }
//        });
        //connect(replyGet, &QNetworkReply::finished, this, &get_post_Window::receiveReplyGet);
        //connect(replyGet, SIGNAL(finished(QNetworkReply *)), this, SLOT(receiveReplyPost(QNetworkReply *)));
}

// 接收http响应--get
void get_post_Window::receiveReplyGet(QNetworkReply *reply)
{
    // 获取状态码
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    // 获取响应数据
    QString reply_data = reply->readAll();

    ui->plainTextEdit->appendPlainText(QString::number(status_code));
    ui->plainTextEdit->appendPlainText(reply_data);
    qDebug() << "********receivereply get successful*******"<<reply_data;
}

// 接收http响应--post
void get_post_Window::receiveReplyPost(QNetworkReply *reply)
{
    // 获取状态码
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    // 获取响应数据
    QString reply_data = reply->readAll();

    ui->plainTextEdit->appendPlainText(QString::number(status_code));
    ui->plainTextEdit->appendPlainText(reply_data);
    qDebug() << "********receivereply POST successful*******"<<reply_data;
}


void get_post_Window::on_UartButton_232_clicked()
{
    QString str ;
    char *pBuff = new char[64];
    int size = m_pSerialPort->read(pBuff, 64);
    if(size > 0) {
        for(int i =0;i<size;i++) {
            QByteArray ba ;
            ba.append(pBuff[i]);
            if(ba==0)
                str.append("00");
             else
                str.append(ba.toHex());
            if(i!=(size))
            str.append(" ");
        }
       qDebug() << str;
       ui->plainTextEdit->appendPlainText(str);
    }
    delete pBuff;
}
