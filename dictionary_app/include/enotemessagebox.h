#ifndef ENOTEMESSAGEBOX_H
#define ENOTEMESSAGEBOX_H

#include "basedialog.h"
#include <QLabel>
#include <QPushButton>
#include<QTextEdit>
#include"mytextlabel.h"

typedef enum _MsgType{
    MSG_OK,
    MSG_OKCANCEL,
}MsgType;

class EnoteMessageBox : public BaseDialog
{
    Q_OBJECT
public:
    EnoteMessageBox(QWidget* , MsgType type = MSG_OK);
    void setMessage(const QString& msg);
    void setOKText(QString& text);
    void setCancelText(QString& text);

signals:
    void OkClicked();
    void CancelClicked();

protected:
    void init();

    QString getStyle();
    QString formatBtnStyle(QString p_normal, QString p_hover,
                           QString p_press, QString p_disable);
    void keyReleaseEvent ( QKeyEvent * event );



protected slots:
    void clickOk();
    void clickCancel();

private:
    MsgType m_type;
    MyTextLabel* m_msgLabel;
    MyTextLabel* m_titleLabel;

    QPushButton* m_btnOk;
    QPushButton* m_btnCancel;
    QRect m_offsetRect;
    QLabel* m_bkLabel;

private slots:
    void orientationChanged();

};

#endif // ENOTEMESSAGEBOX_H
