#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "QtMqtt/qmqtt_client.h"
#include "qmqtt.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void mqsub(const QMQTT::Message& message);
    void somethingmqsub(const QString& topic, const quint8 qos);
    void on_ButtonMqtt_clicked();

    void on_sendButton_clicked();

    void on_sub_topic_editingFinished();
    void aliyunok();
private:
    Ui::MainWindow *ui;
//    QHostAddress MQTTIP=QHostAddress("192.168.188.103");
//    QMQTT::Client *client = new QMQTT::Client(MQTTIP, 1883);
    // 三元组信息设置

    QString    ProductKey="a1jbawDQyka";//需要跟阿里云Iot平台一致;
    QString    DeviceName="ESP8266";//需要跟阿里云Iot平台一致;
    QString    DeviceSecret="P28V2o1MOwnGjW1BuO6qeHsXhWrAfcPw";//需要跟阿里云Iot平台一致;
    QString    RegionId="cn-shanghai";

    QString    targetServer = "a1jbawDQyka.iot-as-mqtt.cn-shanghai.aliyuncs.com";//域名
    QString    clientId="ESP8266|securemode=3,signmethod=hmacsha1|";//这里随便写，最好是设备的Mac地址
    QString     message ="clientId"+clientId+"deviceName"+DeviceName+"productKey"+ProductKey;

    QMQTT::Client *client = new QMQTT::Client(targetServer, 1883,0,false);
};

#endif // MAINWINDOW_H
