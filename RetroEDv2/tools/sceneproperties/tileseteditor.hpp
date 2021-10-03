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
    explicit TilesetEditor(QList<QImage> *tileList, QList<QColor> *pal, QWidget *parent = nullptr);
    ~TilesetEditor();

    QList<ushort> tileIDs;

private:
    Ui::TilesetEditor *ui;

    QList<QColor> *palette;
    QList<QImage> *tiles;
};

#endif // TILESETEDITOR_HPP
