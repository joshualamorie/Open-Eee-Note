/*
  Copyright (C) 2010 ASUSTeK Computer Inc. All rights reserved.
*/



#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QtCore>
#include <QThread>
#include <QDebug>

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <glob.h>
#include <QCopChannel>

#include "stddict.hpp"


class Libs;
class CHomeDictionary;
class ReaderDict;
class CConfig;
class CScanDict;
class MyThread;

// Macro dfinition for debugging
#define __DEBUG__ qDebug() <<__func__<<"()  Line:"<< __LINE__ << "   "

#define POPUP_DIALOG_HEIGHT 280

/**
  * Selection modes of all the available dictionaries
  */
enum selectionMode {
	DICTIONARY,
	NONE
};

/**
  * StarDictionary structure definition
  */
struct StarDictionary
{
	QString name;           // dictionary name
	QString ifoFileName;    // ifoFileName with full path
	bool	bLoaded;           // dictionary is loaded, or not
	size_t	loadIndex;       // loading index of the dictionary in whole collection
	bool	dictSelected;		// mode of selection of the dictionary by the user
};


/*! Dictionary Class definition starts now */
class Dictionary : public QObject
{
    Q_OBJECT
    public:
		/**
		 * @brief Dictionary Class Constructor
		 */
        Dictionary(QString);

        /**
         *   @brief Destructor of Dictionary class
         */
		~Dictionary();

	public:
		/**
		 * Mode of operation of the application
		 */
		enum mode{
			READER_MODE,
			APPLICATION_MODE
		};
		//static Dictionary				*anInstance;
		Libs							*m_pLib;          // Stardict library class - Libs instance
		//Dict							*pDict;
		CHomeDictionary					*HomeDictionary;

		ReaderDict						*dialogReader;
		MyThread						*m_lDictionary;   // Thread to load dictionaries
		mode							m_dictionaryMode; // current mode of the dictionary
		QString							m_lookUpWord;     // input word to be looked up in the dictionary
		QString							outputString;   // data of the result of the word look up
		QVector <QString>				m_lookUpHistory;  // store lookup history
		QVector <StarDictionary>		m_StarDictionaries;  // All present dictionaries collection

		QStringList						m_dictionaryToLoad;  // ifoFileNames of the dictionaries to load
		bool							m_dLoading;
		int								m_iEnableCollationLevel;
		int								m_iCollateFunction;

		QTimer							*m_pTimer;
		QTimer							*m_SetFoucsTimer;
		bool							m_bRealSetComboBoxSel;
		QString							m_PreStatusTitle;// reserve the old title, for restor.
//		bool							m_isEndofsuggestwords; //at the end of dict then is ture;


		//initial its at the construct dictionary object, change it at searchsuggestwords()

	private:
		CConfig							*m_pConfig;	// CConfig instance for managing dictionary configuration and settings files
		CScanDict						*m_pScanDict;
		InstantDictIndex				m_dictIndex;	// structure having the type and index of selected dictionaries
		std::vector <InstantDictIndex>	m_dictMask;	// vector of type InstantDictIndex having the type and index of all the selected dictionaries
		CurrentIndex					*m_iCurrent;	// new CurrentIndex structure array having idx and idx_suggest indexes of words for each selected dictionary
		QStringList						m_searchDictionariesPath;
		QStringList						m_selectedDictionariesPath;
		bool							m_bCreateCache;
		//bool							dictListChanged;
		//bool							m_enterMainApp;
		int								xOfPopupDlg, yOfPopupDlg;
		QRect							m_rcReaderSelectword;

		#ifdef Q_WS_QWS
			QCopChannel *m_channel;
			QCopChannel *m_channelpower;
			QCopChannel *m_doVKBChannel;
		#endif

		/**
		 *   @brief Search Word Explanation
		 */
		QString SearchWordExplanation(glong iWordIndex, int StarDictionaryLoadIndex);

		/**
		 *   @brief Get the dictionaries selection modes (DICTIONARY/ READER/ BOTH/ NONE) from the DictionariesSelection.txt file
		 */
		void getSelectionMode();

		/**
		 *   @brief Save the dictionary selection modes in DictionariesSelection.txt
		 */
		void saveSelectionMode();

		/**
		 *   @brief Get the list of dictionaries to load as per dictionary mode/ Set up updation
		 */
		void getLoadList();

