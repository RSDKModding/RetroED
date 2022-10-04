#include "qgifimage.h"
#include <QPainter>
#include <QImage>
#include <QGuiApplication>
#include <QDebug>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    //![0]
    QGifImage gif(QSize(300, 300));
    //![0]
    //![1]
    QVector<QRgb> ctable;
    ctable << qRgb(255, 255, 255)
           << qRgb(0, 0, 0)
           << qRgb(255, 0, 0)
           << qRgb(0, 255, 0)
           << qRgb(0, 0, 255)
           << qRgb(255, 255, 0)
           << qRgb(0, 255, 255)
           << qRgb(255, 0, 255);

    gif.setGlobalColorTable(ctable, Qt::black);
    gif.setDefaultTransparentColor(Qt::black);
    gif.setDefaultDelay(100);
    //![1]
    //![2]
    QImage image(100, 100, QImage::Format_RGB32);
    image.fill(QColor(Qt::black));
    QPainter p(&image);
    p.setPen(Qt::red);
    p.drawText(15, 15, "Qt");
    p.drawRect(20, 20, 60, 60);

    for (int i=0; i<10; ++i) {
        gif.addFrame(image, QPoint(i*20, i*20));
    }
    //![2]
    //![3]
    gif.save(SRCDIR"demo1.gif");
    //![3]

    return 0;
}
