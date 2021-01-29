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

    void setupUI(FormatHelpers::Scene::Object *obj, Compilerv3::Entity *entityv3,
                 Compilerv4::Entity *entityv4, byte ver);
    void unsetUI();

    void updateUI();

    QComboBox *m_typeBox = nullptr;

private:
    Ui::SceneObjectProperties *ui;

    FormatHelpers::Scene::Object *m_obj = nullptr;
};

#endif // SCENEOBJECTPROPERTIES_H
