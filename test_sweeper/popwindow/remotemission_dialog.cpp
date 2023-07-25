#include "remotemission_dialog.h"
#include "ui_remotemission_dialog.h"

#include <QDebug>

Remotemission_dialog::Remotemission_dialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Remotemission_dialog)
{
    qDebug() << "********Remotemission_dialog open********";
     setWindowFlags(Qt::FramelessWindowHint);//隐藏顶部栏
     setAttribute(Qt::WA_DeleteOnClose);    //防止内存泄漏
    ui->setupUi(this);

    QFont font;
    font.setFamily("Ubuntu");
    font.setPointSize(22);
    ui->label->setFont(font);
    ui->label->setText("远程下发了新任务\n您的任务稍后执行\n\n\n");
    font.setFamily("Source Han Sans CN");
    font.setPointSize(18);
    ui->textEdit->setFont(font);
    ui->textEdit->setText("5s后关闭窗口...");
    ui->textEdit->setStyleSheet("QTextEdit{background-color: rgba(0, 0, 0, 0);border:0px solid #838486;color: rgb(13, 146, 153);  }");//透明+取消线条
    ui->textEdit->setAlignment(Qt::AlignCenter);
}

Remotemission_dialog::~Remotemission_dialog()
{
    qDebug() << "********Remotemission_dialog close********";
    delete ui;
}
