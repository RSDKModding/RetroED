#ifndef SCENETILEPROPERTIES_V5_H
#define SCENETILEPROPERTIES_V5_H

#include <QWidget>
#include <QDialog>

namespace Ui
{
class SceneTilePropertiesv5;
}

class TileCollisionWidgetv5 : public QDialog
{
    Q_OBJECT
public:
    explicit TileCollisionWidgetv5(QWidget *parent = nullptr);
    RSDKv5::TileConfig::CollisionMask *m_cmask = nullptr;
    QImage m_tileImg;

protected:
    void paintEvent(QPaintEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void leaveEvent(QEvent *) override;

private:
    short m_selection = -1;
    short m_highlight = -1;

    bool m_pressedL = false;
    bool m_pressedR = false;
    bool m_enabling = false;
};

class SceneTilePropertiesv5 : public QWidget
{
    Q_OBJECT

public:
    explicit SceneTilePropertiesv5(QWidget *parent = nullptr);
    ~SceneTilePropertiesv5();

    void setupUI(RSDKv5::TileConfig::CollisionMask *cmA, RSDKv5::TileConfig::CollisionMask *cmB,
                 ushort *tile, QImage tileImg);
    void unsetUI();

private:
    Ui::SceneTilePropertiesv5 *ui;
    QImage m_tileImg;
    int m_collisionLyr = 0;

    RSDKv5::TileConfig::CollisionMask *m_cmask[2] = { nullptr, nullptr };
};

#endif // SCENETILEPROPERTIES_V5_H
