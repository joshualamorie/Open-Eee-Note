#ifndef SLIDERWIDGET_H
#define SLIDERWIDGET_H

#include <QWidget>
//#include <QSlider>
#include <stdio.h>
#include <QPushButton>
#include <QLabel>
#include "slider.h"
#include "toolbarwidget.h"

namespace ENoteToolBar
{
    class SliderWidget : public QWidget
    {
        Q_OBJECT
    public:
        SliderWidget(QWidget* parent = 0);
        ~SliderWidget();

        void SetValue(int value);
        void setRange(int min, int max);
        void setFormatStr(QString& s);
        int GetValue();

    signals:
        void pageChanged(int);


    private slots:
        void closeClicked();
        void pageValueChanged(int);
        void moveValue(int);

    protected:
        void paintEvent(QPaintEvent *event);


    private:
        void configControls();
        void configSignals();
        QString getCloseBtnStyle();
        QString getSliderStyle();

    private:
        Slider *m_pSlider;
        QPushButton *m_pCloseBtn;
        QLabel *m_pScale;
        QLabel *m_pPerText;

        int m_maxPages;




        ENoteToolBar::ToolBarStyle m_sliderStyle;
    };
}


#endif // SLIDERWIDGET_H
