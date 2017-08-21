#include "imageviewerwidget.h"

#include <QVBoxLayout>
#include <cmessagebox.h>
#include "global_value.h"


#ifdef DEVICE_EVB
int button_last_image_width = 100;
int layout_spacing = 50;

#else
int button_last_image_width = 40;
int layout_spacing = 30;

#endif

ImageViewerWidget::ImageViewerWidget(QWidget *parent):BaseWidget(parent)
{
    m_middleWidgets = (GalleryMiddleWidgets*)parent;

    // Set background color.
    setObjectName("ImageViewerWidget");
    setStyleSheet("#ImageViewerWidget{background:rgb(0,0,0)}");

    initLayout();
    initConnection();
}

void ImageViewerWidget::initLayout()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    m_imageViewer = new ImageViewer(this);
    m_imageControler = new ImageControler(this);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_imageViewer);
    mainLayout->addWidget(m_imageControler);
    mainLayout->addSpacing(30);
    mainLayout->setMargin(0);

    setLayout(mainLayout);
}

void ImageViewerWidget::initConnection()
{
    connect(this,SIGNAL(imagesResChanged(bool)),this,SLOT(slot_onImagesResChanged(bool)));

    connect(m_imageControler->m_btnLast,SIGNAL(clicked(bool)),this,SLOT(slot_lastImage()));
    connect(m_imageControler->m_btnNext,SIGNAL(clicked(bool)),this,SLOT(slot_nextImage()));
    connect(m_imageControler->m_btnZoomOut,SIGNAL(clicked(bool)),this,SLOT(slot_imageZoomOut()));
    connect(m_imageControler->m_btnZoomIn,SIGNAL(clicked(bool)),this,SLOT(slot_imageZoomIn()));
    connect(m_imageControler->m_btnRotate,SIGNAL(clicked(bool)),this,SLOT(slot_imageRotate()));
    connect(m_imageControler->m_btnDelete,SIGNAL(clicked(bool)),this,SLOT(slot_deleteImage()));
    connect(m_imageControler->m_btnDetail,SIGNAL(clicked(bool)),this,SLOT(slot_viewDetail()));
}

void ImageViewerWidget::updateRes(QString imagePath, QImage *image)
{
    if(imagePath.endsWith(QString("gif"),Qt::CaseInsensitive)){
        m_imageViewer->setMoviePath(imagePath);
    }else{
        m_imageViewer->setPixmap(QPixmap::fromImage(*image));
    }
    m_imagePath = imagePath;
    emit m_middleWidgets->viewerResChanged(imagePath);
}

void ImageViewerWidget::slot_onImagesResChanged(bool update)
{
    QMap<QString,QImage*> &imagesRes = mainWindow->getGalleryWidget()->getImagesRes();
    if(!imagesRes.keys().contains(m_imagePath)&&update)
    {
        QMap<QString,QImage*>::Iterator it = imagesRes.begin();
        updateRes(it.key(),it.value());
    }
}

void ImageViewerWidget::slot_lastImage(){
    QMap<QString,QImage*> &imagesRes = mainWindow->getGalleryWidget()->getImagesRes();
    QMap<QString,QImage*>::Iterator it = imagesRes.begin();
    while(it != imagesRes.end())
    {
        if(it.key() == m_imagePath)
        {
            if(it != imagesRes.begin()){
                --it;
            }else{
                it = imagesRes.end()-1;
            }
            updateRes(it.key(),it.value());
            break;
        }
        ++it;
    }
}

void ImageViewerWidget::slot_nextImage()
{
    QMap<QString,QImage*> &imagesRes = mainWindow->getGalleryWidget()->getImagesRes();
    QMap<QString,QImage*>::Iterator  it = imagesRes.begin();
    while(it!=imagesRes.end())
    {
        if(it.key() == m_imagePath)
        {
            if(it != (imagesRes.end()-1)){
                ++it;
            }else{
                it = imagesRes.begin();
            }
            updateRes(it.key(),it.value());
            break;
        }
        ++it;
    }
}

void ImageViewerWidget::slot_viewDetail()
{
    QFileInfo *info = new QFileInfo(m_imagePath);
    if(info->exists())
    {
        ImageDetailWidget::showImageDetail(this,m_imagePath);
    }
}

void ImageViewerWidget::slot_deleteImage()
{
    QFileInfo *info = new QFileInfo(m_imagePath);
    if(info->exists()){
        int result = CMessageBox::showCMessageBox(this,str_question_delete_image,str_button_delete,str_button_cancel);
        if(result == ChosseResult::RESULT_CONFIRM){
            if(QFile::remove(m_imagePath)){
                QString removePath = m_imagePath;
                slot_nextImage();
                mainWindow->getGalleryWidget()->removeImage(removePath);
                m_middleWidgets->imagesResChanged();
            }
        }
    }
}

void ImageViewerWidget::slot_imageZoomOut()
{
    m_imageViewer->zoomOut();
}

void ImageViewerWidget::slot_imageZoomIn()
{
    m_imageViewer->zoomIn();
}

void ImageViewerWidget::slot_imageRotate()
{
    m_imageViewer->clockwise90();
}
