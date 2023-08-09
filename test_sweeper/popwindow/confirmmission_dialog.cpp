#include "confirmmission_dialog.h"
#include "ui_confirmmission_dialog.h"

#include <QDebug>

ConfirmMission_Dialog::ConfirmMission_Dialog(QString id,QString mapname,QWidget *parent) :
    QDialog(parent),MissionID(id),Map_name(mapname),
    ui(new Ui::ConfirmMission_Dialog)
{
     qDebug() << "********ConfirmMission_Dialog open********";
     setWindowFlags(Qt::FramelessWindowHint);//隐藏顶部栏
     setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
     ui->setupUi(this);

     connect(this,SIGNAL(close_mission_dialog()),this,SLOT(on_ReturnButton_clicked()));                     //连续两次return回调

     flag = 0;

}


ConfirmMission_Dialog::~ConfirmMission_Dialog()
{
    qDebug() << "********ConfirmMission_Dialog close********";
    delete ui;
}

void ConfirmMission_Dialog::on_ReturnButton_clicked()
{
    flag = 0;
    this->close();
}

void ConfirmMission_Dialog::on_OK_Button_clicked()
{
    flag++;
    if(flag == 1){
        mission_list_http = new Mission_http(GET_MISSION_LIST,0,this);                      //判断车端系统当前是否有任务
        //connect(mission_list_http,SIGNAL(finished()),mission_list_http,SLOT(deleteLater()));
        connect(mission_list_http,SIGNAL(Mission_data(QString)),this,SLOT(MissionList_recv(QString)));
        mission_list_http->start();
    }
}

void ConfirmMission_Dialog::list_showdelay(){
        //mission_list_http->stop();
        mission_list_http->exit();
        mission_list_http->wait();
        existmission_dialog->close();
        //existmission_dialog = nullptr;
        while(!mission_list_http->isFinished() && mission_list_http->isRunning());                                                             //等待http释放再释放父对象
        emit close_mission_dialog();
}

void ConfirmMission_Dialog::MissionList_recv(QString str){
    QJson::Parser parser;
    bool ok;
    QVariantMap Misinfo_Map=parser.parse(str.toUtf8(),&ok).toMap();
    if(!ok){
        qDebug() << "********MissionList_recv failed********";
    }
    else{
    //QString Mis_data = Misinfo_Map["missions_info"].toString();

    QStringList strlist1 = str.split("]");                                                              //获取data中  [  ]  的数据
    QStringList strlist2 = str.split("[");
    str.resize(strlist1[0].size());
    str.remove(0,strlist2[0].length()+1);

    qDebug() << "----------------" <<str;

    bool runable = true;
    QStringList strlist = str.split("},");                                                              //按{ }分成字符串列表

    for(int i=0; i<strlist.length()-1; i++){
        if(i%2==0){
            strlist[i] += "},";
            strlist[i+1] += "}";
            QString data = strlist[i] +strlist[i+1];
            QVariantMap datamap=parser.parse(data.toUtf8(),&ok).toMap();
            //QVariantMap missiondata = datamap["mission_param"].toMap();
            //QString Mapname = missiondata["map_name"].toString();
            int status = datamap["status"].toInt();
            if(status == 0 || status == 1 || status == 2 ||status == 3 || status == 4 || status == 5){
                  runable = false;
              }
            qDebug() << "First  GET_MISSION_LIST status" << status;
        }
    }

    if(runable){
        qDebug() << "********First  GET_MISSION_LIST : have no mission running********";
        //mission_list_http->stop();
        mission_list_http->exit();
        mission_list_http->wait();
        mission_list_http = nullptr;
        //如果当前没有任务，则向云端请求下发任务,需重开一个http
        mission_issue_http = new Mission_http(POST_MISSION_ISSUE,MissionID,this);
        //connect(mission_issue_http,SIGNAL(finished()),mission_issue_http,SLOT(deleteLater()));
        connect(mission_issue_http,SIGNAL(Mission_data(QString)),this,SLOT(MissionIssue_recv(QString)));
        mission_issue_http->start();
        //return;
    }else{                                                                                                                              //系统有任务列表，弹窗显示任务执行中...
        qDebug() << "********First  GET_MISSION_LIST : have existed mission running********";
        timer = new QTimer(this);
       // QVariantMap Misparam_Map  = Misinfo_Map["missions_info"].toMap();           //具体的数据内容，可以用来执行操作
        //QVariantMap Mislist_Map= Misparam_Map["mission_param"].toMap();
        //如果当前有任务，弹窗提示任务执行中...
        existmission_dialog = new Existmission_dialog(this);
        existmission_dialog->show();

        timer->singleShot(5000,this,SLOT(list_showdelay()));                                             //弹窗延时5s（测试），实际为20s或30s（产品决定）
        timer->stop();
        timer->deleteLater();
    }
}
}

