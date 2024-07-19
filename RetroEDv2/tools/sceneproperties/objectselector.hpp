#pragma once

#include <QDialog>

namespace Ui
{
class ObjectSelectorv5;
}

class ObjectSelectorv5 : public QDialog
{
    Q_OBJECT

public:
    explicit ObjectSelectorv5(QList<QString> list, QList<GameObjectInfo> objList, bool replace = false,
                              QWidget *parent = nullptr);
    ~ObjectSelectorv5();

    QList<int> objIDList;
    QList<bool> objAddList;
    QString replacedObj = "";

private:
    Ui::ObjectSelectorv5 *ui;

    void filterObjectList(QString filter);
};


