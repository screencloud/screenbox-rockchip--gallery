#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QThread>

#include "gallerywidgets.h"
#include "base/basewindow.h"
#include "global_value.h"

#include "MediaNotificationReceiver.h"

class GalleryWidgets;
class MediaUpdateThread;

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
    void onApplicationClose();
protected:
    void keyPressEvent(QKeyEvent *event);
    // Used for disable or enable application when car-reverse event comes.
    void disableApplication();
    void enableApplication();
private:
    bool mediaHasUpdate;
    GalleryWidgets *m_galleryWid;
    // Thread for media resource update.
    MediaUpdateThread *thread;
    MediaNotificationReceiver * m_receiver;
    void initData();
    void initLayout();
    void initConnection();
public slots:
    // Slot in order to update media resource.
    void slot_setUpdateFlag();
    void slot_updateMedia();

    void slot_handleSearchResult(QFileInfoList fileInfoList);
    void slot_standby();
signals:
    void beginUpdateMediaResource();
    void searchResultAvailable(QFileInfoList fileInfoList);
};

class MediaUpdateThread:public QThread
{
public:
    MediaUpdateThread(QObject *parent ,MainWindow *mainWindow);
    ~MediaUpdateThread();
private:
    MainWindow *m_mainWindow;
protected:
    void run();
};
#endif // MAINWINDOW_H
