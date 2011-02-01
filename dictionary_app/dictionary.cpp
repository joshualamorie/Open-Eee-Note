/*
  Copyright (C) 2010 ASUSTeK Computer Inc. All rights reserved.
*/



#include <stdlib.h>
#include <signal.h>

#include "dictionary.h"
#include "homedict.h"
#include "readerdict.h"
#include "Config.h"
#include "scandict.h"
#include <sys/stat.h>
#include <QCopChannel>
#include <QPainter>
#include <QTimer>
#include <QTextCodec>
#include <string.h>
#include <QFontMetrics>
#include "commonfunction.h"

// #define DEFINEMYLOCALSETFAMILY

#define DEFAULTDICT "Longman English Dictionary"

#define __DEBUG__ qDebug() <<__func__<<"()  Line:"<< __LINE__ << "   "
/**
 * MyThread Class Definition
 */
class MyThread : public QThread
{
public:
	MyThread(Dictionary * dict);
protected:
	Dictionary *m_MyTdpDict;
	void run();
};

/**
 * Constructor of MyThread Class
 */
MyThread::MyThread(Dictionary *dict)
{
	m_MyTdpDict=dict;
}

/**
 *   Function Name: run()
 *   @brief load dictionaries in thread
 */
void MyThread::run()
{
	std::list <std::string> load_list;
	load_list.clear();
	QStringList::iterator i;

	for (i = m_MyTdpDict->m_dictionaryToLoad.begin(); i != m_MyTdpDict->m_dictionaryToLoad.end(); ++i)
	{
		//__DEBUG__ << "m_dictionaryToLoad is " << m_MyTdpDict->m_dictionaryToLoad[0] << "\n";
		load_list.push_back((*i).toStdString());    // encoding prob ?
		//load_list.push_front((*i).toStdString());
	}

	m_MyTdpDict->m_pLib->reload(load_list, m_MyTdpDict->m_iEnableCollationLevel, m_MyTdpDict->m_iCollateFunction);
}


