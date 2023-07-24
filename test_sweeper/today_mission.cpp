#include "today_mission.h"
#include "ui_today_mission.h"
#include<QJson/Parser>
#include<QJson/Serializer>

Today_mission::Today_mission(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Today_mission)
{
    qDebug() << "********Today_mission open********";
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);//隐藏顶部栏
     setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
    ui->setupUi(this);

    isfinished = true;
    ui->widget->installEventFilter(this);

//    //字体颜色-white
//    QPalette pel;
//    pel.setColor(QPalette::ButtonText,Qt::white);
//    ui->ReturnButton->setPalette(pel);
//    ui->ReturnButton->setStyleSheet("background-image: url(:/new/picture/mission_interface/Rectangle 252.png);border:1px solid #5DBFC4;border-radius:5px; ");//消除边框
    //"返回"背景翠绿色+字体白色+线宽1px翠绿色+四边圆角
    ui->ReturnButton->setStyleSheet("QPushButton{background-color: rgb(0, 150, 157);color:white;border:1px solid #00969D;border-radius:5px;  }");//线条

    //滚动按建设计
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

void Today_mission::paint(){
    QPainter painter(ui->widget);
    int x = 45,y=15;
    angle += 8;                             //转动速度
    if(angle == 360) angle=0;

    painter.translate(x+pix.width()/2,y+pix.height()/2);        //旋转中心
    painter.rotate(angle);
    painter.translate(-x-pix.width()/2,-y-pix.height()/2);       //恢复中心点
    painter.drawPixmap(x,y,pix.width(),pix.height(),pix);      //画图
}

bool Today_mission::eventFilter(QObject *watched, QEvent *event){
    if(event->type() == QEvent::Paint && watched == ui->widget){
        paint();
    }
    return QWidget::eventFilter(watched,event);
}

void Today_mission::ScrollBarValchange(int val){
    ui->scrollArea->verticalScrollBar()->setValue(verticalScrollBar->value());
}


