#pragma once

#include <QWidget>

#include <RSDKv1/tileconfigv1.hpp>
#include "tools/sceneviewer.hpp"

namespace Ui
{
class ChunkCollisionEditorv1;
}

class ChunkColViewerv1 : public QLabel
{
    Q_OBJECT
public:
    ChunkColViewerv1(ushort *cSel, FormatHelpers::Chunks *chk, QList<QImage> tiles, bool drawChk = false, QWidget *parent = nullptr)
        : QLabel(parent), cSel(cSel), tiles(tiles), chunks(chk), drawChk(drawChk)
    {
    }
    RSDKv1::TileConfig::CollisionMask *cmask[2][1024];
    int collisionLyr;
    int maskIndex;

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

class ChunkColEditv1 : public QWidget
{
    Q_OBJECT
public:
    explicit ChunkColEditv1(QWidget *parent = nullptr);
    RSDKv1::TileConfig::CollisionMask *cmask = nullptr;
    QImage tileImg;
    byte maskIndex;

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

class ChunkCollisionEditorv1 : public QDialog
{
    Q_OBJECT
public:
    explicit ChunkCollisionEditorv1(FormatHelpers::Chunks *chk, ushort curChunk, QList<QImage> &tiles, SceneViewer *viewer);
    ~ChunkCollisionEditorv1();
    RSDKv1::TileConfig::CollisionMask *cmask[2][1024];

    byte defaultVisualPlane   =  0;
    byte maskIndex            =  0;
    ushort selectedChunk      = -1;
    int selectedDrawTile      = -1;
protected:
    bool event(QEvent *e) override;

public slots:
    void changeSelTile(int c);

private:
    Ui::ChunkCollisionEditorv1 *ui;

    ChunkColViewerv1 *collisionViewer  = nullptr;
    ChunkColViewerv1 *chunkViewer  = nullptr;
    ChunkColEditv1 *colEdit  = nullptr;
    FormatHelpers::Chunks *chunks = nullptr;

    QList<QImage> &tileList;
    byte collisionLyr   =  0;
    QImage imageTileList[64];
    ushort storedTileList[64];
    RSDKv1::TileConfig::CollisionMask *chunkColMask[2][64];

};
