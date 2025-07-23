#pragma once

#include <QWidget>

#include <RSDKv5/tileconfigv5.hpp>
#include "tools/sceneviewer.hpp"

namespace Ui
{
class ChunkCollisionEditor;
}

class ChunkColViewer : public QLabel
{
    Q_OBJECT
public:
    ChunkColViewer(ushort *cSel, FormatHelpers::Chunks *chk, QList<QImage> tiles, bool drawChk = false, QWidget *parent = nullptr)
        : QLabel(parent), cSel(cSel), tiles(tiles), chunks(chk), drawChk(drawChk)
    {
    }
    RSDKv5::TileConfig::CollisionMask *cmask[2][1024];
    int collisionLyr;

    Vector2<int> offset = Vector2<int>(0, 0);

signals:
    void tileSelected(ushort c);

protected:
    bool event(QEvent *e) override;
    void paintEvent(QPaintEvent *) override;

private:
    ushort *cSel            = nullptr;
    int index;

    float zoom = 1.0f;

    QPoint reference;

    bool mouseDownL = false;
    bool mouseDownM = false;
    bool mouseDownR = false;

    QList<QImage> tiles;
    FormatHelpers::Chunks *chunks = nullptr;
    bool drawChk = false;
};

class ChunkColEdit : public QWidget
{
    Q_OBJECT
public:
    explicit ChunkColEdit(QWidget *parent = nullptr);
    RSDKv5::TileConfig::CollisionMask *cmask = nullptr;
    QImage tileImg;
    bool mAngleMode = false;
    Vector2<int> DrawPoint1 = (-1, -1);
    Vector2<int> DrawPoint2 = (-1, -1);

protected:
    void paintEvent(QPaintEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void leaveEvent(QEvent *) override;

signals:
    void updateViewer();

private:
    short selection = -1;
    short highlight = -1;

    bool pressedL = false;
    bool pressedR = false;
    bool enabling = false;
};

class ChunkCollisionEditor : public QDialog
{
    Q_OBJECT
public:
    explicit ChunkCollisionEditor(FormatHelpers::Chunks *chk, ushort curChunk, QList<QImage> &tiles, SceneViewer *viewer);
    ~ChunkCollisionEditor();
    RSDKv5::TileConfig::CollisionMask *cmask[2][1024];

    byte defaultVisualPlane   =  0;
    ushort selectedChunk      = -1;
    int selectedDrawTile      = -1;
protected:
    bool event(QEvent *e) override;

public slots:
    void changeSelTile(int c);

private:
    Ui::ChunkCollisionEditor *ui;

    ChunkColViewer *collisionViewer  = nullptr;
    ChunkColViewer *chunkViewer  = nullptr;
    ChunkColEdit *colEdit  = nullptr;
    FormatHelpers::Chunks *chunks = nullptr;

    QList<QImage> &tileList;
    byte collisionLyr   =  0;
    QImage imageTileList[64];
    ushort storedTileList[64];
    RSDKv5::TileConfig::CollisionMask *chunkColMask[2][64];

};
