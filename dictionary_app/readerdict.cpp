/*
  Copyright (C) 2010 ASUSTeK Computer Inc. All rights reserved.
*/


#include "readerdict.h"

ReaderDict::ReaderDict(QWidget *parent): QDialog(parent, Qt::ToolTip)
{
	setupUi(this);           //!< Call function from ui_dictionary_reader.h for Reader's dictionary result displaying dialog UI design

	/*! Pressing toDictionaryButton shows the main dictionary application */
	connect(toDictionaryButton, SIGNAL(clicked()), this, SLOT(toDictionary()));
	/*! Pressing closeReaderDictButton hides the result dialog */
	connect(closeReaderDictButton, SIGNAL(clicked()), this, SLOT(closeReaderDict()));

	m_pcPopupExplanationArea = new CExplanationArea(this);

	m_pcPopupExplanationArea->setFixedSize(POPUPEXPLANATIONAREAWIDTH, POPUPEXPLANATIONAREAHEIGHT);
	m_pcPopupExplanationArea->setGeometry(QRect(POPUPEXPLANATIONAREAPOSX, POPUPEXPLANATIONAREAPOSY, POPUPEXPLANATIONAREAWIDTH, POPUPEXPLANATIONAREAHEIGHT));
	m_pcPopupExplanationArea->setStyleSheet(QString::fromUtf8("border:none;\n"
"background-color:white;"));

	//QFont explanationFont("Helvetica", EXPLANATIONAREAFONTSIZE, QFont::Normal);
	QFont explanationFont;
#ifdef DEFINEMYLOCALSETFAMILY
	explanationFont.setFamily(ALLTEXTFONT);
#endif
	explanationFont.setPointSize(POPUPEXPLANATIONAREAFONTSIZE);
	explanationFont.setWeight(QFont::Normal);

	m_pcPopupExplanationArea->setFont(explanationFont);
	m_pcPopupExplanationArea->setRectDraw(QRect(10, 0, POPUPEXPLANATIONAREAWIDTH - 20, POPUPEXPLANATIONAREAHEIGHT));

	QFont wordFont;
#ifdef DEFINEMYLOCALSETFAMILY
	wordFont.setFamily(ALLTEXTFONT);
#endif
	wordFont.setPointSize(POPUPWORDFONTSIZE);
	wordFont.setWeight(QFont::Bold);

	this->wordLabel->setFont(wordFont);

	QFont moreFont;
//#ifdef DEFINEMYLOCALSETFAMILY
//	moreFont.setFamily(ALLTEXTFONT);
//#endif
	moreFont.setPointSize(POPUPEXPLANATIONAREAFONTSIZE);
	moreFont.setWeight(QFont::Bold);

	moreFont.setUnderline(true);
	this->toDictionaryButton->setFont(moreFont);
	this->toDictionaryButton->setText(tr("more")); // International thanslations

	this->hide();
}
ReaderDict::~ReaderDict()
{
	if(m_pcPopupExplanationArea)
	{
		delete m_pcPopupExplanationArea;
		m_pcPopupExplanationArea=NULL;
	}
}

void ReaderDict::toDictionary()
{
	this->hide();
    emit goToDictionary();
}

bool ReaderDict::eventFilter( QObject * watched, QEvent * event )
{
	if(event->type() == QEvent::ApplicationDeactivate)
	{
		if(event->type() == QEvent::ApplicationDeactivate)
		{
			qDebug()<<"main deactive";
			QWidgetList list = qApp->topLevelWidgets();
			for(int i =0;i<list.length();i++)
			{
				if(list[i] != this && (list[i]->windowFlags() & 0x8) &&
				   !(list[i]->windowFlags() & 0x2))  //不需要隱藏主窗口,隐藏非dialog且是popup的窗口
				{
					list[i]->hide();
					qDebug() << list[i]->metaObject()->className();
				}
			}
		}
		return false;
	}
	return false;
}

void ReaderDict::closeReaderDict()
{

	this->hide();
}
