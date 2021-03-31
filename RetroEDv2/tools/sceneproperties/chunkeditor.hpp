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
        : QLabel(parent), m_cSel(cSel), m_sel(sel), m_chunks(chk), m_tiles(tiles)
    {
    }

signals:
    void tileDrawn(float mx, float my);
    void tileChanged();

protected:
    bool event(QEvent *e) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    int *m_cSel         = nullptr;
    Vector2<int> *m_sel = nullptr;
    int m_index;

    float m_zoom = 1.0f;

    QList<QImage> m_tiles;
    FormatHelpers::Chunks *m_chunks = nullptr;
};

class ChunkEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ChunkEditor(FormatHelpers::Chunks *chk, QList<QImage> &chunks, QList<QImage> &tiles,
                         bool v1 = false, QWidget *parent = nullptr);
    ~ChunkEditor();

    int m_selectedChunk         = -1;
    Vector2<int> m_selectedTile = Vector2<int>(0, 0);
    int m_selectedDrawTile      = -1;

    void setupUI();

private:
    Ui::ChunkEditor *ui;

    ChunkViewer *m_viewer           = nullptr;
    FormatHelpers::Chunks *m_chunks = nullptr;
};

#endif // CHUNKEDITOR_H
