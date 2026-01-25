#pragma once

#include <QWidget>
#include "RSDKv5/tileconfigv5.hpp"
#include "RSDKv5/stamps.hpp"
#include "../paletteeditor/paletteimport.hpp"

namespace Ui
{
class TilesetEditor;
}

class TilePalette : public QWidget
{
    Q_OBJECT
public:
    explicit TilePalette(QWidget *parent = nullptr);
    QColor color = 0xFF00FF;
    QList<PaletteColor*> palette;
    short selection = -1;

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

class TilesetViewer : public QLabel
{
    Q_OBJECT
public:
    TilesetViewer(int *tSel, int *sSel, QWidget *parent = nullptr)
        : QLabel(parent), tSel(tSel), sSel(sSel)
    {
    }

    Vector2<int> offset = Vector2<int>(0, 0);
    Vector2<int> gridPos = Vector2<int>(0, 0);
    Vector2<int> highlightTile = Vector2<int>(0, 0);
    Vector2<int> stampGrid = Vector2<int>(0,0);

    byte editMode = 0;
    bool inGrid = false;
    byte rowSize = 8;
    QList<ushort> viewerTiles;
    QSet<ushort> angleTiles;
    int selColor = 128;
    RSDKv5::TileConfig *tileConfig = nullptr;
    byte drawTool = 0;
    byte colTool  = 0;
    byte colLyr   = 0;

    Vector2<int> DrawPoint1 = Vector2<int>(-1, -1);
    Vector2<int> DrawPoint2 = Vector2<int>(-1, -1);
    int scale = 4;
    QRect rect{0,0,0,0};

    bool usingAngleLine = false;
    QList<QImage*> tiles;

signals:
    void tileDrawn(ushort tile);
    void tileCopy(ushort tile);
    void tileChanged(ushort tile);
    void tileInfo(ushort tile);
    void updateLists();
    void updateAngles(QSet<ushort> tile);
    void setColor(int slot);

protected:
    bool event(QEvent *e) override;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *event) override;

private:
    int *tSel               = nullptr;
    int *sSel               = nullptr;
    Vector2<int> selection = Vector2<int>(-1, -1);
    int index;
    bool solidityCheck = false;

    float zoom = 0.5f;

    QPoint reference;

    bool mouseDownL = false;
    bool mouseDownM = false;
    bool mouseDownR = false;

};

class TilesetEditor : public QDialog
{
    Q_OBJECT

public:
    explicit TilesetEditor(QList<QImage> &tileList, RSDKv5::TileConfig &tConf, QList<PaletteColor> &pal, QList<RSDKv5::Stamps::StampEntry> &stampList,
                           QWidget *parent = nullptr);
    ~TilesetEditor();

    int selectedTile  = -1;
    int selectedStamp = -1;
    int copiedTile    = -1;
    int angleTile     = -1;

    QList<ushort> tileIDs;
    QList<bool> changedTiles;

    QVector<QRgb> clrTable;
    void setupUI();

protected:
    bool event(QEvent *e) override;

    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::TilesetEditor *ui;
    RSDKv5::TileConfig &tileConfig;
    QList<QImage> &tiles;
    QList<PaletteColor> &palette;
    QList<RSDKv5::Stamps::StampEntry> &stamps;
    TilePalette *palWidget = nullptr;
    PaletteImport *importFile = nullptr;
    byte stampTool = 0;

    inline int getColorIndex(QImage &img, const QVector<QRgb> &clrTable, int x, int y)
    {
        int index    = -1;
        int distance = 0x7FFFFFFF;

        QRgb px = img.pixel(x, y);
        for (int c = 0; c < palette.count(); ++c) {
            if (abs((int)(px - clrTable[c])) < distance) {
                distance = abs((int)(px - clrTable[c]));
                index    = c;
            }
        }

        return index;
    }

    TilesetViewer *viewer = nullptr;
};


