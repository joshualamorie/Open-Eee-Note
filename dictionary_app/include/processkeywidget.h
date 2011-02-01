#ifndef PROCESSKEYWIDGET_H
#define PROCESSKEYWIDGET_H

#include <QObject>
#include<QWidget>
#include<QCopChannel>

class ProcessKeyWidget : public QObject
{
    Q_OBJECT
public:
    ProcessKeyWidget(QObject* );
    void setCheckFocusWidget(QWidget* checkWidget, QWidget* toFocusWidget);

protected slots:
    void channelHandle(const QString & message, const QByteArray & data);

private:
#ifdef Q_WS_QWS
    QCopChannel* m_channel;
#endif
    QWidget* m_checkWidget;
    QWidget* m_toFocusWidget;
};

#endif // PROCESSKEYWIDGET_H
