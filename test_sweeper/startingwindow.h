#ifndef STARTINGWINDOW_H
#define STARTINGWINDOW_H

#include <QWidget>
#include <QThread>
#include "qdatetime.h"
#include <QTimer>
#include <QMessageBox>
#include <QDebug>
#include <QJson/Parser>
#include <QJson/Serializer>
#include <QPainter>
#include <QPaintEvent>

#include "thread/starting_thread.h"
#include "thread/httpthread.h"

namespace Ui {
class Startingwindow;
}

class Startingwindow : public QWidget
{
    Q_OBJECT
public:
    explicit Startingwindow(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    ~Startingwindow();
    int angle;

signals:
    //void CheckW_show();
    void MainW_Show(bool);
private slots:
    void UpdateStartingBar(int);
    void parseData(QString);

private:
    Ui::Startingwindow *ui;
    starting_thread *thread;
    HttpReadThread * HttpThread;
    QTimer *timer;
    QPixmap pix;

    volatile bool data_finished;
    enum StartCheck{
        node_status = 0,
        self_check_info = 1,
        status,
        vehicle_state
    }startcheck_state;
    enum Nodestatus{
        camera = 0,
        canbus = 1,
        control,
        decision_planning,
        ins,
        lidar,
        localization,
        monitor,
        monitor_auxiliary_master,
        prediction,
        robocore,
        uss
    }node;
    enum Infostatus{
        brake = 0,
        close_brake_light = 1,
        close_dust,
        close_headlight,
        close_spray,
        close_turn_left_light,
        close_turn_right_light,
        close_vacuum,
        forward,
        open_brake_light,
        open_dust,
        open_headlight,
        open_spray,
        open_turn_left_light,
        open_turn_right_light,
        open_vacuum,
        reverse,
        turn_left,
        turn_right
    }info;

    volatile uint16_t nodecheck_control;
    volatile uint32_t infocheck_control;
};

#endif // STARTINGWINDOW_H
