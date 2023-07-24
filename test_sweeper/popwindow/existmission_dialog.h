#ifndef EXISTMISSION_DIALOG_H
#define EXISTMISSION_DIALOG_H

#include <QWidget>

namespace Ui {
class Existmission_dialog;
}

class Existmission_dialog : public QWidget
{
    Q_OBJECT

public:
    explicit Existmission_dialog(QWidget *parent = 0);
    ~Existmission_dialog();

private:
    Ui::Existmission_dialog *ui;
};

#endif // EXISTMISSION_DIALOG_H
