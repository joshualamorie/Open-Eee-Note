#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QStyleOptionSlider>

namespace ENoteToolBar
{
    class Slider : public QSlider
    {
        Q_OBJECT
    public:
        Slider(QWidget * parent = 0);


     signals:
        void sliderClick();

    protected slots:
        void readyClick();

    protected:
        virtual void mousePressEvent(QMouseEvent * event);
        virtual void mouseReleaseEvent(QMouseEvent * event);
        virtual void mouseMoveEvent(QMouseEvent * event);
        virtual void paintEvent(QPaintEvent * event);

    private:
        bool  m_canClick;
        int m_curVal;

        QRect m_sliderRect;
        QStyleOptionSlider m_opt;
    };
}


#endif // SLIDER_H
