#ifndef PALETTEEDITOR_H
#define PALETTEEDITOR_H

namespace Ui
{
class PaletteEditor;
}

enum PaletteFormatTypes {
    PALTYPE_ACT,
    PALTYPE_GAMECONFIGv5,
    PALTYPE_GAMECONFIGv5_rev01,
    PALTYPE_STAGECONFIGv5,
    PALTYPE_GAMECONFIGv4,
    PALTYPE_STAGECONFIGv4,
    PALTYPE_STAGECONFIGv3,
    PALTYPE_STAGECONFIGv2,
    PALTYPE_STAGECONFIGv1,
    PALTYPE_COLOURLIST,
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
    short selection = -1;

    // bool m_dragging = false;
    bool enabling;
    bool pressed = false;
};

class PaletteEditor : public QDialog
{
    Q_OBJECT

public:
    class ActionState
    {
    public:
        QString name = "Action";

        QList<PaletteColour> palette;

        RSDKv5::GameConfig gameConfigv5;
        RSDKv5::StageConfig stageConfigv5;
        RSDKv4::GameConfig gameConfigv4;
        RSDKv4::StageConfig stageConfigv4;
        RSDKv3::StageConfig stageConfigv3;
        RSDKv2::StageConfig stageConfigv2;
        RSDKv1::StageConfig stageConfigv1;
    };

    explicit PaletteEditor(QString filepath = "", byte type = 0xFF, QWidget *parent = nullptr);
    ~PaletteEditor();

    bool event(QEvent *event) override;

    inline void updateTitle(bool modified)
    {
        this->modified = modified;
        if (modified)
            emit titleChanged(tabTitle + " *");
        else
            emit titleChanged(tabTitle);
    }

    QList<PaletteColour> palette;

    RSDKv5::GameConfig gameConfigv5;
    RSDKv5::StageConfig stageConfigv5;
    RSDKv4::GameConfig gameConfigv4;
    RSDKv4::StageConfig stageConfigv4;
    RSDKv3::StageConfig stageConfigv3;
    RSDKv2::StageConfig stageConfigv2;
    RSDKv1::StageConfig stageConfigv1;

    QString filePath = "";

    byte palType = 0;
    byte bankID  = 0;

signals:
    void titleChanged(QString title);

protected:
    QSize sizeHint() const override { return QSize(500, 450); }

private:
    PaletteWidget *widget;
    void init();
    void reinit();

    void load(QString path, byte type);
    void switchBank(int id);

    Ui::PaletteEditor *ui;

    bool firstInit = true;

public:
    void undoAction();
    void redoAction();
    void resetAction();
    void doAction(QString name = "Action", bool setModified = true);
    void clearActions();

private:
    QList<ActionState> actions;
    int actionIndex = 0;

    bool modified    = false;
    QString tabTitle = "Palette Editor";
};

#endif // PALETTEEDITOR_H
