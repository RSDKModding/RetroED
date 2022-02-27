#include "includes.hpp"

QString homeDir     = "";
QString appDir      = "";
AppConfig appConfig = AppConfig();

QLabel *statusLabel          = nullptr;
QProgressBar *statusProgress = nullptr;

void printLog(QString msg)
{
#ifdef _WIN32
    {
        int colourCode = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
            WORD wColour = (csbi.wAttributes & 0xF0) + colourCode;
            SetConsoleTextAttribute(hStdOut, wColour);
        }
    }
#endif

    printf("%s", QString("RETROED: " + msg + "\n").toStdString().c_str());
    qDebug() << QString("RETROED: " + msg + "\n");

#ifdef _WIN32
    {
        int colourCode = 0;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
            WORD wColour = (csbi.wAttributes & 0xF0) + colourCode;
            SetConsoleTextAttribute(hStdOut, wColour);
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

void setStatus(QString status, bool useStatus)
{
    if (!statusLabel)
        return;

    if (statusProgress && statusProgress->value() != statusProgress->maximum()) {
        addStatusProgress(1);
    }

    printLog(status + (useStatus ? "   0%" : ""));
    statusLabel->setText(status);
    // statusLabel->repaint();

    if (!statusProgress)
        return;

    if (useStatus) {
        statusProgress->show();
        statusProgress->setValue(0);
    }
    // statusProgress->repaint();
}

void addStatusProgress(float percent)
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

    printLog(QString(" ").repeated(statusLabel->text().length()) + QString(" %1").arg(v, 3) + "%");
    // statusProgress->repaint();
}

void setStatusProgress(float percent)
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

    printLog(QString(" ").repeated(statusLabel->text().length()) + QString(" %1").arg((int)percent, 3)
             + "%");
}
