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
    connect(this,SIGNAL(imagesResChanged(QMap<QString,QImage>,bool)),this,SLOT(slot_onImagesResChanged(QMap<QString,QImage>,bool)));

    connect(m_imageControler->m_btnLast,SIGNAL(clicked(bool)),this,SLOT(slot_lastImage()));
    connect(m_imageControler->m_btnNext,SIGNAL(clicked(bool)),this,SLOT(slot_nextImage()));
    connect(m_imageControler->m_btnZoomOut,SIGNAL(clicked(bool)),this,SLOT(slot_imageZoomOut()));
    connect(m_imageControler->m_btnZoomIn,SIGNAL(clicked(bool)),this,SLOT(slot_imageZoomIn()));
    connect(m_imageControler->m_btnRotate,SIGNAL(clicked(bool)),this,SLOT(slot_imageRotate()));
    connect(m_imageControler->m_btnDelete,SIGNAL(clicked(bool)),this,SLOT(slot_deleteImage()));
    connect(m_imageControler->m_btnDetail,SIGNAL(clicked(bool)),this,SLOT(slot_viewDetail()));
}

void ImageViewerWidget::updateRes(QString imagePath, QImage image)
{
    m_imageViewer->setPixmap(QPixmap::fromImage(image));
    m_imagePath = imagePath;
    m_image = image;
    emit m_middleWidgets->viewerResChanged(imagePath);
}

void ImageViewerWidget::slot_onImagesResChanged(QMap<QString,QImage> imagesRes,bool update)
{
    m_imagesRes = imagesRes;
    if(!m_imagesRes.keys().contains(m_imagePath)&&update)
    {
        QMap<QString,QImage>::Iterator  it = m_imagesRes.begin();
        updateRes(it.key(),it.value());
    }
}

void ImageViewerWidget::slot_lastImage()
{
    QMap<QString,QImage>::Iterator  it = m_imagesRes.begin();
    while(it != m_imagesRes.end())
    {
        if(it.key() == m_imagePath)
        {
            if(it != m_imagesRes.begin()){
                --it;
            }else{
                it = m_imagesRes.end()-1;
            }
            updateRes(it.key(),it.value());
            break;
        }
        ++it;
    }
}

void ImageViewerWidget::slot_nextImage()
{
    QMap<QString,QImage>::Iterator  it = m_imagesRes.begin();
    while(it!=m_imagesRes.end())
    {
        if(it.key() == m_imagePath)
        {
            if(it != (m_imagesRes.end()-1)){
                ++it;
            }else{
                it = m_imagesRes.begin();
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
    if(info->exists())
    {
        int result = CMessageBox::showCMessageBox(this,str_question_delete_image,str_button_delete,str_button_cancel);
        if(result == ChosseResult::RESULT_CONFIRM)
        {
            if(QFile::remove(m_imagePath)){
                QString removePath = m_imagePath;
                slot_nextImage();
                m_imagesRes.remove(removePath);
                m_middleWidgets->imagesResChanged(m_imagesRes);
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
