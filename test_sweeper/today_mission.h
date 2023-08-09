#ifndef TODAY_MISSION_H
#define TODAY_MISSION_H

#include <QWidget>
#include <QString>
#include<QDebug>
#include <QPushButton>
#include <QSignalMapper>
#include <QTime>
#include <QLabel>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QProcess>
#include <QPainter>
#include <QPaintEvent>
#include <QDateTime>

#include "popwindow/confirmmission_dialog.h"
#include "thread/mission_http.h"
#include "history_mission.h"

#define MISSION_MAX  256

namespace Ui {
class Today_mission;
}

class Today_mission : public QWidget
{
    Q_OBJECT

public:
    explicit Today_mission(QWidget *parent = 0);
    ~Today_mission();
    bool isfinished;
    int angle;
    void set_watter_label(QString, int);
protected:
    bool eventFilter(QObject *, QEvent *) override;
signals:
    void close_mission_dialog();
    void RunningMissionID(QString);
private slots:
    void on_ReturnButton_clicked();
    void MissionList_recv(QString);
    void list_btn_clicked(QString);
    void ScrollBarValchange(int);
    void AutoRun(int ,int ,QString );
    void water_flash();
    void RefreshData();
    void RunningMissionIDSlot(QString);
private:
    Ui::Today_mission *ui;
    QPushButton *list_btn[MISSION_MAX];
    QLabel *timelabel[MISSION_MAX];
    QPushButton *statusbutton[MISSION_MAX];
    QSignalMapper *signalMapper;
    ConfirmMission_Dialog *confirmmission_window;
    QScrollBar *verticalScrollBar;
    QString scrollbarsheet;
    QTimer *timer;
    QTimer *Cnt_timer;
    QPixmap pix;
    QLabel *water_label;
    QLabel *rubbish_label;
    QLabel *sign_label;
    void paint();
    QDateTime datetime;
    Mission_http *mission_http;
};

#endif // TODAY_MISSION_H
