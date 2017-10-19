#ifndef EMPTYIMAGESWIDGET_H
#define EMPTYIMAGESWIDGET_H

#include "basewidget.h"
#include <QLabel>
class EmptyImagesWidget:public BaseWidget
{
public:
    EmptyImagesWidget(QWidget *parent);
    QLabel *tip1;
    QLabel *tip2 ;
private:
    void initLayout();
};

#endif // EMPTYIMAGESWIDGET_H
