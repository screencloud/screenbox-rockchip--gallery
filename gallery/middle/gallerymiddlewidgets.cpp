#include "gallerymiddlewidgets.h"
#include <QDir>
#include <qdebug.h>
#include <QApplication>
#include <QDirIterator>
#include <QDateTime>

#include "global_value.h"

GalleryMiddleWidgets::GalleryMiddleWidgets(QWidget *parent):BaseWidget(parent)
{
    // Set background color.
    setObjectName("GalleryMiddleWidgets");
    setStyleSheet("#GalleryMiddleWidgets{background:rgb(43,45,51)}");

    initLayout();
    initConnection();
    initData();
}

void GalleryMiddleWidgets::initData()
{
    m_refreshSuffixList.append("jpg");
    m_refreshSuffixList.append("png");
    m_refreshSuffixList.append("bmp");
    m_refreshSuffixList.append("jpeg");
    m_refreshSuffixList.append("svg");
    m_refreshSuffixList.append("titf");
    m_refreshSuffixList.append("gif");

}

void GalleryMiddleWidgets::initLayout()
{
    m_stackedMainLyout = new QStackedLayout(this);

    m_emptyImgWid = new EmptyImagesWidget(this);
    m_thumbImgWid = new ThumbImageWidget(this);
    m_viewerWid = new ImageViewerWidget(this);

    m_stackedMainLyout->addWidget(m_emptyImgWid);
    m_stackedMainLyout->addWidget(m_thumbImgWid);
    m_stackedMainLyout->addWidget(m_viewerWid);
    m_stackedMainLyout->setCurrentWidget(m_thumbImgWid);

    setLayout(m_stackedMainLyout);
}

void GalleryMiddleWidgets::initConnection()
{
    connect(this,SIGNAL(imageEmpty()),this,SLOT(slot_showEmptyImageTip()));
    connect(this,SIGNAL(imageItemClick(QString,QImage)),this,SLOT(slot_showImageViewer(QString,QImage)));
    connect(this,SIGNAL(imagesResChanged(QMap<QString,QImage>)),this,SLOT(slot_onImagesResChanged(QMap<QString,QImage>)));
}

QFileInfoList GalleryMiddleWidgets::findImgFiles(const QString& path)
{
    QFileInfoList imageFiles;

    QDirIterator it(path,QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot);
    while (it.hasNext()){
        QString name = it.next();
        QFileInfo info(name);
        if (info.isDir()){
            imageFiles.append(findImgFiles(name));
        }else{
            for(int i=0;i<m_refreshSuffixList.count();i++){
                if(info.suffix().compare(m_refreshSuffixList.at(i),Qt::CaseInsensitive) == 0){
                    imageFiles.append(info);
                }
            }
        }
    }
    return imageFiles;
}

void GalleryMiddleWidgets::addRefreshSuffix(QString suffix)
{
    m_refreshSuffixList.append(suffix);
}

/**
 * @brief Used to find all image Res in path
 * @param path
 * @return
 */
QMap<QString,QImage> GalleryMiddleWidgets::getImageResFromPath(const QString& path)
{
    QFileInfoList imageFiles = this->findImgFiles(path);
    QMap<QString,QImage> imageRes;

    QImage tempImage;
    for(int i = 0;i < imageFiles.size();i++)
    {
        if(tempImage.load(imageFiles.at(i).absoluteFilePath()))
        {
            imageRes.insert(imageFiles.at(i).absoluteFilePath(),tempImage);
        }
    }
    return imageRes;
}

void GalleryMiddleWidgets::updateResUi(QMap<QString,QImage> imageRes)
{
    m_imagesInfoList.clear();
    m_imagesRes.clear();

    m_imagesRes = imageRes;
    slot_onImagesResChanged(m_imagesRes);
}

void GalleryMiddleWidgets::slot_showEmptyImageTip()
{
    emit viewerResChanged("");
    m_stackedMainLyout->setCurrentWidget(m_emptyImgWid);
}

void GalleryMiddleWidgets::slot_showImageViewer(QString imagePath, QImage image)
{
    m_stackedMainLyout->setCurrentWidget(m_viewerWid);
    m_viewerWid->updateRes(imagePath,image);
}

void GalleryMiddleWidgets::slot_onImagesResChanged(QMap<QString, QImage> imagesRes)
{
    m_imagesRes = imagesRes;
    if(m_imagesRes.size()>0)
    {
        if(m_stackedMainLyout->currentWidget()==m_emptyImgWid)
        {
            m_stackedMainLyout->setCurrentWidget(m_thumbImgWid);
        }
        emit m_thumbImgWid->imagesResChanged(m_imagesRes);
        emit m_viewerWid->imagesResChanged(m_imagesRes,m_stackedMainLyout->currentWidget()==m_viewerWid);
    }
    else
    {
        emit imageEmpty();
    }
}

void GalleryMiddleWidgets::leaveViewerMode()
{
    m_stackedMainLyout->setCurrentWidget(m_thumbImgWid);
}



