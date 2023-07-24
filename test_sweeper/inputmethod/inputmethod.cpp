#include <QDebug>
#include "inputmethod.h"

InputMethod::InputMethod()
{
    keyboard = new KeyBoard;

    setWindowFlags(Qt::Tool|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
}

InputMethod::~InputMethod()
{
    delete keyboard;
}

/*
* Name : void eventFilter(QObject *obj, QEvent *event);
* Type : QEvent
* Func : judge input method event
* In   : QObject,QEvent
* Out  : bool
*/
bool InputMethod::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type()==QEvent::MouseButtonPress)
    {
        emit setImObjName(obj->objectName()); //send the Object Name of event to judge who trigger it
        //qDebug() <<obj->objectName();
        showKeyBoard();
        return true;
    }

    return QObject::eventFilter(obj,event);
}


/*
* Name : void showKeyBoard();
* Type : function
* Func : show keyBoard
* In   : Null
* Out  : Null
*/
void InputMethod::showKeyBoard()
{
    keyboard->setWindowFlags(Qt::Tool|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    keyboard->move(0,50);
    keyboard->exec();
}



