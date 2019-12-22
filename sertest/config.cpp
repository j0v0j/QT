#include "config.h"
#include <QtCore/QDebug>
CONFIG::CONFIG()
{
    qDebug()<< "service";
    qDebug()<< "mainFun";
    qDebug()<< "subFun" ;
}
void CONFIG::WriteSettings()
{
    //QSettings settings("Software Inc", "Spreadsheet"); // windows在注册表中建立建 Software Inc -> Spreadsheet
        QSettings settings("Option.ini", QSettings::IniFormat); // 当前目录的INI文件
        settings.beginGroup("DevOption");
        settings.setValue("mainFun", 25);
        settings.setValue("subFun", 40);
        settings.setValue("service", 1);
        settings.endGroup();
}
void CONFIG::ReadSettings()
{
    QSettings settings("Option.ini", QSettings::IniFormat);

        int mainFun = settings.value("DevOption1/mainFun").toInt();
        int subFun = settings.value("DevOption1/subFun").toInt();
        QString service = settings.value("DevOption1/service").toString();

//        qDebug()<< "service" << service;
//        qDebug()<< "mainFun" << mainFun;
//        qDebug()<< "subFun" << subFun;
}
