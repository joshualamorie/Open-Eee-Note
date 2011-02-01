#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include <QWidget>
#include<QPainter>
#include<QPixmap>
#include<QMouseEvent>
#include<QPaintEvent>

class ImageEditor : public QWidget
{
public:
    ImageEditor(QWidget* );
    ~ImageEditor();
public:
    void setImage(QImage* );
    void setImage(const QString& );
    void rotate(int );
    void zoom(float);
    float getZoomVal();
    int getRotateVal();
    void reset();
    void setChangeFormat(bool val = true);

    void setShowDropRect(bool val);
    QImage* getOriginalImage(){return m_image;}

    QImage* crop();
    QImage* getFullScreeCrop();


protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    float getImageRadio(int imageWidth, int imageHeight);
    QImage* cropForPhoto();
    QImage* cropForNote();


private:
    void init();
    void adjustRect(const QPoint&);
    int  checkRect(const QPoint&);
    void clearCropRect();
    QRegion getCorpRectRegion();
    void drawCropRect(QPainter& painter );

    void centerBkImage();
    void redrawBkImage(bool resetCropRect = false);

private:
    float m_zoomVal;
    int m_rotateVal;
    int m_nclickRect;

    bool m_bPressFlag;

    bool m_changeFormat;

    bool m_dropImage;

    QImage* m_image;
    QPixmap* m_bkImage;

    QPixmap m_cropRectImg;

    QPixmap m_upLineImg;
    QPixmap m_downLineImg;
    QPixmap m_leftLineImg;
    QPixmap m_rightLineImg;
    QPixmap m_dropPixmap;


    QRect m_rects[8];
    QRect m_cropRect;
    QRect m_lastCropRect;

    QRect m_resetRect;
    float m_resetZoom;

    QRect m_bkRect;

    QPoint m_lastPoint;

    bool m_showDropRect;

};

#endif // IMAGEEDITOR_H
