#ifndef MISSION_DIALOG_H
#define MISSION_DIALOG_H

#include <QDialog>
#include <QScrollArea>
#include <QWidget>
#include <QThread>
#include <QTimer>

#include "history_mission.h"
#include "thread/mission_http.h"
#include"today_mission.h"


namespace Ui {
class Mission_Dialog;
}

class Mission_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Mission_Dialog(QWidget *parent = 0);
    ~Mission_Dialog();
    void set_watter_label(QString , int );

private slots:
    void on_ReturnButton_clicked();


//    void on_BookButton_clicked();

//    void on_TodayButton_clicked();

    void on_HistoryButton_clicked();

    void on_StartButton_clicked();

    void on_HistoryButton_pressed();

    void on_StartButton_pressed();

    void AutoRun(int,int,QString);

    void SendInfo();
signals:
    void mission_show(int,int,QString);
private:
    Ui::Mission_Dialog *ui;
    Mission_http *mission_http;
    History_mission *history_mission;
    Today_mission *today_mission;
    QScrollArea *scrollarea;
    QWidget *scrollwidget;
    QThread *history_thread;
    QThread *today_thread;
    QTimer *timer;
    QDateTime datetime;
    int flag;

};

#endif // MISSION_DIALOG_H
