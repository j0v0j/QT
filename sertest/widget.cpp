#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QtCore/qmath.h>

#include <QFile>
#include<QFileDialog>
#include <QSqlTableModel>



#define CH1     "00 03 02 A8 00 02 45 82"
#define CH2     "00 03 02 AA 00 02 E4 42"
#define CH3     "00 03 02 AC 00 02 04 43"
#define CH4     "00 03 02 AE 00 02 A5 83"
#define CHall   "00 03 02 A8 00 08 C5 85"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    chart->setTitle(QString::fromLocal8Bit("温度"));
    chart->legend()->hide();
    QPen green(Qt::green);
    chart->setBackgroundPen(green);
    chart->setAnimationOptions(QChart::AllAnimations);
    View->setRenderHint(QPainter::Antialiasing);
    View->resize(ui->graphicsView->width(),ui->graphicsView->height());

    ui->graphicsView->setScene(QGra);
    QGra->addWidget(View);
    serial = new QSerialPort;
    model = new QSqlTableModel(this);
    connect(chart,&Chart::C_tem,this,&Widget::tem_command);
    connect(this,&Widget::sendtem,chart,&Chart::gettemp);
    connect(this,&Widget::sendtem2,chart,&Chart::gettemp2);
    connect(this,&Widget::sendtem3,chart,&Chart::gettemp3);
    connect(this,&Widget::sendtem4,chart,&Chart::gettemp4);
    connect(this,&Widget::sendtemall,chart,&Chart::gettempAll);
    open_com();
    cfig = new CONFIG;
    dbfilename();
    rs36_info->chx=CH1;
}

Widget::~Widget()
{
    delete ui;
}


void Widget::open_com()
{
    serial->setPortName("COM5");//设置串口名
    serial->setBaudRate(QSerialPort::Baud9600);//设置波特率为115200
    serial->setDataBits(QSerialPort::Data8);//设置数据位8
    serial->setStopBits(QSerialPort::OneStop);//停止位设置为1
    serial->setFlowControl(QSerialPort::NoFlowControl);//设置为无流控制
    serial->setParity(QSerialPort::NoParity);//设置校验位
    serial->open(QIODevice::ReadWrite);//打开串口
    connect(serial,&QSerialPort::readyRead,this,&Widget::ReadData);

}
int32_t Widget::pow(int32_t x,int32_t y)
{
    int32_t tem=1;
    if(y==0){
        return 1;
    }
    else {
        for (int8_t i=0;i<y;i++) {
            tem=tem*x;
        }
        return tem;
    }
}
void Widget::tem_command()//判断发出的是什么指令
{
    static int8_t flag=0;
    if(revok)
    {
        serial->write(QString2Hex(rs36_info->chx));

//        if(flag++%2){
//            ui->sql_led->setPixmap(QPixmap(":./icons/button_green_24px.ico"));
//        }
//        else {
//            ui->sql_led->setPixmap(QPixmap(":./icons/button_red_24px.ico"));
//        }
        if(rs36_info->chx==CH1){
            tems_command=CH1;
            RY_or_TEM=TempCH1;
        }
        else if(rs36_info->chx==CH2){
            RY_or_TEM=TempCH2;
            tems_command=CH2;
        }
        else if(rs36_info->chx==CH3){
            RY_or_TEM=TempCH3;
            tems_command=CH3;
        }
        else if(rs36_info->chx==CH4){
            RY_or_TEM=TempCH4;
            tems_command=CH4;
        }
        else if(rs36_info->chx==CHall){
            RY_or_TEM=TempCHALL;
            tems_command=CHall;
        }
//        else if(rs36_info->chx==RY1_T||rs36_info->chx==RY1_F)
//            RY_or_TEM=RY1_MODLE;
//        else if(rs36_info->chx==RY2_T||rs36_info->chx==RY2_F)
//            RY_or_TEM=RY2_MODLE;
//        else if(rs36_info->chx==RY3_T||rs36_info->chx==RY3_F)
//            RY_or_TEM=RY3_MODLE;
//        else if(rs36_info->chx==RY4_T ||rs36_info->chx==RY4_F)
//            RY_or_TEM=RY4_MODLE;
//        else

        revok=false;
    }
}
bool Widget::CMPdata(QByteArray okdata,QByteArray data)
{
    for (int8_t i=0;i<okdata.length();i++) {
        if(data[i]==okdata[i]){
            continue;
        }
        else {
            return false;
        }
    }
    return true;
}

