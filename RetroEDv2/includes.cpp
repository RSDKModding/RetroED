#include "includes.hpp"

QString homeDir     = "";
QString appDir      = "";
AppConfig appConfig = AppConfig();

QLabel *statusLabel          = nullptr;
QProgressBar *statusProgress = nullptr;

QString argInitStage  = "";
QString argInitScene  = "";
QString argInitFilter = "";

void SetStatus(QString status, bool useStatus)
{
    if (!statusLabel)
        return;

    PrintLog(status + (useStatus ? "   0%" : ""));
    statusLabel->setText(status);

    if (!statusProgress)
        return;

    if (useStatus) {
        statusProgress->setValue(0);
        statusProgress->show();
    } else
        statusProgress->hide();
}

void AddStatusProgress(float percent)
{
    if (!statusProgress)
        return;
    percent *= statusProgress->maximum();
    int v = statusProgress->value() + percent;
    if (v > statusProgress->maximum())
        v = statusProgress->maximum();
    statusProgress->setValue(v);

    if (statusProgress->value() == statusProgress->maximum())
        statusProgress->hide();

    if (!statusLabel)
        return;

    PrintLog(QString(" ").repeated(statusLabel->text().length()) + QString(" %1").arg(v, 3) + "%");
    // statusProgress->repaint();
}

void SetStatusProgress(float percent)
{
    if (!statusProgress)
        return;
    percent *= statusProgress->maximum();
    if (percent > statusProgress->maximum())
        percent = statusProgress->maximum();
    if ((int)percent == statusProgress->value())
        return;
    statusProgress->setValue(percent);

    if (statusProgress->value() == statusProgress->maximum())
        statusProgress->hide();

    if (!statusLabel)
        return;

    PrintLog(QString(" ").repeated(statusLabel->text().length()) + QString(" %1").arg((int)percent, 3)
             + "%");
}

bool CheckOverwrite(QString &filename, QString format, QWidget *parent){
    if (!filename.endsWith(format)){
        filename += format;
        if (QFile::exists(filename)){
            QApplication::beep();
            QMessageBox::StandardButton overwrite = QMessageBox::question(parent, "Overwrite", "This file already exists, overwrite?", QMessageBox::Yes | QMessageBox::No);
            if (overwrite == QMessageBox::No)
                return false;
        }
    }
    return true;
}
