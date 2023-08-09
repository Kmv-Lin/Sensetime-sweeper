#include "existmission_dialog.h"
#include "ui_existmission_dialog.h"

#include <QDebug>

Existmission_dialog::Existmission_dialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Existmission_dialog)
{
    qDebug() << "********Existmission_dialog open********";
     setWindowFlags(Qt::FramelessWindowHint);//隐藏顶部栏
     setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
    ui->setupUi(this);

    QFont font;
    font.setFamily("Ubuntu");
    font.setPointSize(22);
    ui->label->setFont(font);
    ui->label->setText("任务执行中，请到手机\n或电脑端变更任务状态\n\n\n");
    font.setFamily("Source Han Sans CN");
    font.setPointSize(18);
    ui->textEdit->setFont(font);
    ui->textEdit->setText("20s后关闭窗口...");
    ui->textEdit->setStyleSheet("QTextEdit{background-color: rgba(0, 0, 0, 0);border:0px solid #838486;color: rgb(13, 146, 153);  }");//透明+取消线条
    ui->textEdit->setAlignment(Qt::AlignCenter);
}

Existmission_dialog::~Existmission_dialog()
{
    this->hide();
    qDebug() << "********Existmission_dialog close********";
    delete ui;
}
