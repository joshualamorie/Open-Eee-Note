#ifndef SEARCHBUTTONWIDGET_H
#define SEARCHBUTTONWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "popwidget.h"
#include "btnnofocusstyle.h"
#include"processkeywidget.h"

class JLLineEdit:public QLineEdit
{
	Q_OBJECT
public:

	JLLineEdit(QWidget * parent = 0):QLineEdit(parent){m_bFocusIn = false;}
private:
	virtual void focusInEvent ( QFocusEvent * e );
	virtual void mouseReleaseEvent( QMouseEvent * e );

	bool m_bFocusIn;
};

class SearchButtonWidget:public QWidget
{
       Q_OBJECT
public:

        SearchButtonWidget(QWidget*);
	void setbtnSearchEnable(bool val);
	void setbtnArrowEnable(bool val);
        void setInputString(const QString& s, bool selected = true);
        void setStrings(const QStringList& s, int defaultID = 0);
        void clearInputText();
        void setSelectItemId(int id);
	QString getSearchString();
	QString getCurTypeStr();
        int getCurTypeId();
		JLLineEdit* getLineEdit(){return m_textInput;}
private:
    QString formatBtnStyle(QString p_normal, QString p_hover,
                           QString p_press, QString p_disable);

    QString getLineEditStyle();

    bool eventFilter(QObject *, QEvent *);


signals:
    void typeChanged();
    void search();
    void clearText();


public slots:
    void arrowClick();
    void clearClick();
    void searchClick();
    void popWidgetHide();
    void textInputChanged(const QString&);



private:
    QPushButton* m_btnSearch;
    QPushButton* m_btnArrow;
    QPushButton* m_btnClear;
    QLabel* m_labelBackground;
	JLLineEdit* m_textInput;
    PopWidget* m_popWidget;
    BtnNoFocusStyle m_btnstyle;
    ProcessKeyWidget* m_processWidget;



    QString m_searchText;
    QString m_typeString;

};

#endif // SEARCHBUTTONWIDGET_H
