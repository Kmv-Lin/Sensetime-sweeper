#ifndef SETTING_THREAD_H
#define SETTING_THREAD_H

#include <QMainWindow>
#include <QThread>
#include <QWidget>
#include <QtNetwork>

class setting_thread : public QThread
{
    Q_OBJECT
public:
    explicit setting_thread(QObject *parent = 0);
    void run();
    void stop();
signals:
    void updating_progress(int,QString);

public slots:

protected:

private:
    volatile bool stopped;
};

#endif // SETTING_THREAD_H
