#include "networkdialog.h"
#include "ui_networkdialog.h"

#include "qdatetime.h"
#include<QDebug>

NetWorkDialog::NetWorkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetWorkDialog)
{
    qDebug() << "********NetWorkDialog open********";
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);//隐藏顶部栏
     setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
    ui->setupUi(this);

    //"返回"字体颜色-white
    QPalette pel;
//    pel.setColor(QPalette::ButtonText,Qt::white);
//    //ui->ReturnButton->setStyleSheet("background-color: rgb(0, 150, 152);");
//    ui->ReturnButton->setPalette(pel);
    //"返回"背景翠绿色+字体白色+线宽1px翠绿色+四边圆角
    ui->ReturnButton->setStyleSheet("QPushButton{background-color: rgb(0, 150, 157);color:white;border:1px solid #00969D;border-radius:5px;  }");//线条

    //QPixmap pixmap = QPixmap(":/new/picture/setting_interface/WIFI.png").scaled(this->size());
    QPixmap pixmap = QPixmap(":/new/picture/mainpicture1.png").scaled(this->size());
    QPalette Palette;
    Palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(Palette);

    //滚动按建设计
    ui->scrollArea->setStyleSheet("QScrollArea{background-color: rgba(0, 0, 0, 0);border:0px solid #838486;  }");//透明+取消线条
    ui->scrollAreaWidgetContents->setStyleSheet("background-color: rgba(0, 0, 0, 0);");//透明
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scrollbarsheet = "QScrollBar:vertical { margin:3px 0 3px 0;  width:1px;background-image: url(:/new/picture/mission_interface/mission_bakg.png); } "
                     "QScrollBar::handle:vertical { background-color:#005357; margin:0 0px 0 0px; border-radius:9px; }"
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

    set_http = new setting_http(network,get,"",this);
    connect(set_http,SIGNAL(Set_data(QString)),this,SLOT(network_recv(QString)));
    set_http->start();

    //keyboard = new InputMethod();
    keyboard = new KeyBoard();
    keyboard->move(0,10);
    keyboard->hide();
    connect(keyboard,SIGNAL(setvalue(QString)),this,SLOT(sendpasswd(QString)));
}

void NetWorkDialog::ScrollBarValchange(int val){
    ui->scrollArea->verticalScrollBar()->setValue(verticalScrollBar->value());
}

NetWorkDialog::~NetWorkDialog()
{
    qDebug() << "********NetWorkDialog close********";
    delete ui;
}

void NetWorkDialog::on_ReturnButton_clicked()
{
    // this->close();
//    //this->parentWidget()->show();//显示主窗口-something error,becase parentWidget is const
//    //用于延迟隐藏或删除，防止闪绿屏
//    QTime dieTime =  QTime::currentTime().addMSecs(100);
//    while(QTime::currentTime() < dieTime)
//    QCoreApplication::processEvents(QEventLoop::AllEvents,100);

//    delete ui;    //删除子窗口
}

