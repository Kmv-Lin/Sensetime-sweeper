#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QMainWindow>
#include "inputmethod/keyboardmindialog.h"

namespace Ui {
class settingWindow;
}

class settingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit settingWindow(QWidget *parent = 0);
    ~settingWindow();

private slots:
    void on_ReturnButton_clicked();

    void on_SetNeworkButton_clicked();

    void on_SetVolumeButton_clicked();

    void on_SetDiskButton_clicked();

    void on_SetMissionButton_clicked();

    void on_SysUpdateButton_clicked();

    void on_SetVersionButton_clicked();

    void on_SetDebugButton_clicked();

    void ScrollBarValchange(int);

    void on_ManualCheckButton_clicked();

private:
    Ui::settingWindow *ui;
    KeyboardMinDialog *keyin_set;
    QScrollBar *verticalScrollBar;
    QString scrollbarsheet;

};

#endif // SETTINGWINDOW_H
