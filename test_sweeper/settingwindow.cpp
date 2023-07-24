#include "settingwindow.h"
#include "ui_settingwindow.h"

#include "qdatetime.h"
//#include <QGridLayout>
#include<QDebug>

#include "networkdialog.h"
#include "diskspacedialog.h"
#include "volumedialog.h"
#include "systemupdatedialog.h"
#include "missionsetdialog.h"
#include "versiondialog.h"
#include "debugdialog.h"
#include "inputmethod/keyboardmindialog.h"
#include "selfcheck_window.h"



settingWindow::settingWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::settingWindow)
{
    qDebug() << "********setting window open********";
     setWindowFlags(Qt::FramelessWindowHint);//隐藏顶部栏
     setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
    ui->setupUi(this);

    //"返回"背景翠绿色+字体白色+线宽1px翠绿色+四边圆角
    ui->ReturnButton->setStyleSheet("QPushButton{background-color: rgb(0, 150, 157);color:white;border:1px solid #00969D;border-radius:5px;  }");//线条
    //其他按键:透明+左对齐+线宽1px翠绿色+四边圆角
    ui->SetNeworkButton->setStyleSheet("QPushButton{background-color:transparent;text-align:left;border:1px solid #5DBFC4;border-radius:5px;  }");//线条
    ui->SetDiskButton->setStyleSheet("QPushButton{background-color:transparent;text-align:left;border:1px solid #5DBFC4;border-radius:5px;  }");//线条
    ui->SetVolumeButton->setStyleSheet("QPushButton{background-color:transparent;text-align:left;border:1px solid #5DBFC4;border-radius:5px;  }");//线条
    ui->SetMissionButton->setStyleSheet("QPushButton{background-color:transparent;text-align:left;border:1px solid #5DBFC4;border-radius:5px;  }");//线条
    ui->SysUpdateButton->setStyleSheet("QPushButton{background-color:transparent;text-align:left;border:1px solid #5DBFC4;border-radius:5px;  }");//线条
    ui->SetVersionButton->setStyleSheet("QPushButton{background-color:transparent;text-align:left;border:1px solid #5DBFC4;border-radius:5px;  }");//线条
    ui->SetDebugButton->setStyleSheet("QPushButton{background-color:transparent;text-align:left;border:1px solid #5DBFC4;border-radius:5px;  }");//线条
    ui->ManualCheckButton->setStyleSheet("QPushButton{background-color:transparent;text-align:left;border:1px solid #5DBFC4;border-radius:5px;  }");//线条

    //背景
    QPixmap pixmap = QPixmap(":/new/picture/mainpicture1.png").scaled(this->size());
    QPalette Palette;
    Palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(Palette);

    //滚动按建设计
    ui->scrollArea->setStyleSheet("QScrollArea{background-color: rgba(0, 0, 0, 0);border:0px solid #838486;  }");//透明+取消线条
    ui->scrollAreaWidgetContents->setStyleSheet("background-color: rgba(0, 0, 0, 0);");//透明
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scrollbarsheet = "QScrollBar:vertical { margin:3px 0 3px 0;  width:1px;background-image: url(:/new/picture/mission_interface/mission_bakg.png); } "
                     "QScrollBar::handle:vertical { background-color:#005357; margin:0 0px 0 0px; border-radius:9px; min-height:50px}"
                     "QScrollBar::sub-line:vertical,QScrollBar::add-line:vertical{subcontrol-origin: margin;height:1px;"
                     "border-image: url(:/new/picture/mission_interface/mission_bakg.png); }"
                     "QScrollBar::up-arrow:vertical {border-image: url(:/new/picture/mission_interface/mission_bakg.png);  } "
                     "QScrollBar::down-arrow:vertical { border-image: url(:/new/picture/mission_interface/mission_bakg.png); }"
                    "QScrollBar::sub-page:vertical,QScrollBar::add-page:vertical{background-image: url(:/new/picture/mission_interface/mission_bakg.png); }";
    int btnNum = 9;
    int contextHeight = btnNum*48 + (btnNum+1)*8;
    ui->scrollAreaWidgetContents->setMinimumHeight(contextHeight);//必须这样才能显示滑块

    verticalScrollBar = new QScrollBar(this);
    verticalScrollBar->setGeometry(455,57,20,212);
    //qDebug() << "scroll" << ui->scrollAreaWidgetContents->height();
    verticalScrollBar->setMaximum(btnNum*32);
    verticalScrollBar->setStyleSheet(scrollbarsheet);         //sub-page,add-page
    verticalScrollBar->setSingleStep(24);
    connect(verticalScrollBar,SIGNAL(valueChanged(int)),this,SLOT(ScrollBarValchange(int)));

    verticalScrollBar->show();

//    QButtonGroup *btnGroup = new QButtonGroup(this);
//    btnGroup->addButton(ui->ReturnButton);
//    for(int i = 1; i < btnNum; i++)
//    {
//        QPushButton *pBtn = new QPushButton("按钮",ui->scrollArea);
//        pBtn->setGeometry(10,8*i+48*i,435,48);
//        pBtn->setText(QString("按钮%1").arg(i));
//        pBtn->show();
//        btnGroup->addButton(pBtn);
//    }

//    滚动按建设计 2
//    QGridLayout *pLayout = new QGridLayout();//网格布局
//      //QVBoxLayout*pLayout = new QVBoxLayout();//纵向布局-高度可以不一样
//      //QHBoxLayout*pLayout = new QHBoxLayout();//横向布局-长度可以不一样
//    for(int i = 1; i < 10; i++)
//    {
//        QPushButton *pBtn = new QPushButton();
//        pBtn->setText(QString("按钮%1").arg(i));
//        pBtn->setMinimumSize(QSize(435,48));   //width height 105 48
//        pLayout->addWidget(pBtn);//把按钮添加到布局控件中
//    }
//    ui->scrollArea->widget()->setLayout(pLayout);//把布局放置到QScrollArea的内部QWidget中

    //用于延迟隐藏或删除，防止闪绿屏
    QTime dieTime =  QTime::currentTime().addMSecs(100);
    while(QTime::currentTime() < dieTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents,100);

//    keyin_set= new KeyboardMinDialog();
//    keyin_set->show();
}

