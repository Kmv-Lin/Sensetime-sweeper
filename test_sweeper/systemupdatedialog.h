#ifndef SYSTEMUPDATEDIALOG_H
#define SYSTEMUPDATEDIALOG_H

#include <QDialog>
#include "thread/setting_thread.h"

namespace Ui {
class SystemUpdateDialog;
}

class SystemUpdateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SystemUpdateDialog(QWidget *parent = 0);
    ~SystemUpdateDialog();

private slots:
    void on_ReturnButton_clicked();
    void on_pushButton_clicked();
    void Updating_progress(int,QString);

private:
    Ui::SystemUpdateDialog *ui;
    setting_thread *SetThread;
    QString UpdateProgress;
};

#endif // SYSTEMUPDATEDIALOG_H
