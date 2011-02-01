#ifndef SEARCHBUTTONWIDGETNEW_H
#define SEARCHBUTTONWIDGETNEW_H

#include <QWidget>
#include<QLabel>
#include<QLineEdit>
#include<QPixmap>
#include<QPushButton>
#include"processkeywidget.h"
#include"btnnofocusstyle.h"

class SearchButtonWidgetNew : public QWidget
{
    Q_OBJECT
public:
    SearchButtonWidgetNew(QWidget*);
    void setInputString(const QString& s, bool selected = true);
    void clearInputText();
    void setSearchType(const QString&);
    void setShowTagList(bool val);
    void setShowClose(bool val);
    QString getSearchString();
    QLineEdit* getLineEdit(){return m_textInput;}

signals:
     void search();
     void closeSearch();
     void tagList();

 protected:
     bool eventFilter(QObject *, QEvent *);
      QString getLineEditStyle();
      QString getBtnStyle();
      QString getBtnSearchStyle();
      void adjustWidgetByString(QPushButton *widget);

  protected slots:
      void clickClose();
      void clickTagList();
      void clickSearch();



 private:
     QLabel* m_labelBackground;
     QLabel* m_labelType;
     QLineEdit* m_textInput;
     QPushButton* m_btnClose;
     QPushButton* m_btnTagList;
     QPushButton* m_btnSearch;
     ProcessKeyWidget* m_processWidget;
     BtnNoFocusStyle m_btnstyle;

     QPixmap m_pixmapNormal;
     QPixmap m_pixmapPress;

     bool m_showTagList;

};

#endif // SEARCHBUTTONWIDGETNEW_H
