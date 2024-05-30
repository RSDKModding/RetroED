#pragma once

#include <QWidget>
#include <includes.hpp>
#include <RSDKv5/stageconfigv5.hpp>
#include <RSDKv5/gameconfigv5.hpp>

namespace Ui {
class PaletteImport;
}

class ViewerWidget : public QLabel
{
    Q_OBJECT
public:
    explicit ViewerWidget(QWidget *parent = nullptr) : QLabel(parent) { setMouseTracking(true); }
    QColor color = 0xFF00FF;
    QList<PaletteColor> importPalette;
    QList<PaletteColor> *palette;
    int palSize = 0;

signals:
    void pastePalette(byte x, byte y);
protected:
    bool event(QEvent *e) override;
    void paintEvent(QPaintEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void leaveEvent(QEvent *) override;

private:
    short row = -1;
    short col = -1;
    short selection = -1;
    bool enabling;

    // bool m_dragging = false;
    bool pressed = false;
};

class PaletteImport : public QDialog
{
    Q_OBJECT

public:
    explicit PaletteImport(QList<PaletteColor> import, QList<PaletteColor> &base, bool enableBanks = false, QWidget *parent = nullptr);
    ~PaletteImport();
    RSDKv5::Palette banks[8];
    int activeBank = 0;


private:
    Ui::PaletteImport *ui;
    ViewerWidget *importPalWidget = nullptr;
    ViewerWidget *curPalwidget = nullptr;
    QList<PaletteColor> storedPal;

};

