#ifndef KEYBOARDMINDIALOG_H
#define KEYBOARDMINDIALOG_H

#include <QDialog>
#include <QtGui>
#include <QSignalMapper>
#include <QPushButton>

namespace Ui {
class KeyboardMinDialog;
}

class KeyboardMinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KeyboardMinDialog(QWidget *parent = 0);
    ~KeyboardMinDialog();


signals:
    void keyin_value(const char *password );

private slots:
    void on_closeButton_clicked();
    void setDispText(const QString& text);
    void removeQuiver(const QString& text);
    void clickdelay();
private:
    Ui::KeyboardMinDialog *ui;

    QSignalMapper *signalMapper;
    bool waitingForOperand;
    uint8_t intset_count;
    QString intset_str;
    QString password;

};

#endif // KEYBOARDMINDIALOG_H
