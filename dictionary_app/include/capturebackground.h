#ifndef CAPTUREBACKGROUND_H
#define CAPTUREBACKGROUND_H

#include<QPixmap>
#include<QPainter>

class CaptureBackground
{
public:
    CaptureBackground();
    ~CaptureBackground();

public:
     void captureActiveWin();
     void drawBackWindow(QPainter& painter);
     void mixBackWindow(QPixmap* image);
     QPixmap* getBackPixmap();

private:
      QPixmap* m_backPixmap;
};

#endif // CAPTUREBACKGROUND_H
