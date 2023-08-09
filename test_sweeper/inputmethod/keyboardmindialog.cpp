#include "keyboardmindialog.h"
#include "ui_keyboardmindialog.h"

#include<QDebug>
#include <QtGui/QLineEdit>

#include "weiqianfunctions.h"


KeyboardMinDialog::KeyboardMinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyboardMinDialog)
{
    qDebug() << "********KeyboardMinDialog open********";
     setWindowFlags(Qt::FramelessWindowHint);//隐藏顶部栏
     setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
    ui->setupUi(this);

    waitingForOperand = true;
    intset_count =0;
     password.append("1234");

     timer = new QTimer(this);
     timer->setInterval(8000);
     connect(timer,SIGNAL(timeout()),this,SLOT(passwd_refresh()));

    //密码模式
    ui->Number_lineEdit->setEchoMode(QLineEdit::Normal);
    //display->setEchoMode(QLineEdit::Password);
    ui->Number_lineEdit->setStyleSheet("QLineEdit{background-color: rgb(255, 255, 255);background-image: url();border-width:0;border-style:outset}");

//    ui->closeButton->setStyleSheet("QPushButton{background-image: url(:/new/keypad/picture/Delete.png);background-color: rgba(255, 255, 255, 0);border:0px solid #0F9BA3; }");//线条
       ui->pushButton_0->setStyleSheet("QPushButton{background-image: url(:/new/keypad/picture/Group0.png);background-color: rgb(0, 150, 157);border:1px solid #00969D;border-radius:5px;  }");//线条
       ui->pushButton_1->setStyleSheet("QPushButton{background-image: url(:/new/keypad/picture/Group1.png);background-color: rgb(0, 150, 157);border:1px solid #00969D;border-radius:5px;  }");//线条
       ui->pushButton_2->setStyleSheet("QPushButton{background-image: url(:/new/keypad/picture/Group2.png);background-color: rgb(0, 150, 157);border:1px solid #00969D;border-radius:5px;  }");//线条
       ui->pushButton_3->setStyleSheet("QPushButton{background-image: url(:/new/keypad/picture/Group3.png);background-color: rgb(0, 150, 157);border:1px solid #00969D;border-radius:5px;  }");//线条
       ui->pushButton_4->setStyleSheet("QPushButton{background-image: url(:/new/keypad/picture/Group4.png);background-color: rgb(0, 150, 157);border:1px solid #00969D;border-radius:5px;  }");//线条
       ui->pushButton_5->setStyleSheet("QPushButton{background-image: url(:/new/keypad/picture/Group5.png);background-color: rgb(0, 150, 157);border:1px solid #00969D;border-radius:5px;  }");//线条
       ui->pushButton_6->setStyleSheet("QPushButton{background-image: url(:/new/keypad/picture/Group6.png);background-color: rgb(0, 150, 157);border:1px solid #00969D;border-radius:5px;  }");//线条
       ui->pushButton_7->setStyleSheet("QPushButton{background-image: url(:/new/keypad/picture/Group7.png);background-color: rgb(0, 150, 157);border:1px solid #00969D;border-radius:5px;  }");//线条
       ui->pushButton_8->setStyleSheet("QPushButton{background-image: url(:/new/keypad/picture/Group8.png);background-color: rgb(0, 150, 157);border:1px solid #00969D;border-radius:5px;  }");//线条
       ui->pushButton_9->setStyleSheet("QPushButton{background-image: url(:/new/keypad/picture/Group9.png);background-color: rgb(0, 150, 157);border:1px solid #00969D;border-radius:5px;  }");//线条
       ui->pushButton_10->setStyleSheet("QPushButton{background-image: url(:/new/keypad/picture/Group10.png);background-color: rgb(0, 150, 157);border:1px solid #00969D;border-radius:5px;  }");//线条
       ui->pushButton_11->setStyleSheet("QPushButton{background-image: url(:/new/keypad/picture/Group11.png);background-color: rgb(0, 150, 157);border:1px solid #00969D;border-radius:5px;  }");//线条

       signalMapper=new QSignalMapper(this);

       //number
       signalMapper->setMapping(ui->pushButton_0,"0");
       signalMapper->setMapping(ui->pushButton_1,"1");
       signalMapper->setMapping(ui->pushButton_2,"2");
       signalMapper->setMapping(ui->pushButton_3,"3");
       signalMapper->setMapping(ui->pushButton_4,"4");
       signalMapper->setMapping(ui->pushButton_5,"5");
       signalMapper->setMapping(ui->pushButton_6,"6");
       signalMapper->setMapping(ui->pushButton_7,"7");
       signalMapper->setMapping(ui->pushButton_8,"8");
       signalMapper->setMapping(ui->pushButton_9,"9");
       //signalMapper->setMapping(ui->pushButton_10,"*");
       //signalMapper->setMapping(ui->pushButton_11,"#");



       //number
       connect(ui->pushButton_0,SIGNAL(clicked()),signalMapper,SLOT(map()));
       connect(ui->pushButton_1,SIGNAL(clicked()),signalMapper,SLOT(map()));
       connect(ui->pushButton_2,SIGNAL(clicked()),signalMapper,SLOT(map()));
       connect(ui->pushButton_3,SIGNAL(clicked()),signalMapper,SLOT(map()));
       connect(ui->pushButton_4,SIGNAL(clicked()),signalMapper,SLOT(map()));
       connect(ui->pushButton_5,SIGNAL(clicked()),signalMapper,SLOT(map()));
       connect(ui->pushButton_6,SIGNAL(clicked()),signalMapper,SLOT(map()));
       connect(ui->pushButton_7,SIGNAL(clicked()),signalMapper,SLOT(map()));
       connect(ui->pushButton_8,SIGNAL(clicked()),signalMapper,SLOT(map()));
       connect(ui->pushButton_9,SIGNAL(clicked()),signalMapper,SLOT(map()));
       //connect(ui->pushButton_10,SIGNAL(clicked()),signalMapper,SLOT(map()));
       //connect(ui->pushButton_11,SIGNAL(clicked()),signalMapper,SLOT(map()));

       connect(signalMapper,SIGNAL(mapped(const QString&)),this,SLOT(setDispText(const QString&)));
       connect(signalMapper,SIGNAL(mapped(const QString&)),this,SLOT(removeQuiver(const QString&)));


}

