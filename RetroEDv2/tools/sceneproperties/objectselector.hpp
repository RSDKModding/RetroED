#ifndef OBJECTSELECTOR_V5_H
#define OBJECTSELECTOR_V5_H

#include <QDialog>

namespace Ui
{
class ObjectSelectorv5;
}

class ObjectSelectorv5 : public QDialog
{
    Q_OBJECT

public:
    explicit ObjectSelectorv5(QList<QString> list, QList<GameObjectInfo> objList,
                              QWidget *parent = nullptr);
    ~ObjectSelectorv5();

    QList<int> objIDList;
    QList<bool> objAddList;

private:
    Ui::ObjectSelectorv5 *ui;

    void filterObjectList(QString filter);
};

#endif // OBJECTSELECTOR_H
