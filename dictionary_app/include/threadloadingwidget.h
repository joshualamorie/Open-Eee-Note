#ifndef THREADLOADINGWIDGET_H
#define THREADLOADINGWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include<QShowEvent>
#include<QLabel>
#include"animalthread.h"
#include"basewidget.h"

#include"sendtitlemessage.h"

class ThreadLoadingWidget : public BaseWidget
{
public:
    ThreadLoadingWidget(QWidget*);
    ~ThreadLoadingWidget();


public:
    void toShow();
    void toHide();
    void setMsg(const QString& msg);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void showEvent(QShowEvent *);
    void paintEvent(QPaintEvent *);
private:
      QRect m_offsetRect;
      QLabel* m_bkLabel;
      QLabel* m_msgLabel;

      AnimalThread m_loading;
      QImage* m_bkImage;
      SendTitleMessage m_sendTitleMsg;

};

#endif // THREADLOADINGWIDGET_H
