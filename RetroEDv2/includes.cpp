#include "includes.hpp"

QString homeDir     = "";
QString appDir      = "";
AppConfig appConfig = AppConfig();

QLabel *statusLabel          = nullptr;
QProgressBar *statusProgress = nullptr;

QVector3D *vertsPtr  = nullptr;
QVector2D *tVertsPtr = nullptr;

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
}

void setStatus(QString status, bool useStatus)
{
    if (!statusLabel)
        return;

    printLog("Set Status to: " + status);
    statusLabel->setText(status);
    // statusLabel->repaint();

    if (!statusProgress)
        return;

    statusProgress->setValue(useStatus ? 0 : 100);
    // statusProgress->repaint();
}

void addStatusProgress(float percent)
{
    if (!statusProgress)
        return;
    percent *= statusProgress->maximum();

    statusProgress->setValue(statusProgress->value() + percent);
    // statusProgress->repaint();
}

void refreshScnEditorVerts(int w, int h)
{
    if (vertsPtr)
        delete[] vertsPtr;
    if (tVertsPtr)
        delete[] tVertsPtr;

    vertsPtr  = new QVector3D[(int)((h * w * 6) * 1.1)];
    tVertsPtr = new QVector2D[(int)((h * w * 6) * 1.1)];
}
