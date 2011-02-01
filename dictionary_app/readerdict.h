/*
  Copyright (C) 2010 ASUSTeK Computer Inc. All rights reserved.
*/



#ifndef READERDICT_H
#define READERDICT_H
#include <QPainter>
#include <QScrollBar>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QFocusEvent>
#include "homedict.h"

#include "ui_dictionary_reader.h"

#define POPUPEXPLANATIONAREAPOSX		1
#define POPUPEXPLANATIONAREAPOSY		53
#define POPUPEXPLANATIONAREAWIDTH		258
#define POPUPEXPLANATIONAREAHEIGHT		158
#define POPUPEXPLANATIONAREAFONTSIZE	18
#define POPUPWORDFONTSIZE				30

#define POPUPWIDTH						265
#define POPUPHEIGHT						253

class CExplanationArea;

/**
  * Define ReaderDict class
  */
class ReaderDict : public QDialog, public Ui::readerDialog
{
    Q_OBJECT
	public:
        /**
          * ReaderDict Class Constructor
          */
		ReaderDict(QWidget *parent = 0);
		~ReaderDict();
		CExplanationArea	*m_pcPopupExplanationArea;
		bool	eventFilter( QObject * watched, QEvent * event );

    public slots:
        /**
         *   @fn closeReaderDict()
         *   @brief Hide the result displaying popup dialog and continue reading the book
         */
		void closeReaderDict();

        /**
         *   @fn toDictionary()
         *   @brief emit goToDictionary() signal and show the main dictiionary application
         */
		void toDictionary();

    signals:
        /**
         *   @brief signal connected to showDictionary() of Dictionary class to show main application window
         */
        void goToDictionary();
};
#endif
