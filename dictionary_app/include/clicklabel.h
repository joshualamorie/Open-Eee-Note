#ifndef CLICKLABEL_H
#define CLICKLABEL_H

#include <QLabel>

namespace ENoteToolBar{
    class ClickLabel : public QLabel
    {
        Q_OBJECT
    public:
        ClickLabel(QWidget*);
    protected:
        void mousePressEvent ( QMouseEvent * );
    signals:
        void clicked();
    };

}

#endif // CLICKLABEL_H
