/*
  Copyright (C) 2010 ASUSTeK Computer Inc. All rights reserved.
*/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
#include <QEvent>
#include <QPushButton>

class MousePressEater : public QObject
{
    Q_OBJECT
    public:
        MousePressEater(QObject * p) : QObject(p) {}

    protected:
        bool eventFilter(QObject *obj, QEvent *event)
        {
            QEvent::Type t = event->type();
            switch( t )
            {
                case QEvent::MouseButtonDblClick:
                {
                    QPushButton * pb = static_cast<QPushButton *>(obj);
                    pb->click();
                    return true;
                };
                case QEvent::Enter:
                    return true;
                // case QEvent::MouseButtonPress:
                // case QEvent::MouseButtonRelease:
                // case QEvent::HoverEnter:
                default:
                {
                     // standard event processing
                     return QObject::eventFilter(obj, event);
                }
            }
         }
};

extern MousePressEater *mousePressEater;

#endif
