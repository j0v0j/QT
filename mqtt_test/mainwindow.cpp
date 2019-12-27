#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageAuthenticationCode>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    client->setClientId(clientId);
    connect(client,&QMQTT::Client::subscribed,this,&MainWindow::somethingmqsub);
    client->setUsername(DeviceName + "&" + ProductKey);
//    client->setPassword(QMessageAuthenticationCode::hash(message.toLocal8Bit(), DeviceSecret.toLocal8Bit(), QCryptographicHash::Sha1).toHex());

    client->setPassword("69137A72ED58BE7C41B1D4FB70169E5C32711AB9");
    connect(client,&QMQTT::Client::connected,this,&MainWindow::aliyunok);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::aliyunok()
{
   qDebug()<<"MQTT:aliyun";


}
void MainWindow::mqsub(const QMQTT::Message& message)
{
    qDebug()<<"MQTT:"<<message.topic()<<":"<<message.payload();

}
void MainWindow::somethingmqsub(const QString& topic, const quint8 qos)
{
    qDebug()<<"topic:"<<topic<<"qos"<<qos;

}


void MainWindow::on_ButtonMqtt_clicked()
{
    static bool flag=true;
    if(flag){
        flag=false;
        client->connectToHost();

        connect(client,&QMQTT::Client::received,this,&MainWindow::mqsub);
    }else{
        flag=true;
        client->disconnectFromHost();
    }
}

void MainWindow::on_sendButton_clicked()
{
    QMQTT::Message sendmessage;
    sendmessage.setTopic(ui->topic->text());
    sendmessage.setPayload(ui->meg->text().toUtf8());
    sendmessage.setQos(0);
    client->publish(sendmessage);
}

void MainWindow::on_sub_topic_editingFinished()
{
     client->subscribe(ui->sub_topic->text(),0);
     qDebug()<<"subscript";
}
