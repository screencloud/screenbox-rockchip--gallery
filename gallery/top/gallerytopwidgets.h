#ifndef GALLERYTOPWIDGETS_H
#define GALLERYTOPWIDGETS_H

#include "basewidget.h"
#include "basepushbutton.h"

#include <QLabel>

class GalleryTopWidgets:public BaseWidget
{
public:
    GalleryTopWidgets(QWidget *parent);
    ~GalleryTopWidgets();

    void updateTopTitle(QString title);
    FourStateButton* getReturenButton(){return m_btnreturn;}
private:
    FourStateButton *m_btnreturn;
    FlatButton *m_btnicon;
    FlatButton *m_btnmobile;
    FlatButton *m_btnsetting;
    FlatButton *m_btnmini;
    FlatButton *m_btnexit;

    QLabel *m_titleLabel;
private:
    void initLayout();
};
#endif // GALLERYTOPWIDGETS_H
