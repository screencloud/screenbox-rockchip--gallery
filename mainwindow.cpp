#include "mainwindow.h"
#include "global_value.h"
#include "ueventthread.h"
#include "inotifythread.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :baseWindow(parent),mediaHasUpdate(false)

{
    mainwid=this;
    initLayout();

    connect(m_wid->m_topWid->m_btnmini,SIGNAL(clicked(bool)),this,SLOT(showMinimized()));
    connect(m_wid->m_topWid->m_btnexit,SIGNAL(clicked(bool)),this,SLOT(slot_appQuit()));
    connect(m_wid->m_topWid->m_btnreturn,SIGNAL(clicked(bool)),this,SLOT(slot_returnanimation()));
    connect(this,SIGNAL(beginUpdateMediaResource()),this,SLOT(slot_updateMedia1()));
    connect(this,SIGNAL(updateUiByRes(QMap<QString,QImage>)),this,SLOT(slot_updateUiByRes(QMap<QString,QImage>)));
    slot_updateMedia2();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initLayout(){
   QVBoxLayout *mainLayout = new QVBoxLayout;
    m_wid = new galleryWidgets(this,this);
    mainLayout->addWidget(m_wid);
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
}
void MainWindow::slot_appQuit()
{
    this->close();
}
void MainWindow::slot_returnanimation()
{
     this->close();
}
void MainWindow::slot_updateMedia1()
{
    if(!mediaHasUpdate)
    {
        mediaHasUpdate = true;
        QTimer::singleShot(2000,this,SLOT(slot_updateMedia2()));
    }
}
void MainWindow::slot_updateMedia2()
{
    qDebug()<<"Gallery: Update media resource.";
    mediaUpdateThread *thread = new mediaUpdateThread(this,this);
    thread->start();
    mediaHasUpdate =false;
}
mediaUpdateThread::mediaUpdateThread(QObject *parent,MainWindow *mainWindow):QThread(parent)
{
    m_mainWindow = mainWindow;
    qRegisterMetaType<QFileInfoList>("QFileInfoList");
    qRegisterMetaType<QMap<QString,QImage>>("QMap<QString,QImage>");
}
void mediaUpdateThread::run()
{
    QMap<QString,QImage> imageRes = m_mainWindow->m_wid->m_middleWid->getImageResFromPath(MUSIC_SEARCH_PATH);
    emit m_mainWindow->updateUiByRes(imageRes);
}

void MainWindow::slot_updateUiByRes(QMap<QString,QImage> imageRes)
{
    m_wid->m_middleWid->updateResUi(imageRes);
}
void MainWindow::slot_standby()
{
    system("echo mem > /sys/power/state");
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<"key value is:"<<event->key();
    switch(event->key())
    {
    // update musicPlayer and videoPlayer's volume by Key
    case Qt::Key_VolumeDown:
        QWidget::keyPressEvent(event);
        break;
    case Qt::Key_VolumeUp:
        QWidget::keyPressEvent(event);
        break;
    case Qt::Key_PowerOff:   // when key_power enter
        QTimer::singleShot(100, this, SLOT(slot_standby()));
        break;
    default:
        break;
    }
}
