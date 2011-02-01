/*
  Copyright (C) 2010 ASUSTeK Computer Inc. All rights reserved.
*/


#include "managedict.h"
#include "global.h"

SetUp::SetUp(QWidget *parent, Dictionary * dict): QDialog(parent)
{
    dictManage=dict;                // Local copy of Dictionary class object
    setupUi(this);                  // Call function from ui_dictionary_setup.h for Dictionary Set Up UI design
    this->resize(parent->size());
    this->move(parent->pos());
    this->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    connect(increaseFontButton, SIGNAL(clicked()), this, SLOT(increaseFontSize()));
    connect(decreaseFontButton, SIGNAL(clicked()), this, SLOT(decreaseFontSize()));

    QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);    //!< Size policy of checkboxes
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);

    QFont font3;
    font3.setFamily(QString::fromUtf8("Bitstream Vera Sans"));
    font3.setPointSize(16);
    font3.setBold(false);
    font3.setWeight(50);
    dictNameCheckBox = new QCheckBox[dictManage->GetMaximumDictionaries()];       //!< create new checkboxes
    // Set UI design features of all the checkboxes for all available dictionaries
    for (int i=0; i< dictManage->GetMaximumDictionaries(); i++)
    {
        dictNameCheckBox[i].setParent(checkBoxFrame);
        sizePolicy2.setHeightForWidth(dictNameCheckBox[i].sizePolicy().hasHeightForWidth());
        dictNameCheckBox[i].setSizePolicy(sizePolicy2);
        dictNameCheckBox[i].setMinimumSize(QSize(300, 50));
        dictNameCheckBox[i].setMaximumSize(QSize(16777215, 60));
        dictNameCheckBox[i].setFont(font3);
        dictNameCheckBox[i].setFocusPolicy(Qt::NoFocus);
        // Load appropriate stylesheet - the checkboxes having dictionaries or, the extra ones
        if(i< dictManage->StarDictionaries.size())
            dictNameCheckBox[i].setStyleSheet(dictManage->loadStyleSheet("data/stylesheet/StyleSheetDictCheckBox.txt"));
        else
        {
            dictNameCheckBox[i].setStyleSheet(dictManage->loadStyleSheet("data/stylesheet/StyleSheetDictCheckBoxExtra.txt"));
            dictNameCheckBox[i].setDisabled(true);
        }
        verticalLayout->addWidget(dictNameCheckBox+i);
    }
        verticalLayout_3->addWidget(checkBoxFrame);
        verticalLayout_3->addWidget(readerDictionaryFrame);
        verticalLayout_3->addWidget(fontConfigFrame);

        fontSizeValueLabel->setText(QString::number(dictManage->GetOutputFontSize()));

        readerDictionaryComboBox->setEditable(false);
        readerDictionaryComboBox->setAttribute(Qt::WA_InputMethodEnabled, false);
        readerDictionaryComboBox->view()->setAttribute(Qt::WA_InputMethodEnabled,false);
        readerDictionaryComboBox->setFocusPolicy(Qt::NoFocus);
        readerDictionaryComboBox->view()->setFocusPolicy(Qt::NoFocus);
        readerDictionaryComboBox->view()->setTextElideMode(Qt::ElideRight);
        readerDictionaryComboBox->setStyleSheet(dictManage->loadStyleSheet("data/stylesheet/StyleSheetReaderDictComboBox.txt"));

    // Set all available dictionaries names and check the selected dictionaries
    for (int i=0; i < dictManage->StarDictionaries.size(); i++)
    {
        QString currentIndexDictionary = dictManage->StarDictionaries[i].name;
        dictNameCheckBox[i].setText(currentIndexDictionary);
        readerDictionaryComboBox->insertItem(i, currentIndexDictionary);
        if(dictManage->StarDictionaries[i].dictSelectMode == DICTIONARY || dictManage->StarDictionaries[i].dictSelectMode == BOTH)
            dictNameCheckBox[i].setChecked(true);
        if(dictManage->StarDictionaries[i].dictSelectMode == READER || dictManage->StarDictionaries[i].dictSelectMode == BOTH)
            readerDictionaryComboBox->setCurrentIndex(i);
    }
    readerDictionaryComboBox->clearFocus();
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeSetUp()));      //!< close setup and back to dictionary home

    // set event filter on all QPushButtons
    decreaseFontButton->installEventFilter(mousePressEater);
    increaseFontButton->installEventFilter(mousePressEater);
    closeButton->installEventFilter(mousePressEater);
}


SetUp::~SetUp()
{
    delete [] dictNameCheckBox;
}

void SetUp::increaseFontSize()
{
    if(fontSizeValueLabel->text().toInt() < maximumFontSize)
        fontSizeValueLabel->setText(QString::number(fontSizeValueLabel->text().toInt() + 1));
}


void SetUp::decreaseFontSize()
{
    if(fontSizeValueLabel->text().toInt() > minimumFontSize)
        fontSizeValueLabel->setText(QString::number(fontSizeValueLabel->text().toInt() - 1));
}


void SetUp::closeSetUp()
{
    readerDictionaryComboBox->clearFocus();
    dictManage->UpdateOutputFontSize(fontSizeValueLabel->text().toInt());
    for(int i=0; i < dictManage->StarDictionaries.size(); i++)   // Update the selected dictionaries names
    {
        if(dictNameCheckBox[i].isChecked())
            dictManage->StarDictionaries[i].dictSelectMode = DICTIONARY;
        else
            dictManage->StarDictionaries[i].dictSelectMode = NONE;
    }

    if(readerDictionaryComboBox-> count() != 0)
    {
        int readerDictIndex = readerDictionaryComboBox->currentIndex();
        if (dictManage->StarDictionaries[readerDictIndex].dictSelectMode == DICTIONARY)
            dictManage->StarDictionaries[readerDictIndex].dictSelectMode = BOTH;
        else
            dictManage->StarDictionaries[readerDictIndex].dictSelectMode = READER;
    }
    emit sigClosingSetup(); //emit signal to update set up options in main Dictionary
}
