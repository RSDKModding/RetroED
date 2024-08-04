#pragma once

#include <QWidget>

#include <RSDKv5/scenev5.hpp>

class SceneViewer;

namespace Ui
{
class SceneLayerPropertiesv5;
}

class SceneLayerPropertiesv5 : public QWidget
{
    Q_OBJECT

public:
    explicit SceneLayerPropertiesv5(QWidget *parent = nullptr);
    ~SceneLayerPropertiesv5();

    void setupUI(SceneViewer *viewer, byte lID);
    void unsetUI();

signals:
    void updateEditorLayer(QString name);

private:
    Ui::SceneLayerPropertiesv5 *ui;
};


