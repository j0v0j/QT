#include "temp_sqllite.h"
#include <QSqlDatabase>
#include <qdatetime.h>
#include <QMessageBox>

TEMP_SQLLITE::TEMP_SQLLITE(QObject *parent) : QObject(parent)
{
    qDebug() << QSqlDatabase::drivers();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库
    QDateTime time = QDateTime::currentDateTime();
    QString tablename =time.toString("yyyy-MM-dd");//将数据库的表名命名为日期
    sqlcommand=tr("../sertest/data/%1.db").arg(tablename);
    db.setDatabaseName(sqlcommand);

    if( !db.open() ) //数据库打开失败
    {
        qDebug()<< QString::fromLocal8Bit("错误")<<db.lastError().text();
        return;
    }
    else{
        dbok=true;
    }
    //设置模型

    query=new QSqlQuery;
}

void TEMP_SQLLITE::set_model()
{
    if(dbok==true){
        model = new QSqlTableModel(this);
        model->setTable("temp_and_ry");//指定使用哪个表
    }

}
void TEMP_SQLLITE::ADD_tem_value(double tem1,double tem2,double tem3,double tem4)
{
    static bool sqlfinish=true;
    if(dbok==true && sqlfinish==true){
        QDateTime time = QDateTime::currentDateTime();
        QString str_time =time.toString("yyyy-MM-dd-hh:mm:ss");//得到系统的当前时间

        sqlfinish=false;

        query->exec("create table temp_and_ry(timetext text primary key,Tem1 double, Tem2 double, Tem3 double, Tem4 double);");
        query->prepare("insert into temp_and_ry(timetext, Tem1, Tem2, Tem3, Tem4) values(:timetext, :Tem1, :Tem2, :Tem3, :Tem4);");
        //给字段设置内容 list

        //给字段绑定相应的值 按顺序绑定
        query->bindValue(":timetext",str_time);
        query->bindValue(":Tem1",tem1);
        query->bindValue(":Tem2",tem2);
        query->bindValue(":Tem3",tem3);
        query->bindValue(":Tem4",tem4);
        if(!query->exec()){
            qDebug()<<query->lastError();
        }
        else {
            sqlfinish=true;
//            emit sql_saveok();
            qDebug()<<"sql write!";
        }
    }else {
        qDebug()<<"sql faile!";
    }



}
