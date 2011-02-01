/*
  Copyright (C) 2010 ASUSTeK Computer Inc. All rights reserved.
*/


#include "CDictionaryCollectorItem.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

#define __DEBUG__ qDebug() <<__func__<<"()  Line:"<< __LINE__ << "   "



QPixmap	*CDictionaryCollectorItem::m_HighLightPixmap =NULL; //highlight


void CDictionaryCollectorItem::LoadImageRes()
{
	if(!m_HighLightPixmap)
	{
		m_HighLightPixmap =new QPixmap();
		m_HighLightPixmap->load(QString(":images/v_highlight.png"));
	}
}
void CDictionaryCollectorItem::UnLoadImageRes()
{
	if(m_HighLightPixmap!=NULL)
	{
		delete m_HighLightPixmap;
		m_HighLightPixmap=NULL;
	}
}

CDictionaryCollectorItem::CDictionaryCollectorItem(QWidget *p) : QWidget(p),
	m_index(0),m_Selected(false)
{
	CDictionaryCollectorItem::LoadImageRes();
}


CDictionaryCollectorItem::~CDictionaryCollectorItem()
{}

void CDictionaryCollectorItem::setItemText(QString str)
{
	m_ItemText = str;
}

void CDictionaryCollectorItem::setItemMoreText(QString str)
{
	m_ItemMoreText = str;
}

//void CDictionaryCollectorItem::mousePressEvent(QMouseEvent * event)
//{
//	emit EnterItem(m_index);
//}

void CDictionaryCollectorItem::mousePressEvent(QMouseEvent * event)
{
	m_PtDown.setX(event->pos().x());
	m_PtDown.setY(event->pos().y());
//	qDebug()<<"the down pt is %d, %d"<<mPtDown.x()<<m_PtDown.y();
	//m_mousePressed =true;
	emit ItemPressed_Event(m_index);
}

void CDictionaryCollectorItem::mouseReleaseEvent(QMouseEvent * event)
{
//	if(m_mousePressed)
//	{
		m_PtUp.setX(event->pos().x());
		m_PtUp.setY(event->pos().y());
//		qDebug()<<"the down pt is %d, %d"<<m_PtUp.x()<<m_PtUp.y();
		//QRect rc(0, 0, rcViewItem.width(), rcViewItem.height());
		QRect rc(0, 0,COLLECTITEMWIDTH, COLLECTITEMHIGHT);
		if(rc.contains(m_PtDown) && rc.contains(m_PtUp))
		{
			emit ItemReleased_Event(m_index);
			qDebug() << "emit release event"<<"\n";
		}
//		m_mousePressed =false;
//	}
}



/***********************************************************************
  *how to convert m_ItemText and m_ItemMoreText to screen for display
  *are mainly decieded by rewrite this function
  *1.Utilizing the m_ItemText m_ItemMoreText and m_Index to display
  *2.controlling the positions
  *3.when update or otherrepaint event occur this function action
 **********************************************************************/
void CDictionaryCollectorItem::paintEvent(QPaintEvent * event)
{
	QPainter paint(this);

	//Draw highlight
	if(IsSelect())
	{
		qDebug()<< "Drawing highlight............ "<<"\n";
		paint.drawPixmap(0, 0,COLLECTITEMWIDTH,COLLECTITEMHIGHT, *m_HighLightPixmap);//*m_HighLightPixmap
	}

	//QFont suggestionFont("Helvetica", 22, QFont::Normal);
	QFont suggestionFont;
#ifdef DEFINEMYLOCALSETFAMILY
	suggestionFont.setFamily(ALLTEXTFONT);
#endif
	suggestionFont.setPointSize(24);
	suggestionFont.setWeight(QFont::Normal);
	//suggestionFont.setWeight(QFont::Bold);

	paint.setFont(suggestionFont);

	QString str;
	QFontMetrics fontMetrics(suggestionFont);
	str = fontMetrics.elidedText(m_ItemText, Qt::ElideRight, COLLECTITEMWIDTH-30, Qt::TextSingleLine);
	paint.drawText(5, 2, COLLECTITEMWIDTH-30, 30, Qt::TextSingleLine | Qt::AlignVCenter | Qt::AlignLeft, str);
	//qDebug()<< "<<<<ooooooo>>>> "<<m_ItemText <<"\n";

	//QFont moreFont("Helvetica", 20, QFont::Normal);
	QFont moreFont;
#ifdef DEFINEMYLOCALSETFAMILY
	moreFont.setFamily(ALLTEXTFONT);
#endif
	moreFont.setPointSize(20);
	moreFont.setWeight(QFont::Normal);
//	QRect rect = fontMetrics.boundingRect(185, 5, COLLECTORITEMMORETEXTWIDTH, COLLECTORITEMMORETEXTHEIGHT,
//						Qt::TextWordWrap | Qt::AlignVCenter | Qt::AlignLeft,m_ItemMoreText);
//	int lineheight=fontMetrics.lineSpacing();//37calculate the height between two lines.
//	int nTotalheight =rect.height()+fontMetrics.leading();
//	int ntotalLines =nTotalheight/lineheight;
	QFontMetrics fontMetricsM(moreFont);
	int tmp=fontMetricsM.height()-COLLECTORITEMMORETEXTHEIGHT%fontMetricsM.lineSpacing();
	//__DEBUG__<< "tmp="<<tmp<<"\n";
	int height =(COLLECTORITEMMORETEXTHEIGHT/fontMetricsM.lineSpacing())*fontMetricsM.lineSpacing();
	height=tmp>(69-height)?height:height+fontMetricsM.height();// 100 - 4(lineheight) =96 max expand height

//	int tmp =5-COLLECTORITEMMORETEXTHEIGHT%fontMetricsM.lineSpacing();
//	int ystart = tmp<1? 1:tmp;

	//__DEBUG__<< "ystart="<<"  height="<<height<<"lineSpacing" <<fontMetricsM.lineSpacing()<<"\n";
	paint.setFont(moreFont);

	//paint.drawText(185, 5, 550, 85, Qt::TextWordWrap | Qt::AlignVCenter | Qt::AlignLeft |Qt::AlignTop, m_ItemMoreText);
	paint.drawText(5,35, COLLECTORITEMMORETEXTWIDTH, height,
				Qt::TextWrapAnywhere  | Qt::AlignLeft |Qt::AlignTop , m_ItemMoreText); //| Qt::AlignVCenter

	//if(m_index != COLLECTORLASTITEM)
	//	paint.drawLine(20, COLLECTITEMHIGHT-1, 746, COLLECTITEMHIGHT-1);

	paint.drawLine(0, 0, 746, 0);
}
