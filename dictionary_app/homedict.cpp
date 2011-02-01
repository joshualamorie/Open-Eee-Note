/*
  Copyright (C) 2010 ASUSTeK Computer Inc. All rights reserved.
*/

#include "homedict.h"
#include <QCopChannel>

#define __DEBUG__ qDebug() <<__func__<<"()  Line:"<< __LINE__ << "   "

CHomeDictionary::CHomeDictionary():m_currentHighlight(0)
{
    setupUi(this);          /*! Call function from ui_dictionary_home.h for Dictionary Home UI design */

	setFocusPolicy(Qt::ClickFocus);

	topLayout->setAlignment(Qt::AlignTop | Qt::AlignCenter); //
	groupBox->setLayout(topLayout); //group for sugguet words
    this->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_DeleteOnClose);

        m_bShowMoreItem = false;
        m_slipEnabled = true;
        m_isEndofsuggestwords =true;

	//initila search bar
	m_searchBar =new SearchButtonWidget(this);
	m_stextInput =m_searchBar->getLineEdit();
	m_searchBar->setGeometry(6, 8,m_searchBar->width(),m_searchBar->height());
	m_searchBar->setbtnSearchEnable(true);
	m_searchBar->setbtnArrowEnable(true);

	//verticalLayout->addWidget(m_searchBar);

	explanationArea = NULL;
	wordExplanationLayout = NULL;
	wordLabel = NULL;
	explanationArea = new CExplanationArea;
	wordLabel = new QLabel;

	wordLabel->setStyleSheet("border:none;background:rgb(235, 235, 235);");
	wordLabel->setFixedSize(SCREENWIDTH, 100);

	//QFont wordFont("Helvetica", 30, QFont::Bold);
	QFont wordFont;
#ifdef DEFINEMYLOCALSETFAMILY
	wordFont.setFamily(ALLTEXTFONT);
#endif
	wordFont.setPointSize(30);
	wordFont.setWeight(QFont::Bold);

	wordLabel->setFont(wordFont);

	//QFont explanationFont("Helvetica", EXPLANATIONAREAFONTSIZE, QFont::Normal);
	QFont explanationFont;
#ifdef DEFINEMYLOCALSETFAMILY
	explanationFont.setFamily(ALLTEXTFONT);
#endif
	explanationFont.setPointSize(EXPLANATIONAREAFONTSIZE);
	explanationFont.setWeight(QFont::Normal);

	explanationArea->setFont(explanationFont);
	explanationArea->setFixedSize(SCREENWIDTH, EXPLANATIONAREAHEIGHT);
	explanationArea->setRectDraw(QRect(EXPLANATIONAREASHOWX, 0, EXPLANATIONAREASHOWWIDTH, EXPLANATIONAREASHOWHEIGHT));

	wordExplanationGroupBox = new QGroupBox; //for detail explanations widget
	wordExplanationGroupBox->setObjectName(QString::fromUtf8("wordExplanationGroupBox"));
	wordExplanationGroupBox->setFixedSize(SCREENWIDTH, groupBox->height());
	wordExplanationGroupBox->setStyleSheet(QString::fromUtf8("background:rgb(235, 235, 235);\n"
"border:none;\n"
"margin-left:10px;margin-right:60px\n"
""));
	wordExplanationGroupBox->setGeometry(QRect(0, 70, SCREENWIDTH, groupBox->height()));

	wordExplanationVLayoutWidget = new QWidget(wordExplanationGroupBox);
	verticalLayoutWidget->setGeometry(QRect(-1, -1, 768, 871));

	wordExplanationLayout = new QVBoxLayout(wordExplanationVLayoutWidget);

	wordExplanationLayout->addWidget(wordLabel);
	wordExplanationLayout->addWidget(explanationArea);

	wordExplanationGroupBox->setLayout(wordExplanationLayout);
	wordExplanationGroupBox->hide();

	for(int i = 0; i < NUM_SUGGESTION_WORDS; i++)
	{
		//clear up strSuggestionWords Arr
		strSuggestionWordsArr[i] = "";
		strSuggestionWordsExplanationArr[i] = "";

		//create dynamic CollectorItem and add it to Homedict
		CDictionaryCollectorItem *cSuggestionWordsItem = new CDictionaryCollectorItem;
		cSuggestionWordsItem->setFixedSize(COLLECTITEMWIDTH, COLLECTITEMHIGHT);
		addItem(cSuggestionWordsItem);
	}
	if(m_item.size()>0)
		m_item.at(m_currentHighlight)->setSelect(true);

	QFont inputFont;
#ifdef DEFINEMYLOCALSETFAMILY
	inputFont.setFamily("Arial");
