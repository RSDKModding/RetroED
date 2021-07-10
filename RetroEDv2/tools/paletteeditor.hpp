#ifndef PALETTEEDITOR_H
#define PALETTEEDITOR_H

namespace Ui
{
class PaletteEditor;
}

struct PaletteColour {
    PaletteColour() {}
    PaletteColour(byte r, byte g, byte b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    void read(Reader &reader)
    {
        r = reader.read<byte>();
        g = reader.read<byte>();
        b = reader.read<byte>();
    }

    void write(Writer &writer)
    {
        writer.write(r);
        writer.write(g);
        writer.write(b);
    }

    const inline QColor toQColor() { return QColor(r, g, b); }

    byte r = 0xFF;
    byte g = 0x00;
    byte b = 0xFF;
};

class PaletteWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaletteWidget(QWidget *parent = nullptr);
    QColor colour = 0xFF00FF;
    QList<PaletteColour> *palette;

protected:
    void paintEvent(QPaintEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void leaveEvent(QEvent *) override;

private:
    short highlight = -1;
    short m_selection = -1;

    // bool m_dragging = false;
    bool m_enabling;
    bool m_pressed = false;
};

class PaletteEditor : public QWidget
{
    Q_OBJECT

public:
    explicit PaletteEditor(QString filepath = "", byte type = 0xFF, QWidget *parent = nullptr);
    ~PaletteEditor();

    bool event(QEvent *event) override;

protected:
    QSize sizeHint() const override { return QSize(500, 450); }

private:
    PaletteWidget *widget;
    void init();
    void reinit();

    Ui::PaletteEditor *ui;

    QList<PaletteColour> palette;

    RSDKv4::Gameconfig m_gameconfigv4;
    RSDKv4::Stageconfig stageConfigv4;
    RSDKv3::Stageconfig stageConfigv3;
    RSDKv2::Stageconfig stageConfigv2;
    RSDKv1::Stageconfig stageConfigv1;

    QString m_filename = "";

    bool firstInit = true;
    byte m_palType   = 0;
};

#endif // PALETTEEDITOR_H
