#ifndef IMAGEVIEWERWIDGET_H
#define IMAGEVIEWERWIDGET_H

#include "basewidget.h"
#include "basepushbutton.h"
#include "gallerymiddlewidgets.h"
#include "imagedetailwidget.h"
#include "imageviewer.h"
#include "imagecontroler.h"

#include <QLabel>

class GalleryMiddleWidgets;

/**
 * one of three stacked widgets.
 * it show the all specific image detail and provide some other image operation.
 */
class ImageViewerWidget:public BaseWidget
{
    Q_OBJECT
public:
    ImageViewerWidget(QWidget *parent = 0);

    void updateRes(QString imagePath,QImage image);
private:
    void initLayout();
    void initConnection();
private:
    GalleryMiddleWidgets *m_middleWidgets;

    QMap<QString,QImage> m_imagesRes;
    QString m_imagePath;
    QImage m_image;

    // ImageViewer: display and operate the image
    ImageViewer *m_imageViewer;
    ImageControler *m_imageControler;

protected:

private slots:
    void slot_onImagesResChanged(QMap<QString,QImage>,bool);
    void slot_lastImage();
    void slot_nextImage();
    void slot_viewDetail();
    void slot_deleteImage();
    void slot_imageZoomOut();
    void slot_imageZoomIn();
    void slot_imageRotate();
signals:
    void imagesResChanged(QMap<QString,QImage>,bool);
};

#endif // IMAGEVIEWERWIDGET_H