void KeyboardMinDialog::clickdelay(){
        ui->pushButton_0->setEnabled(true);
        ui->pushButton_1->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
        ui->pushButton_7->setEnabled(true);
        ui->pushButton_8->setEnabled(true);
        ui->pushButton_9->setEnabled(true);
        ui->pushButton_10->setEnabled(true);
        ui->pushButton_11->setEnabled(true);
}

void KeyboardMinDialog::removeQuiver(const QString& text){
        QByteArray bytearray = text.toUtf8();
        char* ch = bytearray.data();
        switch (*ch) {
        case '0':
            ui->pushButton_0->setEnabled(false);
            break;
        case '1':
            ui->pushButton_1->setEnabled(false);
            break;
        case '2':
            ui->pushButton_2->setEnabled(false);
            break;
        case '3':
            ui->pushButton_3->setEnabled(false);
            break;
        case '4':
            ui->pushButton_4->setEnabled(false);
            break;
        case '5':
            ui->pushButton_5->setEnabled(false);
            break;
        case '6':
            ui->pushButton_6->setEnabled(false);
            break;
        case '7':
            ui->pushButton_7->setEnabled(false);
            break;
        case '8':
            ui->pushButton_8->setEnabled(false);
            break;
        case '9':
            ui->pushButton_9->setEnabled(false);
            break;
        case '*':
            ui->pushButton_10->setEnabled(false);
            break;
        case '#':
            ui->pushButton_11->setEnabled(false);
            break;
        default:
            break;
        }
        QTimer::singleShot(120,this,SLOT(clickdelay()));
}

KeyboardMinDialog::~KeyboardMinDialog()
{
    delete ui;
    qDebug() << "********KeyboardMinDialog close********";//确认窗口关闭
}

void KeyboardMinDialog::on_closeButton_clicked()
{
//    char password[4]={'1','2','3','4'};
//    emit keyin_value(password);
    intset_str.clear();
    intset_count=0;
    ui->Number_lineEdit->clear();
    waitingForOperand = true;
    ui->Number_lineEdit->setEchoMode(QLineEdit::Normal);
    ui->Number_lineEdit->setText("请输入密码");
    this->hide();
}

void KeyboardMinDialog::passwd_refresh(){
    intset_str.clear();
    ui->Number_lineEdit->clear();
    waitingForOperand = true;
    ui->Number_lineEdit->setEchoMode(QLineEdit::Normal);
    ui->Number_lineEdit->setText("请输入密码");
}

/*
* Name : void setDispText(const QString& text)
* Type : slot
* Func : insert and show text on display(QLineEdit)
* In   : QString
* Out  : Null
*/
void KeyboardMinDialog::setDispText(const QString& text)
{
     //WeiqianFunctions::Beep();
     timer->stop();
     timer->setInterval(8000);
    if(waitingForOperand)
    {
        ui->Number_lineEdit->clear();
        waitingForOperand = false;
        //密码模式
         ui->Number_lineEdit->setEchoMode(QLineEdit::Password);
    }

    ui->Number_lineEdit->insert(text);
     intset_str.append(text);

    intset_count++;

}


void KeyboardMinDialog::on_pushButton_11_clicked()
{
    //if(intset_count>=4){
        //WeiqianFunctions::Beep();
        intset_count =0;
       // qDebug() <<intset_str << "?" <<password;
        //判断密码
        if(QString::compare(intset_str,password)==0)            //compare返回0表示相等
        {
            //char word[4]={'p','a','s','s'};
            char pass = '1';
             emit keyin_value(&pass);
             this->hide();
            intset_str.clear();
            ui->Number_lineEdit->clear();
            waitingForOperand = true;
            ui->Number_lineEdit->setEchoMode(QLineEdit::Normal);
            ui->Number_lineEdit->setText("请输入密码");
        }else{
            intset_str.clear();
            ui->Number_lineEdit->clear();
            waitingForOperand = true;
            ui->Number_lineEdit->setEchoMode(QLineEdit::Normal);
            ui->Number_lineEdit->setText("密码错误");
            timer->start();
        }


  //  }
}

void KeyboardMinDialog::on_pushButton_10_clicked()
{
    //WeiqianFunctions::Beep();
    if(ui->Number_lineEdit->text() != "请输入密码" && ui->Number_lineEdit->text() != "密码错误"){
        if(intset_count > 0){
            intset_count--;
            intset_str.remove(intset_count,1);
            if(intset_count == 0){
                waitingForOperand = true;
                ui->Number_lineEdit->setEchoMode(QLineEdit::Normal);
                ui->Number_lineEdit->setText("请输入密码");
            }else{
                ui->Number_lineEdit->setText(intset_str);
            }
        }else{
            waitingForOperand = true;
            ui->Number_lineEdit->setEchoMode(QLineEdit::Normal);
            ui->Number_lineEdit->setText("请输入密码");
        }
    }

    //qDebug() << intset_str;
}
