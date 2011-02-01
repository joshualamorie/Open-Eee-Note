/**************************************************
  copyright:asus
  author:mars_li@asus.com.cn
**************************************************/
#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include<QWidget>
#include<QPaintEvent>
#include<QShowEvent>

#include"capturebackground.h"
class BaseWidget:public QWidget, public CaptureBackground
{
public:
    BaseWidget(QWidget* );
    ~BaseWidget();

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void showEvent(QShowEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);



};

#endif // BASEWIDGET_H
