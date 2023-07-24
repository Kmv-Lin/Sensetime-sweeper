#include "versiondialog.h"
#include "ui_versiondialog.h"

#include "qdatetime.h"
#include<QDebug>

VersionDialog::VersionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionDialog)
{
    qDebug() << "********VersionDialog open********";
     setWindowFlags(Qt::FramelessWindowHint);//隐藏顶部栏
     setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
    ui->setupUi(this);

    //"返回"字体颜色-white
    QPalette pel;
    pel.setColor(QPalette::ButtonText,Qt::white);
    //ui->ReturnButton->setStyleSheet("background-color: rgb(0, 150, 152);");
    ui->ReturnButton->setPalette(pel);

    //QPixmap pixmap = QPixmap(":/new/picture/setting_interface/verson.png").scaled(this->size());
    QPixmap pixmap = QPixmap(":/new/picture/mainpicture1.png").scaled(this->size());
    QPalette Palette;
    Palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(Palette);

    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setStyleSheet("QLabel{font-family: 'Ubuntu';font-size: 24px;}");
    ui->label->setText("版 本");

    pel.setColor(QPalette::ButtonText,Qt::black);
    ui->Btn_version->setPalette(pel);
    ui->Btn_version->setStyleSheet("background-color:rgba(0,0,0,0);border:1px solid #5DBFC4;border-radius:5px;font-family: 'Source Han Sans CN';font-size: 22px;text-align:left");
    ui->Btn_version->setText("版本号");

    pel.setColor(QPalette::ButtonText,Qt::black);
    ui->Btn_Wareversion->setPalette(pel);
    ui->Btn_Wareversion->setStyleSheet("background-color:rgba(0,0,0,0);border:1px solid #5DBFC4;border-radius:5px;font-family: 'Source Han Sans CN';font-size: 22px;text-align:left");
    ui->Btn_Wareversion->setText("硬件版本号");

    QString version = "1.2.46.7";
    QString Hardware_version = "1.2.46.7";
    ui->btn_label1->setAlignment(Qt::AlignCenter);
    ui->btn_label1->setStyleSheet("QLabel{font-family: 'Source Han Sans CN';font-size: 22px;}");
    ui->btn_label1->setText(version);
    ui->btn_label2->setAlignment(Qt::AlignCenter);
    ui->btn_label2->setStyleSheet("QLabel{font-family: 'Source Han Sans CN';font-size: 22px;}");
    ui->btn_label2->setText(Hardware_version);
}

VersionDialog::~VersionDialog()
{
    qDebug() << "********VersionDialog close********";
    delete ui;
}

void VersionDialog::on_ReturnButton_clicked()
{
//    this->hide();
//    //this->parentWidget()->show();//显示主窗口-something error,becase parentWidget is const
//    //用于延迟隐藏或删除，防止闪绿屏
//    QTime dieTime =  QTime::currentTime().addMSecs(100);
//    while(QTime::currentTime() < dieTime)
//    QCoreApplication::processEvents(QEventLoop::AllEvents,100);

//    delete ui;    //删除子窗口
}
