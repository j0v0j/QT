#ifndef TEMP_SQLLITE_H
#define TEMP_SQLLITE_H

#include <QObject>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariantList>
#include <QDebug>
#include <QSqlTableModel>

class TEMP_SQLLITE : public QObject
{
    Q_OBJECT
public:
    explicit TEMP_SQLLITE(QObject *parent = nullptr);
    QSqlTableModel *model;
    void ADD_tem_value(double tem1,double tem2,double tem3,double tem4);
signals:
//    void sql_saveok();
public slots:
    void set_model();

private:
//    QSqlDatabase db;//打开数据库
    QSqlQuery *query;//执行SQL语句
    QString sqlcommand;

    bool dbok=false;
};

#endif // TEMP_SQLLITE_H
