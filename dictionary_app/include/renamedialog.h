/**************************************************
  copyright:asus
  author:mars_li@asus.com.cn
**************************************************/
#ifndef RENAMEDIALOG_H
#define RENAMEDIALOG_H

#include"basedialog.h"
#include<QPushButton>
#include<QImage>
#include<QLineEdit>
#include<QLabel>

typedef bool (*existStr)(QString s);

class RenameButton : public QPushButton
{
	Q_OBJECT
public:
	RenameButton(QString btnNormal, QString btnDown, QString btnOver, QString btnDisable, QWidget* parent= NULL);
	QString getBtnStyle();
protected slots:
private:
	QString btnNormal, btnDown, btnOver, btnDisable; //different skins of the button
};

class ReNameDialog : public BaseDialog
{
    Q_OBJECT
public:
    ReNameDialog(QWidget* );
    ~ReNameDialog();
public:
    QString getText();
    void setString(QString& str);
    void setExistStr(existStr val);

protected:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
    void clickOk();

    void init();

    void setTitle(QString str);
    void showEvent(QShowEvent *e);
    bool eventFilter(QObject *, QEvent *);
protected slots:
    void clicked();
    void textChanged(QString);


private:
    QLabel* m_label;
    QPixmap* m_backImg;
    QPoint mRenameBkImgPos;
    QLineEdit * m_edit;
    RenameButton* m_btnOk;
    RenameButton* m_btnCancel;
    QString  m_text;


    existStr m_existStr;
};

#endif // RENAMEDIALOG_H
