#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QJson/Parser>
#include <QJson/Serializer>

#include "mission_dialog.h"
#include "selfcheck_window.h"
#include "popwindow/selfcheck_dialog.h"
#include "startingwindow.h"
//#include "thread/get_post_window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    /////////////-加上这一段，整个工程页面可以显示中文-//////////////////////
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QFont font;
    font.setPointSize(16);
    font.setFamily("Source Han Sans CN");
    //font.setFamily("宋体");
    font.setBold(false);
    a.setFont(font);

    Startingwindow startingw;
    startingw.show();

    //selfcheck_Window checkw;
    //checkw.show();

    MainWindow mainw;
    //mainw.show();

//    selfcheck_Dialog w;
//    w.show();


    //QObject::connect(&checkw,SIGNAL(MainW_Show()), &mainw, SLOT(Receive_MainW_show()) );
    //QObject::connect(&mainw,SIGNAL(CheckW_Show()), &checkw, SLOT(Receive_CheckW_show()) );

//startingwindow->selfcheck_window
    //QObject::connect(&startingw,SIGNAL(CheckW_show()),&checkw,SLOT(Receive_CheckW_show()));

    QObject::connect(&startingw,SIGNAL(MainW_Show(bool)),&mainw,SLOT(Receive_MainW_show(bool)));

    //JSON测试代码
//    const QString data = "{\"username\" :\"admin\", \"password\" :\"123456\", \"age\": 32}";
//    QJson::Parser parser;
//    bool ok;
//    QVariantMap results=parser.parse(data.toUtf8(),&ok).toMap();
//    if(!ok){
//        qDebug() << "********failed********";
//    }
//    else{
//        QString username = results["username"].toString();
//        QString password = results["password"].toString();
//        int age = results["age"].toInt();
//        qDebug() << "********successful*******";
//        qDebug() << "********username is ********"<<username;
//        qDebug() << "********password is ********"<<password;
//        qDebug() << "********age is  ********"<<age;
//    }

//    get_post_Window w;
//    w.show();

    return a.exec();
}
