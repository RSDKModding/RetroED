#include "includes.hpp"
#include <QSvgRenderer>

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
    // qDebug() << QString("RETROED: " + msg + "\n");

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

void Utils::setAttrRecur(QDomElement &&elem, QString strtagname, QString strattr, QString strattrval)
{
    // if it has the tagname then overwritte desired attribute
    if (elem.tagName().compare(strtagname)) {
        elem.setAttribute(strattr, strattrval);
    }
    // loop all children
    for (int i = 0; i < elem.childNodes().count(); ++i) {
        if (!elem.childNodes().at(i).isElement()) {
            continue;
        }
        setAttrRecur(elem.childNodes().at(i).toElement(), strtagname, strattr, strattrval);
    }
}

QIcon Utils::getColoredIcon(QString filename)
{
    if (!QFile::exists(filename))
        filename = ":/icons/ic_help_48px.svg";
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QByteArray baData = file.readAll();
    QDomDocument doc;
    doc.setContent(baData);
    setAttrRecur(doc.documentElement(), "path", "fill", "white");
    QSvgRenderer svgRenderer(doc.toByteArray());
    QPixmap pix(svgRenderer.defaultSize());
    pix.fill(Qt::transparent);
    QPainter pixPainter(&pix);
    svgRenderer.render(&pixPainter);
    return QIcon(pix);
}
