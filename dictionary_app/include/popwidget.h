#ifndef POPWIDGET_H
#define POPWIDGET_H

#include <QWidget>
#include <QStringList>
#include <QLabel>
#include <QList>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>



class PopWidget : public QWidget
{
     Q_OBJECT
public:

    class Item{
    public:
        Item(){}
        Item(const Item& it){
            m_rect = it.m_rect;
            m_str = it.m_str;
        }

    public:
        QRect  m_rect;
        QString m_str;
    };
signals:
    void tohiden();

public:
    PopWidget( QWidget*);
    void setStrings(const QStringList& s);
    void setSelectItem(int index);
    int getSelectItemId(){return m_selectItem;}
    QString getSelectString();


protected:
    void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *);
    void showEvent(QShowEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void drawItem(QPainter& paint);
    QString backgroundStyle();

private:
    QList<Item> m_items;

	int m_selectItem;
};

#endif // POPWIDGET_H