bool Widget::ReadDataok(QByteArray data)
{

    if(rs36_info->chx==CHall){
        QByteArray okdata="0103100000";
        return CMPdata(okdata,data);

    }
    else if(rs36_info->chx==CH1 || rs36_info->chx==CH2||
            rs36_info->chx==CH3 || rs36_info->chx==CH4){
        QByteArray okdata="0103040000";
        return CMPdata(okdata,data);
    }
    else if (RY_T || RY_F) {
        QByteArray okdata="000603";
        qDebug()<<data;
        return CMPdata(okdata,data);

    }
    return true;
}

int32_t Widget::ser2tem()
{
    int32_t tem;
    if(sermodbus.length()==18){
        qDebug()<<"sermodbus 18:"<<sermodbus;
        if(ReadDataok(sermodbus)!=false){
            tem=ConvertHexChar(sermodbus[13])*pow(16,0)+ConvertHexChar(sermodbus[12])*pow(16,1)+
                ConvertHexChar(sermodbus[11])*pow(16,2)+ConvertHexChar(sermodbus[10])*pow(16,3);
            RY_or_TEM=RY_TEMERR;
            sermodbus.clear();
            return tem;
        }
        qDebug()<<"ReadDataok:"<<ReadDataok(sermodbus);
    }
    return 0;

}
void Widget::ser2alltem()
{
//    qDebug()<<"sermodbus:"<<sermodbus;
    if(sermodbus.length()==42){
//        qDebug()<<"sermodbus 42:"<<sermodbus;
        if(ReadDataok(sermodbus)!=false){
            int32_t tem1=ConvertHexChar(sermodbus[13])*pow(16,0)+ConvertHexChar(sermodbus[12])*pow(16,1)+
                ConvertHexChar(sermodbus[11])*pow(16,2)+ConvertHexChar(sermodbus[10])*pow(16,3);


            int32_t tem2=ConvertHexChar(sermodbus[21])*pow(16,0)+ConvertHexChar(sermodbus[20])*pow(16,1)+
                ConvertHexChar(sermodbus[19])*pow(16,2)+ConvertHexChar(sermodbus[18])*pow(16,3);

            int32_t tem3=ConvertHexChar(sermodbus[29])*pow(16,0)+ConvertHexChar(sermodbus[28])*pow(16,1)+
                ConvertHexChar(sermodbus[27])*pow(16,2)+ConvertHexChar(sermodbus[26])*pow(16,3);

            int32_t tem4=ConvertHexChar(sermodbus[37])*pow(16,0)+ConvertHexChar(sermodbus[36])*pow(16,1)+
                ConvertHexChar(sermodbus[35])*pow(16,2)+ConvertHexChar(sermodbus[34])*pow(16,3);
            sendtemall(double(tem1)/100,double(tem2)/100,double(tem3)/100,double(tem4)/100);
            mysqlite->ADD_tem_value(double(tem1)/100,double(tem2)/100,double(tem3)/100,double(tem4)/100);
            ui->temch1->setText(QString::number(double(tem1)/100, 'f', 2));
            ui->temch2->setText(QString::number(double(tem2)/100, 'f', 2));
            ui->temch3->setText(QString::number(double(tem3)/100, 'f', 2));
            ui->temch4->setText(QString::number(double(tem4)/100, 'f', 2));

            //            qDebug()<<"CH1:"<<tem1;
//            qDebug()<<"CH2:"<<tem2;
//            qDebug()<<"CH3:"<<tem3;
//            qDebug()<<"CH4:"<<tem4;
            RY_or_TEM=RY_TEMERR;
            sermodbus.clear();
            revok=true;
        }
    }
}

