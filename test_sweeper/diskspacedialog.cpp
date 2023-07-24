#include "diskspacedialog.h"
#include "ui_diskspacedialog.h"

#include<QDebug>
#include "qdatetime.h"

DiskSpaceDialog::DiskSpaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiskSpaceDialog)
{
    qDebug() << "********DiskSpaceDialog open********";
     setWindowFlags(Qt::FramelessWindowHint);//隐藏顶部栏
     setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
    ui->setupUi(this);

    set_http = new setting_http(disk,get,"",this);
    connect(set_http,SIGNAL(Set_data(QString)),this,SLOT(SetdataSlot(QString)));
    set_http->start();

    Allmem = 938;

    //"返回"字体颜色-white
    QPalette pel;
    pel.setColor(QPalette::ButtonText,Qt::white);
    //ui->ReturnButton->setStyleSheet("background-color: rgb(0, 150, 152);");
    ui->ReturnButton->setPalette(pel);

    //QPixmap pixmap = QPixmap(":/new/picture/setting_interface/disk.png").scaled(this->size());
    QPixmap pixmap = QPixmap(":/new/picture/mainpicture1.png").scaled(this->size());
    QPalette Palette;
    Palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(Palette);

    ui->disk_label->setAlignment(Qt::AlignCenter);
    ui->disk_label->setStyleSheet("QLabel{font-family: 'Source Han Sans CN';font-size: 22px;}");
    ui->disk_label->setText("磁盘空间");

    ui->progressBar->setStyleSheet("QProgressBar::chunk{background-color:rgb(227,131,18);}");

    //内存显示
    ui->mem_label->setAlignment(Qt::AlignRight);
    ui->mem_label->setStyleSheet("QLabel{font-family: 'Source Han Sans CN';font-size: 22px;color:rgb(229,131,16); }");


    pel.setColor(QPalette::ButtonText,QColor(0,150,152));
    ui->pushButton->setPalette(pel);
    ui->pushButton->setStyleSheet("background-color:rgba(0,0,0,0);border:1px solid #5DBFC4;border-radius:5px;font-family: 'Source Han Sans CN';font-size: 26px;");
    ui->pushButton->setText("清空缓存");


}

DiskSpaceDialog::~DiskSpaceDialog()
{

    qDebug() << "********DiskSpaceDialog close********";
    delete ui;
}

void DiskSpaceDialog::on_ReturnButton_clicked()
{
      this->close();
//    //this->parentWidget()->show();//显示主窗口-something error,becase parentWidget is const
//    //用于延迟隐藏或删除，防止闪绿屏
//    QTime dieTime =  QTime::currentTime().addMSecs(100);
//    while(QTime::currentTime() < dieTime)
//    QCoreApplication::processEvents(QEventLoop::AllEvents,100);

//    delete ui;    //删除子窗口
}

void DiskSpaceDialog::SetdataSlot(QString str){
    QJson::Parser parser;
    bool ok;
    QVariantMap Datamap=parser.parse(str.toUtf8(),&ok).toMap();
    if(!ok){
        qDebug() << "********get setting data failed********";
        return;
    }else{
        //set_http->stop();
        set_http->exit();
        set_http->wait();
        int Freemem = Datamap["free_disk_space"].toInt();
        Usedmem = Allmem - Freemem;

        //设置进度条
        int percent = Usedmem*100/(Allmem);
        ui->progressBar->setValue(percent);

        //内存显示
        QString memtext = QString::number(Usedmem) + "G/" + QString::number(Allmem) + "G";
        ui->mem_label->setText(memtext);

    }
}

void DiskSpaceDialog::on_pushButton_clicked()
{
    cleardiskmem = new Cleardiskmem_dialog(this);
    connect(cleardiskmem,SIGNAL(cleardiskmem()),this,SLOT(clear()));
    cleardiskmem->show();
}

void DiskSpaceDialog::clear(){

    ui->progressBar->setValue(0);
    Usedmem = 0;
    QString memtext = QString::number(Usedmem) + "G/" + QString::number(Allmem) + "G";
    ui->mem_label->setText(memtext);
}

