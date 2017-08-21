#ifndef THUMBIMAGEITEM_H
#define THUMBIMAGEITEM_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QStackedLayout>
#include <QHBoxLayout>

#include "brightnessmapper.h"

class ThumbImageItem:public QWidget
{
public:
    ThumbImageItem(QString imagePath,QImage *image);

    void onItemClick();
    QImage* getImage(){return m_image;}
    QString getImagePath(){return m_imagePath;}
    bool getCheckState(){return isChecked;}
private:
    void initLayout();
private:
    bool isChecked;
    bool toUpdate;

    brightnessMapper *imageMapper;
    QStackedLayout *m_stackedLayout;

    QLabel *thumbImage;
    QLabel *m_checkImage;
    QImage *m_image;
    QString m_imagePath;
protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
};

#endif // THUMBIMAGEITEM_H
