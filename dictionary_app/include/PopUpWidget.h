/**************************************************
copyright:asus
author:cici_chen@asus.com.cn
**************************************************/
#pragma once
#include <QWidget>
#include <QRect>

namespace commonDlg
{
	typedef struct tagItemStruct
	{
		QString BtnNormal;
		QString BtnDown;
		QString BtnOver;
		QString BtnDisable;
		QString ItemName;
	}ItemStruct, *LPItemStruct;

	typedef QList<ItemStruct*> ItemList;

	class ItemWidget : public QWidget
	{
		Q_OBJECT

		int			mFontSize;
		ItemStruct*	mItem;
		int			mId;

		QPixmap		mIconNor;
		QPixmap		mIconDown;
		QPixmap		mIconOver;
		QPixmap		mIconDis;

		QSize		mIconSize;
		QSize		mItemNameSize;
		QSize		mItemSize;

		bool		mIsSelected;
		bool		mbView;
		bool		mIsDisable;
		bool		mbLastItem;
		bool		mbLineVisible;
		bool		mbSelectedAlways;

	public:
		ItemWidget(int i, ItemStruct* item, int fontSize, QWidget* parent = NULL);
		~ItemWidget();

		QSize GetItemSize();
		void  SetItemSize(QSize size);
		ItemStruct* GetItem(){return mItem;}
		bool		GetItemSelected(){return mIsSelected;}
		void		SetItemSelected(bool b){mIsSelected = b;}
		bool		GetItemViewState(){return mbView;}
		void		SetItemViewState(bool b){mbView = b;}
		bool		GetItemDisable(){return mIsDisable;}
		void		SetItemDisable(bool b){mIsDisable = b;}
		void		SetLastItem(bool bLast){mbLastItem = bLast;}
		void		SetLineVisible(bool b){mbLineVisible = b;}
        void        SetItemString(QString str){mItem->ItemName = str;}
        void        SetItemIcon(QString str){mItem->BtnDisable = mItem->BtnDown =
                                             mItem->BtnNormal = mItem->BtnOver = str;
                                             LoadItems();}
		void		SetItemSelectedAlways(bool b){mbSelectedAlways = b;}

	protected:
		virtual void paintEvent(QPaintEvent * event);
		virtual void mousePressEvent(QMouseEvent * event);
		virtual void mouseReleaseEvent(QMouseEvent * event);

	private:
		void LoadItems();

	signals:
		void clickItemDown_Event(int);
		void clickItem_Event(int);

		public slots:
	};




	class PopUpWidget : public QWidget
	{
		Q_OBJECT
			
	int					mIndex;
	QPoint				mPos;
	QString				mTitle;
	int					mFontSize;
	int					mItemCount;
	QList<ItemWidget*>	mItemWidgetList;

	QPoint				mPtTitle;
	QSize				mTitleSize;
	QSize				mWidgetSize;
	QRect				mRcWidget;
	QList<QPoint>		mTriangleList;
	int					mSelectItemIndex;

	public:
		PopUpWidget(int index, QPoint Pos, QString title, int fontSize, ItemList itemList, QWidget* parent = NULL);
		~PopUpWidget();
		QSize GetWidgetSize(){return mWidgetSize;}
		QRect GetWidgetPos(){return mRcWidget;}
		void ReInitPopUpWidget(int index, QPoint Pos, QString title, int fontSize, ItemList itemList);
		void Refresh();
		void SetSelectItemIndex(int n);
		void SetWidgetItemSelectedAlways(bool bAlways);
		void SetItemDisable(int n, bool b);
		void SetItemViewState(int n, bool b);
		void SetLineVisible(int n, bool b);
        void ResetItemString(int index, QString str);
        void ResetItemIcon(int index, QString str);

	protected:
		virtual void paintEvent(QPaintEvent * event);
		virtual void focusOutEvent(QFocusEvent * event);
		virtual void mousePressEvent(QMouseEvent * event);
		virtual void mouseReleaseEvent(QMouseEvent * event);
		virtual void mouseMoveEvent(QMouseEvent * event);

	private:
		void InitWidget(int index, QPoint Pos, QString title, int fontSize, ItemList itemList);
		void InitItems(ItemList itemList);
		void SetItemWidgetPos();
		void InitWidgetSize();
		void InitTrianglePos();

	signals:
		void clickWidget_Event(int);
		void PopUpWidgetVisible_Event(int);

	public slots:
		void clickItemDown_EventHandler(int);
		void clickItem_EventHandler(int);
	};
}