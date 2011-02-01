/**************************************************
  copyright:asus
  author:mars_li@asus.com.cn
**************************************************/
#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QPushButton>
#include<QWindowsStyle>
#include<QTimeLine>

namespace ENoteToolBar
{
    class CustomButtonStyle: public QWindowsStyle
    {
        Q_OBJECT
    public:
        CustomButtonStyle();

        void drawPrimitive(QStyle::PrimitiveElement which,
                           const QStyleOption* option,
                           QPainter* painter,
                           const QWidget* widget = 0 ) const;

    };

    class CustomButton : public QPushButton
    {
        Q_OBJECT
    public:
        CustomButton(QWidget* parent = 0);
        void setState(int state);
        void drawNormal();
        ~CustomButton();

    protected slots:

          void pressAnimal(int);
    protected:
        void mousePressEvent(QMouseEvent *e);
        void mouseReleaseEvent(QMouseEvent *e);
        void resizeEvent(QResizeEvent *);
        void paintEvent(QPaintEvent *);

        void drawDisable();
        QColor getTextColor();

    private:
         int m_curState; //1 normal 2 press 3 disable, 4 noactive
         QPixmap* m_bkPixmap;
         QImage* m_bkImage;
         QTimeLine m_timeLine;

    };
}

#endif // CUSTOMBUTTON_H
