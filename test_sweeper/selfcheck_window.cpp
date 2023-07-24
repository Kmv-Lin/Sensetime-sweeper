#include "selfcheck_window.h"
#include "ui_selfcheck_window.h"

#include "qdatetime.h"
#include <QTimer>
#include "qpainter.h"
#include <QMessageBox>
#include<QDebug>

#include "popwindow/selfcheck_dialog.h"
#include "mainwindow.h"

#include "thread/my_thread.h"

selfcheck_Window::selfcheck_Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::selfcheck_Window)
{
    qDebug() << "\n********selfcheck_Window open********";
    setWindowFlags(Qt::FramelessWindowHint);//隐藏顶部栏
    setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
    ui->setupUi(this);

     ui->SelfcheckBar->setValue(0);//进度条初始为0

    //背景
    QPixmap pixmap = QPixmap(":/new/picture/selfcheck/check.png").scaled(this->size());
    QPalette Palette;
    Palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(Palette);

    //图片
    pix = QPixmap(":/new/picture/selfcheck/Ellipse 2.png");
    this->timer = new QTimer();
    timer->start(10);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    this->angle = 0;

    thread= new my_thread(this);
    connect(thread,  SIGNAL(progress(int,const char*)), this, SLOT(UpdateSelfcheckBar(int, const char*)) );
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    thread->start();

    HttpThread = new HttpReadThread(Start_Window,this);
    connect(HttpThread, SIGNAL(selfCheckSignal(QString)), this, SLOT(parseData(QString)));
    HttpThread->start();
}

selfcheck_Window::~selfcheck_Window()
{
    delete ui;
    delete thread;
    qDebug() << "********selfcheck_Window close********";
}

void selfcheck_Window::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int x = 438,y=94;                                                               //旋转中心
    angle += 5;                                                                         //转动速度
    if(angle == 360) angle=0;
    painter.translate(x+pix.width()/2,y+pix.height()/2);        //旋转
    painter.rotate(angle);
    painter.translate(-x-pix.width()/2,-y-pix.height()/2);       //恢复中心点
    painter.drawPixmap(x,y,pix.width(),pix.height(),pix);      //画图
}


//void selfcheck_Window::run_picture(int i)
//{
//    //旋转图片(i=0~100)
//    QMatrix matrix;
//    matrix.rotate(20*i);
//    ui->progresslabel->setPixmap(QPixmap(":/new/picture/selfcheck/Ellipse 2.png").transformed(matrix,Qt::SmoothTransformation));
//}

void selfcheck_Window::UpdateSelfcheckBar(int p, const char* msg)
{
    ui->SelfcheckBar->setValue(p);//Bar
    //run_picture(p);//picture

     //QString  msg_read =  ui->CheckDisplayLabel->text();

    //ui->CheckDisplayLabel->setText(msg_read+'\n'+'\n'+msg);

    if(p == 100){
        thread->stop();
        thread->exit();
        thread->wait();

        HttpThread->stop();
        timer->stop();
        delete timer;
        //emit MainW_Show();

        //延时开启线程
        QTime dieTime =  QTime::currentTime().addMSecs(1000);
        while(QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents,1000);

        this->hide();
    }
}

void selfcheck_Window::exec()
{
    setWindowModality(Qt::ApplicationModal);
    show();
    m_eventloop->exec();

}

void selfcheck_Window::closeEvent(QCloseEvent *event)
{
    m_eventloop->exit();
    return QMainWindow::closeEvent(event);

}

