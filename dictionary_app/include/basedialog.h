/**************************************************
  copyright:asus
  author:mars_li@asus.com.cn
**************************************************/
#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include <QDialog>
#include<QPaintEvent>
#include<QShowEvent>

#include"capturebackground.h"
#include"sendtitlemessage.h"
class BaseDialog : public QDialog, public CaptureBackground
{
    Q_OBJECT
public:
    BaseDialog(QWidget* parent);
    ~BaseDialog();

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual bool eventFilter(QObject *, QEvent *);
    virtual void closeEvent(QCloseEvent *);

private:
    SendTitleMessage m_setTitle;
};

#endif // BASEDIALOG_H
