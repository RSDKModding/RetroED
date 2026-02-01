#pragma once

#include <QWidget>

#include "RSDKv1/tileconfigv1.hpp"
#include "RSDKv5/tileconfigv5.hpp"
#include "../paletteeditor/paletteimport.hpp"
#include <QUndoCommand>

namespace Ui
{
class ChunkEditor;
}

class ChunkCollision : public QLabel
{
    Q_OBJECT
public:
    ChunkCollision(ushort *cSel, FormatHelpers::Chunks *chk, QList<QImage> tiles, bool drawChk = false, QWidget *parent = nullptr)
        : QLabel(parent), cSel(cSel), tiles(tiles), chunks(chk), drawChk(drawChk)
    {
    }
    RSDKv5::TileConfig::CollisionMask *cmask[2][1024];
    int collisionLyr;

    Vector2<int> offset = Vector2<int>(0, 0);

signals:
    void tileSelected(ushort c);

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

class ChunkPalette : public QWidget
{
    Q_OBJECT
public:
    explicit ChunkPalette(QWidget *parent = nullptr);
    QColor color = 0xFF00FF;
    QList<PaletteColor*> palette;
    short selection = -1;
    bool v1Format = false;

signals:
    void setColor(int slot);
    void colorChange(PaletteColor clr);
protected:
    void paintEvent(QPaintEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *) override;
    void leaveEvent(QEvent *) override;

private:
    short highlight = -1;

           // bool m_dragging = false;
    bool enabling;
    bool pressed = false;
};

class ChunkViewer : public QLabel
{
    Q_OBJECT
public:
    ChunkViewer(int *cSel, Vector2<int> *sel, FormatHelpers::Chunks *chk,
                QWidget *parent = nullptr)
        : QLabel(parent), cSel(cSel), selection(sel), chunks(chk)
    {
    }
    Vector2<int> offset = Vector2<int>(0, 0);
    Vector2<int> highlightTile = Vector2<int>(-1, -1);
    Vector2<int> gridPos = Vector2<int>(0, 0);
    byte tileDrawMode = 0;
    byte editMode = 0;

    bool keepProps = false;
    bool ignoreFirstTile = true;
    bool inGrid = false;
    byte rowSize = 8;
    QList<int> viewerTiles;
    QSet<int> angleTiles;
    int selColor = 128;
    QList<QImage*> tiles;
    RSDKv5::TileConfig *tileConfig = nullptr;
    RSDKv1::TileConfig *tileConfigv1 = nullptr;
    byte drawTool = 0;
    byte colTool  = 0;
    byte colLyr   = 0;

    Vector2<int> DrawPoint1 = Vector2<int>(-1, -1);
    Vector2<int> DrawPoint2 = Vector2<int>(-1, -1);

    int scale = 4;
    int colIndex = 0;
    QRect rect{0,0,0,0};

    bool usingAngleLine = false;

signals:
    void tileDrawn(int tileX, int tileY);
    void tileCopy(int tileX, int tileY);
    void tileChanged();
    void updateLists();
    void updateAngles(QSet<int> tile);
    void updateColTilev1(int tile);
    void setColor(int slot);

protected:
    bool event(QEvent *e) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event) override;

private:
    int *cSel               = nullptr;
    Vector2<int> *selection = nullptr;
    bool solidityCheck = false;

    float zoom = 0.5f;

    QPoint reference;

    bool mouseDownL = false;
    bool mouseDownM = false;
    bool mouseDownR = false;

    FormatHelpers::Chunks *chunks = nullptr;
};


class ChunkEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ChunkEditor(FormatHelpers::Chunks *chk, QList<QImage> &chunks, QList<QImage> &tiles, QList<PaletteColor> &stgPal,
                         byte gameVer, RSDKv5::TileConfig &tConf, RSDKv1::TileConfig &tConfv1, QWidget *parent = nullptr);
    ~ChunkEditor();

    int selectedChunk         = -1;
    int copiedChunk           = -1;
    Vector2<int> selectedTile = Vector2<int>(0, 0);
    int angleTile             = -1;

    byte defaultVisualPlane   = 0;
    Vector2<bool> defaultFlip = Vector2<bool>(false, false);
    int selectedDrawTile      = -1;
    byte defaultSolidA        = 3;
    byte defaultSolidB        = 3;
    byte palType = 0;
    QList<ushort> chunkIDs;
    QList<bool> changedChunks;
    QList<ushort> tileIDs;
    QList<bool> changedTiles;

    void setupUI();

protected:
    bool event(QEvent *e) override;

    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::ChunkEditor *ui;

    ChunkViewer *viewer           = nullptr;
    FormatHelpers::Chunks *chunks = nullptr;

    QList<QImage> &tileList;
    QList<QImage> &chunkImgList;
    RSDKv5::TileConfig &tileConfig;
    RSDKv1::TileConfig &tileConfigv1;
    QList<PaletteColor> &stagePal;
    ChunkPalette *palWidget   = nullptr;
    PaletteImport *importFile = nullptr;

    QList<bool> usedTiles;

    QUndoStack *undoStack = nullptr;
    QUndoView *undoView   = nullptr;
    QAction *undoAction   = nullptr;
    QAction *redoAction   = nullptr;
    QList<QAction> actionList;

    QComboBox *colIndex  = nullptr;
    QComboBox *colMode   = nullptr;
    QLabel *colModeLabel = nullptr;
};

class ChunkEditorCommand : public QUndoCommand
{
public:
    explicit ChunkEditorCommand(QUndoCommand *parent = nullptr); // Chunk Editor changes
    //explicit ChunkEditorCommand(QUndoCommand *parent = nullptr); // Tile Editor changes
    //explicit ChunkEditorCommand(QUndoCommand *parent = nullptr); // Collision Editor changes
    //explicit ChunkEditorCommand(QUndoCommand *parent = nullptr); // Palette changes
    void undo() override;
    void redo() override;
};

