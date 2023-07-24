#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "thread/my_thread.h"
#include <QDateTime>
#include <QUdpSocket>

#include "inputmethod/inputmethod.h"
#include "inputmethod/keyboardmindialog.h"
#include "thread/httpthread.h"
#include "weiqianfunctions.h"
#include "serialportreadthread.h"
#include "serialport.h"
#include"com.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QUdpSocket *udpSocket;
    QString ntpIP;
    ~MainWindow();

signals:
    void CheckW_Show();

    void connected();
    void readyRead();

private slots:
    void setNtp();
    void connectsucess();
    void readingDataGrams();
    void setDateTime(QDateTime&);

    void on_SettingButton_clicked();

    void on_MissionButton_clicked();

    void AutoRun1S();
    void Receive_MainW_show(bool);
    void KeyboardMinDialog_sign(const char *password);
    void receiveData(const char *str);
    void sendData(char value);

    void vehicleStateSlot(QString str);

    void set_statue_label(char mode);
    void set_watter_label(QString state, int watter_info);
    void set_Display_Label_IdleState(int finish, int allnum);
    void set_Display_Label_Runningstate(int progress);
    void set_RunningState_Label(int finish, int allnum);
    void set_RunningState_Label1();

    void MainWindow_FSM();

private:
    Ui::MainWindow *ui;

    QTextEdit		*m_pReceiveEdit;
    SerialPortReadThread    *m_pReadThread;//
   SerialPort      *m_pMySerialPort;

    KeyboardMinDialog *keyin;
    InputMethod *inputmethod;
    QDateTime datetime;
    my_thread *thread2;
    HttpReadThread *m_HttpThread;

    QTimer *timer;
    uint8_t keyboard_flag;
    uint8_t keyboard_flag_setting;
    int finish_num;
    int all_num;
    double progress;
    QString SYS_state;

    uint8_t FSM_state;
    char Vehicle_Controller;
    volatile bool data_finish;
};

#endif // MAINWINDOW_H