#endif
//	inputFont.setPointSize(14);
//	inputFont.setWeight(QFont::Normal);
//	m_stextInput->setFont(inputFont);

	//toolbar at bottem
	m_toolBar = new ENoteToolBar::BottomBarWidget(this);
	m_toolBar->setGeometry(0,925, m_toolBar->width(),m_toolBar->height());
	m_toolBar->setLeftArrowBtnEnable(false);
	m_toolBar->setRightArrowBtnEnable(false);
	m_toolBar->setCurPageCount(0);
	m_toolBar->setCaptureEvent(true);

	QStringList strName;
	strName<< tr("Back")<< tr("Home")<< tr("Enter") << tr("Snapshot")<< tr(" ");
	m_toolBar->setStringList(strName);

	m_toolBar->setEnable(0, true);
	m_toolBar->setEnable(1, true);
	m_toolBar->setEnable(2, true);
	m_toolBar->setEnable(3, true);
	m_toolBar->setEnable(4, false);


	m_iCurrentPage = 1;	//index of the suggest word pages
	m_iExplanationPageCount = 1; //index of the complete explanation page

	//connect(m_stextInput, SIGNAL(textChanged(QString)), this, SLOT(changeClearButtonState(QString)));
	connect(this->m_toolBar, SIGNAL(clickmenu(int)), this, SLOT(clicktoolbarButton(int)));

	this->setFocus();

	getPreTitleInfo(); //get current title
	QString StatusTitlepath;
	StatusTitlepath =tr("Dictionary");
	this->renewTitleInfo(StatusTitlepath);

}

CHomeDictionary::~CHomeDictionary()
{
	if (explanationArea != NULL)
	{
		delete explanationArea;
		explanationArea = NULL;
	}

	if (wordLabel != NULL)
	{
		delete wordLabel;
		wordLabel = NULL;
	}
	if (wordExplanationGroupBox != NULL)
	{
		delete wordExplanationGroupBox;
		wordExplanationGroupBox = NULL;
	}
	if (wordExplanationLayout != NULL)
	{
		delete wordExplanationLayout;
		wordExplanationLayout = NULL;
	}
	if (wordExplanationVLayoutWidget != NULL)
	{
		delete wordExplanationVLayoutWidget;
		wordExplanationVLayoutWidget = NULL;
	}
	for(int i = 0; i < m_item.size(); i++)
	{
		CDictionaryCollectorItem *cSuggestionWordsItem = m_item.at(i);
		if(cSuggestionWordsItem)
		{
			delete cSuggestionWordsItem;
			cSuggestionWordsItem = NULL;
		}
	}

	if(m_toolBar)
	{
		delete m_toolBar;
		m_toolBar = NULL;
	}
	if(m_searchBar)
	{
		delete m_searchBar;
		m_searchBar = NULL;
	}
	CDictionaryCollectorItem::UnLoadImageRes();
}

void CHomeDictionary::SlipLeft()
{
	int tempIndex;
	if(!this->groupBox->isHidden())
	{
		tempIndex=this->m_currentHighlight;
		tempIndex--;
		if(this->m_iCurrentPage > 1)
		{

			if(tempIndex<0)
			{
				tempIndex =NUM_SUGGESTION_WORDS-1;
				clicktoolbarButton(6);
			}
			this->setItemHighLighted(tempIndex);
		}
		else if(this->m_iCurrentPage ==1)
		{
			if(tempIndex<0)
			{
				tempIndex =0;
			}
			this->setItemHighLighted(tempIndex);
		}
	}
	// detail explation
	if(!wordExplanationGroupBox->isHidden())
	{
		if(this->m_iCurrentPage > 1)
		{
			clicktoolbarButton(6);
			qDebug() << "slipclicked 6 =press left" << "\n";
		}
	}
}
void CHomeDictionary::SlipRight()
{
	int tempIndex;
	if(!this->groupBox->isHidden())
	{
		tempIndex=this->m_currentHighlight;
                qDebug() << "m_currentHighlight: " <<  m_currentHighlight << "\n";
		tempIndex++;
                qDebug() << "m_isEndofsuggestwords: " <<  m_isEndofsuggestwords << "\n";
                if(!m_isEndofsuggestwords)
		{
			if(tempIndex>NUM_SUGGESTION_WORDS-1)
			{
                            qDebug() << "tempIndex>NUM_SUGGESTION_WORDS-1: " << "\n";
                                tempIndex =0;
				clicktoolbarButton(7);
			}
		}
		else
		{
			if(tempIndex> this->m_EndofswordsLastIndex-1)
			{
                            qDebug() << "tempIndex> this->m_EndofswordsLastIndex-1 : " << "\n";
				tempIndex = this->m_EndofswordsLastIndex-1;
			}
		}
                qDebug() << "tempIndex: " <<  tempIndex << "\n";
		this->setItemHighLighted(tempIndex);
	}
	// detail explation
	if(!this->wordExplanationGroupBox->isHidden())
	{
		if(this->m_iCurrentPage <this->m_iExplanationPageCount)
		{
			clicktoolbarButton(7);
			qDebug() << "Explanation 7 =press right" << "\n";
		}
	}
}
void CHomeDictionary::keyPressEvent (QKeyEvent *e)
{
	switch (e->key())
	{
        case Qt::Key_F6:
		if(m_slipEnabled)
		{
                    qDebug()<<"e->key()==Qt::Key_F6";
			SlipLeft();
			m_slipEnabled=false;
		}
		break;
	case Qt::Key_F12:
		if(m_slipEnabled)
		{
                    qDebug()<<"e->key()==Qt::Key_F12";
			SlipRight();
			m_slipEnabled=false;
		}
		break;
	default:
		QWidget::keyPressEvent(e);
	}
}

