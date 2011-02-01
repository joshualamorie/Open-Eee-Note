/**************************************************
copyright:asus
author:paul_zhang@asus.com.cn
**************************************************/
#pragma once
#include <QWidget>
#include <QPainter>
#include "bottombarwidget.h"
#include "ManageData.h"
#include "TagModel.h"
#include "QLabel"

namespace commonDlg
{
	class Button : public QPushButton
	{
		Q_OBJECT
	public:
		Button(QString btnNormal, QString btnDown, QString btnOver, QString btnDisable, QWidget* parent= NULL);
		Button(QString text, QWidget* parent = NULL);
		QString getBtnStyle();
		protected slots:
			void mouseDoubleClickEvent ( QMouseEvent * event );   
			void mousePressEvent ( QMouseEvent * event );
	private:
		QString btnNormal, btnDown, btnOver, btnDisable; //different skins of the button

	};

	class TagListWidget : public QWidget
	{
		struct TagLine 
		{
			QList<SearchTag> TagList;
			char Title;
			bool  IsTitle;
			TagLine(char title, bool isTitle, QList<SearchTag> tagList = QList<SearchTag>())
				: TagList(tagList), Title(title), IsTitle(isTitle){}
			inline bool isValid() const {return Title != char('\0');}
		};

		class TagLineLayout
		{
			QList<SearchTag> mTagList;
			int mCurrentPos;
			int mLineCount;
			int mColumnCount;
			char mLastFirstAlpha;
		public:
			TagLineLayout(QList<SearchTag> tagList, int columnCount);
			TagLine CreateLine();
			inline int getLineCount() const {return mLineCount;}
		};


		Q_OBJECT
		static const QColor BgColor;
		static const QColor HgColor;

		int mPageIndex;
		QPixmap* mBuffer;
		TagModel* mTagModel;
		int* mPosMap;
		QList<SearchTag> mTagList;
		QPoint mLastPressed;
		int mLastPressedIndex;
		SearchOption mSearchOption;
		Button* mCloseLbl;
		QPair<QPoint, SearchTag> mLastTag;
		//SearchButtonWidget* mSearchButton;
		ENoteToolBar::BottomBarWidget* mBottomBar;
	public:
		TagListWidget(UserTagsType itemType, QWidget* parent);
		~TagListWidget(void);
		bool canForward() const;
		bool canBackward() const;
		void Forward();
		void Backward();
		bool canForwardOne() const;
		bool canBackwardOne() const;
		void ForwardOne();
		void BackwardOne();
		void UpdateBottomBar();
	protected:
		void Init();
		void ReDraw();
		void DrawTag(int left, int top, SearchTag tag, bool isDown);
		void DrawLine(QPainter* painter, QPoint leftTop, QPoint rightBottom);
		void paintEvent(QPaintEvent *e);
		void resizeEvent( QResizeEvent * event );
		void mousePressEvent(QMouseEvent *e);
		void mouseReleaseEvent(QMouseEvent *e);
		bool eventFilter(QObject *obj, QEvent *event);
	private:
		void HighlightTag( int index );
		void DrawTitle(QPainter* painter, char title, int lineIndex);
		void DrawLine(QPainter* painter, QList<SearchTag> tagList, int lineIndex);
	protected slots:
		void MenuClicked_EventHandler(int menuId);
		void Search_EventHandler();
		void ChangeToState(int state);
	signals:
		void TagSelected_Event(QString tagName);
		void Close_Event(bool isHome = false);
	};
}
