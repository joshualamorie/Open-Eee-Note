#ifndef ANIMALTHREAD_H
#define ANIMALTHREAD_H

#include<QThread>
#include<QPixmap>
#include<QMutex>
#include<QEvent>
#include<QWaitCondition>



class StopEvent:public QEvent
{
public:
    StopEvent():QEvent((QEvent::Type)registerEventType(QEvent::User+11)){}
};

class AnimalThread:public QThread
{
    Q_OBJECT
public:
    AnimalThread();
    ~AnimalThread();

    void captureWindow(QWidget* parent);

    void toStart(QWidget* parent);
    void toStop();
    void weakup();



protected:
    void openDisplay(int x, int y, int w, int h);
    void closeDisplay();
    void writeDisplay(int offset , int len);
    void init();

    void run();
private:
    QRect m_offsetRect;
    QImage* m_background;
    QImage m_updatePixmap;
    QImage* m_loadImg[8];
    QRect m_updateRect;
    bool m_isStop;
    int  m_curIndex;
    QMutex m_mutex;
    QWaitCondition m_wait;
};

#endif // ANIMALTHREAD_H