void CHomeDictionary::keyReleaseEvent (QKeyEvent *e)
{
        qDebug() << "keyReleaseEvent \n";
	m_slipEnabled=true;
	QWidget::keyReleaseEvent(e);
}

bool CHomeDictionary::eventFilter( QObject * watched, QEvent * event )
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
				   !(list[i]->windowFlags() & 0x2))
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

void CHomeDictionary::ItemPressed_EventHandle(int index)
{
	qDebug()<< "press ...."<<index <<"\n";
	setItemHighLighted(index);
}

void CHomeDictionary::ItemReleased_EventHandle(int index)
{
	qDebug()<< "release ...."<<index <<"\n";
//	if(m_currentHighlight!=index)
//		return;
	emit  clickItemMoreButton(index);
}

void CHomeDictionary::addItem(CDictionaryCollectorItem * item)
{
	m_item.push_back(item);
	size_t size = m_item.size();
	m_item.at(size - 1)->setIndex(size - 1);
	QString text_int;
	connect(m_item.at(size - 1), SIGNAL(ItemPressed_Event(int)), this, SLOT(ItemPressed_EventHandle(int)));
	connect(m_item.at(size - 1), SIGNAL(ItemReleased_Event(int)), this, SLOT(ItemReleased_EventHandle(int)));

	if(size <= NUM_SUGGESTION_WORDS)
	{
		topLayout->addWidget(m_item.back());
		m_item.at(size - 1)->hide();
	}
}

void CHomeDictionary::setItemHighLighted(int index)
{
        qDebug() << "setItemHighLighted index: " << index << "\n";
        qDebug() << "setItemHighLighted m_currentHighlight: " << m_currentHighlight << "\n";
	if(m_currentHighlight !=index)
	{
                qDebug() << "m_item.at(m_currentHighlight)->setSelect(false): " << m_currentHighlight << "\n";
                m_item.at(m_currentHighlight)->setSelect(false);
		m_item.at(m_currentHighlight)->update();
		m_currentHighlight =index;

		int maxItem = m_item.size()<(NUM_SUGGESTION_WORDS)? m_item.size():(NUM_SUGGESTION_WORDS);
		if(m_currentHighlight <0)
			m_currentHighlight =0;
		else if(m_currentHighlight > maxItem-1)
			m_currentHighlight =maxItem-1;


                qDebug() << "m_item.at(m_currentHighlight)->setSelect(true): " << m_currentHighlight << "\n";
		m_item.at(m_currentHighlight)->setSelect(true);
		m_item.at(m_currentHighlight)->update();
	}
	else
	{
		m_item.at(m_currentHighlight)->update();
	}
}

void CHomeDictionary::ClearSuggestionList()
{
	for (int i = 0; i < NUM_SUGGESTION_WORDS; i++)
	{
		m_item.at(i)->hide();
		strSuggestionWordsArr[i] = "";
	}
	strSuggestionWordsArr[NUM_SUGGESTION_WORDS] = "";
}

void CHomeDictionary::SetSuggestionWord(int nIndex, QString suggestionWord)
{
	if(nIndex <= NUM_SUGGESTION_WORDS)
	{
		if(nIndex < NUM_SUGGESTION_WORDS)
		{
			m_item.at(nIndex)->setItemText(suggestionWord);
			m_item.at(nIndex)->show();
		}
		strSuggestionWordsArr[nIndex] = suggestionWord;
	}
}
//QString CHomeDictionary::GetSuggestionWord(int nIndex)
//{
//
//	return m_item.at(nIndex)->getItemText();
//}

