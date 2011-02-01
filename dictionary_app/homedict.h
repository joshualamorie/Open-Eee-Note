/*
  Copyright (C) 2010 ASUSTeK Computer Inc. All rights reserved.
*/


#ifndef HOMEDICT_H
#define HOMEDICT_H

#include <QtGui>
#include <QtGui/QTextEdit>
#include "ui_dictionary_home.h"
#include "CDictionaryCollectorItem.h"
#include "bottombarwidget.h"
#include <QPainter>
#include "searchbuttonwidget.h"


// 9 for 6", 13 for 9"
#define MAXIMUM_DICTIONARIES_NUMBER		12
#define NUM_SUGGESTION_WORDS			8
#define DEFAULT_SUGGESTION_FONT_SIZE	18
#define MAXCOMBOBOXPOPUPLIST_WIDTH		400

#define SCREENWIDTH						768
#define SCREENHEIGHT					1024

#define EXPLANATIONAREAHEIGHT			774
#define EXPLANATIONAREASHOWHEIGHT		(EXPLANATIONAREAHEIGHT - 44)
#define EXPLANATIONAREASHOWX			8
#define EXPLANATIONAREASHOWWIDTH		(SCREENWIDTH - 50)
#define SUGGESTIONWORDITEMHEIGHT		100 //100

#define EXPLANATIONAREAFONTSIZE			24

class CExplanationArea;


class CHomeDictionary: public QDialog, public Ui::DictionaryDialog
{
    Q_OBJECT
    public:
        CHomeDictionary();
        ~CHomeDictionary();

		void	ClearSuggestionList();
		void	SetSuggestionWord(int nIndex, QString suggestionWord="");
		void	SetSuggestionWordExplanation(int nIndex, QString suggestionWordExplanation);
		QString GetSuggestionWord(int nIndex);
		QString GetSuggestionWordExplanation(int nIndex);
		void	ClearInputText();
		void	ClearInputFocus();
		void	SetInputFocus();
		void	SetInputText(QString inputWord);
		QString GetInputText();
		void	addItem(CDictionaryCollectorItem * item);

		void	setClickedPreButton();
		void	setClickedNextButton();

		void    backupToolbarNum(int left ,int right, int current){m_toolbar_left =left; m_toolbar_right=right; m_toolbar_current =current; };
		int     getToolbar_left(){ return m_toolbar_left; };
		int     getToolbar_right(){ return m_toolbar_right; };
		int		getToolbar_current(){ return m_toolbar_current;};

		bool	eventFilter( QObject * watched, QEvent * event );

		void	getPreTitleInfo();
		void	renewTitleInfo(QString path);// send change status title notify messages
		void	AppToHide();

		int		getLastIndex(){ return m_lastIndex;};
		void	setItemHighLighted(int index);

		void	InitialSelectedFlag();
		void	RenewBottomBarPage();
		void	keyPressEvent (QKeyEvent *e);
		void	keyReleaseEvent (QKeyEvent *e);
		void	SlipLeft();
		void	SlipRight();


	public:
		QLabel				*wordLabel;
		CExplanationArea	*explanationArea;

		QGroupBox			*wordExplanationGroupBox;

		ENoteToolBar::BottomBarWidget*	m_toolBar;
		int		m_toolbar_left; // backup toolbar left and right number
		int		m_toolbar_right;
		int		m_toolbar_current;

		SearchButtonWidget *m_searchBar;
		JLLineEdit * m_stextInput;
		int		m_iCurrentPage;
		int		m_iExplanationPageCount;

		int		m_ExplanUseHeight;
		//JL_ADD: record vertical pixels=maxRow*perlineheight
		//to resolve half line display.
		QString	m_PreTitle; //form dictionary
		int		m_currentHighlight;

		bool	m_slipEnabled;
		bool	m_isEndofsuggestwords;
		int		m_EndofswordsLastIndex;
                bool            m_bShowMoreItem;

	private:
		QString	strSuggestionWordsArr[NUM_SUGGESTION_WORDS + 1];
		QString	strSuggestionWordsExplanationArr[NUM_SUGGESTION_WORDS + 1];
		std::vector<CDictionaryCollectorItem*>	m_item;
		QWidget		*wordExplanationVLayoutWidget;
		QVBoxLayout	*wordExplanationLayout;
                int		m_lastIndex; //for item index in a page


	public slots:
		//void	clickMoreButton(int iIndex);
		void	clicktoolbarButton(int iIndex);
		//void	changeClearButtonState(QString str);
		void	setLastIndex( int index){ m_lastIndex = index;};
		void	ItemPressed_EventHandle(int index);
		void	ItemReleased_EventHandle(int index);
	signals:
		void	clickItemMoreButton(int iIndex);
		void	clickedPreButton();
		void	clickedNextButton();

		//void showpopup(QString strTitle, QString str);

		void	AppExit();
};


class CExplanationArea: public QWidget
{
	Q_OBJECT
	public:
		CExplanationArea(QWidget *p = 0):QWidget(p)
		{
			m_strExplanation = "";
			m_iStartPositionY = 0;
		};
		~CExplanationArea(){};

		void setText(QString str){m_strExplanation = str;}
		void setTextStartPositionY(int iStartPosY){m_iStartPositionY = iStartPosY;}
		void setRectDraw(QRect rc){m_rcDraw = rc;}

	protected:
		void paintEvent(QPaintEvent * event)
		{
			QPainter painter(this);
			//QString str="adfasdlfuyoretgeriudfmnkgnfdkluyheoprthdfghfdlk;hgeo;rihldkhfgaldghd;geo;uh;hgl;sdfhgsuybsgjghjsdhgsjdghvsvafdgaaaaaaaaagwefdsgdfgegd";
			//m_strExplanation =str;
			painter.drawText(m_rcDraw.x(), m_rcDraw.y() + m_iStartPositionY, m_rcDraw.width(), m_rcDraw.height() - m_iStartPositionY, Qt::AlignLeft | Qt::TextWrapAnywhere, m_strExplanation);//| Qt::AlignJustify

		};

	private:
		QString m_strExplanation;
		int		m_iStartPositionY;
		QRect	m_rcDraw;
};
#endif

