#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include"basedialog.h"
#include<QLabel>
#include<QLineEdit>
#include<QPushButton>
#include<QRegExpValidator>
#include"processkeywidget.h"

class InputDialog : public BaseDialog
{
    Q_OBJECT
public:
    InputDialog(QWidget*);
    typedef bool (*checkInput)(const QString& s, InputDialog* pDialog);

public:
    void setMessage(const QString& str);
    void setErrorMessage(const QString& str);
    void setCheckFunc(checkInput check);
    void setInputPassword(bool flag = false);
    void setDefaultText(const QString& str, bool selected = true);
    void setMaxInputLength(int len = 100);
    void setShowLockIcon(bool val);
    QLineEdit* getInputPtr(){return m_input;}
    QString getInputStr();

protected slots:
    void clickOk();
    void clickCancel();
    void textChanged(QString );

private:
    void init();
    bool eventFilter(QObject *, QEvent *);
    QString getDialogStyle();
    QString getLineEditStyle();
    QString getLabelStyle();
    QString formatBtnStyle(QString p_normal, QString p_hover,
                           QString p_press, QString p_disable);
    void adjustGemotry(const QString&);


private:
    QRect m_offsetRect;
    QLabel* m_bkLabel;
    QPushButton* m_btnOk;
    QPushButton* m_btnCancel;
    QLabel* m_labelMsg;
    QLineEdit* m_input;
    QLabel* m_labelLockIcon;
    checkInput m_checkInput;

    QRegExpValidator* m_regVaild;

    QString m_msg;
    QString m_errorMsg;
    QString m_inputMsg;
    ProcessKeyWidget* m_processWidget;


};

#endif // INPUTDIALOG_H
