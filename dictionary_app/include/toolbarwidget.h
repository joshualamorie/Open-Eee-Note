/**************************************************
  copyright:asus
  author:mars_li@asus.com.cn
**************************************************/

#ifndef TOOLBARWIDGET_H
#define TOOLBARWIDGET_H

#include <QWidget>
#include<QPushButton>
#include<QLabel>
#include<QStringList>
#include<QGridLayout>
#include<QWindowsStyle>
#include<QStyle>
#include "clicklabel.h"
#include<QStack>
#include<QMap>
#include<QVector>
#include"custombutton.h"
#include"longpressbutton.h"

/**************************************************

  the button corresponding to 1-6 from left to right
  the 1-6 button emit signal corresponding from btnClick_1-6

  if change button sytle reimplement or modify getBtnStyle function
  if change label style reimplement or modify getLabelStyle bunction

  author:mars_li@asus.com.cn
**************************************************/

namespace ENoteToolBar
{

    class ToolBarStyle : public QWindowsStyle
    {
        Q_OBJECT
    public :
        ToolBarStyle();

        void drawPrimitive(QStyle::PrimitiveElement which,
                           const QStyleOption* option,
                           QPainter* painter,
                           const QWidget* widget = 0 ) const;

    };

    enum BottomBarState{
        s_NORMAL,
        s_RECENTUSED,
        s_MORE,
		s_SORT,
		s_SORT_ASC,
		s_SORT_DESC,
        s_TAG,
        s_CATEGORY,
        s_HOME,
        s_HOME_ListView,
        s_HOME_GalleryView,
        s_NOTE_NORMAL,
        s_NOTE_MORE,
		s_NOTE_MORE_MORE,
		s_NOTE_MORE_TagList,
        s_NOTE_MORE_SORT,
		s_NOTE_MoveToView,
		s_NOTE_SEARCH,
		s_NOTE_TEMPLATE,
        s_READER_NORMAL,
        s_READER_MORE,
        s_READER_MORE_RECENTUSED,
        s_READER_MORE_SORT,
        s_READER_MORE_CATEGORY,
    };



#define BTN_COUNT   5

    class ToolBarWidget : public QWidget
    {
         Q_OBJECT
    public:
        ToolBarWidget(QWidget* parent);
        ~ToolBarWidget();

    signals:
        void clickmenu(int menuId);
        void btnEnable(QPushButton*);


    private slots:
        void buttonClick();
        void rightLabelClick();
        void leftLabelClick();
        void enableButton(QPushButton*);


    public:
        void setBackgroundImage(QImage* image);
        void initWidget();
        void setLeftPage(int id);
        void setRightPage(int id);
        void setStrings(QStringList strs);
        void setString(int id, QString str);
        void setEnable(int id, bool flag);


        ///////////////////////////////////////

        void hideArrowButton();
        void showArrowButton();


        //set the title of someone button


        ///
        void changeState(int state, int layout = 1);
        int  getCurState();
        void setStateStrings(int state, QStringList str);
        void backState();


        int getSignalVal(int buttonId);
        void setCaptureEvent(bool isCapture);

    protected:
        QString getBtnStyle();
        QString getLabelStyle();
        QString getLeftBtnStyle();
        QString getRightBtnStyle();

        void configLabel();





    protected:
        CustomButton* m_btnArry[BTN_COUNT];

        void initSignalConnect();
        void configButton();
        void changeButtonStyle(int layout);
        void changeButtonStyleLayout1();
        void changeButtonStyleLayout2();
        void changeButtonStyleLayout3();
        QString formatBtnStyle(const QString& p_normal, const QString& p_hover,
                               const QString& p_press, const QString& p_disable, int layout = 0);

        bool eventFilter(QObject *, QEvent *);

        LongPressButton* m_btnLeft;
        LongPressButton* m_btnRight;




    private:
        QImage* m_bkImage;

        QGridLayout* m_gridLayout;



        bool m_isCapture;

        int m_curState;
        int m_curLayout; //current layout default = 0

        ToolBarStyle m_btnStyle;

        QMap<int , QStringList> m_stateStrings;


        QStack<int> m_stateStack;

    };
}




#endif // TOOLBARWIDGET_H
