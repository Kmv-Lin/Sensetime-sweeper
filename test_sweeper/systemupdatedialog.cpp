#include "systemupdatedialog.h"
#include "ui_systemupdatedialog.h"

#include "qdatetime.h"
#include<QDebug>

SystemUpdateDialog::SystemUpdateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SystemUpdateDialog)
{
    qDebug() << "********SystemUpdateDialog open********";
     setWindowFlags(Qt::FramelessWindowHint);//隐藏顶部栏
     setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
    ui->setupUi(this);



    //"返回"字体颜色-white
    QPalette pel;
    pel.setColor(QPalette::ButtonText,Qt::white);
    //ui->ReturnButton->setStyleSheet("background-color: rgb(0, 150, 152);");
    ui->ReturnButton->setPalette(pel);

    QPixmap pixmap = QPixmap(":/new/picture/mainpicture1.png").scaled(this->size());
    //QPixmap pixmap = QPixmap(":/new/picture/setting_interface/system_updata.png").scaled(this->size());
    QPalette Palette;
    Palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(Palette);

    ui->textlabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->textlabel->setStyleSheet("QLabel{font-family: 'Source Han Sans CN';font-size: 26px;border-radius:5px;}");
    ui->updating_label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->updating_label->setStyleSheet("QLabel{font-family: 'Source Han Sans CN';font-size: 26px;border-radius:5px;}");

    ui->textlabel->setText("当前版本：");
    UpdateProgress= "1.2.47.6";
    ui->updating_label->setText(UpdateProgress);

    pel.setColor(QPalette::ButtonText,Qt::white);
    ui->pushButton->setPalette(pel);
    ui->pushButton->setStyleSheet("background-image: url(:/new/picture/setting_interface/Rectangle 251.png);border:1px solid #5DBFC4;border-radius:5px;font-family: 'Source Han Sans CN';font-size: 26px;");
    ui->pushButton->setText("点击更新");

}

SystemUpdateDialog::~SystemUpdateDialog()
{
    delete ui;
    qDebug() << "********SystemUpdateDialog close********";
}

void SystemUpdateDialog::on_ReturnButton_clicked()
{
//    this->hide();
//    //this->parentWidget()->show();//显示主窗口-something error,becase parentWidget is const
//    //用于延迟隐藏或删除，防止闪绿屏
//    QTime dieTime =  QTime::currentTime().addMSecs(100);
//    while(QTime::currentTime() < dieTime)
//    QCoreApplication::processEvents(QEventLoop::AllEvents,100);

//    delete ui;    //删除子窗口
}

void SystemUpdateDialog::on_pushButton_clicked()
{
    if(ui->pushButton->text() == "点击更新"){
        ui->pushButton->setText("取消更新");

        SetThread = new setting_thread(this);
        connect(SetThread,SIGNAL(updating_progress(int,QString)),this,SLOT(Updating_progress(int,QString)));
        SetThread->start();

        ui->updating_label->setAlignment(Qt::AlignCenter);
        ui->updating_label->setStyleSheet("QLabel{font-family: 'Source Han Sans CN';font-size: 26px;border-radius:5px;color : rgb(34,198,101);}");
        ui->textlabel->setText("更新中...  ");
    }else if(ui->pushButton->text() == "取消更新"){
        SetThread->stop();
        ui->textlabel->setText("当前版本：");
        ui->pushButton->setText("点击更新");
    }else if(ui->pushButton->text() == "确定"){
        ui->updating_label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->updating_label->setStyleSheet("QLabel{font-family: 'Source Han Sans CN';font-size: 26px;border-radius:5px;}");
        UpdateProgress = "2.2.47.6";
        ui->textlabel->setText("当前版本：");
        ui->updating_label->setText(UpdateProgress);

        ui->pushButton->setStyleSheet("background-image: url(:/new/picture/setting_interface/Rectangle 253.png);border:1px solid #5DBFC4;border-radius:5px;font-family: 'Source Han Sans CN';font-size: 26px;");
        ui->pushButton->setText("暂无更新");
        ui->pushButton->setEnabled(false);
    }


}

void SystemUpdateDialog::Updating_progress(int progress,QString msg){
    QString Updating = QString::number(progress);
    if(progress ==100){
        ui->textlabel->setText("更新完成  ");
        SetThread->stop();
        SetThread->deleteLater();
        ui->pushButton->setText("确定");
    }
    if(msg != "stopped"){
        ui->updating_label->setText(Updating+"%");
    }else{
        ui->updating_label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->updating_label->setStyleSheet("QLabel{font-family: 'Source Han Sans CN';font-size: 26px;border-radius:5px;}");
        ui->updating_label->setText(UpdateProgress);
    }
}
