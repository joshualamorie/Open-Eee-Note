#ifndef LONGPRESSBUTTON_H
#define LONGPRESSBUTTON_H

#include <QPushButton>
#include<QWidget>

class LongPressButton : public QPushButton
{
    Q_OBJECT
public:
    LongPressButton(QWidget* );

signals:
    void normalClick();
    void longClick();

protected slots:
    void sReleased();

    protected:
    void mousePressEvent(QMouseEvent *e);
    void timerEvent(QTimerEvent *e);

private:
    bool m_bLong;
    int m_timerId;

};

#endif // LONGPRESSBUTTON_H
