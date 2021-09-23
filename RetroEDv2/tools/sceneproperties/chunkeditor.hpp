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

signals:
    void tileDrawn(float mx, float my);
    void tileCopy(float mx, float my);
    void tileChanged();

protected:
    bool event(QEvent *e) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    int *cSel               = nullptr;
    Vector2<int> *selection = nullptr;
    int index;

    float zoom = 1.0f;

    QList<QImage> tiles;
    FormatHelpers::Chunks *chunks = nullptr;
};

class ChunkEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ChunkEditor(FormatHelpers::Chunks *chk, QList<QImage> &chunks, QList<QImage> &tiles,
                         bool v1 = false, QWidget *parent = nullptr);
    ~ChunkEditor();

    int selectedChunk         = -1;
    int copiedChunk           = -1;
    Vector2<int> selectedTile = Vector2<int>(0, 0);
    int selectedDrawTile      = -1;

    void setupUI();

protected:
    bool event(QEvent *e) override;

    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::ChunkEditor *ui;

    ChunkViewer *viewer           = nullptr;
    FormatHelpers::Chunks *chunks = nullptr;

    QList<QImage> tileList;
};

#endif // CHUNKEDITOR_H
