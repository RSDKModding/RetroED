#ifndef CHUNKEDITOR_H
#define CHUNKEDITOR_H

#include <QWidget>

namespace Ui
{
class ChunkEditor;
}

class ChunkViewer : public QLabel
{
    Q_OBJECT
public:
    ChunkViewer(int *cSel, Vector2<int> *sel, FormatHelpers::Chunks *chk, QList<QImage> tiles,
                QWidget *parent = nullptr)
        : QLabel(parent), cSel(cSel), selection(sel), chunks(chk), tiles(tiles)
    {
    }

    Vector2<int> offset = Vector2<int>(0, 0);

signals:
    void tileDrawn(float mx, float my);
    void tileCopy(float mx, float my);
    void tileChanged();

protected:
    bool event(QEvent *e) override;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event) override;

private:
    int *cSel               = nullptr;
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

class ChunkEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ChunkEditor(FormatHelpers::Chunks *chk, QList<QImage> &chunks, QList<QImage> &tiles,
                         byte gameVer, QWidget *parent = nullptr);
    ~ChunkEditor();

    int selectedChunk         = -1;
    int copiedChunk           = -1;
    Vector2<int> selectedTile = Vector2<int>(0, 0);

    byte defaultVisualPlane   = 0;
    Vector2<bool> defaultFlip = Vector2<bool>(false, false);
    int selectedDrawTile      = -1;
    byte defaultSolidA        = 3;
    byte defaultSolidB        = 3;

    QList<bool> changedChunks;

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
};

#endif // CHUNKEDITOR_H
