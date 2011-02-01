#ifndef COMMONBUTTON_H
#define COMMONBUTTON_H

#include <QPushButton>

class CommonButton : public QPushButton
{
public:
    CommonButton(QWidget* );

public:
    void setBtnStyle(const QString& p_normal, const QString& p_hover,
                          const QString&p_press, const QString& p_disable);
};

#endif // COMMONBUTTON_H
