#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QTimer>
#include<QDebug>
#include<QForeachContainer>
#include<QForeachContainerBase>

#include <QDebug>
#include <QJson/Parser>
#include <QJson/Serializer>

#include "mission_dialog.h"
#include "settingwindow.h"
#include "selfcheck_window.h"

#include "thread/httpthread.h"
#include "thread/my_thread.h"
#include "inputmethod/inputmethod.h"
#include "inputmethod/keyboardmindialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug() << "******** MainWindow open********";
     setWindowFlags(Qt::FramelessWindowHint);//隐藏顶部栏
     setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏,窗口关闭时，子对象自动调用析构函数
    ui->setupUi(this);

//    //背景
//    QPixmap pixmap = QPixmap(":/new/picture/mainpicture1.png").scaled(this->size());
//    QPalette Palette;
//    Palette.setBrush(QPalette::Background, QBrush(pixmap));
//    this->setPalette(Palette);

   ui->dateTimeEdit->setStyleSheet("QDateTimeEdit{background-color: rgba(0, 0, 0, 0);border:0px solid #838486;  }");//透明+取消线条
   ui->BatterBar->setStyleSheet("QProgressBar::chunk{background:solid #70DADF}QProgressBar{border-color: rgba(255, 255, 255, 0);color: rgb(255, 255, 255);background-color: rgba(255, 255, 255, 0);}");//电池图片+取消线条+颜色#70DADF  QProgressBar{border:0px solid #838486; text-align:Qt::AlignCenter;}
   ui->BatterBar->setValue(0);//Bar
    //ui->mission_widget->setVisible(false);//隐藏任务窗口

   //ui->SettingButton->setStyleSheet("QPushButton{background:solid #0F9BA3;border:1px solid #0F9BA3; border-radius:5px; }");

    thread2 = new my_thread(this);
    timer = new QTimer(this);
