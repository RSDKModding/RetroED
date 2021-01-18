#include "includes.hpp"

QString homeDir     = "";
QString appDir      = "";
AppConfig appConfig = AppConfig();

QIcon icon_up   = QIcon();
QIcon icon_down = QIcon();
QIcon icon_add  = QIcon();
QIcon icon_rm   = QIcon();

QLabel *statusLabel = nullptr;

void setStatus(QString status)
{
    if (!statusLabel)
        return;

    statusLabel->setText(status);
    statusLabel->repaint();
}
