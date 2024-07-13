#pragma once

void PrintLog(QString msg);

namespace Utils
{

void setAttrRecur(QDomElement &&elem, QString strtagname, QString strattr, QString strattrval);
QIcon getColoredIcon(QString filename);

} // namespace Utils


