#include "history_mission.h"
#include "ui_history_mission.h"

#include "mission_dialog.h"
#include<QJson/Parser>
#include<QJson/Serializer>

History_mission::History_mission(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::History_mission)
{
        qDebug() << "********History_mission open********";
        setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);//隐藏顶部栏
        setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
        ui->setupUi(this);

        isfinished = true;
        ui->widget->installEventFilter(this);

        //ui->TodayButton->setPalette(pel);
        //字体颜色-翠绿
        //pel.setColor(QPalette::ButtonText,QColor(0,150,152));
        //ui->BookButton->setPalette(pel);
        //ui->ReturnButton->setStyleSheet("background-image: url(:/new/picture/mission_interface/Rectangle 252.png);border:1px solid #5DBFC4;border-radius:5px; color:rgb(255,255,255);");
        //"返回"背景翠绿色+字体白色+线宽1px翠绿色+四边圆角
         ui->ReturnButton->setStyleSheet("QPushButton{background-color: rgb(0, 150, 157);color:white;border:1px solid #00969D;border-radius:5px;  }");//线条


//        //滚动按建设计
//        scrollarea = new QScrollArea(this);
//        scrollarea->setGeometry(5,57,470,221);
//        scrollarea->setStyleSheet("QScrollArea{background-color: rgba(0, 0, 0, 0);border:0px solid #838486;  }");//透明+取消线条
//        scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//        scrollarea->hide();
        ui->scrollArea->setStyleSheet("QScrollArea{background-color: rgba(0, 0, 0, 0);border:0px solid #838486;  }");//透明+取消线条
        ui->scrollAreaWidgetContents->setStyleSheet("background-color: rgba(0, 0, 0, 0);");//透明
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->textEdit->setStyleSheet("background-color: rgba(0, 0, 0, 0);border:0px;font-family: 'Source Han Sans CN';font-size: 22px;");//透明+取消线条


        scrollbarsheet = "QScrollBar:vertical { margin:3px 0 3px 0;  width:1px;background-image: url(:/new/picture/mission_interface/mission_bakg.png); } "
                         "QScrollBar::handle:vertical { background-color:#005357; margin:0 0px 0 0px; border-radius:5px; }"
                         "QScrollBar::sub-line:vertical,QScrollBar::add-line:vertical{subcontrol-origin: margin;height:1px;"
                         "border-image: url(:/new/picture/mission_interface/mission_bakg.png); }"
                         "QScrollBar::up-arrow:vertical {border-image: url(:/new/picture/mission_interface/mission_bakg.png);  } "
                         "QScrollBar::down-arrow:vertical { border-image: url(:/new/picture/mission_interface/mission_bakg.png); }"
                        "QScrollBar::sub-page:vertical,QScrollBar::add-page:vertical{background-image: url(:/new/picture/mission_interface/mission_bakg.png); }";
        verticalScrollBar = new QScrollBar(this);
        verticalScrollBar->setGeometry(455,57,20,212);
        verticalScrollBar->setStyleSheet(scrollbarsheet);         //sub-page,add-page

        verticalScrollBar->setSingleStep(24);
        verticalScrollBar->hide();

        connect(verticalScrollBar,SIGNAL(valueChanged(int)),this,SLOT(ScrollBarValchange(int)));

        //图片
        pix = QPixmap(":/new/picture/startwindow/Ellipse.png");
        pix = pix.scaled(150,150,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        this->timer = new QTimer();
        timer->start(10);
        connect(timer,SIGNAL(timeout()),this,SLOT(update()));
        this->angle = 0;
        ui->textEdit->setText("加载中");
        ui->textEdit->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

void History_mission::paint(){
    QPainter painter(ui->widget);
    int x = 45,y=15;
    angle += 8;                             //转动速度
    if(angle == 360) angle=0;

    painter.translate(x+pix.width()/2,y+pix.height()/2);        //旋转中心
    painter.rotate(angle);
    painter.translate(-x-pix.width()/2,-y-pix.height()/2);       //恢复中心点
    painter.drawPixmap(x,y,pix.width(),pix.height(),pix);      //画图
}

bool History_mission::eventFilter(QObject *watched, QEvent *event){
    if(event->type() == QEvent::Paint && watched == ui->widget){
        paint();
    }
    return QMainWindow::eventFilter(watched,event);
}

void History_mission::ScrollBarValchange(int val){
    ui->scrollArea->verticalScrollBar()->setValue(verticalScrollBar->value()*2);
}


void History_mission::MissionList_recv(QString str){
    isfinished = false;
    timer->stop();
    delete timer;
    ui->widget->hide();
    QJson::Parser parser;
    bool ok;
    QVariantMap Missionmap=parser.parse(str.toUtf8(),&ok).toMap();
    //qDebug() << "dta" << str;
    if(!ok){
        qDebug() << "********get data failed********";
        return;
    }else{
        ui->ReturnButton->setEnabled(false);
        //qDebug() << "emit1";
        int Totalnum = Missionmap["total"].toInt();
        int btnNum = Totalnum;
        int contextHeight = btnNum*48 + (btnNum+1)*8 +57;
        ui->scrollAreaWidgetContents->setMinimumHeight(contextHeight);//必须这样才能显示滑块
        verticalScrollBar->setMaximum(btnNum*32);
        //qDebug() << "scroll" << ui->scrollArea->verticalScrollBar()->maximum();
        if(contextHeight > ui->scrollArea->height()){
            scrollbarsheet.append(QString("QScrollBar::handle:vertical{min-height:%1px}").arg(50000/contextHeight));
            verticalScrollBar->setStyleSheet(scrollbarsheet);         //sub-page,add-page
            verticalScrollBar->show();
        }

        QStringList strlist1 = str.split("]");
        QStringList strlist2 = str.split("[");
        str.resize(strlist1[0].size());
        str.remove(0,strlist2[0].length()+1);
        //qDebug()<<str;

//        QString Missiondata = Missionmap["data"].toString();
        QStringList strlist = str.split("},");
        for(int i=0; i<strlist.length()-1; i++){
                strlist[i] += '}';
                //qDebug() << "dta" << strlist[i];
        }


        QFont font;
        font.setFamily("Source Han Sans CN");
        font.setPointSize(18);
        //qDebug() << "dta" << strlist[strlist.length()-1];
        for(int i=0; i<Totalnum;++i){
            QVariantMap strmap=parser.parse(strlist[i].toUtf8(),&ok).toMap();
            int MissionID = strmap["id"].toInt();
            QString Start_time = strmap["start_time"].toString();
            QString Start_time_show = Start_time;
            QStringList start1 = Start_time_show.split(".");
            QStringList start2 = Start_time_show.split("T");
            Start_time_show.resize(start1[0].size()-3);
            Start_time_show.remove(0,start2[0].length()+1);

            QString End_time = strmap["end_time"].toString();
            QString End_time_show = End_time;
            QStringList End1 = End_time_show.split(".");
            QStringList End2 = End_time_show.split("T");
            End_time_show.resize(End1[0].size()-3);
            End_time_show.remove(0,End2[0].length()+1);
            //qDebug() << End_time_show;

            QString Map_name = strmap["map_name"].toString();
            QString Status = strmap["status"].toString();
            Status.remove(0,7);
            QString Mission_name = strmap["name"].toString();

            QPushButton *list_btn = new QPushButton(ui->scrollAreaWidgetContents);
            QLabel *timelabel = new QLabel(ui->scrollAreaWidgetContents);
            QPushButton *statusbutton = new QPushButton(ui->scrollAreaWidgetContents);
            //list_btn->setFont(font);

            int y = 57*(i+1);
            list_btn->setGeometry(3,y,438,48);
            list_btn->setStyleSheet("QPushButton{font-family: 'Source Han Sans CN';font-size: 18px;background-color:transparent;text-align:center;border:1px solid #5DBFC4;border-radius:5px;  } ");
            //list_btn->setText(QString("%1-%2    %3    %4").arg(Start_time_show).arg(End_time_show).arg(Mission_name).arg(Status));
            list_btn->setText(QString("%1").arg(Mission_name));


            //timelabel->setGeometry(17,y+7,140,33);
            timelabel->setStyleSheet("font-family: 'Source Han Sans CN';font-size: 18px;background-color:transparent;text-align:left; ");
            QFont font = timelabel->font();
            QString text = Start_time_show+"-"+End_time_show;
            int textWidth = timelabel->fontMetrics().width(text);
            int width = timelabel->width();
            int count = text.count()-1;
            qreal space = qreal(width-textWidth)/count;
            font.setLetterSpacing(QFont::AbsoluteSpacing,space);
            timelabel->setFont(font);
            timelabel->setText(text);
            //timelabel->setText(QString("%1-%2").arg(Start_time_show).arg(End_time_show));
            //timelabel->setText(Start_time_show+"-"+End_time_show);

            //statusbutton->setGeometry(3+350,y+4,77,40);
            statusbutton->setMaximumWidth(77);
            statusbutton->setStyleSheet("font-family: 'Source Han Sans CN';font-size: 18px;background-color:transparent;text-align:right; ");
            if(Status == "DONE"){
                statusbutton->setStyleSheet("font-family: 'Source Han Sans CN';font-size: 18px;background-color:transparent;text-align:right;color : #05BE4F;border:none;");
                statusbutton->setEnabled(false);
                statusbutton->setText("已完成");
            }else if(Status == "ABORTED"){
                statusbutton->setStyleSheet("font-family: 'Source Han Sans CN';font-size: 18px;background-color:transparent;text-align:right;color : #DD960C;border:none;");
                statusbutton->setEnabled(false);
                statusbutton->setText("任务中断");
            }else{
                statusbutton->setStyleSheet("background-image: url(:/new/picture/mission_interface/Rectangle 254.png);font-family: 'Source Han Sans CN';font-size: 18px;text-align:center;color : rgb(255,255,255);border-radius:5px;");
                statusbutton->setEnabled(true);
                statusbutton->setText("一键补扫");
            }
            QHBoxLayout *Layout = new QHBoxLayout();
            Layout->addWidget(timelabel);
            Layout->addWidget(statusbutton);
            list_btn->setLayout(Layout);
            list_btn->show();
            QCoreApplication::processEvents(QEventLoop::AllEvents,10);

//            qDebug() << "ID:" << strmap["id"].toInt();
//            qDebug() << "STARTTIME:" << strmap["start_time"].toString();
//            qDebug() << "ENDTIME:" << strmap["end_time"].toString();
//            qDebug() << "MAPNAME:" << strmap["map_name"].toString();
//            qDebug() << "STATUS" << strmap["status"].toString();
//            qDebug() << "DEVICENAME" << strmap["device_name"].toString();
        }

    }
     isfinished = true;
     //qDebug() << "history_finish";
     ui->ReturnButton->setEnabled(true);
}

History_mission::~History_mission()
{
    qDebug() << "********History_mission close********";
    delete ui;
}

void History_mission::on_ReturnButton_clicked()
{
    //qDebug() << "history_finish" << isfinished;
    this->close();
    emit close_mission_dialog();
}


