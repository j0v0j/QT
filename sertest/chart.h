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

#ifndef CHART_H
#define CHART_H

#include <QtCharts/QChart>
#include <QtCore/QTimer>

QT_CHARTS_BEGIN_NAMESPACE
class QSplineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

//![1]
class Chart: public QChart
{
    Q_OBJECT
public:
    Chart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~Chart();
    void Astop();
    void SHOW();
    void SHOW2();
    void SHOW3();
    void SHOW4();
    void SHOWall();

    void Astart();
    void setTime(int);
    void setshowx(int);
    void setYvalue(int,int);
signals:
    void C_tem();
//    void led_ok()
public slots:
    void handleTimeout();
    void gettemp(double);
    void gettemp2(double);
    void gettemp3(double);
    void gettemp4(double);
    void gettempAll(double,double,double,double);
    void SetcolorCH1(int index);
    void SetcolorCH2(int index);
    void SetcolorCH3(int index);
    void SetcolorCH4(int index);

private:
    QTimer *m_timer=new QTimer(this);
    QSplineSeries *m_series;
    QSplineSeries *m_series2;
    QSplineSeries *m_series3;
    QSplineSeries *m_series4;
    QStringList m_titles;
    QValueAxis *m_axis;
    qreal rel_t=NULL;
    qreal rel_t2=NULL;
    qreal rel_t3=NULL;
    qreal rel_t4=NULL;
    qreal m_step;
    qreal m_x;
    qreal m_y;
    qreal m_x2;
    qreal m_y2;
    qreal m_x3;
    qreal m_y3;
    qreal m_x4;
    qreal m_y4;
    int32_t x=0;
    int showx=0;

};
//![1]

#endif /* CHART_H */
