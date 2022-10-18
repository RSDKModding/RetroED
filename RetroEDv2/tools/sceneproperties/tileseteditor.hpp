#pragma once

#include <QWidget>

namespace Ui
{
class TilesetEditor;
}

class TilesetViewer : public QLabel
{
    Q_OBJECT
public:
    TilesetViewer(int *tSel, QList<QImage> &tiles, QWidget *parent = nullptr)
        : QLabel(parent), tSel(tSel), tiles(tiles)
    {
    }

    Vector2<int> offset = Vector2<int>(0, 0);

protected:
    bool event(QEvent *e) override;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event) override;

private:
    int *tSel               = nullptr;
    Vector2<int> *selection = nullptr;
    int index;

    float zoom = 8.0f;

    QPoint reference;

    bool mouseDownL = false;
    bool mouseDownM = false;
    bool mouseDownR = false;

    QList<QImage> &tiles;
};

class TilesetEditor : public QDialog
{
    Q_OBJECT

public:
    explicit TilesetEditor(QList<QImage> &tileList, QList<PaletteColor> &pal,
                           QWidget *parent = nullptr);
    ~TilesetEditor();

    int selectedTile = -1;
    int copiedTile   = -1;

    QList<ushort> tileIDs;
    QList<bool> changedTiles;

    QList<PaletteColor> &palette;
    QList<QImage> &tiles;

    void setupUI();

protected:
    bool event(QEvent *e) override;

    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::TilesetEditor *ui;

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


