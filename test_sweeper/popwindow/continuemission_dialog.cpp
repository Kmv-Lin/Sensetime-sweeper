#include "continuemission_dialog.h"
#include "ui_continuemission_dialog.h"

Continuemission_dialog::Continuemission_dialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Continuemission_dialog)
{
    qDebug() << "********Continuemission_dialog open********";
    setWindowFlags(Qt::FramelessWindowHint);//隐藏顶部栏
    setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
    ui->setupUi(this);

    ui->cleardisk_label->setAlignment(Qt::AlignCenter);
    ui->cleardisk_label->setStyleSheet("QLabel{font-family: 'Ubuntu';font-size: 24px;}");
    ui->cleardisk_label->setText("是否继续当前任务？");

    QPalette pel;
    pel.setColor(QPalette::ButtonText,Qt::white);
    ui->confirm_button->setPalette(pel);
    ui->confirm_button->setStyleSheet("background-image: url(:/new/picture/mission_interface/Rectangle 255.png);border:1px solid #5DBFC4;border-radius:5px;font-family: 'Ubuntu';font-size: 24px;");
    ui->confirm_button->setText("是，继续");
    pel.setColor(QPalette::ButtonText,QColor(0,150,152));
    ui->cancel_button->setPalette(pel);
    ui->cancel_button->setStyleSheet("background-color:rgba(0,0,0,0);border:1px solid #5DBFC4;border-radius:5px;font-family: 'Ubuntu';font-size: 24px;");
    ui->cancel_button->setText("结束任务");
}

void Continuemission_dialog::RunningMissionIDSlot(QString ID){
    MissionID = ID;
}

Continuemission_dialog::~Continuemission_dialog()
{
    qDebug() << "********Continuemission_dialog close********";
    delete ui;
}

void Continuemission_dialog::on_confirm_button_clicked()
{
    mission_http = new Mission_http(POST_MISSION_RESUME,MissionID,this);                      //判断车端系统当前是否有任务
    connect(mission_http,SIGNAL(Mission_data(QString)),this,SLOT(Mission_continue(QString)));
    mission_http->start();
    this->hide();
}

void Continuemission_dialog::on_cancel_button_clicked()
{
    mission_http = new Mission_http(POST_MISSION_CANCEL,MissionID,this);                      //判断车端系统当前是否有任务
    connect(mission_http,SIGNAL(Mission_data(QString)),this,SLOT(Mission_continue(QString)));
    mission_http->start();
    this->hide();
}

void Continuemission_dialog::Mission_continue(QString str){
    QJson::Parser parser;
    bool ok;
    QVariantMap Misdata=parser.parse(str.toUtf8(),&ok).toMap();
    if(!ok){                                                                                                                             //如果没有数据，失败return
        qDebug() << "********Mission_continue get data failed********";
        return;
  }else{
        QString Status  = Misdata["msg"].toString();
        qDebug() << "msg" << Status;
        if(Status != "OK"){                                                                                                      //别的状况，例如任务已经下发到车端
            qDebug() << "********Mission resume or cancel failed********";
            mission_http->exit();
            mission_http->wait();
            return;
        }else{
            qDebug() << "********Mission resume or cancel successful********";
            mission_http->exit();
            mission_http->wait();
        }
    }
}
