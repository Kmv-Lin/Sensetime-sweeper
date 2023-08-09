#include "mission_dialog.h"
#include "ui_mission_dialog.h"
#include "thread/mission_http.h"
#include "com_mission.h"

#include "qdatetime.h"

#include<QDebug>
static int battery_info;
static int water_info;
static QString water_state;
Mission_Dialog::Mission_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Mission_Dialog)
{

    qDebug() << "********Mission_Dialog open********";
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);//隐藏顶部栏
     setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
    ui->setupUi(this);

//    //字体颜色-white
    QPalette pel;
//    pel.setColor(QPalette::ButtonText,Qt::white);
//    ui->ReturnButton->setPalette(pel);

//    ui->ReturnButton->setStyleSheet("background-image: url(:/new/picture/mission_interface/Rectangle 252.png);border:1px solid #5DBFC4;border-radius:4px; ");//消除边框

    //"返回"背景翠绿色+字体白色+线宽1px翠绿色+四边圆角
    ui->ReturnButton->setStyleSheet("QPushButton{background-color: rgb(0, 150, 157);color:white;border:1px solid #00969D;border-radius:5px;  }");//线条

    //字体颜色-翠绿
    pel.setColor(QPalette::ButtonText,QColor(51,51,51));
    ui->HistoryButton->setPalette(pel);
    ui->StartButton->setPalette(pel);


    ui->StartButton->setStyleSheet("background-color:rgba(0,0,0,0);border:1px solid #5DBFC4;border-radius:4px; ");
    ui->HistoryButton->setStyleSheet("background-color:rgba(0,0,0,0);border:1px solid #5DBFC4;border-radius:4px; ");

    mission_http = new Mission_http(GET_TOKEN,0,this);
    connect(mission_http,SIGNAL(finished()),mission_http,SLOT(deleteLater()));
    mission_http->start();
    connect(mission_http,SIGNAL(Mission_data(QString)),this,SLOT(MissionList_recv(QString)));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(SendInfo())); //关联超时信号和曹函数 10S更新状态     //每10s更新世界时间以及FSM状态
    unsigned int times = 800;
    this->timer->start(times);     //定时器启动

    history_thread = nullptr;
    today_thread = nullptr;
    flag = 0;
    //DataSend = false;
//    QPixmap pixmap = QPixmap(":/new/picture/mission_interface/today_mission.png").scaled(this->size());
//    QPalette Palette;
//    Palette.setBrush(QPalette::Background, QBrush(pixmap));
//    this->setPalette(Palette);
}

void Mission_Dialog::AutoRun(int BatteryInfo,int WaterInfo,QString WaterState)
{
    battery_info = BatteryInfo;
    water_info = WaterInfo;
    water_state = WaterState;
}

void Mission_Dialog::SendInfo(){
    //DataSend = true;
    emit mission_show(battery_info,water_info,water_state);
}

Mission_Dialog::~Mission_Dialog()
{

    //while(!mission_http->isFinished() && mission_http->isRunning()){};
    qDebug() << "********Mission_Dialog close********";
    delete ui;
}

void Mission_Dialog::on_ReturnButton_clicked()
{
//    while(!history_mission->isfinished || !today_mission->isfinished){
//         QCoreApplication::processEvents(QEventLoop::AllEvents,10);
//    }

    //mission_http->stop();
    timer->stop();
    delete timer;
    mission_http->exit();
    mission_http->wait();

    if(history_thread){
    history_thread->quit();
    history_thread->deleteLater();
    //history_thread = nullptr;
}
    if(today_thread){
    today_thread->quit();
    today_thread->deleteLater();
    //today_thread = nullptr;
}

     while(!mission_http->isFinished() && mission_http->isRunning()){};
     this->close();
}

void Mission_Dialog::MissionList_recv(QString str){
        data =str;
        emit MissionList_data(data);
}

