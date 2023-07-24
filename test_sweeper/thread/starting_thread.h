#ifndef STARTING_THREAD_H
#define STARTING_THREAD_H

#include <QMainWindow>
#include <QThread>
#include <QWidget>
#include <QtNetwork>

class starting_thread : public QThread
{
    Q_OBJECT
public:
    explicit starting_thread(QObject *parent = 0);
    void run();
    void stop();
signals:
    void start_progress(int);

public slots:

protected:


private:
    volatile bool stopped;

};

#endif // STARTING_THREAD_H
