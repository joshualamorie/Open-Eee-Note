/**************************************************
  copyright:asus
  author:mars_li@asus.com.cn
**************************************************/
#ifndef LOADINGWIDGET_H
#define LOADINGWIDGET_H

#include"basewidget.h"
#include<QTimer>
#include<QPixmap>
#include<QShowEvent>
#include"sendtitlemessage.h"
#include<QLabel>
#include<mytextlabel.h>

class LoadingWidget:public BaseWidget
{
    Q_OBJECT
public:
    LoadingWidget(QWidget *parent = 0 );
    ~LoadingWidget();

public:
    void setBackground(QImage* image);
    void setBackground(QString& path);
    void setMsg(const QString& msg);
    void setTransparent(bool val); //true transparent, false translucence



public slots:
   void toShow();
   void toHide();
   void toCaptureAndShow();
   void canRotate();


protected slots:
    void timeout();

protected:
    void startAnimal();
    void stopAnimal();
    virtual void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *);
    void init();

private:
    bool m_state;

    QRect m_offsetRect;
    MyTextLabel* m_msgLabel;
    MyTextLabel* m_titleLabel;


    bool m_transState;
    bool m_rotate;

    SendTitleMessage m_sendTitleMsg;

    QImage* m_bkImage;
    QImage* m_bookImage;
    QImage* m_loadImg[8];
    int m_curIndex;
    QTimer* m_timer;
private slots:
    void orientationChanged();
};

#endif // LOADINGWIDGET_H
