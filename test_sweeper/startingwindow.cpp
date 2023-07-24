#include "startingwindow.h"
#include "ui_startingwindow.h"

#include"thread/starting_thread.h"
#include"thread/httpthread.h"
#include <QTime>

Startingwindow::Startingwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Startingwindow)
{
    qDebug() << "\n********StartingWindow open********";
    setWindowFlags(Qt::FramelessWindowHint);//隐藏顶部栏
    setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
    ui->setupUi(this);

    this->data_finished = false;
    this->nodecheck_control = 0x0000;
    this->infocheck_control = 0x00000000;
    startcheck_state = node_status;
    node = camera;
    info = brake;
    //背景
    QPixmap pixmap = QPixmap(":/new/picture/startwindow/start1.png").scaled(this->size());
    QPalette Palette;
    Palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(Palette);

    //图片
    pix = QPixmap(":/new/picture/startwindow/Ellipse.png");
    this->timer = new QTimer();
    timer->start(10);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    this->angle = 0;

    thread = new starting_thread(this);
    QObject::connect(thread,  SIGNAL(start_progress(int)), this, SLOT(UpdateStartingBar(int)) );
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    thread->start();

    HttpThread = new HttpReadThread(Start_Window,this);
    connect(HttpThread, SIGNAL(selfCheckSignal(QString)), this, SLOT(parseData(QString)));
    connect(HttpThread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    HttpThread->start();
}

void Startingwindow::paintEvent(QPaintEvent *){
    QPainter painter(this);
    int x = 303,y=202;
    angle += 3;                             //转动速度
    if(angle == 360) angle=0;
    painter.translate(x+pix.width()/2,y+pix.height()/2);        //旋转中心
    painter.rotate(angle);
    painter.translate(-x-pix.width()/2,-y-pix.height()/2);       //恢复中心点
    painter.drawPixmap(x,y,pix.width(),pix.height(),pix);      //画图
}

void Startingwindow::UpdateStartingBar(int p)
{
    ui->progressBar->setValue(p);//Bar

    if(p == 100){
        thread->stop();
        thread->exit();
        thread->wait();
        HttpThread->stop();
        HttpThread->exit();
        HttpThread->wait();
        timer->stop();
        delete timer;
        //emit CheckW_show();
        emit MainW_Show(data_finished);

        //延时开启线程
        QTime dieTime =  QTime::currentTime().addMSecs(500);
        while(QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents,500);

        this->hide();
    }
}


void Startingwindow::parseData(QString str){
    QJson::Parser parser;
    bool ok;
    QVariantMap startmap=parser.parse(str.toUtf8(),&ok).toMap();
    if(!ok){
        qDebug() << "********get starting data failed********";
        return;
    }else{
#if 1
        data_finished = true;
#else
        //qDebug() <<str;
        /***********************node_status************************/
        QVariantMap results1 = startmap["node_status"].toMap();
        //qDebug() << results1["camera"].toString();
        QVariantMap Camera = results1["camera"].toMap();
        QVariantMap Canbus = results1["canbus"].toMap();
        QVariantMap Control = results1["control"].toMap();
        QVariantMap Decision_plan = results1["decision_planning"].toMap();
        QVariantMap Ins = results1["ins"].toMap();
        QVariantMap Lidar = results1["lidar"].toMap();
        QVariantMap Localization = results1["localization"].toMap();
        QVariantMap Monitor = results1["monitor"].toMap();
        QVariantMap Auxiliary_master = results1["monitor_auxiliary_master"].toMap();
        QVariantMap Prediction = results1["prediction"].toMap();
        QVariantMap Robocore = results1["robocore"].toMap();
        QVariantMap Uss = results1["uss"].toMap();
        /***********************self_check_info************************/
        QVariantMap results2 = startmap["self_check_info"].toMap();
        /***********************status************************/
        QVariantMap results3 = startmap["status"].toMap();

        QVariantMap results4 = startmap["vehicle_state"].toMap();
        switch(startcheck_state){
                case node_status:
                        if(Camera["status"].toString() == "RUNNING"){
                            node = camera;
                            nodecheck_control |= (1<<node);
                        }

                        if(Canbus["status"].toString() == "RUNNING"){
                            node = canbus;
                            nodecheck_control |= (1<<node);
                        }
                        if(Control["status"].toString() == "RUNNING"){
                            node = control;
                            nodecheck_control |= (1<<node);
                        }
                        if(Decision_plan["status"].toString() == "RUNNING"){
                            node = decision_planning;
                            nodecheck_control |= (1<<node);
                        }
                        if(Ins["status"].toString() == "RUNNING"){
                            node = ins;
                            nodecheck_control |= (1<<node);
                        }
                        if(Lidar["status"].toString() == "RUNNING"){
                            node = lidar;
                            nodecheck_control |= (1<<node);
                        }
                        if(Localization["status"].toString() == "RUNNING"){
                            node = localization;
                            nodecheck_control |= (1<<node);
                        }
                        if(Monitor["status"].toString() == "RUNNING"){
                            node = monitor;
                            nodecheck_control |= (1<<node);
                        }
                        if(Auxiliary_master["status"].toString() == "RUNNING"){
                            node = monitor_auxiliary_master;
                            nodecheck_control |= (1<<node);
                        }
                        if(Prediction["status"].toString() == "RUNNING"){
                            node = prediction;
                            nodecheck_control |= (1<<node);
                        }
                        if(Robocore["status"].toString() == "RUNNING"){
                            node = robocore;
                            nodecheck_control |= (1<<node);
                        }
                        if(Uss["status"].toString() == "RUNNING"){
                            node = uss;
                            nodecheck_control |= (1<<node);
                        }
                        //qDebug() << nodecheck_control;
                        if(nodecheck_control == 0x0FFF){
                            startcheck_state = self_check_info;
                        }else{

                        }
                break;

                case self_check_info:
                        qDebug() << "enter self_check_info";
                        if(results2["brake"].toInt() == 1){
                            info = brake;
                            infocheck_control |= (1<<info);
                        }

                        if(results2["close_brake_light"].toInt() == 1){
                            info = close_brake_light;
                            infocheck_control |= (1<<info);
                        }
                        if(results2["close_dust"].toInt() == 1){
                            info = close_dust;
                            infocheck_control |= (1<<info);
                        }

                        if(results2["close_headlight"].toInt() == 1){
                            info = close_headlight;
                            infocheck_control |= (1<<info);
                        }
                        if(results2["close_spray"].toInt() == 1){
                            info = close_spray;
                            infocheck_control |= (1<<info);
                        }

                        if(results2["close_turn_left_light"].toInt() == 1){
                            info = close_turn_left_light;
                            infocheck_control |= (1<<info);
                        }
                        if(results2["close_turn_right_light"].toInt() == 1){
                            info = close_turn_right_light;
                            infocheck_control |= (1<<info);
                        }

                        if(results2["close_vacuum"].toInt() == 1){
                            info = close_vacuum;
                            infocheck_control |= (1<<info);
                        }
                        if(results2["forward"].toInt() == 1){
                            info = forward;
                            infocheck_control |= (1<<info);
                        }

                        if(results2["open_brake_light"].toInt() == 1){
                            info = open_brake_light;
                            infocheck_control |= (1<<info);
                        }
                        if(results2["open_dust"].toInt() == 1){
                            info = open_dust;
                            infocheck_control |= (1<<info);
                        }

                        if(results2["open_headlight"].toInt() == 1){
                            info = open_headlight;
                            infocheck_control |= (1<<info);
                        }
                        if(results2["open_spray"].toInt() == 1){
                            info = open_spray;
                            infocheck_control |= (1<<info);
                        }

                        if(results2["open_turn_left_light"].toInt() == 1){
                            info = open_turn_left_light;
                            infocheck_control |= (1<<info);
                        }
                        if(results2["open_turn_right_light"].toInt() == 1){
                            info = open_turn_right_light;
                            infocheck_control |= (1<<info);
                        }

                        if(results2["open_vacuum"].toInt() == 1){
                            info = open_vacuum;
                            infocheck_control |= (1<<info);
                        }
                        if(results2["reverse"].toInt() == 1){
                            info = reverse;
                            infocheck_control |= (1<<info);
                        }
                        if(results2["turn_left"].toInt() == 1){
                            info = turn_left;
                            infocheck_control |= (1<<info);
                        }

                        if(results2["turn_right"].toInt() == 1){
                            info = turn_right;
                            infocheck_control |= (1<<info);
                        }
                        qDebug() << infocheck_control;
                        if(infocheck_control == 0x0007FFFF){
                            startcheck_state = status;
                        }else{

                        }
                        break;
                case status:
                        qDebug() << "enter status";
                        if(results3["status"].toString() == "success") data_finished = true;
                        break;
        }
        #endif
    }


}

Startingwindow::~Startingwindow()
{
    delete ui;
}
