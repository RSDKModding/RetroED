#pragma once

#include <QWidget>

namespace Ui {
class ChunkReplaceOptions;
}

class ChunkReplaceViewer : public QLabel
{
    Q_OBJECT
public:
    ChunkReplaceViewer(short *cSel, FormatHelpers::Chunks *chk, QList<QImage> tiles,
                QWidget *parent = nullptr)
        : QLabel(parent), cSel(cSel), tiles(tiles), chunks(chk){}

    Vector2<int> offset = Vector2<int>(0, 0);
    bool flipX = false;
    bool flipY = false;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    short *cSel             = nullptr;
    float zoom = 2.0f;

    QList<QImage> tiles;
    FormatHelpers::Chunks *chunks = nullptr;
};

class ChunkReplaceOptions : public QDialog
{
    Q_OBJECT

public:
    explicit ChunkReplaceOptions(byte gameVer, FormatHelpers::Chunks *chk, QList<QImage> &chunks, QList<QImage> &tiles, QWidget *parent = nullptr);
    ~ChunkReplaceOptions();

    short selectedChunk = 0;
    short replacedChunk = 0;

    bool ctrlSelect  = false;
    bool shiftSelect = false;
    bool modified    = false;

    void setupUI();

protected:
    bool event(QEvent *e) override;

private:
    Ui::ChunkReplaceOptions *ui;

    ChunkReplaceViewer *chkView      = nullptr;
    FormatHelpers::Chunks *chunks    = nullptr;
    FormatHelpers::Chunks::Chunk tempChunk;

    QList<QImage> &tileList;
    QList<QImage> &chunkImgList;
};
