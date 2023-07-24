#ifndef MISSIONSETDIALOG_H
#define MISSIONSETDIALOG_H

#include <QDialog>

namespace Ui {
class MissionSetDialog;
}

class MissionSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MissionSetDialog(QWidget *parent = 0);
    ~MissionSetDialog();

private slots:
    void on_ReturnButton_clicked();

private:
    Ui::MissionSetDialog *ui;
};

#endif // MISSIONSETDIALOG_H
