#ifndef THUMBIMAGEWIDGET_H
#define THUMBIMAGEWIDGET_H

#include "basewidget.h"

#include <QObject>
#include <QImage>
#include <QLabel>

#include "gallerymiddlewidgets.h"
#include "thumbimageitem.h"
#include "basepushbutton.h"
#include <base/baselistwidget.h>

class GalleryMiddleWidgets;

/**
 * one of three stacked widgets.
 * it show the all images Thumbnail on special search path.
 */
class ThumbImageWidget:public BaseWidget
{
    Q_OBJECT
public:
    ThumbImageWidget(QWidget *parent);
private:
    void initLayout();
    void initConnection();
    void updateImageCount();
    void updateTipText();
private:
    GalleryMiddleWidgets *m_middleWidgets;
    bool editMode;
    // Control bottom
    QWidget *m_controlBottom;
    QLabel *m_imageCountText;
    QLabel *m_tipText;
    QPushButton *m_btnMode;
    QPushButton *m_btnUpdate;

    BaseListWidget *m_imageListWid;

    QList<ThumbImageItem*> m_selectedItems;
    QMap<QString,QListWidgetItem*> m_thumbs;
signals:
    void imagesResChanged();
private slots:
    void slot_onImagesResChanged();
    void slot_onListItemClick(QListWidgetItem*);
    void slot_changeImageMode();
    void slot_updateImages();
public:
    void onImagesResInsert(QString path,QImage* thumb);
    void onImagesResRemove(QString path);
};


#endif // THUMBIMAGEWIDGET_H
