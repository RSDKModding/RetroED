#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QSplashScreen>
#include <QObject>
#include <QWidget>

class SplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    SplashScreen();
};

#endif // SPLASHSCREEN_H
