#include "volumedialog.h"
#include "ui_volumedialog.h"

#include "qdatetime.h"
#include<QDebug>

VolumeDialog::VolumeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VolumeDialog)
{
    qDebug() << "********VolumeDialog open********";
     setWindowFlags(Qt::FramelessWindowHint);//隐藏顶部栏
     setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
    ui->setupUi(this);

    set_http_get = new setting_http(volume,get,"",this);
    connect(set_http_get,SIGNAL(Set_data(QString)),this,SLOT(VolInit(QString)));
    set_http_get->start();

    //"返回"字体颜色-white
    QPalette pel;
    pel.setColor(QPalette::ButtonText,Qt::white);
    //ui->ReturnButton->setStyleSheet("background-color: rgb(0, 150, 152);");
    ui->ReturnButton->setPalette(pel);

    QPixmap pixmap = QPixmap(":/new/picture/mainpicture1.png").scaled(this->size());
    //QPixmap pixmap = QPixmap(":/new/picture/setting_interface/volume.png").scaled(this->size());
    QPalette Palette;
    Palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(Palette);

    Volumeval = 0;
    ui->textlabel->setAlignment(Qt::AlignCenter);
    ui->textlabel->setStyleSheet("QLabel{font-family: 'Ubuntu';font-size: 64px;color:white; border-radius:5px;}");
//    ui->textlabel->setText(QString::number(Volumeval));
//    ui->Volume_Bar->setValue(Volumeval);

//ui->SettingButton->setStyleSheet("QPushButton{font: bold;font-size: 32px;color:white;background-image: url(:/new/keypad/picture/Rectangle 225.png);background-color: rgba(255, 255, 255, 0);border:1px solid #0F9BA3; border-radius:5px; }");
}

VolumeDialog::~VolumeDialog()
{
    delete ui;
    qDebug() << "********VolumeDialog close********";
}

void VolumeDialog::VolInit(QString str){
    QJson::Parser parser;
    bool ok;
    QVariantMap Datamap=parser.parse(str.toUtf8(),&ok).toMap();
    if(!ok){
        qDebug() << "********VolumeDialog get data failed********";
        //set_http_get->stop();
        set_http_get->exit();
        set_http_get->wait();
        return;
    }else{
        //set_http_get->stop();
        set_http_get->exit();
        set_http_get->wait();
        QString status = Datamap["status"].toString();
        int volval = Datamap["volume"].toInt();
//        if(status != "success"){
//            qDebug() << "********VolumeDialog init failed********";
//            return;
//        }else{
            Volumeval = volval;
            ui->textlabel->setText(QString::number(Volumeval));
            ui->Volume_Bar->setValue(Volumeval);
//        }
    }
}


void VolumeDialog::on_ReturnButton_clicked()
{
    //this->close();
}

void VolumeDialog::on_Btn_high_clicked()
{
    if(Volumeval < 100){
        Volumeval += 5;
        QString str = QString("{\"volume\":%1}").arg(Volumeval);
        set_http = new setting_http(volume,put,str,this);
        connect(set_http,SIGNAL(Set_data(QString)),this,SLOT(VolSlot(QString)));
        set_http->start();
//        ui->textlabel->setText(QString::number(Volumeval));
//        ui->Volume_Bar->setValue(Volumeval);
    }
}

void VolumeDialog::on_Btn_low_clicked()
{
    if(Volumeval >0){
        Volumeval -= 5;
        QString str = QString("{\"volume\":%1}").arg(Volumeval);
        set_http = new setting_http(volume,put,str,this);
        connect(set_http,SIGNAL(Set_data(QString)),this,SLOT(VolSlot(QString)));
        set_http->start();
//        ui->textlabel->setText(QString::number(Volumeval));
//        ui->Volume_Bar->setValue(Volumeval);
    }
}

void VolumeDialog::VolSlot(QString str)
{
    QJson::Parser parser;
    bool ok;
    QVariantMap Datamap=parser.parse(str.toUtf8(),&ok).toMap();
    if(!ok){
        qDebug() << "********VolumeDialog get data failed********";
        //set_http->stop();
        set_http->exit();
        set_http->wait();
        return;
    }else{
        //set_http->stop();
        set_http->exit();
        set_http->wait();
        QString status = Datamap["status"].toString();
//        if(status != "success"){
//            qDebug() << "********VolumeDialog set volume failed********";
//            return;
//        }else{
            ui->textlabel->setText(QString::number(Volumeval));
            ui->Volume_Bar->setValue(Volumeval);
//        }
    }
}
