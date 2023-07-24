#ifndef MY_THREAD_H
#define MY_THREAD_H

#include <QMainWindow>
#include <QThread>
#include <QWidget>
#include <QtNetwork>

class my_thread : public QThread
{
    Q_OBJECT
public:
    explicit my_thread(QObject *parent = 0);
    void run();
    void stop();
signals:
    void progress(int p, const char* msg);

public slots:

protected:


private:
    volatile bool stopped;

};

#endif // MY_THREAD_H
