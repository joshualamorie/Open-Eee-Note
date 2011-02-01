#ifndef IMAGEEDITORWIDGET_H
#define IMAGEEDITORWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "imageeditor.h"
#include <QLabel>
#include <QSlider>
#include<slider.h>
#include"bigpopdialog.h"

class CommonButton;
class CommonRadioButton;

class ImageEditorWidget : public BigPopDialog
{
    Q_OBJECT
public:
    ImageEditorWidget(QWidget*);
    ~ImageEditorWidget();

public:
    void setImage(const QString& photoPath);
    void setImage(QImage* image);
    void disableOption(bool val);
    void hideOption(bool val);
    void changeFormat(bool val);

    QImage* getCropImage();

signals:
    void click(int val);

public slots:
    void clickZoomIn();
    void clickZoomOut();
    void clickCrop(bool);
    void clickLeftRotate();
    void clickRightRotate();
    void clickOk();
    void clickCancel();
    void sliderReleased();
    void clickReset();
    void sliderValueChanged(int );
    void cropChecked(bool );
    void zoomChecked(bool );
    void rotateChecked(bool );

private:
    QString formatBtnStyle(QString p_normal, QString p_hover,
                           QString p_press, QString p_disable);

    QString formatSliderStyle();



private:
    ImageEditor* m_imageEditor;

    CommonButton* m_btnZoomIn;
    CommonButton* m_btnZoomOut;
    ENoteToolBar::Slider* m_slider;
    CommonButton* m_btnCrop;
    CommonButton* m_btnLeftRotate;
    CommonButton* m_btnRightRotate;
    CommonButton* m_btnOk;
    CommonButton* m_btnCancel;


    CommonButton* m_btnReset;

    QLabel* m_labelSplit1;
    QLabel* m_labelSplit2;
    QLabel* m_labelSplit3;

    QLabel* m_label;
    QImage* m_cropImage;


};

#endif // IMAGEEDITORWIDGET_H