bool Widget::Ryok()
{
    if(sermodbus.length()==16){
        if(ReadDataok(sermodbus)!=false){

            revok=true;
            RY_or_TEM=RY_TEMERR;
            rs36_info->chx=tems_command;
//            qDebug()<<sermodbus;
            sermodbus.clear();
            return true;
        }
    }
    return false;
}
void Widget::ReadData()
{
    int32_t tem;
    ser_buf=serial->readAll();
    if(!ser_buf.isEmpty()){
        sermodbus.append(ser_buf.toHex());
    }else{
        return;
    }

    switch(RY_or_TEM)
    {
        case TempCH1://通道1数值
            tem=ser2tem();
            if(tem!=0){
                rs36_info->temp_value[0]=tem;
                ui->temch1->setText(QString::number(double(tem)/100, 'f', 2));
                qDebug()<<"tem1"<<double(tem)/100;
                sendtem(double(tem)/100);
                revok=true;
            }
        break;
        case TempCH2://通道2数值
            tem=ser2tem();
            if(tem!=0){
                rs36_info->temp_value[2]=tem;
                ui->temch2->setText(QString::number(double(tem)/100, 'f', 2));
                qDebug()<<"tem2"<<double(tem)/100;
                sendtem2(double(tem)/100);
                revok=true;
            }
        break;
        case TempCH3://通道3数值
            tem=ser2tem();
            if(tem!=0){
                rs36_info->temp_value[3]=tem;
                ui->temch3->setText(QString::number(double(tem)/100, 'f', 2));
                qDebug()<<"tem3"<<double(tem)/100;
                sendtem3(double(tem)/100);
                revok=true;
            }
        break;
        case TempCH4://通道4数值
            tem=ser2tem();
            if(tem!=0){
                rs36_info->temp_value[3]=tem;
                ui->temch4->setText(QString::number(double(tem)/100, 'f', 2));
                qDebug()<<"tem4"<<double(tem)/100;
                sendtem4(double(tem)/100);
                revok=true;
            }
        break;

        case TempCHALL://通道all数值
            ser2alltem();
        break;
        case RY1_MODLE://继电器1
            if(Ryok()==true){
                if(rs36_info->chx==RY1_T){
                    qDebug()<<"RY1_MODLE-------T";
                }
                else {
                    qDebug()<<"RY1_MODLE-------F";
                }
            }


        break;
        case RY2_MODLE://继电器2
            if(Ryok()==true){
                if(rs36_info->chx==RY2_T){
                    qDebug()<<"RY2_MODLE-------T";
                }
                else {
                    qDebug()<<"RY2_MODLE-------F";
                }
            }

        break;
        case RY3_MODLE://继电器3
            if(Ryok()==true){
                if(rs36_info->chx==RY3_T){
                    qDebug()<<"RY3_MODLE-------T";
                }
                else {
                    qDebug()<<"RY3_MODLE-------F";
                }

            }

        break;
        case RY4_MODLE://继电器4
            if(Ryok()==true){
                if(rs36_info->chx==RY4_T){
                    qDebug()<<"RY4_MODLE-------T";
                }
                else {
                    qDebug()<<"RY4_MODLE-------F";
                }
            }

        break;

    }

}


//void Widget::sqlok()
//{
//    qDebug()<<"sqlok+++++++";
//}
//将单个字符串转换为hex
//0-F -> 0-15
char Widget::ConvertHexChar(char c)
{
    if((c >= '0') && (c <= '9'))
        return c - 0x30;
    else if((c >= 'A') && (c <= 'F'))
        return c - 'A' + 10;//'A' = 65;
    else if((c >= 'a') && (c <= 'f'))
        return c - 'a' + 10;
    else
        return -1;
}


