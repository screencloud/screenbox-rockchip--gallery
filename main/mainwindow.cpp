#include "mainwindow.h"

#include "MediaNotificationReceiver.h"
MainWindow::MainWindow(QWidget *parent):BaseWindow(parent)
  ,mediaHasUpdate(false)
{
    initData();
    initLayout();
    initConnection();
    slot_updateMedia();
}

void MainWindow::initData()
{
    // Initialize global main class of 'MainWindow' for other widgets invokes.
    mainWindow = this;
    // Start media source update thread.
    // Uevent for usb and inotify for file modify.

    m_receiver = new MediaNotificationReceiver();
    m_receiver->receive();
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
    connect(m_receiver,SIGNAL(mediaNotification(MediaNotification*)),this,SLOT(slot_setUpdateFlag()));
}

void MainWindow::slot_setUpdateFlag()
{
    /*
     * This operation setted because that inotify event send no more one siganl.
     * So set a 500ms duration to ignore theres no-use siganls.
     * Note: it is expected to optimize.
     */
    if(!mediaHasUpdate){
        mediaHasUpdate = true;
        QTimer::singleShot(500,this,SLOT(slot_updateMedia()));
    }
}

void MainWindow::slot_updateMedia()
{
    qDebug("Update image resource.");
    thread = new MediaUpdateThread(this,this);
    thread->start();
    mediaHasUpdate =false;
}

void MainWindow::disableApplication()
{
    qDebug("disable gallery application");
    this->setVisible(false);
}

void MainWindow::enableApplication()
{
    qDebug("enable gallery application");
    this->setVisible(true);
}

void MainWindow::slot_handleSearchResult(QFileInfoList fileInfoList)
{
    m_galleryWid->processFileList(fileInfoList);
}

void MainWindow::onApplicationClose()
{
    if(m_receiver){
        delete m_receiver;
    }
    if(thread){
        delete thread;
    }
    this->close();
}

GalleryWidgets* MainWindow::getGalleryWidget()
{
    return m_galleryWid;
}

void MainWindow::slot_standby()
{
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

MediaUpdateThread::~MediaUpdateThread(){
    if(isRunning()){
        requestInterruption();
        quit();
        wait();
    }
}

void MediaUpdateThread::run()
{
    QFileInfoList fileInfoList = m_mainWindow->getGalleryWidget()->findImgFiles(GALLERY_SEARCH_PATH);
    emit m_mainWindow->searchResultAvailable(fileInfoList);
}

MainWindow::~MainWindow()
{
}
