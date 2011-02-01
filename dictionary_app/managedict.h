/*
  Copyright (C) 2010 ASUSTeK Computer Inc. All rights reserved.
*/



#ifndef MANAGEDICT_H
#define MANAGEDICT_H

#include "dictionary.h"
//#include "ui_dictionary_setup.h"
#include <QtGui/QCheckBox>

class Dictionary;

/**
  * Define Set Up class for Dictionary SetUp Menu
  */
class SetUp : public QDialog, public Ui::setupDialog
{
    Q_OBJECT
    public:
        /**
          * Constructor of Set Up Class
          */
        SetUp(QWidget *parent = 0, Dictionary * dict=0);
        /**
          * Destructor of Set Up Class
          */
        ~SetUp();

    private:
        Dictionary *dictManage;      // Dictionary class pointer
        QCheckBox *dictNameCheckBox; // checkbox for select/ deselect the dictionaries
        static const int minimumFontSize = 15;
        static const int maximumFontSize = 30;

    public slots:
        /**
         *   @fn    closeSetUp()
         *   @brief exit from the Set up menu and go back to dictionary home
         *          The newly selected dictionaries and font settings are updated
         */
        void closeSetUp();

        void increaseFontSize();

        void decreaseFontSize();

    signals:
        /**
          * @brief signal emitted on exiting from Setup menu; It is connected to updatingSetup() of Dictionary class
          */
        void sigClosingSetup();
    };
#endif
