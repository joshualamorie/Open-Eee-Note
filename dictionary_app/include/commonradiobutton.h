#ifndef COMMONRADIOBUTTON_H
#define COMMONRADIOBUTTON_H

#include <QRadioButton>

class CommonRadioButton : public QRadioButton
{
    Q_OBJECT;
public:
    CommonRadioButton(QWidget*);

public:
    void setBtnStyle(const QString& normal, const QString& selected,
                     const QString& disable);
};

#endif // COMMONRADIOBUTTON_H