void Mission_Dialog::on_HistoryButton_clicked()
{
#if 1
    flag++;
    if(flag == 1){
        history_thread = new QThread(this);
        history_mission = new History_mission;
        history_mission->moveToThread(history_thread);
        history_thread->start();
        connect(history_mission,SIGNAL(close_mission_dialog()),this,SLOT(on_ReturnButton_clicked()));
        //connect(mission_http,SIGNAL(Mission_data(QString)),history_mission,SLOT(MissionList_recv(QString)));
        connect(this,SIGNAL(MissionList_data(QString)),history_mission,SLOT(MissionList_recv(QString)));
        connect(this,SIGNAL(mission_show(int,int,QString)),history_mission,SLOT(AutoRun(int,int,QString)));
        //history_mission->hide();
        history_mission->show();
        if(data != "") emit MissionList_data(data);
        //用于延迟隐藏或删除，防止闪绿屏
        QTime dieTime =  QTime::currentTime().addMSecs(100);
        while(QTime::currentTime() < dieTime)
         QCoreApplication::processEvents(QEventLoop::AllEvents,100);
        this->hide();
    }

#else
    ui->HistoryButton->hide();
    ui->StartButton->hide();

    scrollarea = new QScrollArea(this);
    scrollarea->setGeometry(5,110,470,162);
    scrollarea->setStyleSheet("QScrollArea{background-color: rgba(0, 0, 0, 0);border:0px solid #838486;  }");//透明+取消线条
    scrollwidget = new QWidget();
    scrollwidget->setStyleSheet("background-color: rgba(0, 0, 0, 0);");//透明

    QHBoxLayout *playout = new QHBoxLayout(this);
    QPushButton *list_btn1 = new QPushButton(this);
    list_btn1->setMinimumSize(QSize(60,30));
    //list_btn1->setStyleSheet("QPushButton{background-color:transparent;text-align:left;border:1px solid #5DBFC4;border-radius:5px;  } ");
    list_btn1->setText("1111111                     duiqi                        1111111");
    //list_btn1->show();
    playout->addWidget(list_btn1);
    scrollwidget->setLayout(playout);

    scrollarea->setWidget(scrollwidget);
    scrollarea->show();
    scrollwidget->show();
#endif
}


void Mission_Dialog::on_StartButton_clicked()
{
    flag++;
    if(flag == 1){

        today_thread = new QThread(this);
        today_mission = new Today_mission;
        today_mission->moveToThread(today_thread);
        today_thread->start();
        connect(today_mission,SIGNAL(close_mission_dialog()),this,SLOT(on_ReturnButton_clicked()));
        connect(today_mission,SIGNAL(RunningMissionID(QString)),this,SLOT(RunningMissionIDSlot(QString)));
        //connect(mission_http,SIGNAL(Mission_data(QString)),today_mission,SLOT(MissionList_recv(QString)));
        connect(this,SIGNAL(MissionList_data(QString)),today_mission,SLOT(MissionList_recv(QString)));
        connect(this,SIGNAL(mission_show(int,int,QString)),today_mission,SLOT(AutoRun(int,int,QString)));
        //today_mission->hide();
    //    ui->HistoryButton->hide();
    //    ui->StartButton->hide();
        today_mission->show();
        if(data != "") emit MissionList_data(data);
        //用于延迟隐藏或删除，防止闪绿屏
        QTime dieTime =  QTime::currentTime().addMSecs(100);
        while(QTime::currentTime() < dieTime)
         QCoreApplication::processEvents(QEventLoop::AllEvents,100);
        this->hide();
    }
}
void Mission_Dialog::RunningMissionIDSlot(QString MissionID){
    emit RunningMissionID(MissionID);
}

void Mission_Dialog::on_HistoryButton_pressed()
{
    //ui->HistoryButton->setStyleSheet("background-image: url(:/new/picture/mission_interface/Rectangle 252.png);border:1px solid #5DBFC4;border-radius:4px; ");
}

void Mission_Dialog::on_StartButton_pressed()
{
    //ui->StartButton->setStyleSheet("background-image: url(:/new/picture/mission_interface/Rectangle 252.png);border:1px solid #5DBFC4;border-radius:4px; ");
}
