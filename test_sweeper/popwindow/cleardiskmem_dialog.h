#ifndef CLEARDISKMEM_DIALOG_H
#define CLEARDISKMEM_DIALOG_H

#include <QWidget>
#include <QDebug>
#include <QJson/Parser>
#include <QJson/Serializer>
#include "thread/setting_http.h"

namespace Ui {
class Cleardiskmem_dialog;
}

class Cleardiskmem_dialog : public QWidget
{
    Q_OBJECT

public:
    explicit Cleardiskmem_dialog(QWidget *parent = 0);
    ~Cleardiskmem_dialog();

signals:
    void cleardiskmem();
private slots:
    void on_confirm_button_clicked();

    void on_cancel_button_clicked();

    void SetdataSlot(QString);
private:
    Ui::Cleardiskmem_dialog *ui;
    setting_http *set_http;
};

#endif // CLEARDISKMEM_DIALOG_H
