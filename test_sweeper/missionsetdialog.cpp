#include "missionsetdialog.h"
#include "ui_missionsetdialog.h"

#include "qdatetime.h"
#include<QDebug>

MissionSetDialog::MissionSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MissionSetDialog)
{
    qDebug() << "********MissionSetDialog open********";
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);//隐藏顶部栏
     setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
    ui->setupUi(this);

    //"返回"字体颜色-white
    QPalette pel;
    pel.setColor(QPalette::ButtonText,Qt::white);
    //ui->ReturnButton->setStyleSheet("background-color: rgb(0, 150, 152);");
    ui->ReturnButton->setPalette(pel);

    QPixmap pixmap = QPixmap(":/new/picture/setting_interface/mission.png").scaled(this->size());
    QPalette Palette;
    Palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(Palette);
}

MissionSetDialog::~MissionSetDialog()
{
    qDebug() << "********MissionSetDialog close********";
    delete ui;
}

void MissionSetDialog::on_ReturnButton_clicked()
{
//    this->hide();
//    //this->parentWidget()->show();//显示主窗口-something error,becase parentWidget is const
//    //用于延迟隐藏或删除，防止闪绿屏
//    QTime dieTime =  QTime::currentTime().addMSecs(100);
//    while(QTime::currentTime() < dieTime)
//    QCoreApplication::processEvents(QEventLoop::AllEvents,100);

//    delete ui;    //删除子窗口
}
