#ifndef REMOTEMISSION_DIALOG_H
#define REMOTEMISSION_DIALOG_H

#include <QWidget>

namespace Ui {
class Remotemission_dialog;
}

class Remotemission_dialog : public QWidget
{
    Q_OBJECT

public:
    explicit Remotemission_dialog(QWidget *parent = 0);
    ~Remotemission_dialog();

private:
    Ui::Remotemission_dialog *ui;
};

#endif // REMOTEMISSION_DIALOG_H
