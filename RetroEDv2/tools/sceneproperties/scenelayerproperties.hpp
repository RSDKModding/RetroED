#pragma once

#include <QWidget>

class SceneViewer;

namespace Ui
{
class SceneLayerProperties;
}

class SceneLayerProperties : public QWidget
{
    Q_OBJECT

public:
    explicit SceneLayerProperties(QWidget *parent = nullptr);
    ~SceneLayerProperties();

    void setupUI(SceneViewer *viewer, byte layerID);
    void unsetUI();

private:
    Ui::SceneLayerProperties *ui;
};


