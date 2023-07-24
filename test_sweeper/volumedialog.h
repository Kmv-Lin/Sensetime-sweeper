#ifndef VOLUMEDIALOG_H
#define VOLUMEDIALOG_H

#include <QDialog>
#include<QJson/Parser>
#include<QJson/Serializer>
#include<QDebug>

#include "thread/setting_http.h"

namespace Ui {
class VolumeDialog;
}

class VolumeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VolumeDialog(QWidget *parent = 0);
    ~VolumeDialog();

private slots:
    void on_ReturnButton_clicked();

    void on_Btn_high_clicked();

    void on_Btn_low_clicked();

    void VolSlot(QString);

    void VolInit(QString);

private:
    Ui::VolumeDialog *ui;
    int Volumeval;
    setting_http * set_http_get;
    setting_http * set_http;
};

#endif // VOLUMEDIALOG_H
