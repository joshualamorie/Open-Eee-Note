#ifndef FORREADERWIDGET_H
#define FORREADERWIDGET_H


#include"basewidget.h"
#include<QTimer>
#include<QImage>
class ForReaderWidget:public BaseWidget
{
    Q_OBJECT
public:
    ForReaderWidget(QWidget*);
    ~ForReaderWidget();
    void setImage(QImage* image);
public slots:
   void toShow();
   void toHide();

protected:
   void paintEvent(QPaintEvent *);

private:
    QPixmap* m_backImg;
};

#endif // FORREADERWIDGET_H
