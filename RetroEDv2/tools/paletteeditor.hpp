#ifndef PALETTEEDITOR_H
#define PALETTEEDITOR_H

#include <QWidget>

namespace Ui
{
class PaletteEditor;
}

class PaletteEditor : public QWidget
{
    Q_OBJECT

public:
    explicit PaletteEditor(QWidget *parent = nullptr);
    ~PaletteEditor();

private:
    Ui::PaletteEditor *ui;
};

#endif // PALETTEEDITOR_H
