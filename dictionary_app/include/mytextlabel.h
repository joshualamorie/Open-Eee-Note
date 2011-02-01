#ifndef MYTEXTLABEL_H
#define MYTEXTLABEL_H

#include <QWidget>
#include<QTextOption>

class MyTextLabel : public QWidget
{
public:
    MyTextLabel(QWidget* );
    void setTextMsg(const QString& str);
    void setAlignment(Qt::Alignment align);
    void setWrapMode(QTextOption::WrapMode wrap);

protected:
    void paintEvent(QPaintEvent *);
    QString m_msgText;

    QTextOption m_textOption;
};

#endif // MYTEXTLABEL_H
