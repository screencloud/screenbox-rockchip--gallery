#include "gallerywidgets.h"

#include <QDirIterator>

GalleryWidgets::GalleryWidgets(QWidget *parent):BaseWidget(parent)
{
    setStyleSheet("QLabel{color:white;}");

    initData();
    initLayout();
    initConnection();
}

void GalleryWidgets::initData(){
    m_refreshSuffixList.append("jpg");
    m_refreshSuffixList.append("png");
    m_refreshSuffixList.append("bmp");
    m_refreshSuffixList.append("jpeg");
    m_refreshSuffixList.append("svg");
    m_refreshSuffixList.append("titf");
    m_refreshSuffixList.append("gif");
}

void GalleryWidgets::initLayout()
{
    QVBoxLayout *vmainlyout = new QVBoxLayout;

    m_topWid = new GalleryTopWidgets(this);
    m_middleWid = new GalleryMiddleWidgets(this);

    vmainlyout->addWidget(m_topWid);
    vmainlyout->addWidget(m_middleWid);
    vmainlyout->setContentsMargins(0,0,0,0);
    vmainlyout->setSpacing(0);

    setLayout(vmainlyout);
}

void GalleryWidgets::initConnection()
{
    connect(m_topWid->getReturenButton(),SIGNAL(clicked(bool)),this,SLOT(slot_onReturnClicked()));
    connect(m_middleWid,SIGNAL(viewerResChanged(QString)),this,SLOT(slot_onViewerResChanged(QString)));
    connect(this,SIGNAL(loadImageComplete()),this,SLOT(slot_onImageLoadComplete()));
}

QFileInfoList GalleryWidgets::findImgFiles(const QString& path)
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

void GalleryWidgets::processFileList(QFileInfoList fileInfoList)
{
    LoadImageThread *thread = new LoadImageThread(this,this,fileInfoList);
    thread->start();
}

void GalleryWidgets::slot_onImageLoadComplete()
{
    slot_onImagesResChanged();
}

void GalleryWidgets::removeImage(QString imagePath)
{
    if(m_imagesRes.keys().contains(imagePath)){
        QMap<QString, QImage*>::iterator it = m_imagesRes.find(imagePath);
        delete it.value();
        m_imagesRes.remove(imagePath);
    }
}

void GalleryWidgets::slot_onImagesResChanged()
{
    emit m_middleWid->imagesResChanged();
}

void GalleryWidgets::addRefreshSuffix(QString suffix)
{
    m_refreshSuffixList.append(suffix);
}

void GalleryWidgets::slot_onReturnClicked()
{
    mainWindow->stopUeventLoop();
    if(m_middleWid->isViewerMode()){
        m_middleWid->leaveViewerMode();
        m_topWid->updateTopTitle(str_top_title);
    }else{
        mainWindow->close();
    }
}

void GalleryWidgets::slot_onViewerResChanged(QString imagePath)
{
    if(imagePath == ""){
        m_topWid->updateTopTitle(str_top_title);
    }else{
        QFileInfo *info = new QFileInfo(imagePath);
        if(info->exists())
        {
            m_topWid->updateTopTitle(info->fileName());
        }
    }
}

LoadImageThread::LoadImageThread(QObject *parent,GalleryWidgets *parentWidget,QFileInfoList fileInfoList):QThread(parent)
{
    m_parent = parentWidget;
    m_infoList = fileInfoList;

    qRegisterMetaType<QFileInfoList>("QFileInfoList");
    qRegisterMetaType<QMap<QString,QImage>>("QMap<QString,QImage*>");
}

void LoadImageThread::run()
{
    QMap<QString,QImage*> &imagesRes = m_parent->getImagesRes();

    QList<QString> filePathList;
    for(int i = 0;i < m_infoList.size();i++){
        filePathList.append(m_infoList.at(i).absoluteFilePath());
    }

    // Traverse map to delete non-existent item.
    QMap<QString,QImage*>::Iterator it;
    if(!filePathList.empty()){
        for(it = imagesRes.begin();it!=imagesRes.end();it++){
            if(!filePathList.contains(it.key())){
                qDebug("delete item: %s",qPrintable(it.key()));
                delete it.value();
                imagesRes.erase(it);
            }
        }
    }else{
        for(it = imagesRes.begin();it!=imagesRes.end();it++){      
            delete it.value();
        }
        imagesRes.clear();
    }

    // Traverse path list to add item.
    for(int i = 0;i < filePathList.size();i++){
        if(!imagesRes.keys().contains(filePathList.at(i))){
            QImage *tempImage = new QImage();
            if(tempImage->load(filePathList.at(i))){
                qDebug("add item: %s",qPrintable(filePathList.at(i)));
                imagesRes.insert(filePathList.at(i),tempImage);
                emit m_parent->loadImageComplete();
            }else{
                delete tempImage;
            }
        }
    }
    emit m_parent->loadImageComplete();
}

GalleryWidgets::~GalleryWidgets()
{

}

