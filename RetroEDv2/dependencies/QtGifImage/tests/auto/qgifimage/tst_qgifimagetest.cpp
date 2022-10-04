#include "qgifimage.h"
#include <QPainter>
#include <QtTest>

class QGifimageTest : public QObject
{
    Q_OBJECT

public:
    QGifimageTest();

private Q_SLOTS:
    void testGifFileLoad();

private:
    QImage rgbImage;
    QImage indexed8Image;
    QGifImage gifImage;
};

QGifimageTest::QGifimageTest()
{
    QImage image(100, 100, QImage::Format_RGB32);
    image.fill(QColor(Qt::red));
    QPainter p(&image);
    p.setPen(Qt::blue);
    p.drawRect(20, 20, 60, 60);

    rgbImage = image;
    indexed8Image = image.convertToFormat(QImage::Format_Indexed8);

    gifImage.load(SRCDIR"test.gif");
}

void QGifimageTest::testGifFileLoad()
{
    QVERIFY2(true, "Failure");
}

QTEST_MAIN(QGifimageTest)

#include "tst_qgifimagetest.moc"