static void MakeDirectory(const char *pPath)
{
	char str[256], *p;

	strcpy(str, pPath);
	p = str;
	while ((p = strchr(p + 1, '/')) != NULL)
	{
		*p = '\0';
		mkdir(str, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
		*p = '/';
	}
	mkdir(str, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
}


Dictionary::Dictionary(QString fullArgument)
{
	m_bRealSetComboBoxSel	= false;
	xOfPopupDlg			= -1;
	yOfPopupDlg			= -1;
	m_rcReaderSelectword.setRect(-1, -1, 0, 0);
//	m_isEndofsuggestwords =true;

#ifdef Q_WS_QWS
	m_channel = new QCopChannel("dictionary",this);
	connect(m_channel, SIGNAL(received(const QString &, const QByteArray &)),
			this, SLOT(MsgHandler(const QString &, const QByteArray &)));

	m_channelpower = new QCopChannel("qte/etablepowerchannel",this);
	connect(m_channelpower, SIGNAL(received(const QString &, const QByteArray &)),
			this, SLOT(MsgHandler(const QString &, const QByteArray &)));

	m_doVKBChannel = new QCopChannel("qte/etablevkbchannel", this);
	connect(m_doVKBChannel, SIGNAL(received(const QString &, const QByteArray &)),
			this, SLOT(MsgHandler(const QString &, const QByteArray &)));
#endif

	m_StarDictionaries.clear();
//	if(!m_lookUpWord.isEmpty())
//	{
//		QString strExplanation;
//		strExplanation = searchWordforPopup(m_lookUpWord);
//		dialogReader->wordLabel->setText(m_lookUpWord);
//		dialogReader->m_pcPopupExplanationArea->setText(strExplanation);
//		modeAction(m_dictionaryMode);
//	}
	HomeDictionary = new CHomeDictionary();
	if(HomeDictionary==NULL)
		return;
	m_pConfig = new CConfig();
	if(m_pConfig==NULL)
		return;
	m_pScanDict = new CScanDict();
	if(m_pScanDict==NULL)
		return;
	m_lDictionary = new MyThread(this);
	if(m_lDictionary==NULL)
		return;

	connect(m_lDictionary, SIGNAL(finished()), this, SLOT(dictionaryLoaded()));

	//connect(HomeDictionary->m_stextInput, SIGNAL(textChanged(QString)), this, SLOT(searchSuggestionWord(QString)));
	connect(HomeDictionary->m_searchBar, SIGNAL(search()), this, SLOT(search_Handler()));
	connect(HomeDictionary->m_stextInput, SIGNAL(textEdited(QString)), this, SLOT(searchSuggestionWord(QString)));
	connect(HomeDictionary->m_searchBar, SIGNAL(typeChanged()), this, SLOT(typeChanged_Handler()));
	connect(HomeDictionary->m_searchBar, SIGNAL(clearText()), this, SLOT(searchSuggestionWord()));
	//connect(HomeDictionary->clearButton, SIGNAL(clicked()), this, SLOT(clearInput()));
	connect(HomeDictionary, SIGNAL(clickItemMoreButton(int)), this, SLOT(clickMoreButton(int)));
	connect(HomeDictionary, SIGNAL(clickedPreButton()), this, SLOT(searchPreviouspageSeggustionWord()));
	connect(HomeDictionary, SIGNAL(clickedNextButton()), this, SLOT(searchNextpageSeggustionWord()));
	connect(HomeDictionary, SIGNAL(AppExit()), this, SLOT(exitDictionary()));

	m_bCreateCache = true;
	m_iEnableCollationLevel = 2;
	m_iCollateFunction = 0;
	m_pLib = new Libs(NULL, m_bCreateCache, m_iEnableCollationLevel, m_iCollateFunction);    /*!< Instance of Libs class */

	char dictDir[256];
	sprintf(dictDir, "%s/dictionary/dict/", getenv("ENOTE_BIN_PATH"));
	QStringList allDictionaryList = m_pScanDict->runScan(dictDir);   // Scan all the dictionaries present at a given location and store their ifo files path names
	QList<QString> dictlist;
	QFont fontname;
	fontname.setPointSize(18);
	QFontMetrics fmetric(fontname);
	QStringList::iterator j;
	for (j = allDictionaryList.begin(); j != allDictionaryList.end(); ++j)
	{
		StarDictionary currentStarDict;
		QString bookname="";
		bool bValidDict = true;
		if(m_pScanDict->loadBookname(*j, bookname))
		{// if non-supported dictionary, such as treedict then do not add this in collection

			if(bookname.isEmpty())
				bValidDict=false;
			for(int nDict = 0; nDict < m_StarDictionaries.size(); nDict++)
			{
				if(m_StarDictionaries[nDict].name == bookname)    //already exist
				{
					bValidDict = false;
					break;
				}
			}
			if(!bValidDict)
				continue;
			currentStarDict.ifoFileName = *j;
			currentStarDict.name = bookname;
			currentStarDict.bLoaded = false;
			currentStarDict.dictSelected = false;
			if(m_StarDictionaries.size() < MAXIMUM_DICTIONARIES_NUMBER)
			{
				m_StarDictionaries.push_back(currentStarDict);

				QString namestr =fmetric.elidedText(m_StarDictionaries.back().name,Qt::ElideRight,220);
				dictlist.append(namestr);

			}

		}
	}
	HomeDictionary->m_searchBar->setStrings(dictlist);



	char settingsDir[256];
	sprintf(settingsDir, "%s/dictionary", getenv("ENOTE_CONFIG_PATH"));
	__DEBUG__ <<" ENOTE_CONFIG_PATH" <<settingsDir << "\n";
	MakeDirectory(settingsDir);    // make dictionary settings directory

	if(m_StarDictionaries.size() == 0)
	{
		HomeDictionary->m_searchBar->setbtnArrowEnable(false);
		//HomeDictionary->dictionarycomboBox->setEnabled(false);
	}

	getSelectionMode();

	for(int nDict = 0; nDict < m_StarDictionaries.size(); nDict++)
	{
		if(m_StarDictionaries[nDict].dictSelected)
		{
			//__DEBUG__ << "nDict is" << nDict << "\n";
			m_bRealSetComboBoxSel = true;
			HomeDictionary->m_searchBar->setSelectItemId(nDict);
			//HomeDictionary->dictionarycomboBox->setCurrentIndex(nDict);
			if(nDict == 0)
			{
				seldictionaryChanged(0);
			}
			break;
		}
	}

	//create reader dictionary's pop up dialog
	dialogReader = new ReaderDict(HomeDictionary);
	dialogReader->setWindowTitle("DictionaryPopup");
	dialogReader->setFocusPolicy(Qt::NoFocus);
	connect(dialogReader, SIGNAL(goToDictionary()),this, SLOT(showDictionary()));

	qApp->installEventFilter(dialogReader);
	qApp->installEventFilter(HomeDictionary);

	m_pTimer = new QTimer;
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(notifyLauncherShowed()));

	m_SetFoucsTimer = new QTimer;
	connect(m_SetFoucsTimer, SIGNAL(timeout()), this, SLOT(Handler_SetInputFocus()));

	parseString(fullArgument);
	//parseString("-popup -x 0 -y 1023 book");

	if(m_dictionaryMode == APPLICATION_MODE)
	{// initial search "a" and display its results
		m_pTimer->start(10);
		m_SetFoucsTimer->start(50);
		QString strExplanation;
		m_lookUpWord ="a";
		searchSuggestionWord(m_lookUpWord);
	}
	modeAction(m_dictionaryMode);
}



Dictionary::~Dictionary()
{

	if(m_pTimer != NULL)
	{
		delete m_pTimer;
		m_pTimer = NULL;
	}
	if(m_SetFoucsTimer != NULL)
	{
		delete m_SetFoucsTimer;
		m_SetFoucsTimer = NULL;
	}
	if(m_pConfig != NULL)
	{
		delete m_pConfig;
		m_pConfig = NULL;
	}
	if(m_pScanDict)
	{
		delete m_pScanDict;
		m_pScanDict = NULL;
	}
	if(dialogReader != NULL)
	{
		delete dialogReader;
		dialogReader = NULL;
	}
	if(HomeDictionary)
	{
		delete HomeDictionary ;
		HomeDictionary = NULL;
	}
	if(m_lDictionary != NULL)
	{
		delete m_lDictionary;
		m_lDictionary = NULL;
	}
	if(m_pLib != NULL)
	{
		delete m_pLib;
		m_pLib = NULL;
	}
	if(m_pTimer != NULL)
	{
		delete m_pTimer;
		m_pTimer = NULL;
	}
	if(m_SetFoucsTimer != NULL)
	{
		delete m_SetFoucsTimer;
		m_SetFoucsTimer = NULL;
	}
	#ifdef Q_WS_QWS
		if(m_channel != NULL)
		{
			delete m_channel;
			m_channel = NULL;
		}
		if(m_channelpower != NULL)
		{
			delete m_channelpower;
			m_channelpower = NULL;
		}
		if(m_doVKBChannel != NULL)
		{
			delete m_doVKBChannel;
			m_doVKBChannel = NULL;
		}
	#endif

}

/*****************************************************************
  * form QCoChannel recive slip bar message then to do action
  *through processing clicktoolbarButton to finish the msg action
  ***************************************************************/
void Dictionary::Toolbar_slipclicked(int KeyIndexPress)
{
	if(this->HomeDictionary)
		this->HomeDictionary->clicktoolbarButton(KeyIndexPress);
}


/****************************************************************************
  * MsgHandler(): handler the messages form QCoChannel
  * It has define MsgHandler such as:
  * slip left/right; popup show; app/shutdown.
  **************************************************************************/
void Dictionary::MsgHandler(const QString &message, const QByteArray &data)
{
	/* -popup -x 100 -y 200 -rot 90 book or \"book\"  */

	QDataStream in(data);
	QString strdata;
	in >> strdata;
	int tempIndex;
	qDebug() << "message: " << message;
	qDebug() << "data:" << strdata<< "\n";

	if(message == "popup")
	{// popup show
		parseString(strdata);
		if(!m_lookUpWord.isEmpty())
		{
			QString strExplanation;
			strExplanation = searchWordforPopup(m_lookUpWord);
			dialogReader->wordLabel->setText(m_lookUpWord);
			dialogReader->m_pcPopupExplanationArea->setText(strExplanation);
			modeAction(m_dictionaryMode);
		}
	}
	else if(message == "app/shutdown")
	{// shut down
		//SaveMyData();	//do some save work here
		if(strdata =="closeAfterSave")
		{
			exitDictionary();
		}
		else
		{
			send_CloseNotify_ToServer(); // notify server to shutdown
		}
	}
	else if(message == "VKB/hide")
	{
	   if(this->HomeDictionary->m_searchBar->hasFocus())
		 this->HomeDictionary->setFocus();
	   qDebug()<<"enotelogin clear focus";
	}
	else if(message =="app/changeSheet" ||message =="app/show")
	{
		qDebug()<< "message"<< message <<"\n";
		qDebug()<< "HomeDictionary Shwoing...." <<"\n";

		//change title info
		this->HomeDictionary->getPreTitleInfo(); //get current title
		QString StatusTitlepath;
                if(!HomeDictionary->m_bShowMoreItem)
                {
                    StatusTitlepath =tr("Dictionary");
                }
                else
                {
                    StatusTitlepath =tr("Look up word");
                }
		this->HomeDictionary->renewTitleInfo(StatusTitlepath);

//		//dictionary context initial
//		this->HomeDictionary->InitialSelectedFlag(); //initial Highlight
//
////		HomeDictionary->m_toolBar->changeState(ENoteToolBar::s_HOME);
//		QStringList strName;
//		strName<< "Back"<< "Home"<< "Enter" << "Snapshot"<< " ";
//		HomeDictionary->m_toolBar->setStringList(strName);
//		HomeDictionary->m_toolBar->setEnable(2, true);
//
//		HomeDictionary->m_stextInput->setText("");
//		searchSuggestionWord("a");
//		this->HomeDictionary->SetInputFocus();
//		//this->HomeDictionary->ClearInputFocus();
//		HomeDictionary->m_toolBar->setCaptureEvent(true);
		//if(this->HomeDictionary->m_searchBar->getLineEdit()->text().isEmpty())
		//	searchSuggestionWord("a");

		this->HomeDictionary->show();
		this->HomeDictionary->raise();
		notifyLauncherShowed();
		this->m_SetFoucsTimer->start(50);
	}
}

void Dictionary::Handler_SetInputFocus()
{
	this->m_SetFoucsTimer->stop();
	if(!this->HomeDictionary->m_stextInput->hasFocus())
	{
		HomeDictionary->SetInputFocus();
	}
}

void Dictionary::send_CloseNotify_ToServer()
{
	#ifdef Q_WS_QWS
		QByteArray appcmd;
		QDataStream out(&appcmd, QIODevice::WriteOnly);
		out << "dictionary";
		QCopChannel::send("eeeserver", "app/readyToClose", appcmd);
		QCopChannel::flush();
	#endif
}

void Dictionary::parseString(QString stringToParse)
{
	QVector <QString> parsedContent;
	QString wordString;
// -popup -x 100 -y 100 -w 10 -h 10 book
// 012345678901234567890123456789012
	m_rcReaderSelectword.setRect(-1, -1, 0, 0);
	int currentIndex=0, newIndex=0;
	int lastIndex=stringToParse.lastIndexOf(" ");
	do
	{
		newIndex=stringToParse.indexOf(" ",currentIndex);
		if(newIndex != -1)
		{
			parsedContent.push_back(stringToParse.mid(currentIndex,newIndex-currentIndex));
			wordString =stringToParse.mid(currentIndex,newIndex-currentIndex);
			currentIndex=newIndex+1;
		}
		else
		{
			parsedContent.push_back(stringToParse.mid(currentIndex));
			break;
		}
	} while(currentIndex <= lastIndex + 1);
	bool popup = false;
	wordString.clear();
	for(int sIndex=0; sIndex < parsedContent.size(); sIndex++)
	{
		if(parsedContent[sIndex] == "-popup")
			popup=true;
		else if(parsedContent[sIndex] == "-x")
			m_rcReaderSelectword.setX(parsedContent[++sIndex].toInt());
		else if(parsedContent[sIndex] == "-y")
			m_rcReaderSelectword.setY(parsedContent[++sIndex].toInt());
		else if(parsedContent[sIndex] == "-w")
			m_rcReaderSelectword.setWidth(parsedContent[++sIndex].toInt());
		else if(parsedContent[sIndex] == "-h")
			m_rcReaderSelectword.setHeight(parsedContent[++sIndex].toInt());
		else
			wordString.append(parsedContent[sIndex]+" ");
	}

	if(wordString != NULL && wordString.at(0)=='"')
	{
		int toIndex = wordString.indexOf('"', 1);
		wordString=wordString.mid(1,toIndex-1);
	}

	if(popup)
	{
		m_dictionaryMode = READER_MODE;
		m_lookUpWord = wordString.trimmed();
		wordString.clear();
		calculatePopupShowPos(m_rcReaderSelectword);
	}
	else
		m_dictionaryMode = APPLICATION_MODE;
}

void Dictionary::modeAction(mode argMode)
{
	switch (argMode)
	{
	case READER_MODE:
		HomeDictionary->hide();
		showPopup(xOfPopupDlg, yOfPopupDlg);
		break;

	case APPLICATION_MODE:
		dialogReader->hide();
		HomeDictionary->show();
		//this->HomeDictionary->setFocus();
		HomeDictionary->SetInputFocus();
		break;
	};
}

void Dictionary::getLoadList()
{
	m_dictionaryToLoad.clear();
	m_searchDictionariesPath.clear();
	m_selectedDictionariesPath.clear();
	for(std::vector<StarDictionary>::size_type i = 0; i < m_StarDictionaries.size(); i++)
	{
		if(m_StarDictionaries[i].dictSelected == true)
		{
			//__DEBUG__ << "i is" << i <<"\n";
			m_searchDictionariesPath.append(m_StarDictionaries[i].ifoFileName);
			m_selectedDictionariesPath.append(m_StarDictionaries[i].ifoFileName);
			m_dictionaryToLoad.append(m_StarDictionaries[i].ifoFileName);
			//__DEBUG__ << "m_dictionaryToLoad is" << m_dictionaryToLoad[m_dictionaryToLoad.size() - 1] <<"\n";
			break;
		}
	}
}

// Tabs must be created before calling this function as after loading finished, new search operation is performed
void Dictionary::loadDictionary()
{
	//dictListChanged = false;
	getLoadList();
	m_dLoading=true;
	if(!m_dictionaryToLoad.isEmpty())
	{
		m_lDictionary->start();
		//__DEBUG__ << "m_dictionaryToLoad is not empty" << "\n";
	}
	else
	{
		dictionaryLoaded();
		//__DEBUG__ << "m_dictionaryToLoad is empty" << "\n";
	}
}

void Dictionary::dictionaryLoaded()
{
	m_dLoading=false;
	//dictListChanged = false;

	// update m_StarDictionaries details and dictMask
	updateStarDictionaryInfo();

	HomeDictionary->SetInputFocus();
	if(HomeDictionary->m_stextInput->text() != "")
	{
		searchSuggestionWord(HomeDictionary->m_stextInput->text());
	}
	else
	{
		searchSuggestionWord("a");
	}
	if(m_dictionaryMode == READER_MODE)
	{
		if(!m_lookUpWord.isEmpty())
		{
			QString strExplanation;
			strExplanation = searchWordforPopup(m_lookUpWord);
			dialogReader->wordLabel->setText(m_lookUpWord);
			dialogReader->m_pcPopupExplanationArea->setText(strExplanation);
			dialogReader->update();
		}
	}
}

void Dictionary::notifyLauncherShowed()
{
	m_pTimer->stop();

	#ifdef Q_WS_QWS
		QByteArray appcmd;
		QDataStream out(&appcmd, QIODevice::WriteOnly);
		out << "dictionary";
		QCopChannel::send("launcher", "app/showed", appcmd);
		QCopChannel::flush();
	#endif
}

// update m_StarDictionaries details and m_dictMask
void Dictionary::updateStarDictionaryInfo()
{
	m_dictMask.clear();
	for(std::vector<StarDictionary>::size_type i = 0; i < m_StarDictionaries.size(); i++)
		m_StarDictionaries[i].bLoaded = false;

	//__DEBUG__ << "m_dictionaryToLoad.size() is " << m_dictionaryToLoad.size() << "\n";
	for (int i= 0; i < m_dictionaryToLoad.size(); i++)
	{
		size_t loadIndex;
		//__DEBUG__ << i << "m_dictionaryToLoad[i].toUtf8().data() is " << m_dictionaryToLoad[i] << "\n";
		if(m_pLib->find_lib_by_filename(m_dictionaryToLoad[i].toUtf8().data(), loadIndex))
		{
			//__DEBUG__ << i << "m_pLib->find_lib_by_filename is true" << "\n";
			for(std::vector<StarDictionary>::size_type j = 0; j < m_StarDictionaries.size(); j++)
			{
				//__DEBUG__ << j << "m_StarDictionaries[j].ifoFileName is " << m_StarDictionaries[j].ifoFileName << "\n";
				if(m_StarDictionaries[j].ifoFileName == m_dictionaryToLoad[i])
				{
					m_StarDictionaries[j].bLoaded = true;
					//__DEBUG__ << j << "m_StarDictionaries[j].bLoaded is true" << "\n";
					m_StarDictionaries[j].loadIndex = loadIndex;
					m_dictIndex.type = InstantDictType_LOCAL;
					m_dictIndex.index = loadIndex;
					m_dictMask.push_back(m_dictIndex);
				}
			}
		}
	}
}

QString Dictionary::SearchWordExplanation(glong iWordIndex, int StarDictionaryLoadIndex)
{
	const gchar *data, *p;
	int nSize;
	QString SearchResult = "";

	data = m_pLib->poGetOrigWordData(iWordIndex, StarDictionaryLoadIndex); // Get the data at the given index position
	if (data != 0)     // if word is found
	{
		nSize = *(guint32 *)data;
		p = data + sizeof(guint32);
		while (p - data < nSize)
		{
			switch (*p++)
			{
			case 'r':   // resource
				SearchResult.append("The dictionary type is not supported...");
				break;
			case 'W':   // wav
				SearchResult.append("The dictionary type is not supported...");
				break;
			case 'P':   // picture
				SearchResult.append("The dictionary type is not supported...");
				break;
			case 'X':   // experimental extension
				SearchResult.append("The dictionary type is not supported...");
				break;
			case 'w':   // MediaWiki markup language
				SearchResult.append("The dictionary type is not supported...");
				break;

			case 'n':   // wordnet
				if (*p != '\0')
					SearchResult.append(QString::fromUtf8(p));
				break;
			case 'x':   // xdxf format
				if (*p != '\0')
					SearchResult.append(QString::fromUtf8(p));
				break;
			case 'k':   // KingSoft PowerWord's data. Xml format
				if (*p != '\0')
					SearchResult.append(QString::fromUtf8(p));
				break;

			default:
				if (*p != '\0')
					SearchResult.append(QString::fromUtf8(p));
				break;
			}   // switch
			p += strlen(p) + 1;
		}   // while
	}

	return SearchResult;
}

void Dictionary::getSelectionMode()
{
	char fileName[256];
	sprintf(fileName, "%s/dictionary/DictionariesSelection.txt", getenv("ENOTE_CONFIG_PATH"));

	m_pConfig->Reset();
	if(m_pConfig->Load(fileName))
	{
		for(std::vector<StarDictionary>::size_type i = 0; i < m_StarDictionaries.size(); i++)
		{
			std::string dictSelectionType = m_pConfig->GetStr(m_StarDictionaries[i].name.toUtf8().data(), "unselected");
			m_StarDictionaries[i].dictSelected = (dictSelectionType == "selected");
		}
	}
	else    // can not load SelectedDictionaries file, so set default selection modes
	{
		if(m_StarDictionaries.size() > 0)
		{
			m_StarDictionaries[0].dictSelected = true;
		}
		for(std::vector<StarDictionary>::size_type i = 1; i < m_StarDictionaries.size(); i++)
		{
			if( m_StarDictionaries[i].name == DEFAULTDICT)
			{
				m_StarDictionaries[0].dictSelected = false;
				m_StarDictionaries[i].dictSelected = true;
			}
			else
			{
				m_StarDictionaries[i].dictSelected = false;
			}
		}
	}
}

void Dictionary::saveSelectionMode()
{
	char fileName[256];
	sprintf(fileName, "%s/dictionary/DictionariesSelection.txt", getenv("ENOTE_CONFIG_PATH"));
	//__DEBUG__ << fileName << "\n";

	m_pConfig->Reset();
	if(!m_pConfig->Load(fileName))
	{
		m_pConfig->SaveAs(fileName);  // if file does not exist, then create new file
		m_pConfig->Load(fileName);
	}
	for(std::vector<StarDictionary>::size_type i = 0; i < m_StarDictionaries.size(); i++)
		m_pConfig->SetStr(m_StarDictionaries[i].name.toUtf8().data(), (m_StarDictionaries[i].dictSelected)?"selected":"unselected");
	m_pConfig->Save();
}

void Dictionary::showDictionary()
{
	m_dictionaryMode=APPLICATION_MODE;

	searchSuggestionWord(dialogReader->wordLabel->text());

	modeAction(m_dictionaryMode);
	HomeDictionary->m_searchBar->setInputString(dialogReader->wordLabel->text(),false);
}


/************************************************************************
  Show Popup
  **********************************************************************/
void Dictionary::showPopup(int argX, int argY)
{
	if(argX != -1 && argY != -1)
	{
		dialogReader->move(argX, argY);
	}
	dialogReader->update();//update result to paintEvent
	dialogReader->raise();// show on top layout J_ADD
	dialogReader->show();
}


/***********************************************************************
  The difference of searching Previous and Next is that when utilizing
  the same searching engine should given the different suggested word
  then will get the usefull result.
  Previous: back searching
	  Next: follow the normal way
***********************************************************************/
void Dictionary::searchPreviouspageSeggustionWord()
{
	searchSuggestionWord(HomeDictionary->GetSuggestionWord(0), false, false);
}

void Dictionary::searchNextpageSeggustionWord()
{
	searchSuggestionWord(HomeDictionary->GetSuggestionWord(NUM_SUGGESTION_WORDS), true, false);
}

void Dictionary::typeChanged_Handler()
{
	seldictionaryChanged(HomeDictionary->m_searchBar->getCurTypeId());
}
/**********************************************************
* support only one dictionary be selected
* so the processing is like this:
* 1. clear all the m_StarDictionaries to be non selected
* 2. set the index StarDictionary to be selected.
**********************************************************/
void Dictionary::seldictionaryChanged(int iIndex)
{
	if(!m_bRealSetComboBoxSel)
	{
		return;
	}
	for(int i = 0; i < m_StarDictionaries.size(); i++)
	{
		m_StarDictionaries[i].dictSelected = false;
	}
	//__DEBUG__ << "ComboBox iIndex is " << iIndex << "\n";
	m_StarDictionaries[iIndex].dictSelected = true;
	saveSelectionMode();

	loadDictionary();
}


/************************************************************************************
 *Display more ditail explanation of the word being cliked
 *
 *JL_ADD :resloved the half line display phenomenon
 *by calculate how many lines and resize the draw region
 *one page cann't display completely
 *how to control dispaly avoid halfline phenomenon
 * 1. add new menber int lineheight which is the height between two lines.
 * 2. first calculate nline, there are how many lines, get by rect.height/lineheight
 * 3. second calculate there are how many lines can show in a Explanation page.
 ***********************************************************************************/
void Dictionary::clickMoreButton(int iIndex)
{
	HomeDictionary->m_toolBar->setLeftArrowBtnEnable(false);
	HomeDictionary->m_toolBar->setRightArrowBtnEnable(false);
	HomeDictionary->m_iCurrentPage = 1;
	HomeDictionary->m_toolBar->setCurPageCount(0); //first do not dispaly pagecount

	HomeDictionary->m_toolBar->setEnable(2, false);

	HomeDictionary->wordLabel->setText(HomeDictionary->GetSuggestionWord(iIndex));
	HomeDictionary->explanationArea->setText(HomeDictionary->GetSuggestionWordExplanation(iIndex));

	HomeDictionary->explanationArea->setTextStartPositionY(0);

	//QFont explanationFont("Helvetica", EXPLANATIONAREAFONTSIZE, QFont::Normal);
	QFont explanationFont;
#ifdef DEFINEMYLOCALSETFAMILY
	explanationFont.setFamily(ALLTEXTFONT);
#endif
	qDebug()<< "<< detail explation family= "<< explanationFont.family()<<'\n';
	//explanationFont.setFamily("Helvetica");
	//explanationFont.setFamily("ALLTEXTFONT");
	qDebug()<< ">>detail explation family= "<< explanationFont.family()<<'\n';

	explanationFont.setPointSize(EXPLANATIONAREAFONTSIZE);
	explanationFont.setWeight(QFont::Normal);
//QString str="adfasdlfuyoretgeriudfmnkgnfdkluyheoprthdfghfdlk;hgeo;rihldkhfgaldghd;geo;uh;hgl;sdfhgsuybsgjghjsdhgsjdghvsvafdgaaaaaaaaagwefdsgdfgegd";

	QFontMetrics fontMetrics(explanationFont);
	QRect rect = fontMetrics.boundingRect(EXPLANATIONAREASHOWX, 0, EXPLANATIONAREASHOWWIDTH, EXPLANATIONAREASHOWHEIGHT, Qt::AlignLeft | Qt::TextWrapAnywhere ,
								 HomeDictionary->GetSuggestionWordExplanation(iIndex));// | Qt::AlignJustify

	int lineheight=fontMetrics.lineSpacing();//37calculate the height between two lines.
	int nTotalheight =rect.height()+fontMetrics.leading();
	int ntotalLines =nTotalheight/lineheight;

	int nlinesInPage =EXPLANATIONAREASHOWHEIGHT/lineheight;

	HomeDictionary->m_ExplanUseHeight =nlinesInPage*lineheight;
	HomeDictionary->explanationArea->setRectDraw(QRect(EXPLANATIONAREASHOWX, 0, EXPLANATIONAREASHOWWIDTH, HomeDictionary->m_ExplanUseHeight));

	HomeDictionary->m_iExplanationPageCount = (ntotalLines%nlinesInPage)?(ntotalLines/nlinesInPage+1):ntotalLines/nlinesInPage;
	HomeDictionary->m_iExplanationPageCount = rect.height() / EXPLANATIONAREASHOWHEIGHT + 1;
	if(HomeDictionary->m_iExplanationPageCount > 1)
	{
		HomeDictionary->m_toolBar->setRightArrowBtnEnable(true);
		HomeDictionary->m_toolBar->setCurPageCount(HomeDictionary->m_iCurrentPage);
		//HomeDictionary->m_toolBar->setRightPage(HomeDictionary->m_iCurrentPage + 1);
	}

        HomeDictionary->m_bShowMoreItem = true;
        QString StatusTitlepath;
        StatusTitlepath =tr("Look up word");
        HomeDictionary->renewTitleInfo(StatusTitlepath);

	HomeDictionary->m_stextInput->setText(HomeDictionary->GetSuggestionWord(iIndex));
	HomeDictionary->groupBox->hide();
	HomeDictionary->verticalLayout->addWidget(HomeDictionary->wordExplanationGroupBox);
	HomeDictionary->wordExplanationGroupBox->show();
}
void Dictionary::NotifytoServePlaySound()
{
#ifdef Q_WS_QWS
	QByteArray appcmd;
	QDataStream out(&appcmd, QIODevice::WriteOnly);
	out << "dictionary";
	QCopChannel::send("eeeserver", "app/playsound", appcmd);
	QCopChannel::flush();
#endif
}
void Dictionary::clearInput()
{
	NotifytoServePlaySound(); //play sounds when press button

	HomeDictionary->m_toolBar->setLeftArrowBtnEnable(false);
	HomeDictionary->m_toolBar->setRightArrowBtnEnable(false);

	HomeDictionary->ClearInputText();
	HomeDictionary->SetInputFocus();  // set the focus on textInput to display virtual keyboard so that the user can input new word
	HomeDictionary->ClearSuggestionList();
	HomeDictionary->wordExplanationGroupBox->hide();
	HomeDictionary->verticalLayout->addWidget(HomeDictionary->groupBox);
	HomeDictionary->groupBox->show();
}

void Dictionary::exitDictionary()
{
	//this->HomeDictionary->renewTitleInfo(this->HomeDictionary->m_PreTitle);//restor the status title
	QApplication::instance()->quit();
}

int Dictionary::getIndexOfStarDictionary(QString filePath)
{
	for(std::vector<StarDictionary>::size_type i = 0; i < m_StarDictionaries.size(); i++)
	{
		if(filePath == m_StarDictionaries[i].ifoFileName)
			return i;
	}
}

/******************************************************************************
search word for popup explanations:
finished basic function. find the lookup word in selected dictionaries if there
is no any words then return NULL;else return explanations.
remain question: (How to deal with abnormal situation)
1. if the lookup word cannot found in dictionaries How to deal with;
2. if the explanations of the word too many that cann't too display completely;
3. follows Q2 find the suggest word nearest the lookup word, should dispaly
the suggest as the same, where is to display?
******************************************************************************/
QString Dictionary::searchWordforPopup(QString str)
{
	glong iWordIndex, idx_suggest;
	//iWordIndex conserve offset of word in dictionary
	//idx_suggest conserve offset of suggest word
	const gchar *pCurrentWord;
	bool bWordFound;
	QString nofind= tr("not find!");
	//CurrentIndex cindex;
	if(m_searchDictionariesPath.size() == 0 || str.isEmpty())
	{
		return nofind;
	}

	m_iCurrent = new CurrentIndex[m_searchDictionariesPath.size()];
	if(m_iCurrent==NULL)
	{
		return "";
	}
	//structure array of idx and idx_suggest indexes of words

	//bWordFound use to storage the flag of status of searching from dictionaries
	m_lookUpWord = str;//words prepared for searching
	if(m_lookUpWord.isEmpty())
	{
		delete [] m_iCurrent;
		return "";//return NULL
	}
	for (int nIndex = 0; nIndex < m_searchDictionariesPath.size() ; nIndex++)
	{// Look up the word in all the selected dictionaries
		int StarDictionaryIndex;
		int StarDictionaryLoadIndex;
		// CurrentIndex set default VULL values
//		cindex.idx = INVALID_INDEX;
//		cindex.idx_suggest = INVALID_INDEX;
//		cindex.synidx = INVALID_INDEX;
//		cindex.synidx_suggest = INVALID_INDEX;
		m_iCurrent[nIndex].idx = INVALID_INDEX;
		m_iCurrent[nIndex].idx_suggest = INVALID_INDEX;
		m_iCurrent[nIndex].synidx = INVALID_INDEX;
		m_iCurrent[nIndex].synidx_suggest = INVALID_INDEX;
		bWordFound =false;
		StarDictionaryIndex = getIndexOfStarDictionary(m_searchDictionariesPath[nIndex]);
		if(!m_StarDictionaries[StarDictionaryIndex].bLoaded)
		{// bLoaded ==false m_iCurrent = new CurrentIndex[m_searchDictionariesPath
			continue;
		}
		StarDictionaryLoadIndex = m_StarDictionaries[StarDictionaryIndex].loadIndex;

		//SimpleLookupWord(): When finded return TRUE;else FALSE.
		bWordFound= m_pLib->SimpleLookupWord(m_lookUpWord.toUtf8().data(), iWordIndex, idx_suggest, StarDictionaryLoadIndex, 0);
		if(!bWordFound && iWordIndex < 0)   // if even no similar word is found in the dictionary
		{// no find
			iWordIndex = INVALID_INDEX;
			idx_suggest = INVALID_INDEX;
		}
		else
		{// once found then stop searching word
//			cindex.idx = iWordIndex;
//			cindex.idx_suggest = idx_suggest;
			m_iCurrent[nIndex].idx = iWordIndex;
			m_iCurrent[nIndex].idx_suggest = idx_suggest;
			pCurrentWord=m_pLib->poGetCurrentWord(m_iCurrent, m_dictMask, 0);
//			QString strtemp = QString((char *)pCurrentWord);
			if(!strcasecmp(pCurrentWord,m_lookUpWord.toUtf8().data()))
			{
				str = SearchWordExplanation(iWordIndex, StarDictionaryLoadIndex);
				return (str);
			}
		}
	} //for nIndex
	//m_lookUpWord.clear();
	delete [] m_iCurrent;
	return nofind;
}

/*******************************************************************************
*calculate popup show position
*change the position of POPUP dailog (xOfPopupDlg,yOfPopupDlg)
*
*input : QRect rcSelect
*output: xOfPopupDlg,yOfPopupDlg
*need the parameter as follows:
*		POPUPEXPLANATIONAREAWIDTH; POPUPEXPLANATIONAREAHEIGHT
*	    SCREENWIDTH; SCREENHEIGHT
*JL ADD
******************************************************************************/
void Dictionary::calculatePopupShowPos(QRect rcSelect)
{
	//initial check out rcSelect whether illegal

	int width,height;

	if (rcSelect.left()<0 || rcSelect.top()<0)
		return;
	width =rcSelect.width();
	height=rcSelect.height();
	if (width<0 || height<0)
	{
		height=0;
		width =0;
	}
	if (rcSelect.right()>SCREENWIDTH ||rcSelect.bottom()>SCREENHEIGHT )
	{
		height=0;
		width =0;
	}

	if(SCREENWIDTH-rcSelect.left()>POPUPWIDTH)
	{//1.4
		if(SCREENHEIGHT-(rcSelect.top()+height) >POPUPHEIGHT)
		{//1
			xOfPopupDlg=rcSelect.left();
			yOfPopupDlg=rcSelect.top()+height;
		}
		else if(SCREENHEIGHT-(rcSelect.top()+height)>0)
		{//4
			xOfPopupDlg=rcSelect.left();
			yOfPopupDlg=rcSelect.top()-POPUPHEIGHT;
		}
	}
	else if(SCREENWIDTH-rcSelect.left()>0)
	{//2.3
		if(SCREENHEIGHT-(rcSelect.top()+height)>POPUPHEIGHT)
		{//2
			xOfPopupDlg=rcSelect.left()+width-POPUPWIDTH;
			yOfPopupDlg=(rcSelect.top()+height);
		}
		else if(SCREENHEIGHT-(rcSelect.top()+height)>0)
		{//3

			xOfPopupDlg=rcSelect.left()+width-POPUPWIDTH;
			yOfPopupDlg=rcSelect.top()-POPUPHEIGHT;
		}
	}
}
void Dictionary::search_Handler()
{
	QString str =this->HomeDictionary->m_searchBar->getSearchString();
	//searchSuggestionWord(str);
	clickMoreButton(this->HomeDictionary->m_currentHighlight);
}

void Dictionary::FilterInputWords(QString &str)
{
	//QString tempstr;
	if(str.isEmpty())
		return;
	//ignore the previous " ";
	str.trimmed();
//	while(str.count())
//	{
//		if(str.startsWith(" "))
//				str.remove(0,1);
//		else
//			break;
//	}
//	//ignore the ends " ";
//	while(str.count())
//	{
//		if(str.endsWith(" "))
//			str.remove(str.count()-1,1);
//		else
//			break;
//	}
}

void Dictionary::searchSuggestionWord(QString str, bool bforwardNext, bool bsearchfrominput)
{
		const gchar *data;
        const gchar *pCurrentWord, *pNextWord;
        glong iWordIndex, idx_suggest;
        bool *bWordFound;

        qDebug() << "search str: " << str << "\n";

		QTime timer;
		timer.start();
		//FilterInputWords(str);
		str=str.trimmed();
		if(str.isEmpty())
			str ="a";
		if(bsearchfrominput)
		{
			HomeDictionary->m_iCurrentPage = 1;
			HomeDictionary->m_toolBar->setLeftArrowBtnEnable(false);
			HomeDictionary->m_toolBar->setRightArrowBtnEnable(false);
			HomeDictionary->m_toolBar->setCurPageCount(0);
		}
		HomeDictionary->setItemHighLighted(0);
		HomeDictionary->m_toolBar->setEnable(2,true); //enbale enter button
		if(m_searchDictionariesPath.size() == 0)
		{
			//__DEBUG__ << "m_searchDictionariesPath is empty" << "\n";
			return ;
		}
		m_iCurrent = new CurrentIndex[m_searchDictionariesPath.size()]; //structure array of idx and idx_suggest indexes of words
		if(m_iCurrent==NULL)
		{
			return ;
		}
		bWordFound = new bool [m_searchDictionariesPath.size()];
		if(bWordFound==NULL)
		{
			return ;
		}

		m_lookUpWord = str;
		HomeDictionary->ClearSuggestionList();

		HomeDictionary->wordExplanationGroupBox->hide();
		HomeDictionary->verticalLayout->addWidget(HomeDictionary->groupBox);
		HomeDictionary->groupBox->show();

		if(m_lookUpWord.isEmpty())
		{
			//__DEBUG__ << "m_lookUpWord is empty" << "\n";
			if(bWordFound)
			{
				delete [] bWordFound;
				m_iCurrent=NULL;
			}
			if(m_iCurrent)
			{
				delete [] m_iCurrent;
				m_iCurrent=NULL;
			}
			return ;
        }
		bool noEndofsuggestWords=true;
                HomeDictionary->m_isEndofsuggestwords =false;
		int TemLastindex=0;

		for (int nIndex = 0; nIndex < m_searchDictionariesPath.size() && noEndofsuggestWords; nIndex++)    // Look up the word in all the selected dictionaries
        {
			int StarDictionaryIndex;
			int StarDictionaryLoadIndex;
			bWordFound[nIndex]=false;
			outputString.clear();
			m_iCurrent[nIndex].idx = INVALID_INDEX;
			m_iCurrent[nIndex].idx_suggest = INVALID_INDEX;
			m_iCurrent[nIndex].synidx = INVALID_INDEX;
			m_iCurrent[nIndex].synidx_suggest = INVALID_INDEX;
			StarDictionaryIndex = getIndexOfStarDictionary(m_searchDictionariesPath[nIndex]);
			if(!m_StarDictionaries[StarDictionaryIndex].bLoaded)
			{
				//__DEBUG__ << "!m_StarDictionaries[StarDictionaryIndex].bLoaded" << "\n";
				continue;
			}
			StarDictionaryLoadIndex = m_StarDictionaries[StarDictionaryIndex].loadIndex;

			bWordFound[nIndex]= m_pLib->SimpleLookupWord(m_lookUpWord.toUtf8().data(), iWordIndex, idx_suggest, StarDictionaryLoadIndex, 0);
			if(!bWordFound[nIndex] && iWordIndex < 0)   // if even no similar word is found in the dictionary
			{
				data=NULL;
				iWordIndex = INVALID_INDEX;
				idx_suggest = INVALID_INDEX;
			}
			else
			{
				m_iCurrent[nIndex].idx = iWordIndex;
				m_iCurrent[nIndex].idx_suggest = idx_suggest;
			}

			pCurrentWord=m_pLib->poGetCurrentWord(m_iCurrent, m_dictMask, 0);
                        qDebug() << "pCurrentWord : " << (char*)pCurrentWord << "\n";
			if(pCurrentWord)
			{
				if(bforwardNext)
				{
					HomeDictionary->SetSuggestionWord(0, QString::fromUtf8(pCurrentWord));
					QString str = SearchWordExplanation(iWordIndex, StarDictionaryLoadIndex);
					HomeDictionary->SetSuggestionWordExplanation(0, str);
					TemLastindex++;
					//pNextWord = pCurrentWord;
					for (int suggestionIndex = 1; suggestionIndex <= NUM_SUGGESTION_WORDS; suggestionIndex++)
					{
						pNextWord = m_pLib->poGetNextWord(NULL, m_iCurrent, m_dictMask, 0);   // Get the suggested word
						if (pNextWord)
						{
                                                        qDebug() << "pNextWord : " << (char*)pNextWord << "\n";
                                                        HomeDictionary->SetSuggestionWord(suggestionIndex, QString::fromUtf8(pNextWord));
							QString str = SearchWordExplanation(m_iCurrent->idx, StarDictionaryLoadIndex);
							HomeDictionary->SetSuggestionWordExplanation(suggestionIndex, str);
							TemLastindex++;
						}
						else
						{
							//delete [] bWordFound;
							//__DEBUG__ << "pNextWord is empty" << "\n";
							noEndofsuggestWords=false;
							break;
						}
					}
				}
				else
				{
					HomeDictionary->SetSuggestionWord(NUM_SUGGESTION_WORDS, QString::fromUtf8(pCurrentWord));
					QString str = SearchWordExplanation(iWordIndex, StarDictionaryLoadIndex);
					HomeDictionary->SetSuggestionWordExplanation(NUM_SUGGESTION_WORDS, str);
					pNextWord = pCurrentWord;
					for (int suggestionIndex = NUM_SUGGESTION_WORDS - 1; suggestionIndex >= 0; suggestionIndex--)
					{
						pNextWord = m_pLib->poGetPreWord(pNextWord, m_iCurrent, m_dictMask, 0);   // Get the suggested word
						if (pNextWord)
						{
                                                        qDebug() << "pNextWord : " << (char*)pNextWord << "\n";
                                                        HomeDictionary->SetSuggestionWord(suggestionIndex, QString::fromUtf8(pNextWord));
							QString str = SearchWordExplanation(m_iCurrent->idx, StarDictionaryLoadIndex);
							HomeDictionary->SetSuggestionWordExplanation(suggestionIndex, str);
							TemLastindex++;
						}
						else
						{
							//delete [] bWordFound;
							noEndofsuggestWords=false;
							break;
						}
					}
				}
			}
        } //for nIndex

		if(TemLastindex !=0 &&m_lookUpWord !="a")
		{

			QString search=this->HomeDictionary->GetSuggestionWord(0);
			qDebug()<< "m_lookUpWord      ="<<m_lookUpWord <<"\n";
			qDebug()<< "GetSuggestionWord ="<<search <<"\n";
//                        if(!search.contains(m_lookUpWord,Qt::CaseInsensitive))
//				TemLastindex =0;
			qDebug()<< "TemLastindex ="<<TemLastindex <<"\n";
		}
		if(TemLastindex==0)
		{
			if(bWordFound)
			{
				delete [] bWordFound;
				bWordFound=NULL;
			}
			if(m_iCurrent)
			{
				delete [] m_iCurrent;
				m_iCurrent=NULL;
			}
			m_iCurrent = new CurrentIndex[m_searchDictionariesPath.size()]; //structure array of idx and idx_suggest indexes of words
			if(m_iCurrent==NULL)
				return ;
			bWordFound = new bool [m_searchDictionariesPath.size()];
			if(bWordFound==NULL)
				return ;
			m_lookUpWord = "a";
			HomeDictionary->ClearSuggestionList();
			__DEBUG__ << "lookup word is: " << str << "\n";


			bool noEndofsuggestWords=true;
                        HomeDictionary->m_isEndofsuggestwords =false;
			int TemLastindex=0;

			for (int nIndex = 0; nIndex < m_searchDictionariesPath.size() && noEndofsuggestWords; nIndex++)    // Look up the word in all the selected dictionaries
			{
				int StarDictionaryIndex;
				int StarDictionaryLoadIndex;
				bWordFound[nIndex]=false;
				outputString.clear();
				m_iCurrent[nIndex].idx = INVALID_INDEX;
				m_iCurrent[nIndex].idx_suggest = INVALID_INDEX;
				m_iCurrent[nIndex].synidx = INVALID_INDEX;
				m_iCurrent[nIndex].synidx_suggest = INVALID_INDEX;
				StarDictionaryIndex = getIndexOfStarDictionary(m_searchDictionariesPath[nIndex]);
				if(!m_StarDictionaries[StarDictionaryIndex].bLoaded)
				{
					//__DEBUG__ << "!m_StarDictionaries[StarDictionaryIndex].bLoaded" << "\n";
					continue;
				}
				StarDictionaryLoadIndex = m_StarDictionaries[StarDictionaryIndex].loadIndex;

				bWordFound[nIndex]= m_pLib->SimpleLookupWord(m_lookUpWord.toUtf8().data(), iWordIndex, idx_suggest, StarDictionaryLoadIndex, 0);
				if(!bWordFound[nIndex] && iWordIndex < 0)   // if even no similar word is found in the dictionary
				{
					data=NULL;
					iWordIndex = INVALID_INDEX;
					idx_suggest = INVALID_INDEX;
				}
				else
				{
					m_iCurrent[nIndex].idx = iWordIndex;
					m_iCurrent[nIndex].idx_suggest = idx_suggest;
				}

				pCurrentWord=m_pLib->poGetCurrentWord(m_iCurrent, m_dictMask, 0);
				if(pCurrentWord)
				{
					if(bforwardNext)
					{
						HomeDictionary->SetSuggestionWord(0, QString::fromUtf8(pCurrentWord));
						QString str = SearchWordExplanation(iWordIndex, StarDictionaryLoadIndex);
						HomeDictionary->SetSuggestionWordExplanation(0, str);
						TemLastindex++;
						//pNextWord = pCurrentWord;
						for (int suggestionIndex = 1; suggestionIndex <= NUM_SUGGESTION_WORDS; suggestionIndex++)
						{
							pNextWord = m_pLib->poGetNextWord(NULL, m_iCurrent, m_dictMask, 0);   // Get the suggested word
							if (pNextWord)
							{
								HomeDictionary->SetSuggestionWord(suggestionIndex, QString::fromUtf8(pNextWord));
								QString str = SearchWordExplanation(m_iCurrent->idx, StarDictionaryLoadIndex);
								HomeDictionary->SetSuggestionWordExplanation(suggestionIndex, str);
								TemLastindex++;
							}
							else
							{
								//delete [] bWordFound;
								//__DEBUG__ << "pNextWord is empty" << "\n";
								noEndofsuggestWords=false;
								break;
							}
						}
					}
					else
					{
						HomeDictionary->SetSuggestionWord(NUM_SUGGESTION_WORDS, QString::fromUtf8(pCurrentWord));
						QString str = SearchWordExplanation(iWordIndex, StarDictionaryLoadIndex);
						HomeDictionary->SetSuggestionWordExplanation(NUM_SUGGESTION_WORDS, str);
						pNextWord = pCurrentWord;
						for (int suggestionIndex = NUM_SUGGESTION_WORDS - 1; suggestionIndex >= 0; suggestionIndex--)
						{
							pNextWord = m_pLib->poGetPreWord(pNextWord, m_iCurrent, m_dictMask, 0);   // Get the suggested word
							if (pNextWord)
							{
								HomeDictionary->SetSuggestionWord(suggestionIndex, QString::fromUtf8(pNextWord));
								QString str = SearchWordExplanation(m_iCurrent->idx, StarDictionaryLoadIndex);
								HomeDictionary->SetSuggestionWordExplanation(suggestionIndex, str);
								TemLastindex++;
							}
							else
							{
								//delete [] bWordFound;
								noEndofsuggestWords=false;
								break;
							}
						}
					}
				}
			} //for nIndex
		}

		if(!noEndofsuggestWords)
		{
			int tempIndex=this->HomeDictionary->m_currentHighlight;
			if(tempIndex >TemLastindex-1)
					tempIndex =TemLastindex-1;
			this->HomeDictionary->setItemHighLighted(tempIndex);
		}

		qDebug("Searching words spend time: %dms\n", timer.elapsed());
		int left=0,right=0,current=1;
		if(!HomeDictionary->groupBox->isHidden())
		{
			if(bforwardNext)
			{//true
				if(noEndofsuggestWords)
				{
					if(!bsearchfrominput)
					{//false
						HomeDictionary->m_iCurrentPage += 1;
						current =HomeDictionary->m_iCurrentPage;
					}
					HomeDictionary->RenewBottomBarPage();
					//HomeDictionary->m_toolBar->setLeftPage(HomeDictionary->m_iCurrentPage - 1);
					left=HomeDictionary->m_iCurrentPage - 1;
					if(HomeDictionary->GetSuggestionWord(NUM_SUGGESTION_WORDS) != "")
					{
						//HomeDictionary->m_toolBar->setRightPage(HomeDictionary->m_iCurrentPage + 1);
						right=HomeDictionary->m_iCurrentPage + 1;
					}
					else
					{
						HomeDictionary->m_toolBar->setRightArrowBtnEnable(false);
						//HomeDictionary->m_toolBar->setRightPage(0);
						right=0;
					}
				}
				else
				{// end of suggest words, so stop next button and sliber right
                                        HomeDictionary->m_isEndofsuggestwords =true;
					HomeDictionary->m_EndofswordsLastIndex=TemLastindex;
					if(!bsearchfrominput)
					{
						HomeDictionary->m_iCurrentPage += 1;
						current =HomeDictionary->m_iCurrentPage;
					}
					HomeDictionary->RenewBottomBarPage();
					//HomeDictionary->m_toolBar->setLeftPage(HomeDictionary->m_iCurrentPage - 1);
					left =HomeDictionary->m_iCurrentPage - 1;
					HomeDictionary->m_toolBar->setRightArrowBtnEnable(false);
					//HomeDictionary->m_toolBar->setRightPage(0);
					right = 0;

				}
			}
			else
			{// press preview button
				if(HomeDictionary->m_iCurrentPage > 1)
				{
					HomeDictionary->m_iCurrentPage -= 1;
					current =HomeDictionary->m_iCurrentPage;
				}
				else
				{
					HomeDictionary->m_iCurrentPage = 1;
					current =1;
				}
				HomeDictionary->RenewBottomBarPage();
				//HomeDictionary->m_toolBar->setLeftPage(HomeDictionary->m_iCurrentPage - 1);
				left = HomeDictionary->m_iCurrentPage - 1;
				if(HomeDictionary->GetSuggestionWord(NUM_SUGGESTION_WORDS) != "")
				{
					//HomeDictionary->m_toolBar->setRightPage(HomeDictionary->m_iCurrentPage + 1);
					right = HomeDictionary->m_iCurrentPage + 1;
				}
				else
				{
					HomeDictionary->m_toolBar->setRightArrowBtnEnable(false);
					right=0;
				}

			}
		}
		else
		{// ----------------------------------------------------
			if(bforwardNext)
			{//true
				if(noEndofsuggestWords)
				{
					if(!bsearchfrominput)
					{//false
						HomeDictionary->m_iCurrentPage += 1;
					}
					HomeDictionary->RenewBottomBarPage();
					//HomeDictionary->m_toolBar->setLeftPage(HomeDictionary->m_iCurrentPage - 1);
					if(HomeDictionary->GetSuggestionWord(NUM_SUGGESTION_WORDS) != "")
					{
						//HomeDictionary->m_toolBar->setRightPage(HomeDictionary->m_iCurrentPage + 1);
					}
					else
					{
						HomeDictionary->m_toolBar->setRightArrowBtnEnable(false);
						//HomeDictionary->m_toolBar->setRightPage(0);
					}
				}
				else
				{// end of suggest words, so stop next button and sliber right
                                        HomeDictionary->m_isEndofsuggestwords =true;
					HomeDictionary->m_EndofswordsLastIndex=TemLastindex;
					if(!bsearchfrominput)
					{
						HomeDictionary->m_iCurrentPage += 1;
					}
					HomeDictionary->RenewBottomBarPage();
					HomeDictionary->m_toolBar->setRightArrowBtnEnable(false);
					//HomeDictionary->m_toolBar->setLeftPage(HomeDictionary->m_iCurrentPage - 1);
					//HomeDictionary->m_toolBar->setRightPage(0);

				}
			}
			else
			{// press preview button
				if(HomeDictionary->m_iCurrentPage > 1)
				{
					HomeDictionary->m_iCurrentPage -= 1;
				}
				else
				{
					HomeDictionary->m_iCurrentPage = 1;
				}
				HomeDictionary->RenewBottomBarPage();
				//HomeDictionary->m_toolBar->setLeftPage(HomeDictionary->m_iCurrentPage - 1);
				if(HomeDictionary->GetSuggestionWord(NUM_SUGGESTION_WORDS) != "")
				{
					//HomeDictionary->m_toolBar->setRightPage(HomeDictionary->m_iCurrentPage + 1);
				}
				else
				{
					HomeDictionary->m_toolBar->setRightArrowBtnEnable(false);
					//HomeDictionary->m_toolBar->setRightPage(0);
				}
			}
		}
		HomeDictionary->backupToolbarNum(left,right,current);
		//__DEBUG__ << "m_isEndofsuggestwords is " <<m_isEndofsuggestwords<<"\n";
		m_lookUpWord.clear();
		if(bWordFound)
		{
			delete [] bWordFound;
			bWordFound=NULL;
		}
		if(m_iCurrent)
		{
			delete [] m_iCurrent;
			m_iCurrent=NULL;
		}

		qDebug("Searching words spend time: %dms\n", timer.elapsed());

        return;
}




/**
 *   @brief main function - create dictionary application app
 */
int main(int argc, char *argv[])
{
	char path[256];
	QApplication app(argc, argv);
	Q_INIT_RESOURCE(resource);
	Q_INIT_RESOURCE(commondlgres);

	setDefaultFont();
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	// Set language
	QSettings *settings=new QSettings(QString("%1/setting/sys.config").arg(getenv("ENOTE_CONFIG_PATH")),QSettings::IniFormat);
	QString lan=settings->value("language/default","en").toString();
	delete settings;
	settings=NULL;
	QTranslator translator;
	translator.load(QString("dictionary_%1").arg(lan),QString("%1/dictionary").arg(getenv("ENOTE_TRANSLATOR_PATH")));
	app.installTranslator(&translator);



#ifdef Q_WS_QWS
	if(QCopChannel::isRegistered("dictionary"))
	{
		QByteArray appDshow;
		QDataStream outDshow(&appDshow, QIODevice::WriteOnly);
		outDshow << "dictionary sheet1";
		QCopChannel::send("dictionary", "app/changeSheet", appDshow);
		QCopChannel::flush();
		return 0;
	}
#endif

	QString aString ="";
	for ( int i = 1; i < argc; i++ )
		aString.append(QString::fromUtf8(argv[i]) + " ");

	Dictionary dict(aString);

	return app.exec();
}
