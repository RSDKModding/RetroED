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
    RSDKv4::TileConfig::CollisionMask *m_cmask = nullptr;
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
    QImage m_tileImg;
    int m_collisionLyr = 0;

    RSDKv4::TileConfig::CollisionMask *m_cmask[2] = { nullptr, nullptr };
};

#endif // SCENETILEPROPERTIES_H
