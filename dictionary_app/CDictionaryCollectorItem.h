/*
  Copyright (C) 2010 ASUSTeK Computer Inc. All rights reserved.
*/

#ifndef __PHOTOCOLECTORITEM_H
#define __PHOTOCOLECTORITEM_H

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QtGui/QTextEdit>
#include <QGroupBox>
#include <QPoint>
#include <QMouseEvent>

//"Helvetica"
// ALLTEXTFONT is mean the font style of whole project.
#ifndef ALLTEXTFONT
	#define ALLTEXTFONT		"unifont"
#endif


#define COLLECTORLASTITEM	7


#define	COLLECTITEMWIDTH				728 //high light 726x110
#define	COLLECTITEMHIGHT				105

#define COLLECTORITEMMORETEXTWIDTH		700
#define COLLECTORITEMMORETEXTHEIGHT		75

/**************************************************************************************
  * class CDictionaryCollectorItem:
  * manage the explanation area of searching word, display the item text also consider
  * exceeding the rang
  *
**************************************************************************************/
class CDictionaryCollectorItem : public QWidget
{
	Q_OBJECT

public:
	CDictionaryCollectorItem(QWidget *p = 0);
	~CDictionaryCollectorItem();

	void	setItemText(QString str);
	QString    getItemText(){return m_ItemText;};
	void	setItemMoreText(QString str);
	void	setIndex(int index) { m_index = index; };
	int		getIndex() const { return m_index; };

	void	setSelect(bool select){ m_Selected =select; };
	bool	IsSelect(){ return m_Selected;};

	static void LoadImageRes();
	static void UnLoadImageRes();

protected:
	void	mousePressEvent(QMouseEvent * event);
	void	mouseReleaseEvent(QMouseEvent * event);
	void	paintEvent(QPaintEvent * event);

signals:
	void	ItemPressed_Event(int index);
	void	ItemReleased_Event(int iIndex);

public:
	QString			m_ItemText;
	QString			m_ItemMoreText;
	int				m_index;
	bool			m_Selected;

	QPoint			m_PtDown;
	QPoint			m_PtUp;
	//bool			m_mousePressed;

	static QPixmap	*m_HighLightPixmap; //highlight
};

#endif
