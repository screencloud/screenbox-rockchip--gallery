#include "gallerytopwidgets.h"
#include "global_value.h"

#include <QHBoxLayout>

GalleryTopWidgets::GalleryTopWidgets(QWidget *parent):BaseWidget(parent)
{
    // Set background color.
    setObjectName("GalleryTopWidgets");
    setStyleSheet("#GalleryTopWidgets{background:rgb(56,58,66)}");

    initLayout();
}

void GalleryTopWidgets::initLayout()
{
    QHBoxLayout *hmainyout=new QHBoxLayout;

    m_btnReturn=new FourStateButton(return_resource_str,this);
    m_btnReturn->setFixedSize(return_icon_width,return_icon_height);

    m_btnIcon=new FlatButton(this);
    m_btnIcon->setFixedSize(top_icon_size,top_icon_size);
    m_btnIcon->setStyleSheet("QPushButton{background:transparent;border-image:url(:/image/gallery/gallery_top_icon.png)}");

    m_titleLabel = new QLabel(str_top_title,this);
    QFont font = m_titleLabel->font();
    font.setPixelSize(font_size_big);
    m_titleLabel->setFont(font);
    m_titleLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *lyout1 = new QHBoxLayout;
    lyout1->addWidget(m_btnReturn);
    lyout1->addSpacing(30);
    lyout1->addWidget(m_btnIcon);
    lyout1->addStretch(0);


    hmainyout->addLayout(lyout1,1);
    hmainyout->addWidget(m_titleLabel,1);
    hmainyout->addStretch(1);
    hmainyout->setContentsMargins(0,0,0,0);
    setLayout(hmainyout);
}

void GalleryTopWidgets::updateTopTitle(QString title)
{
    m_titleLabel->setText(title);
}
