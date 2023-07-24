#include "mission_dialog.h"
#include "ui_mission_dialog.h"
#include "thread/mission_http.h"
#include "com_mission.h"

#include "qdatetime.h"

#include<QDebug>

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

    history_mission = new History_mission(this);
    connect(history_mission,SIGNAL(close_mission_dialog()),this,SLOT(on_ReturnButton_clicked()));
    connect(mission_http,SIGNAL(Mission_data(QString)),history_mission,SLOT(MissionList_recv(QString)));

    history_mission->hide();

    today_mission = new Today_mission(this);
    connect(today_mission,SIGNAL(close_mission_dialog()),this,SLOT(on_ReturnButton_clicked()));
    connect(mission_http,SIGNAL(Mission_data(QString)),today_mission,SLOT(MissionList_recv(QString)));
    today_mission->hide();


//    QPixmap pixmap = QPixmap(":/new/picture/mission_interface/today_mission.png").scaled(this->size());
//    QPalette Palette;
//    Palette.setBrush(QPalette::Background, QBrush(pixmap));
//    this->setPalette(Palette);
}


Mission_Dialog::~Mission_Dialog()
{

    //while(!mission_http->isFinished() && mission_http->isRunning()){};
    qDebug() << "********Mission_Dialog close********";
    delete ui;
}

void Mission_Dialog::on_ReturnButton_clicked()
{
    while(!history_mission->isfinished || !today_mission->isfinished){
         QCoreApplication::processEvents(QEventLoop::AllEvents,10);
    }

    //mission_http->stop();
    mission_http->exit();
    mission_http->wait();

//    thread->quit();
//    thread->deleteLater();

     while(!mission_http->isFinished() && mission_http->isRunning()){};
     this->close();
}


void Mission_Dialog::on_HistoryButton_clicked()
{
#if 1
    history_mission->show();

    //用于延迟隐藏或删除，防止闪绿屏
    QTime dieTime =  QTime::currentTime().addMSecs(100);
    while(QTime::currentTime() < dieTime)
     QCoreApplication::processEvents(QEventLoop::AllEvents,100);
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
//    ui->HistoryButton->hide();
//    ui->StartButton->hide();
    today_mission->show();

    //用于延迟隐藏或删除，防止闪绿屏
    QTime dieTime =  QTime::currentTime().addMSecs(100);
    while(QTime::currentTime() < dieTime)
     QCoreApplication::processEvents(QEventLoop::AllEvents,100);

}

void Mission_Dialog::on_HistoryButton_pressed()
{
    //ui->HistoryButton->setStyleSheet("background-image: url(:/new/picture/mission_interface/Rectangle 252.png);border:1px solid #5DBFC4;border-radius:4px; ");
}

void Mission_Dialog::on_StartButton_pressed()
{
    //ui->StartButton->setStyleSheet("background-image: url(:/new/picture/mission_interface/Rectangle 252.png);border:1px solid #5DBFC4;border-radius:4px; ");
}
