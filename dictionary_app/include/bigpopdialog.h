#ifndef BIGPOPDIALOG_H
#define BIGPOPDIALOG_H

#include<QDialog>
#include<QPaintEvent>
#include<QPushButton>



class BigPopDialog:public QDialog
{
    Q_OBJECT
public:
    BigPopDialog(QWidget* );
    ~BigPopDialog();

    QRect getOffsetRect(){return m_offsetRect;}

protected:
    virtual void paintEvent(QPaintEvent *);

//    virtual void showEvent(QShowEvent *);

private:
    QRect m_offsetRect;
    QPixmap* m_background;

};

#endif // BIGPOPDIALOG_H