//    unsigned int times = 1000;
    connect(timer, SIGNAL(timeout()), this, SLOT(AutoRun1S())); //关联超时信号和曹函数 10S更新状态     //每10s更新世界时间以及FSM状态

     m_pMySerialPort = new SerialPort();
     m_pReadThread = new SerialPortReadThread();
     connect(m_pReadThread, SIGNAL(receiveSignal(const char*)), this, SLOT(receiveData(const char*)));      //开启串口读数据，传递参数是读取的数据

    //inputmethod = new InputMethod(this);
    keyin= new KeyboardMinDialog(this);                 //构造主界面时直接构建输入密码界面，然后隐藏  ——正常来讲不是这个逻辑
    keyin->hide();
     connect(keyin, SIGNAL(keyin_value(const char * )), this, SLOT(KeyboardMinDialog_sign(const char *)));  //判断输入密码是用于打开设置面板还是向串口发送数据，密码正确触发keyin_value信号

     m_HttpThread = new HttpReadThread(Main_Window,this);
     connect(m_HttpThread, SIGNAL(vehicleStateSignal(QString)), this, SLOT(vehicleStateSlot(QString)));//推送到主界面    string传输的是HTTP获取的reply数据,vehicleStateSlot对数据进行解析处理

     ui->RunningState_label->hide();
     ui->RunningState_label->setStyleSheet("QLabel{background-color: rgb(15, 155, 163);border:1px solid #5DBFC4;border-top-right-radius:5px;border-top-left-radius:5px;color: rgb(255, 255, 255);  }");//线条

     water_timer = new QTimer(this);
     connect(water_timer, SIGNAL(timeout()), this, SLOT(water_flash())); //关联超时信号和曹函数 10S更新状态     //每10s更新世界时间以及FSM状态

     finish_num = 0;
     all_num = 0;
     //RemoteCtrlState_flag = false;
     FSM_state = 0;

     Vehicle_Controller = 0xE0;
     progress=0;

     this->setNtp();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
    delete inputmethod;
    delete keyin;
    qDebug() << "********MainWindow close********";//确认窗口关闭
}
void MainWindow::vehicleStateSlot(QString str)
{
    //qDebug() <<"str"<<str;
    //const QString str = "{\"username\" :\"admin\", \"password\" :\"123456\", \"age\": 32}";
    QJson::Parser parser;
    bool ok;
    QVariantMap variantmap=parser.parse(str.toUtf8(),&ok).toMap();
    if(!ok){
        qDebug() << "********vehicleStateSlot failed********";
        data_finish = 0;
    }
    else{
        data_finish = 1;
        QVariantMap result=variantmap["result"].toMap();
        QVariantMap battery=result["battery"].toMap();
        QVariantMap mission_info=result["mission_info"].toMap();
        QVariantMap watter=result["watter"].toMap();
        QVariantMap current_mission = result["current_mission"].toMap();

//        foreach(QVariant result, variantmap["result"].toList()){
//            //QVariantMap map = record.toMap();
//             qDebug() << "***battery_info is **"<< result.toString();
//        }

        //任务数据
        finish_num = mission_info["finished_missions_num"].toInt();//for bug fix
        all_num = mission_info["mission_all_num"].toInt();//for bug fix

        //显示电量
         battery_info = battery["battery_info"].toInt();
         if(battery_info <=15){
            ui->BatterBar->setStyleSheet("QProgressBar::chunk{background:solid #EC5249}QProgressBar{border-color: rgba(255, 255, 255, 0);color: rgb(255, 255, 255);background-color: rgba(255, 255, 255, 0);}");
         }else{
            ui->BatterBar->setStyleSheet("QProgressBar::chunk{background:solid #70DADF}QProgressBar{border-color: rgba(255, 255, 255, 0);color: rgb(255, 255, 255);background-color: rgba(255, 255, 255, 0);}");
         }
         ui->BatterBar->setValue(battery_info);//Bar
        //qDebug() << "********battery_info is ********"<<battery_info;

         //显示水量
        water_state = watter["state"].toString();
        water_info = watter["watter_info"].toInt();
        set_watter_label(watter["state"].toString(), watter["watter_info"].toInt());
        //qDebug() << "********watter_info is ********"<<watter["watter_info"].toInt();
        emit mission_show(battery_info,water_info,water_state);
        //车辆状态
        //QString vehicle_state = variantmap["vehicle_state"].toString();
        SYS_state = variantmap["vehicle_state"].toString();
        //qDebug() << "********vehicle_state is  ********"<<vehicle_state;

        if(QString::compare(SYS_state,QString("RemoteCtrlState"))==0)
        {
//            set_statue_label(0xF0);
//            ui->RunningState_label->hide();
            ;
        }
//        else if(QString::compare(SYS_state,QString("IdleState"))==0)
//        {
//            finish_num = mission_info["finished_missions_num"].toInt();//for bug fix
//            all_num = mission_info["mission_all_num"].toInt();//for bug fix

//            set_statue_label(0xE0);
//            ui->RunningState_label->hide();
//            set_Display_Label_IdleState( finish_num, all_num);
//        }
        else if(QString::compare(SYS_state,QString("RunningState"))==0)
        {
                //set_statue_label(0xE0);
               //ui->RunningState_label->setStyleSheet("QLabel{background-color: rgb(15, 155, 163);border:1px solid #5DBFC4;border-radius:5px;color: rgb(255, 255, 255);  }");//线条
//               ui->RunningState_label->show();

                progress = current_mission["progress"].toDouble();
//               //qDebug() << "********progress  is ********"<<progress;
//               set_Display_Label_Runningstate(int(progress*100));
//               set_RunningState_Label(finish_num, all_num);
        }
        else if(QString::compare(SYS_state,QString("SuspendState"))==0)
        {
        ;
        }

    }


}

void MainWindow::on_SettingButton_clicked()
{
     //inputmethod->showKeyBoard();

    keyin->show();
    keyboard_flag_setting = 1;

    //WeiqianFunctions::Beep();

}

void MainWindow::on_MissionButton_clicked()         //打开任务窗口
{
    //ui->mission_widget->setVisible(false);//打开任务窗口
     Mission_Dialog  *win = new Mission_Dialog(this);
     connect(this,SIGNAL(mission_show(int,int,QString)),win,SLOT(AutoRun(int,int,QString)));
     //win->setWindowFlags(Qt::Window);                //set as window增加窗口属性，会增加标题和边框！！
     win->show();
    //win->showFullScreen(); //隐藏顶部栏

     //用于延迟隐藏或删除，防止闪绿屏
     QTime dieTime =  QTime::currentTime().addMSecs(100);
     while(QTime::currentTime() < dieTime)
      QCoreApplication::processEvents(QEventLoop::AllEvents,100);

     //this->hide();
     //WeiqianFunctions::Beep();

}