void settingWindow::ScrollBarValchange(int val){
    ui->scrollArea->verticalScrollBar()->setValue(verticalScrollBar->value());
}


settingWindow::~settingWindow()
{
    delete ui;
    //delete keyin_set;
     qDebug() << "********setting window close********";//确认窗口关闭
}

void settingWindow::on_ReturnButton_clicked()
{
    //以下方式会导致内存泄漏，用曹函数关闭即可.close()!!!!!!!
//    this->hide();
//    //this->parentWidget()->show();//显示主窗口-something error,becase parentWidget is const
//    //用于延迟隐藏或删除，防止闪绿屏
//    QTime dieTime =  QTime::currentTime().addMSecs(100);
//    while(QTime::currentTime() < dieTime)
//    QCoreApplication::processEvents(QEventLoop::AllEvents,100);

//    this->close();
    //delete ui;    //删除子窗口

}

void settingWindow::on_SetNeworkButton_clicked()
{
    NetWorkDialog *win = new NetWorkDialog(this);
    win->setWindowFlags(Qt::Window);                //set as window增加窗口属性，会增加标题和边框！！
    win->showFullScreen();                                      //全屏显示才能消除标题和边框

    //用于延迟隐藏或删除，防止闪绿屏
    QTime dieTime =  QTime::currentTime().addMSecs(100);
    while(QTime::currentTime() < dieTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}

void settingWindow::on_SetVolumeButton_clicked()
{
    VolumeDialog *win = new VolumeDialog(this);
    win->setWindowFlags(Qt::Window);                //set as window增加窗口属性，会增加标题和边框！！
    win->showFullScreen();                                      //全屏显示才能消除标题和边框

    //用于延迟隐藏或删除，防止闪绿屏
    QTime dieTime =  QTime::currentTime().addMSecs(100);
    while(QTime::currentTime() < dieTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}

void settingWindow::on_SetDiskButton_clicked()
{
    DiskSpaceDialog *win = new DiskSpaceDialog(this);
    win->setWindowFlags(Qt::Window);                //set as window增加窗口属性，会增加标题和边框！！
    win->showFullScreen();                                      //全屏显示才能消除标题和边框

    //用于延迟隐藏或删除，防止闪绿屏
    QTime dieTime =  QTime::currentTime().addMSecs(100);
    while(QTime::currentTime() < dieTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}

void settingWindow::on_SetMissionButton_clicked()
{
    MissionSetDialog *win = new MissionSetDialog(this);
    win->setWindowFlags(Qt::Window);                //set as window增加窗口属性，会增加标题和边框！！
    win->showFullScreen();                                      //全屏显示才能消除标题和边框

    //用于延迟隐藏或删除，防止闪绿屏
    QTime dieTime =  QTime::currentTime().addMSecs(100);
    while(QTime::currentTime() < dieTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}

void settingWindow::on_SysUpdateButton_clicked()
{
    SystemUpdateDialog *win = new SystemUpdateDialog(this);
    win->setWindowFlags(Qt::Window);                //set as window增加窗口属性，会增加标题和边框！！
    win->showFullScreen();                                      //全屏显示才能消除标题和边框

    //用于延迟隐藏或删除，防止闪绿屏
    QTime dieTime =  QTime::currentTime().addMSecs(100);
    while(QTime::currentTime() < dieTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}

void settingWindow::on_SetVersionButton_clicked()
{
    VersionDialog *win = new VersionDialog(this);
    win->setWindowFlags(Qt::Window);                //set as window增加窗口属性，会增加标题和边框！！
    win->showFullScreen();                                      //全屏显示才能消除标题和边框

    //用于延迟隐藏或删除，防止闪绿屏
    QTime dieTime =  QTime::currentTime().addMSecs(100);
    while(QTime::currentTime() < dieTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}

void settingWindow::on_SetDebugButton_clicked()
{
    DebugDialog *win = new DebugDialog(this);
    win->setWindowFlags(Qt::Window);                //set as window增加窗口属性，会增加标题和边框！！
    win->showFullScreen();                                     //全屏显示才能消除标题和边框

    //用于延迟隐藏或删除，防止闪绿屏
    QTime dieTime =  QTime::currentTime().addMSecs(100);
    while(QTime::currentTime() < dieTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}

/*需要创建手动自检窗口*/
void settingWindow::on_ManualCheckButton_clicked()
{
    selfcheck_Window *win = new selfcheck_Window(this);
    win->setWindowFlags(Qt::Window);                //set as window增加窗口属性，会增加标题和边框！！
    win->showFullScreen();                                     //全屏显示才能消除标题和边框

    //用于延迟隐藏或删除，防止闪绿屏
    QTime dieTime =  QTime::currentTime().addMSecs(100);
    while(QTime::currentTime() < dieTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}
