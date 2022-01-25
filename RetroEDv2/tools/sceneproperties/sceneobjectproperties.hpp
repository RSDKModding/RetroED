#ifndef SCENEOBJECTPROPERTIES_H
#define SCENEOBJECTPROPERTIES_H

#include <QWidget>

namespace Ui
{
class SceneObjectProperties;
}

class SceneObjectProperties : public QWidget
{
    Q_OBJECT

public:
    explicit SceneObjectProperties(QWidget *parent = nullptr);
    ~SceneObjectProperties();

    void setupUI(SceneViewer::EntityInfo *entity, int entityID, Compilerv2::Entity *entityv2,
                 Compilerv3::Entity *entityv3, Compilerv4::Entity *entityv4, byte ver);
    void unsetUI();

    void updateUI();

    int callRSDKEdit(SceneViewer *viewer, bool shouldReturnVal, int entityID, int variableID,
                     int variableValue, bool *called = nullptr);

    PropertyBrowser *properties = nullptr;

private:
    Ui::SceneObjectProperties *ui;

    SceneViewer::EntityInfo *entityPtr = nullptr;

    QMessageBox *msgBox = nullptr;
};

#endif // SCENEOBJECTPROPERTIES_H