void MainWindow::AutoRun1S()
{
    //qDebug() << "********entry time out 1S********";
   // thread2->start();
//     ui->statue_label->clear();
    //datetime = QDateTime::currentDateTimeUtc();

//     timer->stop();s     //定时器close
    datetime = QDateTime::currentDateTime();
    //qDebug() << "QDateTime" << datetime.toString();
    ui->dateTimeEdit->setDateTime(datetime);

    MainWindow_FSM();

}
void MainWindow::setNtp()
{
    ntpIP = "time.windows.com";    //windows NTP服务器地址
    udpSocket = new QUdpSocket(this);
    connect(udpSocket,SIGNAL(connected()),this,SLOT(connectsucess()));
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(readingDataGrams()));
    udpSocket->connectToHost(ntpIP,123);
}

void MainWindow::setDateTime(QDateTime &dateTime){
//    qDebug() << "current time" << dateTime.toString();
    QString date = dateTime.toString("yyyy-MM-dd hh:mm:ss");
    QString showtime = QString("date -s \"%1\"").arg(date);

    QProcess::startDetached(showtime);
    QProcess::startDetached("hwclock -w");
    QProcess::startDetached("sync");
}


void MainWindow::connectsucess()
{
    qint8 LI=0;  //无预告
    qint8 VN=3;  //版本号
    qint8 MODE=3; //客户端几
    qint8 STRATUM=0;//表示本地时钟层次水平
    qint8 POLL=4; //测试间隔
    qint8 PREC=-6; //表示本地时钟精度
    QDateTime Epoch(QDate(1900, 1, 1));
    qint32 second=quint32(Epoch.secsTo(QDateTime::currentDateTime()));

    qint32 temp=0;
    QByteArray timeRequest(48, 0);
    timeRequest[0]=(LI <<6) | (VN <<3) | (MODE);
    timeRequest[1]=STRATUM;
    timeRequest[2]=POLL;
    timeRequest[3]=PREC & 0xff;
    timeRequest[5]=1;
    timeRequest[9]=1;

    //40到43字节保存原始时间戳，即客户端发送的时间
    timeRequest[40]=(temp=(second&0xff000000)>>24);
    temp=0;
    timeRequest[41]=(temp=(second&0x00ff0000)>>16);
    temp=0;
    timeRequest[42]=(temp=(second&0x0000ff00)>>8);
    temp=0;
    timeRequest[43]=((second&0x000000ff));
    udpSocket -> flush();
    udpSocket -> write(timeRequest);
    udpSocket -> flush();
}

void MainWindow::readingDataGrams()
{
    QByteArray newTime;
    QDateTime Epoch(QDate(1900, 1, 1));
    QDateTime unixStart(QDate(1970, 1, 1));
    do
    {
        newTime.resize(udpSocket->pendingDatagramSize());
        udpSocket->read(newTime.data(), newTime.size());
    }while(udpSocket->hasPendingDatagrams());

    QByteArray TransmitTimeStamp ;
    TransmitTimeStamp=newTime.right(8);
    quint32 seconds=TransmitTimeStamp[0];
    quint8 temp=0;
    for(int j=1;j<=3;j++)
    {
        seconds=seconds<<8;
        temp=TransmitTimeStamp[j];
        seconds=seconds+temp;
    }
    QDateTime time;
    time.setTime_t(seconds-Epoch.secsTo(unixStart));
    time = time.addSecs(60*60*8);
    qDebug() << "current time" << time.toString();
    setDateTime(time);

    this->udpSocket->disconnectFromHost();
    this->udpSocket->close();
}





void MainWindow::Receive_MainW_show(bool data_finished){
    this->show();
    unsigned int times = 500;
    this->timer->start(times);     //定时器启动
    this->data_finish = data_finished;
    keyboard_flag = 1;
    keyboard_flag_setting = 0;      //在点击设置按钮时，该位置1

    /*采用HTML格式同时显示图片和文字*/
    ui->statue_label2->setText("\
    <table width=\"100%\">\
    <tbody>\
    <tr>\
    <td align=\"middle\" valign=\"middle\"><> 空闲中... </td>\
    </tr>\
    </tbody>\
    </table>");

    //启动串口接收rr
    QByteArray name = QString("COM3").toUtf8();
    char *pName = const_cast<char*>(name.data());
    QByteArray parity = QString("8").toUtf8();
    char *pParity = parity.data();
    m_pMySerialPort->open(pName, 115200, 8, pParity, 1);
    m_pReadThread->setWeiQianFunctions(m_pMySerialPort);
    m_pReadThread->start();
    qDebug() << "********m_pMySerialPort COM3 open  in mainwindow********";

    //启动网口接收
    m_HttpThread->start();
}