void ConfirmMission_Dialog::issue_showdelay(){
        //mission_issue_http->stop();
        mission_issue_http->exit();
        mission_issue_http->wait();
        //mission_issue_http = nullptr;
        existmission_dialog->close();
        //existmission_dialog = nullptr;
        while(!mission_issue_http->isFinished() && mission_issue_http->isRunning() );                                                             //等待http释放再释放父对象
        emit close_mission_dialog();
}

void ConfirmMission_Dialog::MissionIssue_recv(QString str){                                     //POST请求云端下发任务
    QJson::Parser parser;
    bool ok;
    QVariantMap MisIssue=parser.parse(str.toUtf8(),&ok).toMap();
    if(!ok){                                                                                                                             //如果没有数据，失败return
        qDebug() << "********MissionIssue_recv get data failed********";
        return;
  }else{
        QString Status  = MisIssue["msg"].toString();
        qDebug() << "msg" << Status;
        if(Status != "OK"){                                                                                                      //别的状况，例如任务已经下发到车端
            qDebug() << "********MissionIssue_recv: mission is running********";
            timer = new QTimer(this);
            //如果当前有任务，弹窗提示任务执行中...
            existmission_dialog = new Existmission_dialog(this);
            existmission_dialog->show();

            timer->singleShot(5000,this,SLOT(issue_showdelay()));                                     //弹窗延时5s（测试），实际为20s或30s（产品决定）
            timer->stop();
            timer->deleteLater();
        }else{                                                                                                                           //下发任务成功
            //mission_issue_http->stop();
            mission_issue_http->exit();
            mission_issue_http->wait();
            mission_issue_http = nullptr;
            qDebug() << "********MissionIssue_recv get data successfully********";
            mission_list_http = new Mission_http(GET_MISSION_LIST,0,this);                    //再次判断车端系统当前是否有任务
            //connect(mission_list_http,SIGNAL(finished()),mission_list_http,SLOT(deleteLater()));
            connect(mission_list_http,SIGNAL(Mission_data(QString)),this,SLOT(MissionList_recv_again(QString)));
            mission_list_http->start();
        }
  }

}

void ConfirmMission_Dialog::list_showdelay_again(){
        //mission_list_http->stop();
        mission_list_http->exit();
        mission_list_http->wait();
        remotemission_dialog->close();
        //remotemission_dialog = nullptr;
        while(!mission_list_http->isFinished() && mission_list_http->isRunning());                                                             //等待http释放再释放父对象
        emit close_mission_dialog();
}

void ConfirmMission_Dialog::MissionList_recv_again(QString str){                        //再次判断车端系统当前是否有任务
    QJson::Parser parser;
    bool ok;

    QStringList strlist1 = str.split("]");                                                              //获取data中  [  ]  的数据
    QStringList strlist2 = str.split("[");
    str.resize(strlist1[0].size());
    str.remove(0,strlist2[0].length()+1);

    qDebug() << "----------------" <<str;

    bool runable = true;
    QStringList strlist = str.split("},");                                                              //按{ }分成字符串列表

    for(int i=0; i<strlist.length()-1; i++){
        if(i%2==0){
            strlist[i] += "},";
            strlist[i+1] += "}";
            QString data = strlist[i] +strlist[i+1];
            QVariantMap datamap=parser.parse(data.toUtf8(),&ok).toMap();
            QVariantMap missiondata = datamap["mission_param"].toMap();
            QString Mapname = missiondata["map_name"].toString();
            int status = datamap["status"].toInt();
            if(status == 0 || status == 1 || status == 2 || status == 4 || status == 5){
                  runable = false;
              }
              if(status == 3 && Mapname != Map_name){
                  runable = false;
              }
            qDebug() << "MissionList_recv_again map_name" << Mapname;
            qDebug() << "MissionList_recv_again status" << status;
        }
    }

    if(runable){                                                                                        //没有远程任务启动，任务下发成功
        qDebug() << "********MissionList_recv_again have no extra mission ********";
        //mission_list_http->stop();
        mission_list_http->exit();
        mission_list_http->wait();
        while(!mission_list_http->isFinished() && mission_list_http->isRunning());                                                             //等待http释放再释放父对象
        emit close_mission_dialog();
        emit close_todaymission();
        emit RunningMissionID(MissionID);
        qDebug() << "running mission id:" << MissionID;
        //return;
    }else{                                                                                                                          //如果有任务，弹窗提示:远程下发了新任务，您的任务稍后执行
         timer = new QTimer(this);
        qDebug() << "********MissionList_recv_again have extra mission ********";
        remotemission_dialog = new Remotemission_dialog(this);
        remotemission_dialog->show();
        timer->singleShot(5000,this,SLOT(list_showdelay_again()));                                             //弹窗延时5s（测试），实际为20s或30s（产品决定）
        timer->stop();
        timer->deleteLater();
    }
}
