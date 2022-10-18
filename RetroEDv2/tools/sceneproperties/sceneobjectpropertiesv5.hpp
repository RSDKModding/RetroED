#pragma once

#include <QWidget>

#include "sceneincludesv5.hpp"

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

    void setupUI(SceneEntity *entity);
    void unsetUI();

    void updateUI();

    PropertyBrowser *properties = nullptr;

signals:
    void typeChanged(SceneEntity *entity, byte type);

private:
    Ui::SceneObjectPropertiesv5 *ui;

    SceneEntity *entityPtr = nullptr;

    QMessageBox *msgBox = nullptr;
};


