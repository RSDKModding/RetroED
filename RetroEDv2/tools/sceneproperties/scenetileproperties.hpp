#pragma once

#include <QWidget>

#include <RSDKv5/tileconfigv5.hpp>
#include "tools/sceneviewer.hpp"

namespace Ui
{
class SceneTileProperties;
}

class TileCollisionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TileCollisionWidget(QWidget *parent = nullptr);
    RSDKv5::TileConfig::CollisionMask *cmask = nullptr;
    QImage tileImg;

protected:
    void paintEvent(QPaintEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void leaveEvent(QEvent *) override;

private:
    short selection = -1;
    short highlight = -1;

    bool pressedL = false;
    bool pressedR = false;
    bool enabling = false;
};

class SceneTileProperties : public QWidget
{
    Q_OBJECT

public:
    explicit SceneTileProperties(QWidget *parent = nullptr);
    ~SceneTileProperties();

    void setupUI(RSDKv5::TileConfig::CollisionMask *cmA, RSDKv5::TileConfig::CollisionMask *cmB,
                 ushort tID, QList<QImage> &tiles, SceneViewer *viewer);
    void unsetUI();

private:
    Ui::SceneTileProperties *ui;
    TileCollisionWidget *edit;
    QImage tileImg;
    int collisionLyr     =  0;
    int selectedDrawTile = -1;

    RSDKv5::TileConfig::CollisionMask *cmask[2] = { nullptr, nullptr };
};


