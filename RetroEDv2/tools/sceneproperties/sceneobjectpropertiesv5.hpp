#ifndef SCENEOBJECTPROPERTIES_V5_H
#define SCENEOBJECTPROPERTIES_V5_H

#include <QWidget>

class VariableInfo
{
public:
    QString name = "Variable";
    byte type    = 0;

    VariableInfo() {}
};

struct SceneEntity {
    ushort slotID      = 0;
    byte type          = 0;
    Vector2<float> pos = Vector2<float>(0, 0);
    Rect<int> selBox   = Rect<int>(-0x10, -0x10, 0x10, 0x10);
    QList<RSDKv5::Scene::AttributeValue> variables;

    SceneEntity() {}

    bool operator==(const SceneEntity &other) const
    {
        return slotID == other.slotID && type == other.type && pos.x == other.pos.x
               && pos.y == other.pos.y /*&& variables == other.variables*/;
    }
};

struct SceneObject {
    QString name = "";
    QList<VariableInfo> variables;

    SceneObject() {}

    bool operator==(const SceneObject &other) const
    {
        return name == other.name /*&& variables == other.variables*/;
    }
};

namespace Ui
{
class SceneObjectPropertiesv5;
}

class SceneObjectPropertiesv5 : public QWidget
{
    Q_OBJECT

public:
    explicit SceneObjectPropertiesv5(QWidget *parent = nullptr);
    ~SceneObjectPropertiesv5();

    void setupUI(QList<SceneObject> *objects, SceneEntity *entity);
    void unsetUI();

    void updateUI();

    QComboBox *typeBox          = nullptr;
    PropertyBrowser *properties = nullptr;

private:
    Ui::SceneObjectPropertiesv5 *ui;

    SceneEntity *entityPtr = nullptr;
};

#endif // SCENEOBJECTPROPERTIES_V5_H
