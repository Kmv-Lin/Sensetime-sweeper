#ifndef NETWORKDIALOG_H
#define NETWORKDIALOG_H

#include <QDialog>
#include<QJson/Parser>
#include<QJson/Serializer>
#include <QDebug>
#include <QHBoxLayout>
#include <QSignalMapper>

#include "thread/setting_http.h"
#include "inputmethod/inputmethod.h"
#include "inputmethod/keyboardmindialog.h"
#include "inputmethod/keyboard.h"

//#define btnNum 64

namespace Ui {
class NetWorkDialog;
}

class NetWorkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetWorkDialog(QWidget *parent = 0);
    ~NetWorkDialog();

private slots:
    void on_ReturnButton_clicked();
    void network_recv(QString);
    void list_btn_clicked(QString);
    void sendpasswd(QString);
    void NetworkSlot(QString);
    void ScrollBarValchange(int);

private:
    Ui::NetWorkDialog *ui;
    setting_http *set_http;
    QSignalMapper *signalMapper;
    KeyboardMinDialog *keyin;
    //InputMethod *keyboard;
    KeyBoard *keyboard;
    QString Wifiname;
    QScrollBar *verticalScrollBar;
    QString scrollbarsheet;
};

#endif // NETWORKDIALOG_H
