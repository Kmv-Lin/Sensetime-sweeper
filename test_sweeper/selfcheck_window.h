#ifndef SELFCHECK_WINDOW_H
#define SELFCHECK_WINDOW_H

#include <QMainWindow>
#include "qdatetime.h"
#include <QTimer>
#include <qeventloop.h>
#include <QJson/Parser>
#include<QJson/Serializer>
#include "thread/my_thread.h"
#include "thread/httpthread.h"



namespace Ui {
class selfcheck_Window;
}

class selfcheck_Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit selfcheck_Window(QWidget *parent = 0);
    ~selfcheck_Window();
    void paintEvent(QPaintEvent *);
    void exec();

protected:
    void closeEvent(QCloseEvent *event);
    QEventLoop *m_eventloop;

signals:
    void MainW_Show();

private slots:
    void UpdateSelfcheckBar(int p, const char* msg);
    void parseData(QString str);

private:
    void run_picture(int i);

    Ui::selfcheck_Window *ui;

    HttpReadThread *HttpThread;
    my_thread *thread;
    QTimer  *timer;
    QPixmap pix;
    int angle;
};

#endif // SELFCHECK_WINDOW_H
