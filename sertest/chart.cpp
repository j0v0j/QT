/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "chart.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QTime>
#include <QtCore/QDebug>

#include "QPointF"


Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    m_series(0),m_series2(0),m_series3(0),m_series4(0),
    m_x(1),m_y(1),m_x2(1),m_y2(1),m_x3(1),m_y3(1),m_x4(1),m_y4(1),
    m_axis(new QValueAxis),m_step(0)
{
    qsrand((uint) QTime::currentTime().msec());
    QObject::connect(m_timer, &QTimer::timeout, this, &Chart::handleTimeout);
    m_timer->setInterval(3000);

    m_series = new QSplineSeries(this);
    m_series2 = new QSplineSeries(this);
    m_series3 = new QSplineSeries(this);
    m_series4 = new QSplineSeries(this);
    QPen green(Qt::red);
    green.setWidth(1);

    m_series->setPen(green);
    m_series->append(m_x, m_y);

    m_series2->setPen(green);
    m_series2->append(m_x2, m_y2);

    m_series3->setPen(green);
    m_series3->append(m_x3, m_y3);

    m_series4->setPen(green);
    m_series4->append(m_x4, m_y4);

    addSeries(m_series);
    addSeries(m_series2);
    addSeries(m_series3);
    addSeries(m_series4);
    createDefaultAxes();
    setAxisX(m_axis, m_series);
    setAxisX(m_axis, m_series2);
    setAxisX(m_axis, m_series3);
    setAxisX(m_axis, m_series4);
    m_axis->setTickCount(2);
    axisX()->setRange(0, 20);
    axisY()->setRange(0, 50);

//    m_timer->start();
}

Chart::~Chart()
{

}
void Chart::SetcolorCH1(int index)
{
    if(index==0){
        QPen pencolor(Qt::red);
        pencolor.setWidth(1);
        m_series->setPen(pencolor);
    }else if(index==1){
        QPen pencolor(Qt::yellow);
        pencolor.setWidth(1);
        m_series->setPen(pencolor);
    }else if(index==2){
        QPen pencolor(Qt::blue);
        pencolor.setWidth(1);
        m_series->setPen(pencolor);
    }else if(index==3){
        QPen pencolor(Qt::black);
        pencolor.setWidth(1);
        m_series->setPen(pencolor);
    }else if(index==4){
        QPen pencolor(Qt::green);
        pencolor.setWidth(1);
        m_series->setPen(pencolor);
    }
    else {
        return;
    }
}
void Chart::SetcolorCH2(int index)
{
    if(index==0){
        QPen pencolor(Qt::red);
        pencolor.setWidth(1);
        m_series2->setPen(pencolor);
    }else if(index==1){
        QPen pencolor(Qt::yellow);
        pencolor.setWidth(1);
        m_series2->setPen(pencolor);
    }else if(index==2){
        QPen pencolor(Qt::blue);
        pencolor.setWidth(1);
        m_series2->setPen(pencolor);
    }else if(index==3){
        QPen pencolor(Qt::black);
        pencolor.setWidth(1);
        m_series2->setPen(pencolor);
    }else if(index==4){
        QPen pencolor(Qt::green);
        pencolor.setWidth(1);
        m_series2->setPen(pencolor);
    }
    else {
        return;
    }
}

void Chart::SetcolorCH3(int index)
{
    if(index==0){
        QPen pencolor(Qt::red);
        pencolor.setWidth(1);
        m_series3->setPen(pencolor);
    }else if(index==1){
        QPen pencolor(Qt::yellow);
        pencolor.setWidth(1);
        m_series3->setPen(pencolor);
    }else if(index==2){
        QPen pencolor(Qt::blue);
        pencolor.setWidth(1);
        m_series3->setPen(pencolor);
    }else if(index==3){
        QPen pencolor(Qt::black);
        pencolor.setWidth(1);
        m_series3->setPen(pencolor);
    }else if(index==4){
        QPen pencolor(Qt::green);
        pencolor.setWidth(1);
        m_series3->setPen(pencolor);
    }
    else {
        return;
    }
}

void Chart::SetcolorCH4(int index)
{
    if(index==0){
        QPen pencolor(Qt::red);
        pencolor.setWidth(1);
        m_series4->setPen(pencolor);
    }else if(index==1){
        QPen pencolor(Qt::yellow);
        pencolor.setWidth(1);
        m_series4->setPen(pencolor);
    }else if(index==2){
        QPen pencolor(Qt::blue);
        pencolor.setWidth(1);
        m_series4->setPen(pencolor);
    }else if(index==3){
        QPen pencolor(Qt::black);
        pencolor.setWidth(1);
        m_series4->setPen(pencolor);
    }else if(index==4){
        QPen pencolor(Qt::green);
        pencolor.setWidth(1);
        m_series4->setPen(pencolor);
    }
    else {
        return;
    }
}

void Chart::handleTimeout()
{
    emit C_tem();
    switch(showx){
        case 0:
            SHOW();
        break;
        case 1:
            SHOW2();
        break;
        case 2:
            SHOW3();
        break;
        case 3:
            SHOW4();
        break;
        case 4:
            SHOWall();
        break;
    }

    if(x-20>0)
        axisX()->setRange(x-20, x+5);
//    axisY()->setRange(Y-20, Y+5);
}
void Chart::setshowx(int index)
{
    showx=index;

}
void Chart::SHOW()
{
    m_series->append(++x, rel_t);
    m_series2->clear();
    m_series3->clear();
    m_series4->clear();
}
void Chart::SHOW2()
{
    m_series2->append(++x, rel_t2);
    m_series->clear();
    m_series3->clear();
    m_series4->clear();
}
void Chart::SHOW3()
{
    m_series3->append(++x, rel_t3);
    m_series2->clear();
    m_series->clear();
    m_series4->clear();
}
void Chart::SHOW4()
{
    m_series4->append(++x, rel_t4);
    m_series2->clear();
    m_series3->clear();
    m_series->clear();
}
void Chart::SHOWall()
{
    m_series->append(++x, rel_t);
    m_series2->append(x, rel_t2);
    m_series3->append(x, rel_t3);
    m_series4->append(x, rel_t4);
}

void Chart::Astop()
{
    m_timer->stop();

}

void Chart::Astart()
{
    m_timer->start();
}
void Chart::setTime(int nu)
{
    m_timer->setInterval(nu);
}
void Chart::gettemp(double t)
{
    rel_t=t;
}
void Chart::gettemp2(double t)
{
    rel_t2=t;
}
void Chart::gettemp3(double t)
{
    rel_t3=t;
}
void Chart::gettemp4(double t)
{
    rel_t4=t;
}
void Chart::gettempAll(double t,double t2,double t3,double t4)
{
    rel_t=t;rel_t2=t2;rel_t3=t3;rel_t4=t4;

}
void Chart::setYvalue(int y1,int y2){
    axisY()->setRange(y1, y2);
}
