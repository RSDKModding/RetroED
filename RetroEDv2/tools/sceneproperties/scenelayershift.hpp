#pragma once

#include <QDialog>
#include "includes.hpp"
#include "sceneincludesv5.hpp"

namespace Ui
{
class SceneLayerShift;
}

class SceneLayerShift : public QDialog
{
    Q_OBJECT

public:
    explicit SceneLayerShift(SceneHelpers::TileLayer layer, byte shiftSize = 0, QWidget *parent = nullptr);
    ~SceneLayerShift();

    void SetupUI();
    int shiftX   = 0;
    int shiftY   = 0;
    int size     = 0;

    bool keepDimensions = false;
    bool shiftEnt = false;

private:
    Ui::SceneLayerShift *ui;
};

