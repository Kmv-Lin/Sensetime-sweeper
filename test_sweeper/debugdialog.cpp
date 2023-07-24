#include "debugdialog.h"
#include "ui_debugdialog.h"

#include<QDebug>
#include "qdatetime.h"
#include "thread/get_post_window.h"

DebugDialog::DebugDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugDialog)
{
    qDebug() << "********DebugDialog open********";
     setWindowFlags(Qt::FramelessWindowHint);//隐藏顶部栏
     setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
    ui->setupUi(this);

    //"返回"字体颜色-white
    QPalette pel;
    pel.setColor(QPalette::ButtonText,Qt::white);
    //ui->ReturnButton->setStyleSheet("background-color: rgb(0, 150, 152);");
    ui->ReturnButton->setPalette(pel);

    //QPixmap pixmap = QPixmap(":/new/picture/setting_interface/debug.png").scaled(this->size());
    QPixmap pixmap = QPixmap(":/new/picture/mainpicture1.png").scaled(this->size());
    QPalette Palette;
    Palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(Palette);

    ui->textlabel->setAlignment(Qt::AlignCenter);
    ui->textlabel->setStyleSheet("QLabel{font-family: 'Source Han Sans CN';font-size: 26px;border-radius:5px;}");
    QString DebugTime = "2023-7-10";
    QString textshow = "最近一次调试："+DebugTime;
    ui->textlabel->setText(textshow);


    pel.setColor(QPalette::ButtonText,Qt::white);
    ui->Connect_Test_Button->setPalette(pel);
    ui->Connect_Test_Button->setStyleSheet("background-image: url(:/new/picture/setting_interface/Rectangle 251.png);border:1px solid #5DBFC4;border-radius:5px;font-family: 'Source Han Sans CN';font-size: 26px;");
    ui->Connect_Test_Button->setText("开始调试");
}

DebugDialog::~DebugDialog()
{

    qDebug() << "********DebugDialog close********";
    delete ui;

}

void DebugDialog::on_ReturnButton_clicked()
{
//    this->hide();
//    //this->parentWidget()->show();//显示主窗口-something error,becase parentWidget is const
//    //用于延迟隐藏或删除，防止闪绿屏
//    QTime dieTime =  QTime::currentTime().addMSecs(100);
//    while(QTime::currentTime() < dieTime)
//    QCoreApplication::processEvents(QEventLoop::AllEvents,100);

//    delete ui;    //删除子窗口
}

void DebugDialog::on_Connect_Test_Button_clicked()
{
    get_post_Window *get_post_test = new get_post_Window();
    get_post_test->show();

    //get_post_test->deleteLater();
}
