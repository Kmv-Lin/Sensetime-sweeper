#ifndef SELFCHECK_DIALOG_H
#define SELFCHECK_DIALOG_H

#include <QDialog>

namespace Ui {
class selfcheck_Dialog;
}

class selfcheck_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit selfcheck_Dialog(QWidget *parent = 0);
    ~selfcheck_Dialog();

private:
    Ui::selfcheck_Dialog *ui;
};

#endif // SELFCHECK_DIALOG_H