void selfcheck_Window::parseData(QString str)
{
//    str = "{\"light\": {\"left_light\": \"passed\","
//          " \"right_light\": \"passed\",   \"brake_light\": \"passed\"},\"underpan\"：\"passed\"}";
    unsigned int i = 0;
    QJson::Parser parser;
    bool ok;
    bool status = true;
    QVariantMap startmap=parser.parse(str.toUtf8(),&ok).toMap();
    //qDebug() << "str="<<str;
    if(!ok){
        qDebug() << "********failed********";
        //exit(1);
    }
    else{
#if 0
        QVariantMap results1 = results["robo"].toMap();
        QVariantMap results2 = results["light"].toMap();
        QVariantMap results3 = results["sweeper"].toMap();
        QVariantMap results4 = results["vehicle_control"].toMap();
        //QVariantMap results5 = results["underpan"].toMap();
#else
        /***********************node_status************************/
//        QLabel *CheckDisplayLabel = new QLabel();
//        CheckDisplayLabel->setParent(ui->scrollAreaWidgetContent);
        QFont font;
        font.setFamily("Ubuntu");
        font.setPointSize(20);

        QVariantMap results1 = startmap["node_status"].toMap();
        QVariantMap Camera = results1["camera"].toMap();
#endif

#if 1
//        //有问题
//        if(Camera["status"].toString() == "RUNNING"){
//            CheckDisplayLabel->setFont(font);
//            CheckDisplayLabel->setGeometry(3,1,438,48);
//            CheckDisplayLabel->setText("xxxxxxxxx");
//            CheckDisplayLabel->show();
//        }
#else
        // 底盘检测结果处理
        if(results["underpan"].toString() == "passed"){
            ++i;
            //m_pProgressBar->setValue(i);
            QString  msg_read =  ui->CheckDisplayLabel->text();
           ui->CheckDisplayLabel->setText(msg_read+'\n'+'\n'+"底盘检测完成");
            //m_pChassisCheck->setText(tr("底盘检测完成"));
        }else{
            status = false;
            //QMessageBox::warning(this,"","底盘异常，排除故障后再试");
            selfcheck_Dialog  *win = new selfcheck_Dialog(this);
            win->show();
        }

        // 车辆控制检测结果处理
        if(results4["brake"].toString() == "passed"){
            ++i;
            //m_pProgressBar->setValue(i);
        }else{
            //QMessageBox::warning(this,"","刹车异常，排除故障后再试");
            selfcheck_Dialog  *win = new selfcheck_Dialog(this);
            win->show();
        }

        if(results4["left_turn"].toString() == "passed"){
            ++i;
            //m_pProgressBar->setValue(i);
        }else{
            ;//DefMessageBox::Information(NULL, "左转异常，排除故障后再试", OK);
        }

        if(results4["right_turn"].toString() == "passed"){
            ++i;
            ;//m_pProgressBar->setValue(i);
        }else{
            ;//DefMessageBox::Information(NULL, "右转异常，排除故障后再试", OK);
        }

        if(results4["forward"].toString() == "passed"){
            ++i;
            ;//m_pProgressBar->setValue(i);
        }else{
            ;//DefMessageBox::Information(NULL, "前进异常，排除故障后再试", OK);
        }

        if(results4["reverse"].toString() == "passed"){
            ++i;
            ;//m_pProgressBar->setValue(i);
        }else{
            ;//DefMessageBox::Information(NULL, "后退异常，排除故障后再试", OK);
        }

        if((results4["brake"].toString() == "passed") &&
                (results4["left_turn"].toString() == "passed") &&
                (results4["right_turn"].toString() == "passed") &&
                (results4["forward"].toString() == "passed") &&
                (results4["reverse"].toString() == "passed"))
        {
            ;//m_pControlModuleCheck->setText(tr("车辆控制检测完成."));
            QString  msg_read =  ui->CheckDisplayLabel->text();
           ui->CheckDisplayLabel->setText(msg_read+'\n'+'\n'+"车辆控制检测完成.");
        }
        else
        {
            status = false;
        }

        // 边刷检测结果处理
        if(results3["side_brush"].toString() == "passed"){
            ++i;
//            m_pProgressBar->setValue(i);
//            m_pSweeperModuleCheck->setText(tr("清扫模块完成"));
        }else{
            status = false;
//            DefMessageBox::Information(NULL, "边刷异常，排除故障后再试", OK);
        }

        // 车灯检测结果处理
        if(results2["brake_light"].toString() == "passed"){
            ++i;
//            m_pProgressBar->setValue(i);
        }else{
//            DefMessageBox::Information(NULL, "刹车灯异常，排除故障后再试", CANCEL);
        }

//        if(results2["left_light"].toString() == "passed"){
//            ++i;
//            m_pProgressBar->setValue(i);
//        }else{
//            DefMessageBox::Information(NULL, "左转灯异常，排除故障后再试", CANCEL);
//        }

//        if(results2["right_light"].toString() == "passed"){
//            ++i;
//            m_pProgressBar->setValue(i);
//        }else{
//            DefMessageBox::Information(NULL, "右转灯异常，排除故障后再试", CANCEL);
//        }

//        if((results2["brake_light"].toString() == "passed") &&
//                (results2["left_light"].toString() == "passed") &&
//                (results2["right_light"].toString() == "passed"))
//        {
//            m_pLampModuleCheck->setText(tr("车灯检测中......"));
//        }
//        else
//        {
//            status = false;
//        }

//         // Robox检测结果处理
//        if(results1["localization_node"].toString() == "passed"){
//            ++i;
//            m_pProgressBar->setValue(i);
//        }else{
//            DefMessageBox::Information(NULL, "localization_node异常，排除故障后再试", OK);
//        }

//        if(results1["ins_node"].toString() == "passed"){
//            ++i;
//            m_pProgressBar->setValue(i);
//        }else{
//            DefMessageBox::Information(NULL, "ins_node异常，排除故障后再试", OK);
//        }

//        if(results1["decision_planning_node"].toString() == "passed"){
//            ++i;
//            m_pProgressBar->setValue(i);
//        }else{
//            DefMessageBox::Information(NULL, "decision_planning_node异常，排除故障后再试", OK);
//        }

//        if(results1["control_node"].toString() == "passed"){
//            ++i;
//            m_pProgressBar->setValue(i);
//        }else{
//            DefMessageBox::Information(NULL, "control_node异常，排除故障后再试", OK);
//        }

//        if(results1["lidar_node"].toString() == "passed"){
//            ++i;
//            m_pProgressBar->setValue(i);
//        }else{
//            DefMessageBox::Information(NULL, "lidar_node异常，排除故障后再试", OK);
//        }

//        if(results1["camera_node"].toString() == "passed"){
//            ++i;
//            m_pProgressBar->setValue(i);
//        }else{
//            DefMessageBox::Information(NULL, "camera_node异常，排除故障后再试", OK);
//        }

//        if(results1["canbus_node"].toString() == "passed"){
//            ++i;
//            m_pProgressBar->setValue(i);
//        }else{
//            DefMessageBox::Information(NULL, "canbus_node异常，排除故障后再试", OK);
//        }

//        if(results1["perception_node"].toString() == "passed"){
//            ++i;
//            m_pProgressBar->setValue(i);
//        }else{
//            DefMessageBox::Information(NULL, "perception_node异常，排除故障后再试", OK);
//        }

//        if(results1["gnss_node"].toString() == "passed"){
//            ++i;
//            m_pProgressBar->setValue(i);
//        }else{
//            DefMessageBox::Information(NULL, "gnss_node异常，排除故障后再试", OK);
//        }

        if((results1["localization_node"].toString() == "passed") &&
                (results1["ins_node"].toString() == "passed") &&
                (results1["decision_planning_node"].toString() == "passed") &&
                (results1["control_node"].toString() == "passed") &&
                (results1["lidar_node"].toString() == "passed") &&
                (results1["camera_node"].toString() == "passed") &&
                (results1["canbus_node"].toString() == "passed") &&
                (results1["perception_node"].toString() == "passed") &&
                (results1["gnss_node"].toString() == "passed"))
        {
            QString  msg_read =  ui->CheckDisplayLabel->text();
           ui->CheckDisplayLabel->setText(msg_read+'\n'+'\n'+"自动驾驶检测完成");
        }
        else
        {
            status = false;
        }
    }

    if(true == status)
    {
        ;
//        Singleton *p = Singleton::getInstance();
//        this->hide();
//        p->m_pMainWd->show();
    }
#endif
    }
}
