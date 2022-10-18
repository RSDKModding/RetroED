#pragma once

#include <QWidget>

#include "sceneincludesv5.hpp"

namespace Ui
{
class SceneScrollPropertiesv5;
}

class SceneScrollPropertiesv5 : public QWidget
{
    Q_OBJECT

public:
    explicit SceneScrollPropertiesv5(QWidget *parent = nullptr);
    ~SceneScrollPropertiesv5();

    void setupUI(SceneHelpers::TileLayer::ScrollIndexInfo *info);
    void unsetUI();

private:
    Ui::SceneScrollPropertiesv5 *ui;
};


