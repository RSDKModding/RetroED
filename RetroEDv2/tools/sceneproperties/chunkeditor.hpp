#ifndef CHUNKEDITOR_H
#define CHUNKEDITOR_H

#include <QWidget>

namespace Ui
{
class ChunkEditor;
}

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

    FormatHelpers::Chunks *m_chunks = nullptr;
};

#endif // CHUNKEDITOR_H
