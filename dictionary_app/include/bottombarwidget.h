#ifndef BOTTOMBARWIDGET_H
#define BOTTOMBARWIDGET_H

#include <QWidget>
#include <QPushButton>
#include<QLabel>
#include"longpressbutton.h"
#include"custombutton.h"
#include"customradiobutton.h"
#include"floatbar.h"

namespace ENoteToolBar
{
    class BottomBarWidget : public QWidget
    {
        Q_OBJECT
    public:
        class BottomItem{
        public:
            int baseID;
            int selectId;
            bool btnState[5];
            QStringList m_strs;
        };

    public:
        BottomBarWidget(QWidget*);

    public:
        void addFloatBar(int baseId, const QStringList& strs);
        void showFloatBar(int baseId);
        void setFloatBarRadioState(bool val);
        void mapHomeKey(bool val = true){m_bMapHomeKey = val;}

        void hideFloatBar();
        void setStringList(const QStringList& strs, int baseId = 0);
        void setString(int id, const QString& s, int baseId = 0);
        void setBaseBarId(int baseId);
        void setEnable(int id, bool flag, int baseId = 0);
        void setCurPageCount(int id );
        void setCurPercent(int count );
        void setLeftArrowBtnEnable(bool flag);
        void setRightArrowBtnEnable(bool flag);
        void hideArrowButton();
        void showArrowButton();
        int  getCurBaseId();
        void setFloatBarSelectId(int id, int baseId);
        int  getFloatBarSelectId(int baseId);
        void setCaptureEvent(bool isCapture);

    signals:
        void clickmenu(int menuId);
        void btnEnable(QPushButton*);


    public slots:
        void emitClick(int menuId);


    private slots:
        void buttonClick();
        void enableButton(QPushButton*);
        void rightLabelClick();
        void leftLabelClick();
        void floatBarhide();
        void closeKeyBoard();

    private:
        void init();
        QString formatBtnStyle(const QString& p_normal, const QString& p_hover,
                               const QString& p_press, const QString& p_disable);
        QString getNormalBtnStyle();

        bool eventFilter(QObject *, QEvent *);

    private:
        CustomButton* m_btnArry[5];
        QObject* m_btnSignal;
        QList<BottomItem> m_items;
        int  m_curBaseId;

        LongPressButton* m_btnLeft;
        LongPressButton* m_btnRight;

        int m_curPage;
        QLabel* m_pageLabel;
        bool m_isCapture;

        FloatBar* m_floatBar;

        bool m_closeKeyBoard;
        bool m_bMapHomeKey;
        bool m_hasPress;

    };
}

#endif // BOTTOMBARWIDGET_H
