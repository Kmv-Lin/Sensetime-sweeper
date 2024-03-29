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
    //void on_BookButton_clicked();
    //void on_TodayButton_clicked();
    void AutoRun(int,int,QString);
private:
    Ui::History_mission *ui;
    QScrollBar *verticalScrollBar;
    QString scrollbarsheet;
    QTimer *timer;
    QPixmap pix;
    QScrollArea *scrollarea;
    QLabel *lab;
    void paint();
    QDateTime datetime;
};

#endif // HISTORY_MISSION_H
