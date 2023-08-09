#ifndef CONFIRMMISSION_DIALOG_H
#define CONFIRMMISSION_DIALOG_H

#include <QDialog>
#include "thread/mission_http.h"
#include "existmission_dialog.h"
#include "remotemission_dialog.h"
#include <QTimer>

namespace Ui {
class ConfirmMission_Dialog;
}

class ConfirmMission_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmMission_Dialog(QString id = "",QString mapname = "",QWidget *parent = 0);
    ~ConfirmMission_Dialog();

signals:
    void close_mission_dialog();
    void close_todaymission();
    void RunningMissionID(QString);
private slots:
    void on_ReturnButton_clicked();
    void MissionList_recv(QString);
    void MissionList_recv_again(QString);
    void MissionIssue_recv(QString);
    void on_OK_Button_clicked();
    void list_showdelay();
    void list_showdelay_again();
    void issue_showdelay();
private:
    QString MissionID;
    QString Map_name;
    Ui::ConfirmMission_Dialog *ui;
    Mission_http *mission_list_http;
    Mission_http *mission_issue_http;
    Existmission_dialog *existmission_dialog;
    Remotemission_dialog *remotemission_dialog;
    QTimer *timer;
    int flag;
};

#endif // CONFIRMMISSION_DIALOG_H
