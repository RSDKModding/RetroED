#pragma once

#include <QWidget>

#include <RSDKv5/scenev5.hpp>

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

    void setupUI(RSDKv5::Scene *scn, byte lID);
    void unsetUI();

private:
    Ui::SceneLayerPropertiesv5 *ui;

    RSDKv5::Scene *scene = nullptr;
    int layerID          = 0;
};


