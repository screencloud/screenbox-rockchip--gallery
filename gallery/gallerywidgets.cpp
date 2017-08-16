#include "gallerywidgets.h"

GalleryWidgets::GalleryWidgets(QWidget *parent):BaseWidget(parent)
{
    setStyleSheet("QLabel{color:white;}");

    initLayout();
    initConnection();
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
}

void GalleryWidgets::slot_onReturnClicked()
{
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

GalleryWidgets::~GalleryWidgets()
{
}

