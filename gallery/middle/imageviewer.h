#ifndef IMAGEBROWSER_IMAGEHANDLER_H
#define IMAGEBROWSER_IMAGEHANDLER_H

#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QWheelEvent>
#include <QResizeEvent>
#include <QGestureEvent>
#include <QPinchGesture>
#include <QMovie>

class ImageViewer : public QLabel
{
    Q_OBJECT
public:
    explicit ImageViewer(QWidget *parent = 0);
    ~ImageViewer();

    void setPixmap(const QPixmap &pixmap);
    void setMoviePath(QString filePath);

    void showOriginalSize();
    void showSuitableSize();
    void zoomIn();
    void zoomOut();
    void clockwise90();
    void anticlockwise90();

private:
    void ariseScale(int rate);
signals:
    void percentageChanged(double percentage);
    void rightButtonClicked();

protected:
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool event(QEvent *event) Q_DECL_OVERRIDE;

signals:

public slots:

private:
    bool gifShow;
    QPixmap m_pixmap;
    QMovie *m_movie;

    double m_scale;
    double m_percentage;

    QPoint m_pressPoint;
    double m_originX;
    double m_originY;
    double m_basicX;
    double m_basicY;

    int m_suitableWidth;
    int m_suitableHeight;

    qreal currentStepScaleFactor;
};
#endif // IMAGEBROWSER_IMAGEHANDLER_H
