#ifndef GALLERYMIDDLEWIDGETS_H
#define GALLERYMIDDLEWIDGETS_H

#include "basewidget.h"
#include "emptyimageswidget.h"
#include "thumbimagewidget.h"
#include "imageviewerwidget.h"

#include <QStackedLayout>
#include <QListWidget>
#include <QFileInfoList>

class ThumbImageWidget;
class ImageViewerWidget;

/**
 * The middle part of application.
 *
 * There is three stacked widget on it and it will change show state
 * by images state.
 * The three stacked widgets are: viewer、empty、thumb
 */
class GalleryMiddleWidgets:public BaseWidget
{
    Q_OBJECT
public:
    GalleryMiddleWidgets(QWidget *parent);

    bool isViewerMode(){return m_stackedMainLyout->currentWidget() == (QWidget*)m_viewerWid;}
    void leaveViewerMode();
private:
    // Main stacked layout
    QStackedLayout *m_stackedMainLyout;
    EmptyImagesWidget *m_emptyImgWid;
    ThumbImageWidget *m_thumbImgWid;
    ImageViewerWidget *m_viewerWid;

    void initLayout();
    void initConnection();
    QFileInfoList findImgFiles(const QString& path);
signals:
    void imagesResChanged();
    void sig_imagesResInsert(QString path,QImage* img);
    void sig_imagesResRemove(QString path,QImage* img);
    void imageEmpty();
    void imageItemClick(QString imagePath,QImage *image);
    void viewerResChanged(QString filePath);
private slots:
    void slot_showEmptyImageTip();
    void slot_showImageViewer(QString imagePath,QImage *image);
    void slot_onImagesResChanged();
    void slot_imagesResInsert(QString path,QImage* img);
    void slot_imagesResRemove(QString path,QImage* img);
};

#endif // GALLERYMIDDLEWIDGETS_H
