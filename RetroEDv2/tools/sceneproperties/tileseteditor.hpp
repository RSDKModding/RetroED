#ifndef TILESETEDITOR_HPP
#define TILESETEDITOR_HPP

#include <QDialog>

namespace Ui
{
class TilesetEditor;
}

class TilesetEditor : public QDialog
{
    Q_OBJECT

public:
    explicit TilesetEditor(QList<QImage> &tileList, QList<PaletteColour> &pal,
                           QWidget *parent = nullptr);
    ~TilesetEditor();

    QList<ushort> tileIDs;

private:
    Ui::TilesetEditor *ui;

    inline int getColourIndex(QImage &img, const QVector<QRgb> &clrTable, int x, int y)
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
    };

    QList<PaletteColour> &palette;
    QList<QImage> &tiles;
};

#endif // TILESETEDITOR_HPP