void MainWindow::KeyboardMinDialog_sign(const char *password)
{
    //qDebug() <<password;

    if(keyboard_flag_setting){                                              //判断输入密码是用于打开设置面板还是向串口发送数据
            keyboard_flag_setting = 0;
            if(*password=='1'){                                                 //传递参数password为1,表示密码输入正确
                settingWindow  *win = new settingWindow(this);
                win->setWindowFlags(Qt::Window);                //set as window增加窗口属性，会增加标题和边框！！
                win->showFullScreen();                                      //全屏显示才能消除标题和边框

                //用于延迟隐藏或删除，防止闪绿屏
                QTime dieTime =  QTime::currentTime().addMSecs(100);
                while(QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents,100);

                //m_pMySerialPort->close();
            }
    }

    if(keyboard_flag==0){  //控制面板的密码请求  手动按键被按下
             if(*password=='1'){
                sendData(0x01);
                //set_statue_label(0x02);  no need here
             }
    }

     keyin->hide();
}

void MainWindow::receiveData(const char *str)
{
    //qDebug() <<str;
    if(receivecont<0xFFFFFFFF)
        receivecont++;
    else
        receivecont=0;

  //  QString tmp =QString.toInt(receivecont);
    //ui->Display_Label->setText(QString::number(receivecont));

    //手动按键被按下
    if(str[1] != keyboard_flag ){
        keyboard_flag = str[1] ;
         if(keyboard_flag==0){
            keyin->show();
        }
         else
             sendData(0x00);
    }

    //底盘车辆接管状态
    //set_statue_label(str[15] );
    Vehicle_Controller = str[15];



}

void MainWindow::sendData(char value)
{
//    QByteArray temp = ui->dateTimeEdit->text().toUtf8();
//    char *pBuff = temp.data();

    char temp1[16]={0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x20, 0x02, 0x01, 0x00, 0x30, 0x63, 0x00, 0x00, 0x00, 0xBB };

    temp1[1] = value;
    uint8_t check_sum=0;
    for(uint8_t i=0;i<sizeof(temp1)-1;i++)
        check_sum += temp1[i];

    temp1[15] = check_sum;

    m_pMySerialPort->write(temp1, sizeof(temp1));
}


void MainWindow::set_statue_label(char mode)
{
    if(mode== 0xF0){
        /*采用HTML格式同时显示图片和文字 vertical-align=\"middle\"*/
        ui->statue_label2->setText("\
        <table width=\"100%\">\
        <tbody>\
        <tr>\
        <td ><img src=\":/new/picture/Cloud.png\" ></td>\
        <td align=\"middle\" valign=\"middle\"><>云控模式 </td>\
        </tr>\
        </tbody>\
        </table>");
    }
    else if(mode== 0xE0){
        /*采用HTML格式同时显示图片和文字 vertical-align=\"middle\"*/
        if(!data_finish){
              ui->statue_label2->setText("\
            <table width=\"100%\">\
            <tbody>\
            <tr>\
            <td align=\"middle\" valign=\"middle\"><> 等待连接中... </td>\
            </tr>\
            </tbody>\
            </table>");
        }else{
            ui->statue_label2->setText("\
            <table width=\"100%\">\
            <tbody>\
            <tr>\
            <td align=\"middle\" valign=\"middle\"><> 空闲中... </td>\
            </tr>\
            </tbody>\
            </table>");
        }
    }
    else if(mode== 0x01){
        /*采用HTML格式同时显示图片和文字 vertical-align=\"middle\"*/
        ui->statue_label2->setText("\
        <table width=\"100%\">\
        <tbody>\
        <tr>\
        <td ><img src=\":/new/picture/remote.png\" ></td>\
        <td align=\"middle\" valign=\"middle\"><> 遥控模式 </td>\
        </tr>\
        </tbody>\
        </table>");
    }
    else if(mode== 0x02){
        /*采用HTML格式同时显示图片和文字 vertical-align=\"middle\"*/
        ui->statue_label2->setText("\
        <table width=\"100%\">\
        <tbody>\
        <tr>\
        <td ><img src=\":/new/picture/manual.png\" ></td>\
        <td align=\"middle\" valign=\"middle\"><> 手动模式 </td>\
        </tr>\
        </tbody>\
        </table>");
    }
    else if(mode== 0xFF){
        /*采用HTML格式同时显示图片和文字 vertical-align=\"middle\"*/
        ui->statue_label2->setText("\
        <table width=\"100%\">\
        <tbody>\
        <tr>\
        <td ><img src=\":/new/picture/Auto.png\" ></td>\
        <td align=\"middle\" valign=\"middle\"><> 自动模式 </td>\
        </tr>\
        </tbody>\
        </table>");
    }

}

