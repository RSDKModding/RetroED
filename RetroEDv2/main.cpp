#include "includes.hpp"
#include "phantomstyle.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setMajorVersion(3);
    format.setMinorVersion(2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    PhantomStyle *style = new PhantomStyle(); // TODO: is this deleted ever???

    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    a.setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication::setStyle(style);

    QPalette pal;
    pal.setColor(QPalette::Window, QColor(0xFF292929));
    pal.setColor(QPalette::Base, QColor(0xFF232323));
    pal.setColor(QPalette::AlternateBase, QColor(0xFF2B2C2D));
    pal.setColor(QPalette::ToolTipBase, QColor(0xFF171819));
    pal.setColor(QPalette::ToolTipText, QColor(0xFFD4D1D0));
    pal.setColor(QPalette::Text, QColor(0xFFD4D1D0));
    pal.setColor(QPalette::WindowText, QColor(0xFFD4D1D0));
    pal.setColor(QPalette::Button, QColor(0xFF333333));
    pal.setColor(QPalette::ButtonText, QColor(0xFFD4D1D0));
    pal.setColor(QPalette::BrightText, QColor(0xFFD4D1D0));

    pal.setColor(QPalette::Highlight, QColor(0xFF545555));
    pal.setColor(QPalette::Light, QColor(0xFF292A2B));
    pal.setColor(QPalette::Midlight, QColor(0xFF333333));
    pal.setColor(QPalette::Mid, QColor(0xFF292929));
    pal.setColor(QPalette::Dark, QColor(0xFF232323));
    pal.setColor(QPalette::HighlightedText, QColor(0xFFD4D1D0));
    pal.setColor(QPalette::Disabled, QPalette::WindowText, QColor(0x60A4A6A8));
    pal.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(0x60A4A6A8));
    pal.setColor(QPalette::Disabled, QPalette::Text, QColor(0x60A4A6A8));
    pal.setColor(QPalette::Shadow, Qt::black);

    QApplication::setPalette(pal);

    appDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/";
    if (!QDir(appDir).exists())
        QDir(appDir).mkpath(appDir);

    if (!QFile(appDir + "appConfig.bin").exists())
        appConfig.write(appDir + "appConfig.bin");
    else
        appConfig.read(appDir + "appConfig.bin");

    homeDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/RetroED/";
    if (!QDir(homeDir).exists())
        QDir(homeDir).mkpath(homeDir);

    if (!QFile(homeDir + "RSDKv4FileList.txt").exists()) {
        Reader reader(":/resources/RSDKv4FileList.txt");
        QByteArray bytes = reader.readByteArray(reader.m_filesize);
        reader.close();

        Writer writer(homeDir + "RSDKv4FileList.txt");
        writer.write(bytes);
        writer.flush();
    }

    if (!QFile(homeDir + "RSDKv5FileList.txt").exists()) {
        Reader reader(":/resources/RSDKv5FileList.txt");
        QByteArray bytes = reader.readByteArray(reader.m_filesize);
        reader.close();

        Writer writer(homeDir + "RSDKv5FileList.txt");
        writer.write(bytes);
        writer.flush();
    }

    MainWindow w;
    w.show();
    return a.exec();
}
