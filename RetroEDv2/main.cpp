#include "includes.hpp"
#include "phantomstyle.h"

#include <QApplication>
#include "version.hpp"

#include "splashscreen.hpp"

void DebugMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg  = msg.toLocal8Bit();
    const char *file     = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    switch (type) {
        case QtDebugMsg:
            fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line,
                    function);
            break;
        case QtInfoMsg:
            fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line,
                    function);
            break;
        case QtWarningMsg:
            fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line,
                    function);
            break;
        case QtCriticalMsg:
            fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line,
                    function);
            break;
        case QtFatalMsg:
            fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line,
                    function);
            break;
    }
}

void InitConsole()
{
#ifdef _WIN32
    // detach from the current console window
    // if launched from a console window, that will still run waiting for the new console (below) to
    // close it is useful to detach from Qt Creator's <Application output> panel
    FreeConsole();

    // create a separate new console window
    AllocConsole();

    // attach the new console to this application's process
    AttachConsole(GetCurrentProcessId());

    // reopen the std I/O streams to redirect I/O to the new console
    freopen("CON", "w", stdout);
    freopen("CON", "w", stderr);
    freopen("CON", "r", stdin);
#endif
}

class RE2Style : public PhantomStyle
{
public:
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                            QPainter *painter, const QWidget *widget) const override
    {
        if (appConfig.lightMode && control == CC_ToolButton) {
            auto tbopt = qstyleoption_cast<const QStyleOptionToolButton *>(option);
            QStyleOptionToolButton opt_;
            opt_.QStyleOptionToolButton::operator=(*tbopt);

            if (!opt_.icon.isNull()) {
                QPixmap pixmap(opt_.iconSize);
                pixmap.fill(Qt::black);
                QImage image = pixmap.toImage();
                image.setAlphaChannel(
                    opt_.icon.pixmap(opt_.iconSize).toImage().convertToFormat(QImage::Format_Alpha8));
                opt_.icon = QIcon(QPixmap::fromImage(image));
            }

            PhantomStyle::drawComplexControl(control, &opt_, painter, widget);
            return;
        }
        PhantomStyle::drawComplexControl(control, option, painter, widget);
    }
};

QPalette lightPal;
QPalette darkPal;

int main(int argc, char *argv[])
{
    InitConsole();
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication a(argc, argv);

    SplashScreen splash;
    splash.showMessage("Setting up display format...");
    splash.show();

    splash.showMessage("Setting up appConfig...");

    appDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/";
    if (!QDir(appDir).exists())
        QDir(appDir).mkpath(appDir);

    if (!QFile(appDir + "appConfig.bin").exists())
        appConfig.write(appDir + "appConfig.bin");
    else
        appConfig.read(appDir + "appConfig.bin");

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setMajorVersion(3);
    format.setMinorVersion(2);
    format.setSwapInterval(appConfig.vSync);
    format.setProfile(QSurfaceFormat::CoreProfile);

    QSurfaceFormat::setDefaultFormat(format);

    QCoreApplication::processEvents();
    splash.showMessage("Configuring style...");

    qInstallMessageHandler(DebugMessageHandler);
    RE2Style *style = new RE2Style();

    QApplication::setStyle(style);

    QCoreApplication::processEvents();

    lightPal = QApplication::palette();

    darkPal.setColor(QPalette::Window, QColor(0xFF292929));
    darkPal.setColor(QPalette::Base, QColor(0xFF232323));
    darkPal.setColor(QPalette::AlternateBase, QColor(0xFF2B2C2D));
    darkPal.setColor(QPalette::ToolTipBase, QColor(0xFF171819));
    darkPal.setColor(QPalette::ToolTipText, QColor(0xFFD4D1D0));
    darkPal.setColor(QPalette::Text, QColor(0xFFD4D1D0));
    darkPal.setColor(QPalette::WindowText, QColor(0xFFD4D1D0));
    darkPal.setColor(QPalette::Button, QColor(0xFF333333));
    darkPal.setColor(QPalette::ButtonText, QColor(0xFFD4D1D0));
    darkPal.setColor(QPalette::BrightText, QColor(0xFFD4D1D0));
    darkPal.setColor(QPalette::Highlight, QColor(0xFF545555));
    darkPal.setColor(QPalette::Light, QColor(0xFF292A2B));
    darkPal.setColor(QPalette::Midlight, QColor(0xFF333333));
    darkPal.setColor(QPalette::Mid, QColor(0xFF292929));
    darkPal.setColor(QPalette::Dark, QColor(0xFF232323));
    darkPal.setColor(QPalette::HighlightedText, QColor(0xFFD4D1D0));
    darkPal.setColor(QPalette::Disabled, QPalette::WindowText, QColor(0x60A4A6A8));
    darkPal.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(0x60A4A6A8));
    darkPal.setColor(QPalette::Disabled, QPalette::Text, QColor(0x60A4A6A8));
    darkPal.setColor(QPalette::Shadow, Qt::black);

    if (!appConfig.lightMode)
        QApplication::setPalette(darkPal);

    homeDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/RetroED/";
    if (!QDir(homeDir).exists())
        QDir(homeDir).mkpath(homeDir);

    splash.showMessage("Setting up file lists...");

    // copy RSDKv4 file list if it doesn't exist
    if (!QFile(homeDir + "RSDKv4FileList.txt").exists()) {
        Reader reader(":/resources/RSDKv4FileList.txt");
        QByteArray bytes = reader.readByteArray(reader.filesize);
        reader.close();

        Writer writer(homeDir + "RSDKv4FileList.txt");
        writer.write(bytes);
        writer.flush();
    }

    // copy RSDKv5 file list if it doesn't exist
    if (!QFile(homeDir + "RSDKv5FileList.txt").exists()) {
        Reader reader(":/resources/RSDKv5FileList.txt");
        QByteArray bytes = reader.readByteArray(reader.filesize);
        reader.close();

        Writer writer(homeDir + "RSDKv5FileList.txt");
        writer.write(bytes);
        writer.flush();
    }

    // copy RSDKv5U file list if it doesn't exist
    if (!QFile(homeDir + "RSDKv5UFileList.txt").exists()) {
        Reader reader(":/resources/RSDKv5UFileList.txt");
        QByteArray bytes = reader.readByteArray(reader.filesize);
        reader.close();

        Writer writer(homeDir + "RSDKv5UFileList.txt");
        writer.write(bytes);
        writer.flush();
    }

    PrintLog("====================================================");
    PrintLog("New RetroED instance: " + QDateTime::currentDateTime().toString());
    PrintLog(QString("Version: ") + RE_VERSION);
    PrintLog("====================================================");

    splash.showMessage("Setting up GameLink...");

    GameLink::Setup();

    MainWindow w;
    splash.finish(&w);
    w.show();
    return a.exec();
}
