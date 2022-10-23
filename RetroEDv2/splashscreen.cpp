#include "splashscreen.hpp"
#include "version.hpp"

#include <QPainter>
#include <QApplication>

SplashScreen::SplashScreen()
    : QSplashScreen(QPixmap(":/resources/splash.png"), Qt::WindowStaysOnTopHint), svg(QPixmap(":/resources/splash.svg"))
{
}

void SplashScreen::finish(QWidget* main) {
    QSplashScreen::finish(main);
    main->raise();
}

void SplashScreen::drawContents(QPainter *painter) {
    painter->drawPixmap(0, 0, width(), height(), svg);
    
    QFont font = QApplication::font();
    font.setPointSize(11);
    painter->setFont(font);
    painter->setPen(Qt::white);
    painter->drawText(5, 5, width() - 10, height() - 10, Qt::AlignBottom | Qt::AlignRight, RE_VERSION);
    
    font.setPointSize(10);
    font.setItalic(true);
    painter->setFont(font);
    painter->drawText(5, 5, width() - 10, height() - 10, Qt::AlignBottom | Qt::AlignLeft, message());
    //painter->drawText(506 - 10, 300 - 10, 506 - 10, 300 - 10, Qt::AlignRight | Qt::AlignBottom, RE_VERSION);
}
