#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QThread>

#include "gallerywidgets.h"
#include "base/basewindow.h"
#include "global_value.h"

class GalleryWidgets;

/**
 * The main window of application.
 *
 * Used for global control.such as keypress response、media update、
 * initial interface etc.
 */
class MainWindow : public BaseWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    GalleryWidgets* getGalleryWidget();
protected:
    void keyPressEvent(QKeyEvent *event);
private:
    bool mediaHasUpdate;
    GalleryWidgets *m_galleryWid;

    void initLayout();
    void initConnection();
public slots:
    // Slot in order to update media resource.
    void slot_setUpdateFlag();
    void slot_updateMedia();

    void slot_updateUiByRes(QMap<QString,QImage> imageRes);
    void slot_standby();
signals:
    void beginUpdateMediaResource();
    void updateUiByRes(QMap<QString,QImage> imageRes);
};

class MediaUpdateThread:public QThread
{
public:
    MediaUpdateThread(QObject *parent ,MainWindow *mainWindow);
private:
    MainWindow *m_mainWindow;
protected:
    void run();
};
#endif // MAINWINDOW_H
