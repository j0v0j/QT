#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QGraphicsScene>
#include "chart.h"
#include <QtCharts/QChartView>
#include "config.h"
#include "temp_sqllite.h"


#define CH1     "00 03 02 A8 00 02 45 82"
#define CH2     "00 03 02 AA 00 02 E4 42"
#define CH3     "00 03 02 AC 00 02 04 43"
#define CH4     "00 03 02 AE 00 02 A5 83"
#define CHall   "00 03 02 A8 00 08 C5 85"

#define RY1_T   "00 06 03 d4 00 01 09 a7"
#define RY1_F   "00 06 03 d4 00 00 c8 67"

#define RY2_T   "00 06 03 d5 00 01 58 67"
#define RY2_F   "00 06 03 D5 00 00 99 A7"

#define RY3_T   "00 06 03 d6 00 01 a8 67"
#define RY3_F   "00 06 03 d6 00 00 69 a7"

#define RY4_T   "00 06 03 d7 00 01 f9 a7"
#define RY4_F   "00 06 03 d7 00 00 38 67"

#define RY_T   rs36_info->chx==RY1_T || rs36_info->chx==RY2_T||rs36_info->chx==RY3_T || rs36_info->chx==RY4_T
#define RY_F   rs36_info->chx==RY1_F || rs36_info->chx==RY2_F||rs36_info->chx==RY3_F || rs36_info->chx==RY4_F


#define TempCH1     1
#define TempCH2     2
#define TempCH3     3
#define TempCH4     4
#define TempCHALL   5
#define RY1_MODLE   6
#define RY2_MODLE   7
#define RY3_MODLE   8
#define RY4_MODLE   9
#define RY_TEMERR      10

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

typedef struct  temperature //假定数据库存储内容
{
    double temp_value[4];
    QString time;
    QString chx;
    QString RY_x[4];
    bool RY_xstatu[4];
}RS36_Info;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
signals:
    void sendtem(double);
    void sendtem2(double);
    void sendtem3(double);
    void sendtem4(double);
    void sendtemall(double,double,double,double);
private slots:
    void open_com();
    void ReadData();
    char ConvertHexChar(char c);
    QByteArray QString2Hex(QString hexStr);
    int32_t pow(int32_t x,int32_t y);
    bool CMPdata(QByteArray,QByteArray);

    bool ReadDataok(QByteArray);
    void on_comboBox_currentIndexChanged(int index);

    void on_start_motion_clicked();

    void on_stop_motion_clicked();

    void on_horizontalSlider_sliderMoved(int position);
    void tem_command();
    int32_t ser2tem();
    void ser2alltem();

    void on_Select_ch_activated(int index);

    void on_CH1color_activated(int index);
    void on_CH2color_2_activated(int index);
    void on_CH3color_3_activated(int index);
    void on_CH4color_4_activated(int index);
    void on_push_RY1_clicked();

    void on_push_RY2_clicked();

    void on_push_RY3_clicked();

    void on_push_RY4_clicked();

    void on_ini_write_clicked();

    void on_ini_read_clicked();




    void on_FindButton_clicked();

    void on_sqltest_clicked();

    void on_TRIhandle_clicked();

    void on_TRItem_clicked();

    void on_TRItime_clicked();
    bool Ryok();
    void on_YSlider_2_valueChanged(int value);

    void on_YSlider_valueChanged(int value);
    void dbfilename();
    void sqlitefind();
private:
    Ui::Widget *ui;
    QByteArray    sermodbus;
    QSerialPort *serial;
    QByteArray  ser_buf;//串口Buffer
    QGraphicsScene *QGra=new QGraphicsScene(this);
    Chart *chart = new Chart;
    bool revok=true;
    int8_t RY_or_TEM=0;
    int Slider_2;
    int Slider;
    RS36_Info *rs36_info= new RS36_Info;
    QChartView *View = new QChartView(chart);
    CONFIG *cfig;
    TEMP_SQLLITE *mysqlite= new TEMP_SQLLITE;
    QString tems_command;
    QSqlTableModel *model;
};
#endif // WIDGET_H
