#ifndef GALLERYTOPWIDGETS_H
#define GALLERYTOPWIDGETS_H

#include "basewidget.h"
#include "basepushbutton.h"

#include <QLabel>

class GalleryTopWidgets:public BaseWidget
{
public:
    GalleryTopWidgets(QWidget *parent);
    ~GalleryTopWidgets(){}

    void updateTopTitle(QString title);
    FourStateButton* getReturenButton(){return m_btnReturn;}
private:
    FourStateButton *m_btnReturn;
    FlatButton *m_btnIcon;
    QLabel *m_titleLabel;

    void initLayout();
};
#endif // GALLERYTOPWIDGETS_H