void NetWorkDialog::network_recv(QString str){
    QJson::Parser parser;
    bool ok;
    QVariantMap Networkmap=parser.parse(str.toUtf8(),&ok).toMap();
    //qDebug() << "dta" << Missionmap;
    if(!ok){
        qDebug() << "********network_recv failed********";
        set_http->exit();
        set_http->wait();
        return;
    }else{
        set_http->exit();
        set_http->wait();
        QString Network_enable = Networkmap["network_enable"].toString();
        if(Network_enable != "true"){
            return;
        }
        int btnNum = 0;
        signalMapper = new QSignalMapper(this);                                             //按钮批量映射
        connect(signalMapper,SIGNAL(mapped(QString)),this,SLOT(list_btn_clicked(QString)));
          foreach(QVariant wifi,Networkmap["wifi_list"].toList()){
              qDebug() << "\t-" << wifi.toString();
              QLabel *btnLeft = new QLabel();
              btnLeft->setStyleSheet("font-family: 'Source Han Sans CN';font-size:22px;background-image: url(:/new/picture/setting_interface/Vector1.png);background-position:left;background-repeat:no-repeat;padding-left:42px;");
              btnLeft->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
              btnLeft->setFixedWidth(270);
              QLabel *btnRight = new QLabel();
              btnRight->setStyleSheet("font-family: 'Source Han Sans CN';font-size:22px;background-image: url(:/new/picture/setting_interface/Polygon 1.png);background-position:right;background-repeat:no-repeat;padding-right:10px;");
              btnRight->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
              QPushButton *list_btn = new QPushButton(ui->scrollAreaWidgetContents);
              connect(list_btn,SIGNAL(clicked(bool)),signalMapper,SLOT(map()));
              signalMapper->setMapping(list_btn,wifi.toString());
              //list_btn->setStyleSheet("QPushButton{font-family: 'Source Han Sans CN';font-size: 18px;background-color:transparent;text-align:justify;border:1px solid #5DBFC4;border-radius:5px;  } ");
              list_btn->setStyleSheet("QPushButton{font-family: 'Source Han Sans CN';background-color:transparent;border:1px solid #5DBFC4;border-radius:5px;  } ");
              int y = 56*btnNum;
              list_btn->setGeometry(11,y,435,48);
              btnLeft->setText(wifi.toString());
              btnRight->setText("连接");
              btnRight->setStyleSheet("font-family: 'Source Han Sans CN';font-size:22px;background-image: url(:/new/picture/setting_interface/Polygon 1.png);background-position:right;background-repeat:no-repeat;padding-right:10px;image: url(:/new/picture/setting_interface/13DLOCK 1.png);");
              //btnLeft->setText(QString("<html><head/><body><p><img src=':/new/picture/setting_interface/Vector1.png'/>%1</span></p></body></html>").arg("  "+wifi.toString()));
              //btnRight->setText(QString("<html><head/><body><p><img src=':/new/picture/setting_interface/13DLOCK 1.png'/>连接  </span><img src=':/new/picture/setting_interface/Polygon 1.png'/></p></body></html>"));
              QHBoxLayout *Layout = new QHBoxLayout();
              Layout->addWidget(btnLeft);
              Layout->addWidget(btnRight);
              list_btn->setLayout(Layout);
              btnNum++;
              list_btn->show();
          }
          int contextHeight = btnNum*48 + (btnNum+1)*8;
          ui->scrollAreaWidgetContents->setMinimumHeight(contextHeight);//必须这样才能显示滑块
          //verticalScrollBar->setMaximum(ui->scrollArea->verticalScrollBar()->maximum());
          verticalScrollBar->setMaximum(btnNum*32);
          //qDebug() << "scroll" << ui->scrollArea->verticalScrollBar()->maximum();
          if(contextHeight > ui->scrollArea->height()){
              scrollbarsheet.append(QString("QScrollBar::handle:vertical{min-height:%1px}").arg(20000/contextHeight));
              verticalScrollBar->setStyleSheet(scrollbarsheet);         //sub-page,add-page
              verticalScrollBar->show();
          }

    }
}

void NetWorkDialog::list_btn_clicked(QString name)
{
    qDebug() << "WIFI name:"<< name;
    Wifiname = name;

    keyboard->show();
}

void NetWorkDialog::sendpasswd(QString passwd)
{
    qDebug() << "WIFI name:"<< Wifiname;
    qDebug() << "WIFI passwd:"<< passwd;
    QString str = QString("{\"ssid\":\"%1\",\"password\":\"%2\"}").arg(Wifiname).arg(passwd);
    qDebug() << str;
    set_http = new setting_http(network,post,str,this);
    connect(set_http,SIGNAL(Set_data(QString)),this,SLOT(NetworkSlot(QString)));
    set_http->start();
}
void NetWorkDialog::NetworkSlot(QString str)
{
    QJson::Parser parser;
    bool ok;
    QVariantMap Datamap=parser.parse(str.toUtf8(),&ok).toMap();
    if(!ok){
        qDebug() << "********Network post get data failed********";
        //set_http->stop();
        set_http->exit();
        set_http->wait();
        return;
    }else{
        //set_http->stop();
        set_http->exit();
        set_http->wait();
        QString status = Datamap["status"].toString();
        if(status != "success"){
            qDebug() << "********Network post failed********";
            return;
        }else{
               qDebug() << "********Network post success********";
        }
    }
}
