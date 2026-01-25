#pragma once

#include <QWidget>
#include <tools/paletteeditor/paletteimport.hpp>

namespace Ui {
class ImportGFXTool;
}

class GFXImportViewer : public QLabel
{
    Q_OBJECT
public:
    GFXImportViewer(short *cSel, QList<QImage> gfx, bool scImg = false, QWidget *parent = nullptr)
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

class ImportGFXTool : public QDialog
{
    Q_OBJECT

public:
    explicit ImportGFXTool(byte gameVer, QList<QImage> &chunkSrc, QList<QImage> &chunksDst, QList<PaletteColor> stagePal, QList<QRgb> importPal, QWidget *parent = nullptr);
    ~ImportGFXTool();

    short selectedChunk = 0;
    short replacedChunk = 0;

    QList<PaletteColor> curPalette;
    QList<PaletteColor> palFramePalette;
    QSet<QRgb> chkPal;
    ViewerWidget *paletteFrame = nullptr;
    QSet<int> rplChkIDs;
    bool generateCol = false;

    bool ctrlSelect  = false;
    bool shiftSelect = false;
    bool modified    = false;

    void setupUI();

protected:
    bool event(QEvent *e) override;

private:
    Ui::ImportGFXTool *ui;

    GFXImportViewer *chkView = nullptr;
    QList<QImage> &chunkImgList;
};
