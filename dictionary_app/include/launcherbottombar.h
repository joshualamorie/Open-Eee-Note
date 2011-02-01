#ifndef LAUNCHERBOTTOMBAR_H
#define LAUNCHERBOTTOMBAR_H

#include <QWidget>
#include "toolbarwidget.h"
#include "floatbar.h"

#include <QPoint>
#include<QStringList>

/**************************************************
  copyright:asus
  author:mars_li@asus.com.cn
**************************************************/

namespace ENoteToolBar
{


    class LauncherBottomBar : public QWidget
    {

        Q_OBJECT
    public:
        LauncherBottomBar(QWidget* parent = 0);

        ~LauncherBottomBar();
        void setPopupLeve1MenuPos(QPoint pos);
        void setPopupLeve2MenuPos(QPoint pos);

        void setLeftPage(int pages);
        void setRightPage(int pages);
        void setCaptureEvent(bool isCapture);
        //id range is 0-4
        void setBtnEnable(int id, bool flag);

        void changeState(int state, int layout = 0);
        int  getCurState();
        void setStateStrings(int state, QStringList str);
        void backState();

        void configStrings();
        void setString(int id, QString str);


    public:
        void hidePopupMenu();
        void setStrings(QStringList strs);

    public:
        static  void initResource();

    protected:
        void initMenu();
        void resizeEvent(QResizeEvent * event);
        int  initSignalMap();


    signals:
        void clickmenu(int menuId);

    public slots:
        void bottomMenuClick(int menuId);
        void floatBarClick(int id);

    private:

        ToolBarWidget* m_ptoolBar;


        QPoint m_floatBar_level_1_pos;
        QPoint m_floatBar_level_2_pos;

        FloatBar* m_pfloatBar_level_1;
        FloatBar* m_pfloatBar_level_2;

        QMap<int, QVector<int> > m_signalMap;

    };
}

#endif // LAUNCHERBOTTOMBAR_H
