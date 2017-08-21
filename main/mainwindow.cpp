#include "mainwindow.h"
#include "inotifythread.h"

MainWindow::MainWindow(QWidget *parent) :BaseWindow(parent)
  ,mediaHasUpdate(false)
{
    // Initialize global main class of 'MainWindow' for other widgets invokes.
    mainWindow = this;

    initLayout();
    initConnection();
    slot_updateMedia();
}

void MainWindow::initLayout(){
    QVBoxLayout *mainLayout = new QVBoxLayout;

    m_galleryWid = new GalleryWidgets(this);
    mainLayout->addWidget(m_galleryWid);

    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    setLayout(mainLayout);
}

void MainWindow::initConnection()
{
    connect(this,SIGNAL(beginUpdateMediaResource()),this,SLOT(slot_setUpdateFlag()));
    connect(this,SIGNAL(searchResultAvailable(QFileInfoList)),this,SLOT(slot_handleSearchResult(QFileInfoList)));
}

void MainWindow::slot_setUpdateFlag()
{
    /*
     * This operation setted because that inotify event send no more one siganl.
     * So set a 2 seconds duration to ignore theres no-use siganls.
     * Note: it is expected to optimize.
     */
    if(!mediaHasUpdate)
    {
        mediaHasUpdate = true;
        QTimer::singleShot(2000,this,SLOT(slot_updateMedia()));
    }
}

void MainWindow::slot_updateMedia()
{
    qDebug("Update image resource.");
    MediaUpdateThread *thread = new MediaUpdateThread(this,this);
    thread->start();
    mediaHasUpdate =false;
}

void MainWindow::slot_handleSearchResult(QFileInfoList fileInfoList)
{
    m_galleryWid->processFileList(fileInfoList);
}

GalleryWidgets* MainWindow::getGalleryWidget()
{
    return m_galleryWid;
}

void MainWindow::slot_standby()
{
    system("echo mem > /sys/power/state");
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_PowerOff:
        // When key_power enter
        QTimer::singleShot(100, this, SLOT(slot_standby()));
        break;
    default:
        break;
    }
}

MediaUpdateThread::MediaUpdateThread(QObject *parent,MainWindow *mainWindow):QThread(parent)
{
    m_mainWindow = mainWindow;
    qRegisterMetaType<QFileInfoList>("QFileInfoList");
}

void MediaUpdateThread::run()
{
    QFileInfoList fileInfoList = m_mainWindow->getGalleryWidget()->findImgFiles(GALLERY_SEARCH_PATH);
    emit m_mainWindow->searchResultAvailable(fileInfoList);
}

MainWindow::~MainWindow()
{
}