//字符串转Hex(QByteArray)类型
QByteArray Widget::QString2Hex(QString hexStr)
{
    QByteArray senddata;
    int hexdata, lowhexdata;
    int hexdatalen = 0;
    int len = hexStr.length();
    senddata.resize(len/2);
    char lstr, hstr;
    for(int i = 0; i < len; )
    {
        //将第一个不为' '的字符赋给hstr;
        hstr = hexStr[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        //当i >= len时，跳出循环
        if(i >= len)
            break;
        //当i < len时，将下一个字符赋值给lstr;
        lstr = hexStr[i].toLatin1();
        //将hstr和lstr转换为0-15的对应数值
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        //
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata * 16 + lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
    return senddata;
}




void Widget::on_comboBox_currentIndexChanged(int index)
{
    switch(index)
    {
        case 0:
            View->chart()->setTheme(QChart::ChartThemeLight);
            return;
        case 1:
            View->chart()->setTheme(QChart::ChartThemeBlueCerulean);
            return;
        case 2:
            View->chart()->setTheme(QChart::ChartThemeDark);
            return;
        case 3:
            View->chart()->setTheme(QChart::ChartThemeBrownSand);
            return;
        case 4:
            View->chart()->setTheme(QChart::ChartThemeBlueNcs);
            return;
        case 5:
            View->chart()->setTheme(QChart::ChartThemeHighContrast);
            return;
        case 6:
            View->chart()->setTheme(QChart::ChartThemeBlueIcy);
            return;
        case 7:
            View->chart()->setTheme(QChart::ChartThemeQt);
            return;
    }
}

void Widget::on_start_motion_clicked()
{
    chart->Astart();
}

void Widget::on_stop_motion_clicked()
{
    chart->Astop();
}

void Widget::on_horizontalSlider_sliderMoved(int position)
{
    chart->setTime(position);
}



void Widget::on_Select_ch_activated(int index)
{
    chart->setshowx(index);
    switch(index)
    {
        case 0:
            rs36_info->chx=CH1;
            RY_or_TEM=TempCH1;
            tems_command=rs36_info->chx;
            return;
        case 1:
            rs36_info->chx=CH2;
            RY_or_TEM=TempCH2;
            tems_command=rs36_info->chx;
            return;
        case 2:
            rs36_info->chx=CH3;
            RY_or_TEM=TempCH3;
            tems_command=rs36_info->chx;
            return;
        case 3:
            rs36_info->chx=CH4;
            RY_or_TEM=TempCH4;
            tems_command=rs36_info->chx;
            return;
        case 4:
            rs36_info->chx=CHall;
            RY_or_TEM=TempCHALL;
            tems_command=rs36_info->chx;
            return;

    }
}

void Widget::on_CH1color_activated(int index)
{
    chart->SetcolorCH1(index);
}
void Widget::on_CH2color_2_activated(int index)
{
    chart->SetcolorCH2(index);
}
void Widget::on_CH3color_3_activated(int index)
{
    chart->SetcolorCH3(index);
}
void Widget::on_CH4color_4_activated(int index)
{
    chart->SetcolorCH4(index);
}
void Widget::on_push_RY1_clicked()
{
    static bool flag=true;
    if(flag==true){
        ui->push_RY1->setIcon(QIcon(":./icons/G_MG.ico"));
        rs36_info->chx=RY1_T;
        flag=false;
    }else {
        ui->push_RY1->setIcon(QIcon(":./icons/R_MG.ico"));
        rs36_info->chx=RY1_F;
        flag=true;
    }
    RY_or_TEM=RY1_MODLE;
    serial->write(QString2Hex(rs36_info->chx));

}


void Widget::on_push_RY2_clicked()
{
    static bool flag=true;
    if(flag==true){
        ui->push_RY2->setIcon(QIcon(":./icons/G_MG.ico"));
        rs36_info->chx=RY2_T;
        flag=false;
    }else {
        ui->push_RY2->setIcon(QIcon(":./icons/R_MG.ico"));
        rs36_info->chx=RY2_F;
        flag=true;
    }
    RY_or_TEM=RY2_MODLE;
    serial->write(QString2Hex(rs36_info->chx));

}

void Widget::on_push_RY3_clicked()
{
    static bool flag=true;
    if(flag==true){
        ui->push_RY3->setIcon(QIcon(":./icons/G_MG.ico"));
        rs36_info->chx=RY3_T;
        flag=false;
    }else {
        ui->push_RY3->setIcon(QIcon(":./icons/R_MG.ico"));
        rs36_info->chx=RY3_F;
        flag=true;
    }
    RY_or_TEM=RY3_MODLE;
    serial->write(QString2Hex(rs36_info->chx));

}

void Widget::on_push_RY4_clicked()
{

    static bool flag=true;
    if(flag==true){
        ui->push_RY4->setIcon(QIcon(":./icons/G_MG.ico"));
        rs36_info->chx=RY4_T;
        flag=false;
    }else {
        ui->push_RY4->setIcon(QIcon(":./icons/R_MG.ico"));
        rs36_info->chx=RY4_F;
        flag=true;
    }
    RY_or_TEM=RY4_MODLE;
    serial->write(QString2Hex(rs36_info->chx));

}

void Widget::on_ini_write_clicked()
{
    cfig->WriteSettings();
}

void Widget::on_ini_read_clicked()
{
    cfig->ReadSettings();
}


void Widget::sqlitefind()
{
    QString sqlcommand=tr("../sertest/data/%1.db").arg(ui->dbname->currentText());
    qDebug()<<"sqlcommand"<<sqlcommand;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","WCH");

    db.setDatabaseName(sqlcommand);
    qDebug()<<"+++++++++";
    if( !db.open() ) //数据库打开失败
    {
        qDebug()<< QString::fromLocal8Bit("错误")<<db.lastError().text();
        return;
    }
    model->setTable("temp_and_ry");//指定使用哪个表
    qDebug()<<"-------------";
    ui->tableView->setModel(model);
    qDebug()<<"11111111111111";
    //显示model里的数据
    model->select();
    qDebug()<<"222222222222222";
    model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("时间"));
    model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("CH1温度(℃)"));
    model->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("CH2温度(℃)"));
    model->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("CH3温度(℃)"));
    model->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("CH4温度(℃)"));
        //设置model的编辑模式，手动提交修改
    qDebug()<<"33333333333333";
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    qDebug()<<"4444444444";
        //    ui->dbname->
        dbfilename();

}
void Widget::on_FindButton_clicked()
{
    //把model放在view
    sqlitefind();
}

