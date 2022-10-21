#include "splashscreen.hpp"
#include "version.hpp"

SplashScreen::SplashScreen() : QSplashScreen(QPixmap(":/resources/splash.png"), Qt::WindowStaysOnTopHint)
{
    
}
