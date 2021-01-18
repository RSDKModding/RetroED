#include "includes.hpp"
#include <QSvgRenderer>

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

QIcon Utils::getColouredIcon(QString filename)
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
