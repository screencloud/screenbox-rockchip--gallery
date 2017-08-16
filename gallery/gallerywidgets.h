#ifndef GALLERYWIDGETS_H
#define GALLERYWIDGETS_H

#include "basewidget.h"
#include "top/gallerytopwidgets.h"
#include "middle/gallerymiddlewidgets.h"
#include "global_value.h"

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

private:
    void initConnection();
    void initLayout();

    GalleryTopWidgets *m_topWid;
    GalleryMiddleWidgets *m_middleWid;
private slots:
    void slot_onReturnClicked();
    void slot_onViewerResChanged(QString);
};

#endif // GALLERYWIDGETS_H
