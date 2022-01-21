#ifndef SCENETILEPROPERTIES_H
#define SCENETILEPROPERTIES_H

#include <QWidget>
#include <QDialog>

namespace Ui
{
class SceneTileProperties;
}

class TileCollisionWidget : public QDialog
{
    Q_OBJECT
public:
    explicit TileCollisionWidget(QWidget *parent = nullptr);
    RSDKv4::TileConfig::CollisionMask *cmask = nullptr;
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

    void setupUI(RSDKv4::TileConfig::CollisionMask *cmA, RSDKv4::TileConfig::CollisionMask *cmB,
                 ushort tID, QImage tileImg);
    void unsetUI();

private:
    Ui::SceneTileProperties *ui;
    QImage tileImg;
    int collisionLyr = 0;

    RSDKv4::TileConfig::CollisionMask *cmask[2] = { nullptr, nullptr };
};

#endif // SCENETILEPROPERTIES_H