void CHomeDictionary::SetSuggestionWordExplanation(int nIndex, QString suggestionWordExplanation)
{
	if(nIndex <= NUM_SUGGESTION_WORDS)
	{
		if(nIndex < NUM_SUGGESTION_WORDS)
		{
			m_item.at(nIndex)->setItemMoreText(suggestionWordExplanation);
		}
		strSuggestionWordsExplanationArr[nIndex] = suggestionWordExplanation;
	}
}

QString CHomeDictionary::GetSuggestionWord(int nIndex)
{
	if(nIndex <= NUM_SUGGESTION_WORDS)
	{
		return strSuggestionWordsArr[nIndex];
	}
	return "";
}

QString CHomeDictionary::GetSuggestionWordExplanation(int nIndex)
{
	if(nIndex <= NUM_SUGGESTION_WORDS)
	{
		return strSuggestionWordsExplanationArr[nIndex];
	}
	return "";
}

void CHomeDictionary::SetInputText(QString inputWord)
{
	m_stextInput->setText(inputWord);
	m_stextInput->setCursorPosition(0);
}

QString CHomeDictionary::GetInputText()
{
	return m_stextInput->text();
}

void CHomeDictionary::ClearInputText()
{
	m_stextInput->clear();
}

void CHomeDictionary::ClearInputFocus()
{
	m_stextInput->clearFocus();
}

void CHomeDictionary::SetInputFocus()
{
	m_stextInput->setFocus();
}

//void CHomeDictionary::clickMoreButton(int iIndex)
//{
//	emit clickItemMoreButton(iIndex);
//}

void CHomeDictionary::InitialSelectedFlag()
{
	m_currentHighlight=0;
	if(m_item.size()>0)
	{
		for(int i=0;i< m_item.size();i++)
			m_item.at(i)->setSelect(false);
		m_item.at(m_currentHighlight)->setSelect(true);
	}
}

void CHomeDictionary::getPreTitleInfo()
{
//	QProcessEnvironment pe = QProcessEnvironment::systemEnvironment();
//	QString path = pe.value("ENOTE_CONFIG_PATH");
//	path += "/server/.StatusBarIF";
	QString path = "/tmp/.StatusBarIF";
	//qDebug() << "path is "<< path;
	QFile file(path);
	if(file.open(QIODevice::ReadOnly))
	{
		QString title, status;
		//qDebug() << "in file open";
		QTextStream out(&file);
		
		out >> status;
		//qDebug() << status;
		file.close();
		QStringList lines = status.split(QRegExp("\\n"));
		//qDebug() << "status is " << status;
		for (QStringList::Iterator it = lines.begin();it != lines.end(); it++)
		{
			int pos = (*it).indexOf('=') + 1;
			if (pos < 1)
				continue;
			if ((*it).startsWith("Title="))
			{
				title = (*it).mid(pos);
				m_PreTitle =title;
				qDebug() << "get pre title is " << title;
				//msg.append("BSSID:\t" + (*it).mid(pos) + "\n");
				//   qDebug() << "BSSID is " << bssid_connect;
			}
		}
	}
}

void CHomeDictionary::renewTitleInfo(QString path)
{
	#ifdef Q_WS_QWS
		QByteArray appcmd;
		QDataStream out(&appcmd, QIODevice::WriteOnly);
		out << path;
		QCopChannel::send("statusbar", "title changed", appcmd);
		QCopChannel::flush();
	#endif
		qDebug() << "send title << >> " << path <<"\n";
}

void CHomeDictionary::AppToHide()
{
	renewTitleInfo(m_PreTitle); //restroe Title

#ifdef Q_WS_QWS
	QByteArray appcmd;
	QDataStream out(&appcmd, QIODevice::WriteOnly);
	out << "dictionary";
	QCopChannel::send("launcher", "App/hide", appcmd);
	QCopChannel::flush();
#endif
	qDebug()<<"send message: "<< "launcher, App/hide\n";
}


