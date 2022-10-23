#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QSplashScreen>
#include <QObject>
#include <QWidget>
#include <QPixmap>

class SplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    SplashScreen();
    void finish(QWidget* main);

protected:
    void drawContents(QPainter* painter) override;

private:
    QPixmap svg;
};

#endif // SPLASHSCREEN_H
