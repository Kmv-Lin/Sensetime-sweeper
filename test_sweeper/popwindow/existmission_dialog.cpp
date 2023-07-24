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
}

Existmission_dialog::~Existmission_dialog()
{
    this->hide();
    qDebug() << "********Existmission_dialog close********";
    delete ui;
}
