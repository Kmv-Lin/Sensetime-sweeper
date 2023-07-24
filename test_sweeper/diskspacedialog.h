#ifndef DISKSPACEDIALOG_H
#define DISKSPACEDIALOG_H

#include <QDialog>
#include <QPalette>
#include <QJson/Parser>
#include <QJson/Serializer>
#include"popwindow/cleardiskmem_dialog.h"
#include "thread/setting_http.h"

namespace Ui {
class DiskSpaceDialog;
}

class DiskSpaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DiskSpaceDialog(QWidget *parent = 0);
    ~DiskSpaceDialog();

private slots:
    void on_ReturnButton_clicked();
    void on_pushButton_clicked();
    void clear();
    void SetdataSlot(QString str);

private:
    Ui::DiskSpaceDialog *ui;
    Cleardiskmem_dialog *cleardiskmem;
    setting_http *set_http;
    int Allmem;
    int Usedmem;
};

#endif // DISKSPACEDIALOG_H
