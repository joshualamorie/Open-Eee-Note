/**************************************************
copyright:asus
author:cici_chen@asus.com.cn
**************************************************/
#pragma once
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include "Helper.h"

namespace commonDlg
{
	class MenuButton : public QWidget
	{
		Q_OBJECT
	private:
		QString		mBtnNormal, mBtnDown, mBtnOver, mBtnDisable; //different skins of the button
		QString		mMenuItemName;
		int			mMaxMenuItemLen;
		bool		mIsBtnDown;
		bool		mIsBtnDisale;
		int			mNumLine;

		QSize		mIconSize;
		QSize		mItemNameSize;
		QSize		mItemSize;

		QPixmap		mIconNor;
		QPixmap		mIconDown;
		QPixmap		mIconOver;
		QPixmap		mIconDis;

	public:
		MenuButton(QString btnNormal, QString btnDown, QString btnOver, QString btnDisable, 
			QString menuName, int maxLen, QWidget* parent = NULL);
		~MenuButton();

		void ReInitMenuButton(QString menuName);
		QSize getWidgetSize(){return mItemSize;}
		void SetBtnDisable(bool bDisable){mIsBtnDisale = bDisable; update(); }

	protected:
		virtual void mousePressEvent (QMouseEvent * event);
		virtual void mouseReleaseEvent (QMouseEvent * event);
		virtual void mouseDoubleClickEvent(QMouseEvent * event);
		virtual void paintEvent (QPaintEvent * event);

	private:
		void InitItems();
		void setWidgetSize();
		int getActualWidth();
		int getActualHeight();
		QRectF getTextRect();

	signals:
		void clickWidget();

	public slots:
		void mouseDown_EventHandler(QPoint pt);
		void mouseUp_EventHandler(QPoint pt);
		void mouseDoubleClick_EventHandler(QMouseEvent *event);
	};
}
