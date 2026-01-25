#pragma once

#include <QWidget>
#include <tools/paletteeditor/paletteimport.hpp>

namespace Ui {
class GFXReplaceTool;
}

class GFXReplaceViewer : public QLabel
{
    Q_OBJECT
public:
    GFXReplaceViewer(short *cSel, QList<QImage> gfx, bool scImg = false, QWidget *parent = nullptr)
        : QLabel(parent), cSel(cSel), scaleImg(scImg), gfx(gfx) {}

    Vector2<int> offset = Vector2<int>(0, 0);
    bool flipX = false;
    bool flipY = false;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    short *cSel   = nullptr;
    bool scaleImg = false;
    float zoom    = 1.0f;

    QList<QImage> gfx;
};

class GFXReplaceTool : public QDialog
{
    Q_OBJECT

public:
    explicit GFXReplaceTool(short listSize, QList<QImage> &gfxSrc, FormatHelpers::Chunks *chkS = nullptr, QWidget *parent = nullptr);
    ~GFXReplaceTool();

    short selectedItem = 0;
    short replacedItem = 0;

    QSet<int> rplIDs;
    QMap<int,int> srcToRplIDs;

    bool ctrlSelect  = false;
    bool shiftSelect = false;
    bool modified    = false;

    void setupUI();
    FormatHelpers::Chunks chunksDst;

protected:
    bool event(QEvent *e) override;

private:
    Ui::GFXReplaceTool *ui;

    GFXReplaceViewer *gfxView        = nullptr;
    FormatHelpers::Chunks *chunksFmt = nullptr;
    FormatHelpers::Chunks::Chunk tempChunk;

    QList<QImage> &imgList;
};
