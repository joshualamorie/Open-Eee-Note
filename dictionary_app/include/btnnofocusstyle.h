#ifndef BTNNOFOCUSSTYLE_H
#define BTNNOFOCUSSTYLE_H

#include<QWindowsStyle>

class QPainter;

class BtnNoFocusStyle: public QWindowsStyle
{
    Q_OBJECT
public:
    BtnNoFocusStyle();

    void drawPrimitive(QStyle::PrimitiveElement which,
                       const QStyleOption* option,
                       QPainter* painter,
                       const QWidget* widget = 0 ) const;
};

#endif // BTNNOFOCUSSTYLE_H
