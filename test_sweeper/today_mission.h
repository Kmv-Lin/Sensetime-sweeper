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

#include "popwindow/confirmmission_dialog.h"

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
protected:
    bool eventFilter(QObject *, QEvent *) override;
signals:
    void close_mission_dialog();

private slots:
    void on_ReturnButton_clicked();
    void MissionList_recv(QString);
    void list_btn_clicked(QString);
    void ScrollBarValchange(int);

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
    QPixmap pix;
    void paint();

};

#endif // TODAY_MISSION_H
