#pragma once

#include <QWidget>

#include "tools/compiler/compilerv2.hpp"
#include "tools/compiler/compilerv3.hpp"
#include "tools/compiler/compilerv4.hpp"

class SceneEntity;

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

    void setupUI(SceneEntity *entity, int entityID, Compilerv2::Entity *entityv2,
                 Compilerv3::Entity *entityv3, Compilerv4::Entity *entityv4, byte ver);
    void unsetUI();

    void updateUI();

    int callRSDKEdit(void *e, bool shouldReturnVal, int entityID, int variableID, int variableValue,
                     bool *called = nullptr);

    PropertyBrowser *properties = nullptr;

signals:
    void typeChanged(SceneEntity *entity, byte type, bool keepVals = false);

private:
    Ui::SceneObjectProperties *ui;

    SceneEntity *entityPtr = nullptr;

    QMessageBox *msgBox = nullptr;
};


