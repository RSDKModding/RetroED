#pragma once

#include <QWidget>

#include "sceneincludesv5.hpp"

namespace Ui
{
class SceneScrollProperties;
}

class SceneScrollProperties : public QWidget
{
    Q_OBJECT

public:
    explicit SceneScrollProperties(QWidget *parent = nullptr);
    ~SceneScrollProperties();

    void setupUI(SceneHelpers::TileLayer::ScrollIndexInfo *info);
    void unsetUI();

private:
    Ui::SceneScrollProperties *ui;
};


