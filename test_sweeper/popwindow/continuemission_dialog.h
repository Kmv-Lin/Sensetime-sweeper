#ifndef CONTINUEMISSION_DIALOG_H
#define CONTINUEMISSION_DIALOG_H

#include <QWidget>
#include <QDebug>
#include <QJson/Parser>
#include <QJson/Serializer>
#include "thread/setting_http.h"
#include "thread/mission_http.h"

namespace Ui {
class Continuemission_dialog;
}

class Continuemission_dialog : public QWidget
{
    Q_OBJECT

public:
    explicit Continuemission_dialog(QWidget *parent = 0);
    ~Continuemission_dialog();

private slots:
    void on_confirm_button_clicked();

    void on_cancel_button_clicked();
    void RunningMissionIDSlot(QString);
    void Mission_continue(QString);
private:
    Ui::Continuemission_dialog *ui;
    QString MissionID;
    Mission_http *mission_http;
};

#endif // CONTINUEMISSION_DIALOG_H