void Widget::on_sqltest_clicked()
{
    static double i=1;
    mysqlite->ADD_tem_value(i+0.6,i+1.5,i+2.3,i+3.3);
    i=i+4;
}




void Widget::on_TRIhandle_clicked()
{
    ui->TRIhandle->setCheckState(Qt::Checked);
    ui->TRItem->setCheckState(Qt::Unchecked);
    ui->TRItime->setCheckState(Qt::Unchecked);
    static bool flag=true;
    if(flag){
        flag=false;


    }else {
        flag=true;
    }
}

void Widget::on_TRItem_clicked()
{
    ui->TRIhandle->setCheckState(Qt::Unchecked);
    ui->TRItem->setCheckState(Qt::Checked);
    ui->TRItime->setCheckState(Qt::Unchecked);
    static bool flag=true;
    if(flag){
        flag=false;
    }else {
        flag=true;
    }
}

void Widget::on_TRItime_clicked()
{
    ui->TRIhandle->setCheckState(Qt::Unchecked);
    ui->TRItem->setCheckState(Qt::Unchecked);
    ui->TRItime->setCheckState(Qt::Checked);
    static bool flag=true;
    if(flag){
        flag=false;
    }else {
        flag=true;
    }
}


void Widget::on_YSlider_2_valueChanged(int value)
{
    Slider_2=value;
    if(Slider>Slider_2){
         chart->setYvalue(Slider_2,Slider);
    }

    else if(Slider==Slider_2){
        chart->setYvalue(Slider_2,Slider+1);
    }
    else {
        chart->setYvalue(Slider,Slider_2);
    }
}

void Widget::on_YSlider_valueChanged(int value)
{
    Slider=value;
    if(Slider>Slider_2){
         chart->setYvalue(Slider_2,Slider);
    }
    else if(Slider==Slider_2){
        chart->setYvalue(Slider_2,Slider+1);
    }
    else {
           chart->setYvalue(Slider,Slider_2);
    }
}
void Widget::dbfilename()
{
    static int file_nu=65534;//初始值大于
    QStringList filter;
     filter<<"*.db";
     QDir *dir=new QDir("../sertest/data/");//查看文件夹下的db数据库文件。
        QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
        if(file_nu<=fileInfo->count())
        {
            for(int i = file_nu;i<fileInfo->count(); i++)
            {
                QString file=fileInfo->at(i).fileName();
                file.chop(3);
                qDebug()<<file;
                ui->dbname->addItem(file);
            }
        }else {
            for(int i = 0;i<fileInfo->count(); i++)
            {
                QString file=fileInfo->at(i).fileName();
                file.chop(3);
                qDebug()<<file;
                ui->dbname->addItem(file);
            }
        }
        file_nu=fileInfo->count();
}
