#ifndef CUSTOMRADIOBUTTON_H
#define CUSTOMRADIOBUTTON_H

#include <QWidget>
#include<QPushButton>
#include <QRadioButton>
#include<QMouseEvent>

class CustomRadioButton : public QPushButton
{
public:
    CustomRadioButton(QWidget*);
    void setMyCheck(bool val);
    bool getMyCheck(){return m_isChecked;}

protected:
    void initBtnStyle();
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *);

private:
    bool m_isChecked;
    bool m_mouseDown;

};

#endif // CUSTOMRADIOBUTTON_H