		/**
		 *   @brief Update the Stardictionary info after (re)loading dictionaries
		 */
		void updateStarDictionaryInfo();

		/**
		 *   @brief Get the index of the dictionary in the Stardictionary collection vector from the .ifo file path
		 *   @param filePath .ifo file path of the dictionary
		 *   @return index of the dictionary in the Stardictionary collection vector
		 */
		int getIndexOfStarDictionary(QString filePath);

		/**
		 *   @brief parse the received string to find out the usage mode; x and y position of looked up word, orientation angle of reading for popup dialog display
		 *   @param stringToParse
		 */
		void parseString(QString stringToParse);

		/**
		 *   @brief Take appropriate action as per dictionary mode
		 *   @param argMode mode of the dictionary
		 */
		void modeAction(mode argMode);

		/**
		 *   @brief load dictionaries (m_dictionaryToLoad) in a separate thread
		 */
		void loadDictionary();

		void calculatePopupShowPos(QRect rcSelect);

		/**
		  * reply eeeserver that the app(dictionary) has been shut down
		  */
		void send_CloseNotify_ToServer();

		/**
		  *to emit signal that toolbar slip has been clicked
		  */
		void Toolbar_slipclicked(int );



		void NotifytoServePlaySound();// send playsound message to eeeserver

		/**
		 *   @fn    seldictionaryChanged(int iIndex)
		 *   @brief select dictionary from combobox
		 */
		void seldictionaryChanged(int iIndex);

		void FilterInputWords(QString &str);
		
	public slots:

		/**
		 *   @fn    MsgHandler()
		 *   @brief Signal handler to get the notification on receiving linux signal when new message string for look up/ application show is received from the (Ebook) Reader
		 *          looks up the message in the file on receiving signal SIGUSR1
		 *   @param nSignal
		 */
		void MsgHandler(const QString &, const QByteArray &);

		/**
		 *   @brief show popup dialog in appropriate orientation and position
		 *   @param argX x-coordinate of the looked up word
		 *   @param argY y-coordinate of the looked up word
		 */
		void showPopup(int argX = -1, int argY = -1);

		/**
		 *   @fn    searchPreviouspageSeggustion Word();
		 *   @brief search previous page Seggustion Word
		 */
		void searchPreviouspageSeggustionWord();

		/**
		 *   @fn    searchNextpageSeggustionWord();
		 *   @brief search Next page SeggustionWord
		 */
		void searchNextpageSeggustionWord();



		/**
		 *   @fn    clickMoreButton(QString str)
		 *   @brief click item more button
		 */
		void clickMoreButton(int iIndex);

		/**
		 *   @fn    clearInput()
		 *   @brief Clears the input and shows the v.keyboard for new word input,
		 *   The old output and suggestion content is not cleared, and updated as per new word's search result
		 */
		void clearInput();

		/**
		 *   @fn    searchWordforPopup()
		 *   @brief searches the word in the dictionary/ies and displays the result as per mode of operation (in tabs/ popup dialog)
		 */
		QString searchWordforPopup(QString str);

		/**
		 *   @fn    searchSuggestionWord(QString str)
		 *   @brief searches the word in the dictionary/ies and displays the result as per mode of operation (in tabs/ popup dialog)
		 *   @param bforwardNext true indicate search next, false indicate search previous
		 */
		void searchSuggestionWord(QString str="", bool bforwardNext = true, bool bsearchfrominput = true);

		/**
		 *   @fn    exitDictionary()
		 *   @brief Hides the main dictionary application window
		 *   The application is not closed as then showing the popup when looking up the word from reader takes some time due to loading the dictionary again
		 */
		void exitDictionary();

		/**
		 *   @fn showDictionary()
		 *   @brief This slot is called when user clicks on the maximise button in the pop up dialog to go to main dictionary application
		 */
		void showDictionary();

		/**
		 *   @fn dictionaryLoaded()
		 *   @brief This slot is called when the dictionaries loading thread is finished execution
		 *          It also updates the Stardictionary info (using updateStarDictionaryInfo() function) for the new loaded dictionaries
		 */
		void dictionaryLoaded();

		void notifyLauncherShowed();

		void search_Handler();

		void typeChanged_Handler();

		void Handler_SetInputFocus();
};
#endif
