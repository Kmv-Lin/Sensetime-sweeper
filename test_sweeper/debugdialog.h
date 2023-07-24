#ifndef DEBUGDIALOG_H
#define DEBUGDIALOG_H

#include <QDialog>

namespace Ui {
class DebugDialog;
}

class DebugDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DebugDialog(QWidget *parent = 0);
    ~DebugDialog();

private slots:
    void on_ReturnButton_clicked();

    void on_Connect_Test_Button_clicked();

private:
    Ui::DebugDialog *ui;
};

#endif // DEBUGDIALOG_H