void MainWindow::set_watter_label(QString state, int watter_info)
{

    if(watter_info<=10){
        water_timer->start(700);
        ui->water_label->setStyleSheet("QLabel{background-image: url(:/new/picture/water_low.png);border:0px solid #838486;  }");
    }
    else{
        water_timer->stop();
        ui->water_label->setStyleSheet("QLabel{background-image: url(:/new/picture/water_normal.png);border:0px solid #838486;  }");
        if(ui->water_label->isHidden()){
            ui->water_label->show();
        }
    }
}

void MainWindow::water_flash(){
    if(ui->water_label->isHidden()){
        ui->water_label->show();
    }else{
        ui->water_label->hide();
    }
}

void MainWindow::set_Display_Label_IdleState(int finish, int allnum)
{
    char buffer[256];

    sprintf(buffer,"已完成任务：%d/%d",finish,allnum);

    ui->Display_Label->setText(buffer);

//    /*采用HTML格式同时显示图片和文字*/
//    ui->Display_Label->setText("\
//    <table width=\"100%\">\
//    <tbody>\
//    <tr>\
//    <td align=\"middle\" valign=\"middle\"><> 已完成任务：</td>\
//    </tr>\
//    </tbody>\
//    </table>");

}
void MainWindow::set_Display_Label_Runningstate(int progress)
{
    char buffer[256];

    sprintf(buffer,"当前任务完成：%d%%", progress);

    ui->Display_Label->setText(buffer);

}



void MainWindow::set_RunningState_Label(int finish, int allnum)
{
    char buffer[256];

    sprintf(buffer,"    清扫作业中...             %d/%d",finish,allnum);

    ui->RunningState_label->setText(buffer);

}
void MainWindow::set_RunningState_Label1()
{
    char buffer[256];

    sprintf(buffer,"    车辆作业中 ");

    ui->RunningState_label->setText(buffer);

}

void MainWindow::MainWindow_FSM()
{
    switch (FSM_state) {
    case 0:
        if((QString::compare(SYS_state,QString("RunningState"))==0) || QString::compare(SYS_state,QString("RemoteCtrlState"))==0){
            FSM_state = 1;
            ui->RunningState_label->show();
        }
        else if((Vehicle_Controller==0x02) ||(Vehicle_Controller==0x01)||(Vehicle_Controller==0xFF))
        {
            FSM_state = 1;
            ui->RunningState_label->show();
        }
        else if((QString::compare(SYS_state,QString("SuspendState"))==0)){
            ui->RunningState_label->hide();
            FSM_state = 2;
        }
        else
        {
            set_statue_label(0xE0);

            set_Display_Label_IdleState( finish_num, all_num);
        }
        break;
    case 1:
        if((Vehicle_Controller==0xE0) && (QString::compare(SYS_state,QString("IdleState"))==0)){
            FSM_state = 0;
            ui->RunningState_label->hide();
        }
        else if((QString::compare(SYS_state,QString("SuspendState"))==0)){
            FSM_state = 2;
            ui->RunningState_label->hide();
        }
        else
        {
            if(Vehicle_Controller==0xFF)
            {
                if(QString::compare(SYS_state,QString("RemoteCtrlState"))==0)
                    set_statue_label(0xF0);
                else
                    set_statue_label(0xFF);

                set_Display_Label_Runningstate(int(progress*100));
                set_RunningState_Label(finish_num, all_num);
            }
            else{
                set_Display_Label_IdleState( finish_num, all_num);//for bug fix
                set_statue_label(Vehicle_Controller);
                set_RunningState_Label1();
            }

        }
        break;

    case 2:
        if((Vehicle_Controller==0xE0)||(QString::compare(SYS_state,QString("IdleState"))==0)){
            FSM_state = 0;
             ui->RunningState_label->hide();
        }
        else if((QString::compare(SYS_state,QString("RunningState"))==0) ){
            ui->RunningState_label->show();
            FSM_state = 1;
        }
        else
        {
            if(Vehicle_Controller ==0xFF){
                if(QString::compare(SYS_state,QString("RemoteCtrlState"))==0)
                    set_statue_label(0xF0);
                else
                    set_statue_label(0xFF);
            }
            else
                set_statue_label(Vehicle_Controller);

            ui->Display_Label->setText("暂停作业");
        }
        break;
    default:FSM_state=0;
        break;
    }



}
