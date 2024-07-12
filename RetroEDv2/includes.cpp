#include "includes.hpp"

QString homeDir     = "";
QString appDir      = "";
AppConfig appConfig = AppConfig();

QLabel *statusLabel          = nullptr;
QProgressBar *statusProgress = nullptr;

QString argInitStage  = "";
QString argInitScene  = "";
QString argInitFilter = "";

void PrintLog(QString msg)
{
#ifdef _WIN32
    {
        int colorCode = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
            WORD wColor = (csbi.wAttributes & 0xF0) + colorCode;
            SetConsoleTextAttribute(hStdOut, wColor);
        }
    }
#endif

    printf("%s", QString("RETROED: " + msg + "\n").toStdString().c_str());
    qDebug() << QString("RETROED: " + msg + "\n");

#ifdef _WIN32
    {
        int colorCode = 0;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
            WORD wColor = (csbi.wAttributes & 0xF0) + colorCode;
            SetConsoleTextAttribute(hStdOut, wColor);
        }
    }
#endif

    if (!QFile(homeDir + "RetroED.log").exists()) {
        Writer writer(homeDir + "RetroED.log");
        writer.flush();
    }

    QFile f(homeDir + "RetroED.log");
    if (f.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QString fMsg = msg + "\n";
        f.write(fMsg.toStdString().c_str(), fMsg.length());

        f.close();
    }
}

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
