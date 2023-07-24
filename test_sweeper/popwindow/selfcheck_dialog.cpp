#include "selfcheck_dialog.h"
#include "ui_selfcheck_dialog.h"

#include<QDebug>

selfcheck_Dialog::selfcheck_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selfcheck_Dialog)
{
    qDebug() << "********selfcheck_Dialog open********";
     setWindowFlags(Qt::FramelessWindowHint);//隐藏顶部栏
     setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
    ui->setupUi(this);

    //字体颜色-white
    //"返回"背景E93800+字体白色+线宽1px E93800+四边圆角
    ui->OK_Button->setStyleSheet("QPushButton{background-color: rgb(233, 56, 0);color:white;border:1px solid #E93800;border-radius:5px;  }");//线条

//    QPalette pel;
//    pel.setColor(QPalette::ButtonText,Qt::white);
//    ui->OK_Button->setPalette(pel);

}

selfcheck_Dialog::~selfcheck_Dialog()
{
    qDebug() << "********selfcheck_Dialog close********";
    delete ui;
}
