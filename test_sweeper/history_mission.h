#ifndef HISTORY_MISSION_H
#define HISTORY_MISSION_H

#include <QMainWindow>
#include <QString>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QProcess>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include<QDebug>
#include <QLabel>
#include <QScrollArea>
#include <QDateTime>
#include <QProgressBar>

#include "thread/mission_http.h"

#define HTTP_MAXNUM 32

namespace Ui {
class History_mission;
}

class History_mission : public QMainWindow
{
    Q_OBJECT

public:
    explicit History_mission(QWidget *parent = 0);
    ~History_mission();
    int angle;
    bool isfinished;
    void set_watter_label(QString, int);
    bool info;
protected:
    //void paintEvent(QPaintEvent *) override;
    bool eventFilter(QObject *, QEvent *) override;
signals:
    void close_mission_dialog();

private slots:
    void on_ReturnButton_clicked();
    void MissionList_recv(QString);
    void ScrollBarValchange(int);
    void AutoRun(int,int,QString);
    void water_flash();
    void RefreshData();
private:
    Ui::History_mission *ui;
    QScrollBar *verticalScrollBar;
    QString scrollbarsheet;
    QTimer *timer;
    QTimer *Cnt_timer;
    QPixmap pix;
    QScrollArea *scrollarea;
    QLabel *water_label;
    QLabel *rubbish_label;
    QLabel *sign_label;
    void paint();
    QDateTime datetime;
    Mission_http *mission_http;
};

#endif // HISTORY_MISSION_H
