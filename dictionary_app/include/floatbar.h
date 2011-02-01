/**************************************************
  copyright:asus
  author:mars_li@asus.com.cn
**************************************************/

#ifndef FLOATBAR_H
#define FLOATBAR_H

#include <QWidget>
#include<QStringList>
#include<QPushButton>
#include<QLabel>
#include"customradiobutton.h"

namespace ENoteToolBar
{


    class FloatBar : public QWidget {
        Q_OBJECT
    public:
        FloatBar( QWidget *parent = 0);
        ~FloatBar();

    public:
        void setBaseItem(int baseId, QStringList strs);
        void setBtnText(int id, const QString& str);
        void setAutoClose(bool val);
        void toShow();
        void toHide();
        void setInstallFilter(bool val);
        void setEnable(int id, bool val);
        bool isRadioState(){return m_radioState;}

        int  getSelectId(){return m_selectId;}
        void setSelectId(int);
        void setRadioState(bool val);
        int getBaseId(){return m_baseId;}



    protected:
        void changeEvent(QEvent *e);
        void hideEvent(QHideEvent *);
        bool eventFilter(QObject *, QEvent *);
        void checkRadio();
        QString getStyle();

    signals:
        void clickButtonID(int id);
        void hideSignal();

    protected slots:
        void buttonClick();
        void init();


    private:
        bool m_radioState;
        bool m_binstallFilter;
        bool m_autoClose;
        int m_baseId;
        int m_selectId;
        QStringList m_btnList;
        CustomRadioButton* m_btnArry[5];
        QLabel* m_splitLabels[4];
        QLabel* m_background;
    };
}

#endif // FLOATBAR_H
