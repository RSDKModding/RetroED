#pragma once

#include <QWidget>

#include <RSDKv5/tileconfigv5.hpp>
#include "tools/sceneviewer.hpp"

namespace Ui
{
class ChunkCollisionEditor;
}

class ChunkEditorViewer : public QLabel
{
    Q_OBJECT
public:
    ChunkEditorViewer(ushort *cSel, Vector2<int> *sel, FormatHelpers::Chunks *chk, QList<QImage> tiles, QWidget *parent = nullptr)
        : QLabel(parent), cSel(cSel), selection(sel), tiles(tiles), chunks(chk)
    {
    }
    RSDKv5::TileConfig::CollisionMask *cmask[2][1024];
    int collisionLyr;

    Vector2<int> offset = Vector2<int>(0, 0);

signals:
    void tileDrawn(float mx, float my);
    void tileCopy(float mx, float my);
    void tileChanged();

protected:
    bool event(QEvent *e) override;
    void paintEvent(QPaintEvent *event) override;

private:
    ushort *cSel            = nullptr;
    Vector2<int> *selection = nullptr;
    int index;

    float zoom = 1.0f;

    QPoint reference;

    bool mouseDownL = false;
    bool mouseDownM = false;
    bool mouseDownR = false;

    QList<QImage> tiles;
    FormatHelpers::Chunks *chunks = nullptr;
};

class ChunkCollisionViewer : public QWidget
{
    Q_OBJECT
public:
    explicit ChunkCollisionViewer(QWidget *parent = nullptr);
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

class ChunkCollisionEditor : public QDialog
{
    Q_OBJECT
public:
    explicit ChunkCollisionEditor(FormatHelpers::Chunks *chk, ushort curChunk, QList<QImage> &tiles, SceneViewer *viewer);
    ~ChunkCollisionEditor();
    RSDKv5::TileConfig::CollisionMask *cmask[2][1024];
    Vector2<int> selectedTile = Vector2<int>(0, 0);

    byte defaultVisualPlane   = 0;
    ushort selectedChunk      = -1;
    int selectedDrawTile      = -1;
protected:
    bool event(QEvent *e) override;

private:
    Ui::ChunkCollisionEditor *ui;

    ChunkEditorViewer *collisionViewer  = nullptr;
    ChunkCollisionViewer *colEdit  = nullptr;
    FormatHelpers::Chunks *chunks = nullptr;

    QList<QImage> &tileList;
    byte collisionLyr   =  0;
    QImage imageTileList[64];
    ushort storedTileList[64];
    RSDKv5::TileConfig::CollisionMask *chunkColMask[2][64];

};
