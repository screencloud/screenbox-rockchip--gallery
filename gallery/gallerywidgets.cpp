#include "gallerywidgets.h"

#include <QDirIterator>

#include <QDebug>
#include <QCryptographicHash>
#include <QThread>

GalleryWidgets::GalleryWidgets(QWidget *parent):BaseWidget(parent)
{
    setStyleSheet("QLabel{color:white;}");

    initLayout();
    initConnection();
    initData();
}

void GalleryWidgets::initData(){
    m_loadImageThread = new LoadImageThread(this,this,m_middleWid);

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
    while (!QThread::currentThread()->isInterruptionRequested()&& it.hasNext()){
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
    m_loadImageThread->setFileInfoList(fileInfoList);
    m_loadImageThread->start();
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
    // Destory load-image thread while exit application.
    if(m_middleWid->isViewerMode()){
        m_middleWid->leaveViewerMode();
        m_topWid->updateTopTitle(str_top_title);
    }else{
        if(m_loadImageThread && m_loadImageThread->isRunning()){
            m_loadImageThread->stopThread();
        }
        mainWindow->onApplicationClose();
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

LoadImageThread::LoadImageThread(QObject *parent,GalleryWidgets *parentWidget,GalleryMiddleWidgets *middleWid):QThread(parent)
{
    m_parent = parentWidget;
    m_middleWid=middleWid;
    qRegisterMetaType<QFileInfoList>("QFileInfoList");
    qRegisterMetaType<QMap<QString,QImage>>("QMap<QString,QImage*>");
}

LoadImageThread::~LoadImageThread()
{
}

void LoadImageThread::setFileInfoList(QFileInfoList infoList)
{
    m_infoList = infoList;
}

void LoadImageThread::stopThread(){
    requestInterruption();
    quit();
    wait();
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
            emit m_middleWid->sig_imagesResRemove(it.key(),NULL);
            delete it.value();
        }
        imagesRes.clear();
    }

    QDir  thumbDir("/tmp/thumb/");
    if(!thumbDir.exists()){
        thumbDir.mkdir(thumbDir.absolutePath());
    }
    QImage *tempImage;

    // Traverse path list to add item.
    for(int i = 0;i < filePathList.size() && !isInterruptionRequested();i++){
        if(!imagesRes.keys().contains(filePathList.at(i))){
            tempImage = new QImage();
            QString src_path=filePathList.at(i);

            QFileInfo thumb(thumbDir.absolutePath()+"/"+fileMD5(src_path)+".thumb");

            if(!thumb.exists()){
                compressImg(src_path,thumb.absoluteFilePath());
            }
            if(tempImage->load(thumb.absoluteFilePath())){
                imagesRes.insert(filePathList.at(i),tempImage);
                emit m_middleWid->sig_imagesResInsert(filePathList.at(i),tempImage);
            }else{
                delete tempImage;
            }
        }
    }
    emit m_parent->loadImageComplete();
}
QString LoadImageThread::fileMD5(QString path){
    QString md5;
    QByteArray bb;
    bb = QCryptographicHash::hash ( path.toLatin1(), QCryptographicHash::Md5 );
    md5.append(bb.toHex());
    return md5;
}

GalleryWidgets::~GalleryWidgets()
{
}

void LoadImageThread::compressImg(QString src,QString out)
{
    QImage img;
    img.load(src);
    QImage result = img.scaled(800, 600, Qt::KeepAspectRatio, Qt::FastTransformation).scaled(260, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    bool isSuccess = result.save(out, "JPEG", 100);
    if (!isSuccess)
    {
        qDebug() << "save image fail!";
    }
    QFile file(out);
    qint64 fsz = file.size();

    int quality = 100;
    while (fsz > 2048)
    {
        quality = quality - 5;
        isSuccess = result.save(out, "JPEG", quality);
        if (!isSuccess)
        {
            qDebug() << "save image fail! quality=" << quality;
        }
        fsz = file.size();
        if (quality <= 0)
            break;
    }
}