void CHomeDictionary::clicktoolbarButton(int iIndex)
{
	if(this->m_stextInput->hasFocus())
		this->setFocus();
	switch(iIndex)
	{
	case 0:
	{
		//MENU_BACK
		if(!groupBox->isHidden())
		{
			renewTitleInfo(m_PreTitle); //restroe Title
			//AppToHide(); //restore title and send hide msg to
			//this->m_searchBar->clearInputText();
			this->hide();
			{
			#ifdef Q_WS_QWS
				QByteArray appcmd;
				QDataStream out(&appcmd, QIODevice::WriteOnly);
				out << "dictionary";
				QCopChannel::send("eeeserver", "app/clickUP", appcmd);
				QCopChannel::flush();
			#endif
			}
			//emit AppExit();
		}
		else if(!wordExplanationGroupBox->isHidden())
		{// is showing
			wordExplanationGroupBox->hide();

                        m_bShowMoreItem = false;
                        QString StatusTitlepath;
                        StatusTitlepath =tr("Dictionary");
                        renewTitleInfo(StatusTitlepath);

			m_toolBar->setRightArrowBtnEnable(true);
			m_toolBar->setLeftArrowBtnEnable(true);
			m_iCurrentPage = getToolbar_current();
			if(getToolbar_right() ==0)
				m_toolBar->setRightArrowBtnEnable(false);
			if(getToolbar_left() ==0)
				m_toolBar->setLeftArrowBtnEnable(false);
			m_toolBar->setCurPageCount(m_iCurrentPage);
			m_toolBar->setCaptureEvent(true);

			m_toolBar->setEnable(2, true);
			this->m_searchBar->clearInputText();
			groupBox->show();
		}

	}
		break;
	case 1:

		AppToHide(); //restore title and send hide msg to eeeserver
		{
			#ifdef Q_WS_QWS
				QByteArray appcmd1;
				QDataStream out1(&appcmd1, QIODevice::WriteOnly);
				out1 << "dictionary";
				QCopChannel::send("eeeserver", "app/raiseLauncher", appcmd1);
				QCopChannel::flush();
				qDebug()<<"send message: "<< "eeeserver, app/raiseLauncher\n";
			#endif
		}
		//this->hide();

		break;
	case 2:
		//MENU_ENTER = 2,
                emit clickItemMoreButton(m_currentHighlight);
		qDebug()<< "Enter Index =="<<m_currentHighlight<<"\n";
		break;
	case 3:
		//SCREEN SHOT = 3,
		{
			QByteArray appcmdShot;
			QDataStream outShot(&appcmdShot, QIODevice::WriteOnly);
                        outShot << QString("dictionary");
		#ifdef Q_WS_QWS
			QCopChannel::send("eeeserver", "app/screenshot", appcmdShot);
			qDebug()<<"send message: "<< "eeeserver, app/screenshot \n";
			QCopChannel::flush();
		#endif
		}
		break;
	case 4:
		//MENU_MORE_NOTE = 4
		break;
	case 6:
		//MENU_ARROW_LEFT = 6,
		if(!groupBox->isHidden())
		{//check if at suggesion word mode
			emit clickedPreButton();
		}
		if(!wordExplanationGroupBox->isHidden())
		{//check if at detail explanation mode
			m_iCurrentPage--;
			//explanationArea->setTextStartPositionY(-(m_iCurrentPage - 1) * EXPLANATIONAREASHOWHEIGHT);
			explanationArea->setTextStartPositionY(-(m_iCurrentPage - 1) * m_ExplanUseHeight);
			explanationArea->update();

			RenewBottomBarPage();
		}
		break;
	case 7:
		//MENU_ARROW_RIGHT = 7,
		if(!groupBox->isHidden())
		{
			emit clickedNextButton();
		}
		if(!wordExplanationGroupBox->isHidden())
		{
			m_iCurrentPage++;
			//explanationArea->setTextStartPositionY(-(m_iCurrentPage - 1) * EXPLANATIONAREASHOWHEIGHT);
			explanationArea->setTextStartPositionY(-(m_iCurrentPage - 1) * m_ExplanUseHeight);
			explanationArea->update();

			RenewBottomBarPage();
		}
		break;
	}
}

void CHomeDictionary::RenewBottomBarPage()
{
	m_toolBar->setRightArrowBtnEnable(true);
	m_toolBar->setLeftArrowBtnEnable(true);
	m_toolBar->setCurPageCount(m_iCurrentPage);
	if(!wordExplanationGroupBox->isHidden())
	{
		if(m_iCurrentPage == m_iExplanationPageCount)
			m_toolBar->setRightArrowBtnEnable(false);
	}
	if(m_iCurrentPage == 1)
		m_toolBar->setLeftArrowBtnEnable(false);
}
void CHomeDictionary::setClickedPreButton()
{
	if(this->m_iCurrentPage !=1)
		emit clickedPreButton();
}

void CHomeDictionary::setClickedNextButton()
{
	emit clickedNextButton();
}

//void CHomeDictionary::changeClearButtonState(QString str)
//{
//	if(str != "")
//	{
//		clearButton->setEnabled(true);
//	}
//	else
//	{
//		clearButton->setEnabled(false);
//	}
//}
