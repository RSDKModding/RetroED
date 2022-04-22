#ifndef UTILS_H
#define UTILS_H

namespace Utils
{

void setAttrRecur(QDomElement &&elem, QString strtagname, QString strattr, QString strattrval);
QIcon getColoredIcon(QString filename);

} // namespace Utils

#endif // UTILS_H
