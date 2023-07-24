#include "cleardiskmem_dialog.h"
#include "ui_cleardiskmem_dialog.h"

#include <QDebug>

Cleardiskmem_dialog::Cleardiskmem_dialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cleardiskmem_dialog)
{
     qDebug() << "********Cleardiskmem_dialog open********";
     setWindowFlags(Qt::FramelessWindowHint);//隐藏顶部栏
     setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
     ui->setupUi(this);

     ui->cleardisk_label->setAlignment(Qt::AlignCenter);
     ui->cleardisk_label->setStyleSheet("QLabel{font-family: 'Ubuntu';font-size: 24px;}");
     ui->cleardisk_label->setText("确认清理缓存吗?");

     QPalette pel;
     pel.setColor(QPalette::ButtonText,Qt::white);
     ui->confirm_button->setPalette(pel);
     ui->confirm_button->setStyleSheet("background-image: url(:/new/picture/mission_interface/Rectangle 255.png);border:1px solid #5DBFC4;border-radius:5px;font-family: 'Ubuntu';font-size: 24px;");
     ui->confirm_button->setText("确认");
     pel.setColor(QPalette::ButtonText,QColor(0,150,152));
     ui->cancel_button->setPalette(pel);
     ui->cancel_button->setStyleSheet("background-color:rgba(0,0,0,0);border:1px solid #5DBFC4;border-radius:5px;font-family: 'Ubuntu';font-size: 24px;");
     ui->cancel_button->setText("取消");

}

Cleardiskmem_dialog::~Cleardiskmem_dialog()
{
    qDebug() << "********Cleardiskmem_dialog close********";
    delete ui;
}

void Cleardiskmem_dialog::on_confirm_button_clicked()
{
    set_http = new setting_http(disk,deleteResource,"",this);
    connect(set_http,SIGNAL(Set_data(QString)),this,SLOT(SetdataSlot(QString)));
    set_http->start();
//    emit cleardiskmem();
//    this->close();
}

void Cleardiskmem_dialog::on_cancel_button_clicked()
{
    this->close();
}

void Cleardiskmem_dialog::SetdataSlot(QString str){
    QJson::Parser parser;
    bool ok;
    QVariantMap Datamap=parser.parse(str.toUtf8(),&ok).toMap();
    if(!ok){
        qDebug() << "********Cleardiskmem_dialog get data failed********";
        return;
    }else{
        //set_http->stop();
        set_http->exit();
        set_http->wait();
        QString status = Datamap["status"].toString();
//        if(status != "success"){
//            qDebug() << "********Cleardiskmem_dialog clear disk  failed********";
//            this->close();
//        }else{
            emit cleardiskmem();
            this->close();
//        }

    }

}
