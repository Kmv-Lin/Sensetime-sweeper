#ifndef MISSION_DIALOG_H
#define MISSION_DIALOG_H

#include <QDialog>
#include <QScrollArea>
#include <QWidget>
#include <QThread>

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

private slots:
    void on_ReturnButton_clicked();


//    void on_BookButton_clicked();

//    void on_TodayButton_clicked();

    void on_HistoryButton_clicked();

    void on_StartButton_clicked();

    void on_HistoryButton_pressed();

    void on_StartButton_pressed();
signals:

private:
    Ui::Mission_Dialog *ui;
    Mission_http *mission_http;
    History_mission *history_mission;
    Today_mission *today_mission;
    QScrollArea *scrollarea;
    QWidget *scrollwidget;
    QThread *thread;
};

#endif // MISSION_DIALOG_H
