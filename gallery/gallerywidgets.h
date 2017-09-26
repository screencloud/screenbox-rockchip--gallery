#ifndef GALLERYWIDGETS_H
#define GALLERYWIDGETS_H

#include "basewidget.h"
#include "top/gallerytopwidgets.h"
#include "middle/gallerymiddlewidgets.h"
#include "global_value.h"

class LoadImageThread;

/**
 * The applicatiion contains top and middle widget and
 * each has it's own layout and connection control.
 */
class GalleryWidgets:public BaseWidget
{
    Q_OBJECT
public:
    GalleryWidgets(QWidget *parent);
    ~GalleryWidgets();

    GalleryMiddleWidgets* getMiddleWidget(){return m_middleWid;}
    /* Used for find out all image suffix files */
    QFileInfoList findImgFiles(const QString& path);
    /* This method used to slove the search result of 'MediaUpdateThread' */
    void processFileList(QFileInfoList fileInfoList);


    QMap<QString,QImage*>& getImagesRes(){return m_imagesRes;}
    void addRefreshSuffix(QString suffix);
    void removeImage(QString imagePath);
private:
    // Saved image resource\ Key with image path and value with image.
    QMap<QString,QImage*> m_imagesRes;
    // List of search suffix when refresh image resource.
    QList<QString> m_refreshSuffixList;

    GalleryTopWidgets *m_topWid;
    GalleryMiddleWidgets *m_middleWid;

    LoadImageThread *m_loadImageThread;

    void initData();
    void initConnection();
    void initLayout();

private slots:
    void slot_onReturnClicked();
    void slot_onViewerResChanged(QString);
public slots:
    void slot_onImagesResChanged();
    void slot_onImageLoadComplete();
signals:
    void loadImageComplete();
};

class LoadImageThread: public QThread
{
public:
    LoadImageThread(QObject *parent,GalleryWidgets *parentWidget,GalleryMiddleWidgets *middleWid);
    ~LoadImageThread();

    void setFileInfoList(QFileInfoList infoList);
    void stopThread();
private:
    GalleryWidgets *m_parent;
    QFileInfoList m_infoList;
    GalleryMiddleWidgets *m_middleWid;
protected:
    void run();
    void compressImg(QString src,QString out);
    QString fileMD5(QString path);
};
#endif // GALLERYWIDGETS_H