void Today_mission::MissionList_recv(QString str){
    isfinished = false;
    timer->stop();
    delete timer;
    ui->widget->hide();
    QJson::Parser parser;
    bool ok;
    QVariantMap Missionmap=parser.parse(str.toUtf8(),&ok).toMap();

    //qDebug() << "dta" << Missionmap;
    if(!ok){
        qDebug() << "********get data failed********";
        return;
    }else{
        ui->ReturnButton->setEnabled(false);
        //qDebug() << "emit2";
        int Totalnum = Missionmap["total"].toInt();
        int btnNum = Totalnum;
        int contextHeight = btnNum*48 + (btnNum+1)*8 +57;
        ui->scrollAreaWidgetContents->setMinimumHeight(contextHeight);//必须这样才能显示滑块

        QStringList strlist1 = str.split("]");                                                              //获取data中  [  ]  的数据
        QStringList strlist2 = str.split("[");
        str.resize(strlist1[0].size());
        str.remove(0,strlist2[0].length()+1);
        //qDebug()<<str;

        QStringList strlist = str.split("},");                                                              //按{ }分成字符串列表
        for(int i=0; i<strlist.length()-1; i++){
                strlist[i] += '}';
        }
        QTime *time = new QTime();
        QString CurrentTime = time->currentTime().toString("hh:mm");
        //qDebug() << CurrentTime;


        signalMapper = new QSignalMapper(this);                                             //按钮批量映射
        connect(signalMapper,SIGNAL(mapped(QString)),this,SLOT(list_btn_clicked(QString)));
        int btnsum = 0;
        for(int i=0; i<Totalnum;++i){
            QVariantMap strmap=parser.parse(strlist[i].toUtf8(),&ok).toMap();
            QString Status = strmap["status"].toString();                                       //状态字符串处理
            Status.remove(0,7);
            //qDebug() << Status;
            if(Status == "PLANNING" || Status == "DISPATCHING_ERROR"){                                    //只显示待处理的任务，这里暂时显示ABORT
                QString Start_time = strmap["start_time"].toString();
                QString Start_time_show = Start_time;
                QStringList start1 = Start_time_show.split(".");
                QStringList start2 = Start_time_show.split("T");
                Start_time_show.resize(start1[0].size()-3);                                             //开始时间字符串处理
                Start_time_show.remove(0,start2[0].length()+1);
                if(Status == "PLANNING"  && CurrentTime  >= Start_time_show){
                    qDebug() << "task time out";
                    continue;
                }
                QString MissionID = strmap["id"].toString();
                QString End_time = strmap["end_time"].toString();
                QString End_time_show = End_time;
                QStringList End1 = End_time_show.split(".");
                QStringList End2 = End_time_show.split("T");
                End_time_show.resize(End1[0].size()-3);                                                 //结束时间字符串处理
                End_time_show.remove(0,End2[0].length()+1);
                //qDebug() << End_time_show;

                QString Map_name = strmap["map_name"].toString();
                QString Mission_name = strmap["name"].toString();
                list_btn[btnsum] = new QPushButton(ui->scrollAreaWidgetContents);
                timelabel[btnsum] = new QLabel(ui->scrollAreaWidgetContents);
                statusbutton[btnsum] = new QPushButton(ui->scrollAreaWidgetContents);
                int y = 57*(btnsum+1);
                list_btn[btnsum]->setGeometry(3,y,438,48);
                list_btn[btnsum]->setStyleSheet("QPushButton{font-family: 'Source Han Sans CN';font-size: 18px;background-color:transparent;text-align:center;border:1px solid #5DBFC4;border-radius:5px;  } ");
                list_btn[btnsum]->setText(QString("%1").arg(Mission_name));
                connect(list_btn[btnsum],SIGNAL(clicked(bool)),signalMapper,SLOT(map()));
                connect(statusbutton[btnsum],SIGNAL(clicked(bool)),signalMapper,SLOT(map()));
                signalMapper->setMapping(list_btn[btnsum],MissionID);
                signalMapper->setMapping(statusbutton[btnsum],MissionID);
                //list_btn[btnsum]->show();

                timelabel[btnsum]->setGeometry(3+14,y+7,100,33);
                //timelabel[btnsum]->setStyleSheet("font-family: 'Source Han Sans CN';font-size: 18px;background-color:transparent;text-align:left;");
                timelabel[btnsum]->setStyleSheet("font-family: 'Source Han Sans CN';font-size: 18px;background-color:transparent;text-align:left; ");
                QFont font = timelabel[btnsum]->font();
                QString text = Start_time_show+"-"+End_time_show;
                int textWidth = timelabel[btnsum]->fontMetrics().width(text);
                int width = timelabel[btnsum]->width();
                int count = text.count()-1;
                qreal space = qreal(width-textWidth)/count;
                font.setLetterSpacing(QFont::AbsoluteSpacing,space);
                timelabel[btnsum]->setFont(font);
                timelabel[btnsum]->setText(text);
                timelabel[btnsum]->show();

                //connect(statusbutton[btnsum],SIGNAL(clicked(bool)),list_btn[btnsum],SLOT(click()));
                statusbutton[btnsum]->setGeometry(3+350,y+4,77,40);
                statusbutton[btnsum]->setStyleSheet("font-family: 'Source Han Sans CN';font-size: 18px;background-color:transparent;text-align:right; ");
                if(Status == "DONE"){
                    statusbutton[btnsum]->setStyleSheet("font-family: 'Source Han Sans CN';font-size: 18px;background-color:transparent;text-align:right;color : #05BE4F;");
                    statusbutton[btnsum]->setEnabled(false);
                    statusbutton[btnsum]->setText("已完成");
                }else if(Status == "ABORTED"){
                    statusbutton[btnsum]->setStyleSheet("font-family: 'Source Han Sans CN';font-size: 18px;background-color:transparent;text-align:right;color : #DD960C;");
                    statusbutton[btnsum]->setEnabled(false);
                    statusbutton[btnsum]->setText("任务中断");
                }else{
                    statusbutton[btnsum]->setEnabled(true);
                    statusbutton[btnsum]->setStyleSheet("background-image: url(:/new/picture/mission_interface/Rectangle 254.png);font-family: 'Source Han Sans CN';font-size: 18px;text-align:right;color : rgb(255,255,255);border-radius:5px;");
                    statusbutton[btnsum]->setText("一键补扫");
                }
                statusbutton[btnsum]->show();
                list_btn[btnsum]->show();
                btnsum++;


            }else{
                continue;
            }
            QCoreApplication::processEvents(QEventLoop::AllEvents,10);
        }
        int btnrealHeight = btnsum*48 + (btnsum+1)*8 +57;                                //重新设置滑动框的大小
        ui->scrollAreaWidgetContents->setMinimumHeight(btnrealHeight);//必须这样才能显示滑块
        verticalScrollBar->setMaximum(btnsum*32);
        //qDebug() << "scroll" << ui->scrollArea->verticalScrollBar()->maximum();
        if(btnrealHeight > ui->scrollArea->height()){
            scrollbarsheet.append(QString("QScrollBar::handle:vertical{min-height:%1px}").arg(20000/contextHeight));
            verticalScrollBar->setStyleSheet(scrollbarsheet);         //sub-page,add-page
            verticalScrollBar->show();
        }

    }

    isfinished = true;
    //qDebug() << "today_finish";
    ui->ReturnButton->setEnabled(true);
}

void Today_mission::list_btn_clicked(QString MissionID)
{
    qDebug() << "device id:"<< MissionID;
    confirmmission_window = new ConfirmMission_Dialog(MissionID,this);
    confirmmission_window->show();
    //confirmmission_window = nullptr;
}
void Today_mission::on_ReturnButton_clicked()
{
    //qDebug() << "today_finish" << isfinished;
    this->close();
    emit close_mission_dialog();
    //qDebug() << "xxxx";
}

Today_mission::~Today_mission()
{
    qDebug() << "********Today_mission close********";
    delete ui;
}
