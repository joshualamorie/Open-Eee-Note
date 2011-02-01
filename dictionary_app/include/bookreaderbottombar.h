#ifndef BOOKREADERBOTTOMBAR_H
#define BOOKREADERBOTTOMBAR_H

#include "toolbarwidget.h"
namespace ENoteToolBar{
    class BookReaderBottomBar : public ToolBarWidget
    {
        Q_OBJECT
    public:

        BookReaderBottomBar(QWidget*);

        virtual void configClickLabel();

        void SetLeftPercent(int val);
        void SetRightPercent(int val);

        void SetLeftEnable(bool flag);
        void SetRightEnable(bool flag);

    signals:
 //       void labelClicked(bool bLeft);

    protected slots:
 //       void leftLabelClicked();
 //       void rightLabelClicked();

    };
}


#endif // BOOKREADERBOTTOMBAR_H
