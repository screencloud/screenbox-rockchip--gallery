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

    QMap<QString,QImage> getImageResFromPath(const QString& path = QString(""));
    void updateResUi(QMap<QString,QImage> imageRes);
    bool isViewerMode(){return m_stackedMainLyout->currentWidget() == (QWidget*)m_viewerWid;}
    void leaveViewerMode();
    void addRefreshSuffix(QString suffix);
private:
    GalleryMiddleWidgets *m_middleWidgets;
    QFileInfoList m_imagesInfoList;
    QMap<QString,QImage> m_imagesRes;

    // Main stacked layout
    QStackedLayout *m_stackedMainLyout;
    EmptyImagesWidget *m_emptyImgWid;
    ThumbImageWidget *m_thumbImgWid;
    ImageViewerWidget *m_viewerWid;

    // List of search suffix when refresh image resource.
    QList<QString> m_refreshSuffixList;

    void initLayout();
    void initConnection();
    void initData();
    QFileInfoList findImgFiles(const QString& path);
signals:
    void imagesResChanged(QMap<QString,QImage>);
    void imageEmpty();
    void imageItemClick(QString imagePath,QImage image);
    void viewerResChanged(QString filePath);
private slots:
    void slot_showEmptyImageTip();
    void slot_showImageViewer(QString imagePath,QImage image);
    void slot_onImagesResChanged(QMap<QString,QImage>);
};

#endif // GALLERYMIDDLEWIDGETS_H
