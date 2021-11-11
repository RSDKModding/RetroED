#include "includes.hpp"
#include "ui_paletteeditor.h"

PaletteEditor::PaletteEditor(QString filepath, byte type, QWidget *parent)
    : QWidget(parent), widget(new PaletteWidget(this)), ui(new Ui::PaletteEditor)
{
    if (QFile::exists(filepath)) {
        palType = type;
        switch (type) {
            case PALTYPE_ACT: { //.act
                QList<PaletteColour> pal;
                Reader reader(filepath);
                pal.clear();
                while (!reader.isEOF()) {
                    PaletteColour clr;
                    clr.read(reader);
                    pal.append(clr);
                }
                palette = pal;
                break;
            }
            case PALTYPE_GAMECONFIGv5:
            case PALTYPE_GAMECONFIGv5_rev01: {
                gameConfigv5 = RSDKv5::GameConfig(filepath, type == PALTYPE_GAMECONFIGv5_rev01);
                palette.clear();
                for (int r = 0; r < 16; ++r) {
                    if (gameConfigv5.palettes[0].activeRows[r]) {
                        for (int c = 0; c < 16; ++c) {
                            palette.append(
                                PaletteColour(gameConfigv5.palettes[0].colours[r][c].red(),
                                              gameConfigv5.palettes[0].colours[r][c].green(),
                                              gameConfigv5.palettes[0].colours[r][c].blue()));
                        }
                    }
                    else {
                        for (int c = 0; c < 16; ++c) {
                            palette.append(PaletteColour(0xFF, 0x00, 0xFF));
                        }
                    }
                }
                break;
            }
            case PALTYPE_STAGECONFIGv5: {
                stageConfigv5 = RSDKv5::StageConfig(filepath);
                palette.clear();
                for (int r = 0; r < 16; ++r) {
                    if (stageConfigv5.palettes[0].activeRows[r]) {
                        for (int c = 0; c < 16; ++c) {
                            palette.append(
                                PaletteColour(stageConfigv5.palettes[0].colours[r][c].red(),
                                              stageConfigv5.palettes[0].colours[r][c].green(),
                                              stageConfigv5.palettes[0].colours[r][c].blue()));
                        }
                    }
                    else {
                        for (int c = 0; c < 16; ++c) {
                            palette.append(PaletteColour(0xFF, 0x00, 0xFF));
                        }
                    }
                }
                break;
            }
            case PALTYPE_GAMECONFIGv4: {
                gameConfigv4 = RSDKv4::GameConfig(filepath);
                palette.clear();
                for (auto &c : gameConfigv4.palette.colours) {
                    palette.append(PaletteColour(c.r, c.g, c.b));
                }
                break;
            }
            case PALTYPE_STAGECONFIGv4: {
                stageConfigv4 = RSDKv4::StageConfig(filepath);
                palette.clear();
                for (auto &c : stageConfigv4.palette.colours) {
                    palette.append(PaletteColour(c.r, c.g, c.b));
                }
                break;
            }
            case PALTYPE_STAGECONFIGv3: {
                stageConfigv3 = RSDKv3::StageConfig(filepath);
                palette.clear();
                for (auto &c : stageConfigv3.palette.colours) {
                    palette.append(PaletteColour(c.r, c.g, c.b));
                }
                break;
            }
            case PALTYPE_STAGECONFIGv2: {
                stageConfigv2 = RSDKv2::StageConfig(filepath);
                palette.clear();
                for (auto &c : stageConfigv2.palette.colours) {
                    palette.append(PaletteColour(c.r, c.g, c.b));
                }
                break;
            }
            case PALTYPE_STAGECONFIGv1: {
                stageConfigv1 = RSDKv1::StageConfig(filepath);
                palette.clear();
                for (auto &c : stageConfigv1.palette.colours) {
                    palette.append(PaletteColour(c.r, c.g, c.b));
                }
                break;
            }
        }
    }
    ui->setupUi(this);
    init();
}

PaletteEditor::~PaletteEditor() {}

void PaletteEditor::init()
{
    setWindowTitle("Palette Editor");
    widget->palette = &palette;

    connect(ui->importPal, &QPushButton::clicked, [=] {
        QStringList types        = { "Adobe Colour Table Palettes (*.act)",
                              "rev02 (plus) RSDKv5 GameConfig Palettes (*GameConfig*.bin)",
                              "rev01 (pre-plus) RSDKv5 GameConfig Palettes (*GameConfig*.bin)",
                              "RSDKv5 StageConfig Palettes (*StageConfig*.bin)",
                              "RSDKv4 GameConfig Palettes (*GameConfig*.bin)",
                              "RSDKv4 StageConfig Palettes (*StageConfig*.bin)",
                              "RSDKv3 StageConfig Palettes (*StageConfig*.bin)",
                              "RSDKv2 StageConfig Palettes (*StageConfig*.bin)",
                              "RSDKv1 StageConfig Palettes (*Zone*.zcf)" };
        QList<QString> typesList = { "Adobe Colour Table Palettes (*.act)",
                                     "rev02 (plus) RSDKv5 GameConfig Palettes (*GameConfig*.bin)",
                                     "rev01 (pre-plus) RSDKv5 GameConfig Palettes (*GameConfig*.bin)",
                                     "RSDKv5 StageConfig Palettes (*StageConfig*.bin)",
                                     "RSDKv4 GameConfig Palettes (*GameConfig*.bin)",
                                     "RSDKv4 StageConfig Palettes (*StageConfig*.bin)",
                                     "RSDKv3 StageConfig Palettes (*StageConfig*.bin)",
                                     "RSDKv2 StageConfig Palettes (*StageConfig*.bin)",
                                     "RSDKv1 StageConfig Palettes (*Zone*.zcf)" };
        QFileDialog filedialog(this, tr("Open Palette"), "",
                               tr(types.join(";;").toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            int type         = typesList.indexOf(filedialog.selectedNameFilter());
            QString fileName = filedialog.selectedFiles()[0];

            disconnect(ui->importPal, nullptr, nullptr, nullptr);

            switch (type) {
                case PALTYPE_ACT: { //.act
                    QList<PaletteColour> pal;
                    Reader reader(fileName);
                    pal.clear();
                    while (!reader.isEOF()) {
                        PaletteColour clr;
                        clr.read(reader);
                        pal.append(clr);
                    }
                    palette = pal;
                    break;
                }
                case PALTYPE_GAMECONFIGv5:
                case PALTYPE_GAMECONFIGv5_rev01: {
                    gameConfigv5 = RSDKv5::GameConfig(fileName, type == PALTYPE_GAMECONFIGv5_rev01);
                    palette.clear();
                    for (int r = 0; r < 16; ++r) {
                        if (gameConfigv5.palettes[0].activeRows[r]) {
                            for (int c = 0; c < 16; ++c) {
                                palette.append(
                                    PaletteColour(gameConfigv5.palettes[0].colours[r][c].red(),
                                                  gameConfigv5.palettes[0].colours[r][c].green(),
                                                  gameConfigv5.palettes[0].colours[r][c].blue()));
                            }
                        }
                        else {
                            for (int c = 0; c < 16; ++c) {
                                palette.append(PaletteColour(0xFF, 0x00, 0xFF));
                            }
                        }
                    }
                    break;
                }
                case PALTYPE_STAGECONFIGv5: {
                    stageConfigv5 = RSDKv5::StageConfig(fileName);
                    palette.clear();
                    for (int r = 0; r < 16; ++r) {
                        if (stageConfigv5.palettes[0].activeRows[r]) {
                            for (int c = 0; c < 16; ++c) {
                                palette.append(
                                    PaletteColour(stageConfigv5.palettes[0].colours[r][c].red(),
                                                  stageConfigv5.palettes[0].colours[r][c].green(),
                                                  stageConfigv5.palettes[0].colours[r][c].blue()));
                            }
                        }
                        else {
                            for (int c = 0; c < 16; ++c) {
                                palette.append(PaletteColour(0xFF, 0x00, 0xFF));
                            }
                        }
                    }
                    break;
                }
                case PALTYPE_GAMECONFIGv4: {
                    RSDKv4::GameConfig config(fileName);
                    palette.clear();
                    for (auto &c : config.palette.colours) {
                        palette.append(PaletteColour(c.r, c.g, c.b));
                    }
                    break;
                }
                case PALTYPE_STAGECONFIGv4: {
                    RSDKv4::StageConfig config(fileName);
                    palette.clear();
                    for (auto &c : config.palette.colours) {
                        palette.append(PaletteColour(c.r, c.g, c.b));
                    }
                    break;
                }
                case PALTYPE_STAGECONFIGv3: {
                    RSDKv3::StageConfig config(fileName);
                    palette.clear();
                    for (auto &c : config.palette.colours) {
                        palette.append(PaletteColour(c.r, c.g, c.b));
                    }
                    break;
                }
                case PALTYPE_STAGECONFIGv2: {
                    RSDKv2::StageConfig config(fileName);
                    palette.clear();
                    for (auto &c : config.palette.colours) {
                        palette.append(PaletteColour(c.r, c.g, c.b));
                    }
                    break;
                }
                case PALTYPE_STAGECONFIGv1: {
                    RSDKv1::StageConfig config(fileName);
                    palette.clear();
                    for (auto &c : config.palette.colours) {
                        palette.append(PaletteColour(c.r, c.g, c.b));
                    }
                    break;
                }
            }

            init();
        }
    });

    connect(ui->exportPal, &QPushButton::clicked, [=] {
        QFileDialog filedialog(this, tr("Export Palette"), "",
                               tr("Adobe Colour Table Palettes (*.act)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            QString fileName = filedialog.selectedFiles()[0];

            Writer writer(fileName);
            for (auto &c : palette) c.write(writer);
            writer.flush();
        }
    });

    if (firstInit) {
        ui->widgetLayout->addWidget(widget, 1);
    }

    firstInit = false;
}

void PaletteEditor::reinit() { palette.clear(); }

PaletteWidget::PaletteWidget(QWidget *parent) : QWidget(parent) { setMouseTracking(true); }

void PaletteWidget::leaveEvent(QEvent *)
{
    highlight = -1;
    update();
}

void PaletteWidget::mousePressEvent(QMouseEvent *event)
{
    selection = highlight;
    enabling  = (event->button() & Qt::LeftButton) == Qt::LeftButton;
    pressed   = true;
}

void PaletteWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    ColourDialog dlg(palette->at(selection));
    if (dlg.exec() == QDialog::Accepted) {
        PaletteColour clr;
        clr.r = dlg.colour().r;
        clr.g = dlg.colour().g;
        clr.b = dlg.colour().b;
        palette->replace(selection, clr);
    }
}

void PaletteWidget::mouseReleaseEvent(QMouseEvent *) { pressed = false; }

void PaletteWidget::mouseMoveEvent(QMouseEvent *event)
{
    byte x = floor((float)(event->x() / ((qreal)width() / 16)));
    byte y = floor((float)(event->y() / ((qreal)height() / 16)));

    if (y > 15)
        y = 15;

    highlight = x % 16 + y * 16;

    update();
}

void PaletteWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    QRectF rect(0, 0, (qreal)width() / 16, (qreal)height() / 16);
    short index = -1;
    if (!palette)
        return;

    for (byte y = 0; y < palette->count() / 0x10; ++y) {
        for (byte x = 0; x < 0x10; ++x) {
            ++index;
            if (index >= palette->count())
                return;
            // if (index >= m_palette->m_pal.count()) continue;
            QPen pen(qApp->palette().base(), 2);
            p.setPen(pen);
            PaletteColour clr = palette->at(index);
            p.setBrush(clr.toQColor());

            p.drawRect(rect.translated(x * (qreal)width() / 16, y * (qreal)height() / 16));

            if (highlight == index) {
                p.setBrush(qApp->palette().highlight());
                p.setPen(Qt::NoPen);
                p.setOpacity(0.7);
                p.drawRect(rect.translated(x * (qreal)width() / 16, y * (qreal)height() / 16));
                p.setOpacity(1.0);
            }
        }
    }
}

bool PaletteEditor::event(QEvent *event)
{
    QStringList types        = { "Adobe Colour Table Palettes (*.act)",
                          "rev02 (plus) RSDKv5 GameConfig Palettes (*GameConfig*.bin)",
                          "rev01 (pre-plus) RSDKv5 GameConfig Palettes (*GameConfig*.bin)",
                          "RSDKv5 StageConfig Palettes (*StageConfig*.bin)",
                          "RSDKv4 GameConfig Palettes (*GameConfig*.bin)",
                          "RSDKv4 StageConfig Palettes (*StageConfig*.bin)",
                          "RSDKv3 StageConfig Palettes (*StageConfig*.bin)",
                          "RSDKv2 StageConfig Palettes (*StageConfig*.bin)",
                          "RSDKv1 StageConfig Palettes (*Zone*.zcf)" };
    QList<QString> typesList = { "Adobe Colour Table Palettes (*.act)",
                                 "rev02 (plus) RSDKv5 GameConfig Palettes (*GameConfig*.bin)",
                                 "rev01 (pre-plus) RSDKv5 GameConfig Palettes (*GameConfig*.bin)",
                                 "RSDKv5 StageConfig Palettes (*StageConfig*.bin)",
                                 "RSDKv4 GameConfig Palettes (*GameConfig*.bin)",
                                 "RSDKv4 StageConfig Palettes (*StageConfig*.bin)",
                                 "RSDKv3 StageConfig Palettes (*StageConfig*.bin)",
                                 "RSDKv2 StageConfig Palettes (*StageConfig*.bin)",
                                 "RSDKv1 StageConfig Palettes (*Zone*.zcf)" };

    if (event->type() == (QEvent::Type)RE_EVENT_NEW) {
        palette.clear();
        reinit();
        return true;
    }

    if (event->type() == (QEvent::Type)RE_EVENT_OPEN) {
        QFileDialog filedialog(this, tr("Open Palette"), "",
                               tr(types.join(";;").toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            setStatus("Opening palette \"" + Utils::getFilenameAndFolder(filedialog.selectedFiles()[0])
                      + "\"");
            reinit();

            QString filepath = filedialog.selectedFiles()[0];
            int type         = typesList.indexOf(filedialog.selectedNameFilter());
            palType          = type;
            switch (type) {
                case PALTYPE_ACT: { //.act
                    QList<PaletteColour> pal;
                    Reader reader(filepath);
                    pal.clear();
                    while (!reader.isEOF()) {
                        PaletteColour clr;
                        clr.read(reader);
                        pal.append(clr);
                    }
                    palette = pal;
                    break;
                }
                case PALTYPE_GAMECONFIGv5:
                case PALTYPE_GAMECONFIGv5_rev01: {
                    gameConfigv5 = RSDKv5::GameConfig(filepath, type == PALTYPE_GAMECONFIGv5_rev01);
                    palette.clear();
                    for (int r = 0; r < 16; ++r) {
                        if (gameConfigv5.palettes[0].activeRows[r]) {
                            for (int c = 0; c < 16; ++c) {
                                palette.append(
                                    PaletteColour(gameConfigv5.palettes[0].colours[r][c].red(),
                                                  gameConfigv5.palettes[0].colours[r][c].green(),
                                                  gameConfigv5.palettes[0].colours[r][c].blue()));
                            }
                        }
                        else {
                            for (int c = 0; c < 16; ++c) {
                                palette.append(PaletteColour(0xFF, 0x00, 0xFF));
                            }
                        }
                    }
                    break;
                }
                case PALTYPE_STAGECONFIGv5: {
                    stageConfigv5 = RSDKv5::StageConfig(filepath);
                    palette.clear();
                    for (int r = 0; r < 16; ++r) {
                        if (stageConfigv5.palettes[0].activeRows[r]) {
                            for (int c = 0; c < 16; ++c) {
                                palette.append(
                                    PaletteColour(stageConfigv5.palettes[0].colours[r][c].red(),
                                                  stageConfigv5.palettes[0].colours[r][c].green(),
                                                  stageConfigv5.palettes[0].colours[r][c].blue()));
                            }
                        }
                        else {
                            for (int c = 0; c < 16; ++c) {
                                palette.append(PaletteColour(0xFF, 0x00, 0xFF));
                            }
                        }
                    }
                    break;
                }
                case PALTYPE_GAMECONFIGv4: {
                    gameConfigv4 = RSDKv4::GameConfig(filepath);
                    palette.clear();
                    for (auto &c : gameConfigv4.palette.colours) {
                        palette.append(PaletteColour(c.r, c.g, c.b));
                    }
                    break;
                }
                case PALTYPE_STAGECONFIGv4: {
                    stageConfigv4 = RSDKv4::StageConfig(filepath);
                    palette.clear();
                    for (auto &c : stageConfigv4.palette.colours) {
                        palette.append(PaletteColour(c.r, c.g, c.b));
                    }
                    break;
                }
                case PALTYPE_STAGECONFIGv3: {
                    stageConfigv3 = RSDKv3::StageConfig(filepath);
                    palette.clear();
                    for (auto &c : stageConfigv3.palette.colours) {
                        palette.append(PaletteColour(c.r, c.g, c.b));
                    }
                    break;
                }
                case PALTYPE_STAGECONFIGv2: {
                    stageConfigv2 = RSDKv2::StageConfig(filepath);
                    palette.clear();
                    for (auto &c : stageConfigv2.palette.colours) {
                        palette.append(PaletteColour(c.r, c.g, c.b));
                    }
                    break;
                }
                case PALTYPE_STAGECONFIGv1: {
                    stageConfigv1 = RSDKv1::StageConfig(filepath);
                    palette.clear();
                    for (auto &c : stageConfigv1.palette.colours) {
                        palette.append(PaletteColour(c.r, c.g, c.b));
                    }
                    break;
                }
            }

            appConfig.addRecentFile(palType, TOOL_PALETTEDITOR, filepath, QList<QString>{});
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE) {
        if (!QFile::exists(filePath)) {
            QFileDialog filedialog(this, tr("Save Palette"), "",
                                   tr(typesList[palType].toStdString().c_str()));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {
                setStatus("Saving palette...");

                QString filepath = filedialog.selectedFiles()[0];
                switch (palType) {
                    case PALTYPE_ACT: {
                        Writer writer(filepath);
                        for (auto &c : palette) {
                            c.write(writer);
                        }
                        writer.flush();
                        break;
                    }
                    case PALTYPE_GAMECONFIGv5:
                    case PALTYPE_GAMECONFIGv5_rev01: {
                        for (int r = 0; r < 16; ++r) {
                            if (true) {
                                for (int c = 0; c < 16; ++c) {
                                    for (int c = 0; c < 16; ++c) {
                                        gameConfigv5.palettes[0].colours[r][c] =
                                            QColor(palette[c].r, palette[c].g, palette[c].b);
                                    }
                                }
                            }
                            else {
                                for (int c = 0; c < 16; ++c) {
                                    gameConfigv5.palettes[0].colours[r][c] = QColor(0xFF, 0x00, 0xFF);
                                }
                            }
                        }
                        gameConfigv5.readMode = palType != PALTYPE_GAMECONFIGv5_rev01;
                        gameConfigv5.write(filepath);
                        break;
                    }
                    case PALTYPE_STAGECONFIGv5: {
                        for (int r = 0; r < 16; ++r) {
                            if (true) {
                                for (int c = 0; c < 16; ++c) {
                                    for (int c = 0; c < 16; ++c) {
                                        stageConfigv5.palettes[0].colours[r][c] =
                                            QColor(palette[c].r, palette[c].g, palette[c].b);
                                    }
                                }
                            }
                            else {
                                for (int c = 0; c < 16; ++c) {
                                    stageConfigv5.palettes[0].colours[r][c] = QColor(0xFF, 0x00, 0xFF);
                                }
                            }
                        }
                        stageConfigv5.write(filepath);
                        break;
                    }
                    case PALTYPE_GAMECONFIGv4: {
                        gameConfigv4.palette.colours.clear();
                        for (int c = 0; c < 96; ++c) {
                            if (c < palette.count())
                                gameConfigv4.palette.colours.append(
                                    Colour(palette[c].r, palette[c].g, palette[c].b));
                            else
                                gameConfigv4.palette.colours.append(Colour(0xFF, 0x00, 0xFF));
                        }
                        gameConfigv4.write(filepath);
                        break;
                    }
                    case PALTYPE_STAGECONFIGv4: {
                        stageConfigv4.palette.colours.clear();
                        for (int c = 0; c < 32; ++c) {
                            if (c < palette.count())
                                stageConfigv4.palette.colours.append(
                                    Colour(palette[c].r, palette[c].g, palette[c].b));
                            else
                                stageConfigv4.palette.colours.append(Colour(0xFF, 0x00, 0xFF));
                        }
                        stageConfigv4.write(filepath);
                        break;
                    }
                    case PALTYPE_STAGECONFIGv3: {
                        stageConfigv3.palette.colours.clear();
                        for (int c = 0; c < 32; ++c) {
                            if (c < palette.count())
                                stageConfigv3.palette.colours.append(
                                    Colour(palette[c].r, palette[c].g, palette[c].b));
                            else
                                stageConfigv3.palette.colours.append(Colour(0xFF, 0x00, 0xFF));
                        }
                        stageConfigv3.write(filepath);
                        break;
                    }
                    case PALTYPE_STAGECONFIGv2: {
                        stageConfigv2.palette.colours.clear();
                        for (int c = 0; c < 32; ++c) {
                            if (c < palette.count())
                                stageConfigv2.palette.colours.append(
                                    Colour(palette[c].r, palette[c].g, palette[c].b));
                            else
                                stageConfigv2.palette.colours.append(Colour(0xFF, 0x00, 0xFF));
                        }
                        stageConfigv2.write(filepath);
                        break;
                    }
                    case PALTYPE_STAGECONFIGv1: {
                        stageConfigv1.palette.colours.clear();
                        for (int c = 0; c < 32; ++c) {
                            if (c < palette.count())
                                stageConfigv1.palette.colours.append(
                                    Colour(palette[c].r, palette[c].g, palette[c].b));
                            else
                                stageConfigv1.palette.colours.append(Colour(0xFF, 0x00, 0xFF));
                        }
                        stageConfigv1.write(filepath);
                        break;
                    }
                }

                appConfig.addRecentFile(palType, TOOL_PALETTEDITOR, filepath, QList<QString>{});
                return true;
            }
        }
        else {
            setStatus("Saving palette...");

            QString filepath = filePath;
            switch (palType) {
                case PALTYPE_ACT: {
                    Writer writer(filepath);
                    for (auto &c : palette) {
                        c.write(writer);
                    }
                    writer.flush();
                    break;
                }
                case PALTYPE_GAMECONFIGv5:
                case PALTYPE_GAMECONFIGv5_rev01: {
                    for (int r = 0; r < 16; ++r) {
                        if (true) {
                            for (int c = 0; c < 16; ++c) {
                                for (int c = 0; c < 16; ++c) {
                                    gameConfigv5.palettes[0].colours[r][c] =
                                        QColor(palette[c].r, palette[c].g, palette[c].b);
                                }
                            }
                        }
                        else {
                            for (int c = 0; c < 16; ++c) {
                                gameConfigv5.palettes[0].colours[r][c] = QColor(0xFF, 0x00, 0xFF);
                            }
                        }
                    }
                    gameConfigv5.readMode = palType != PALTYPE_GAMECONFIGv5_rev01;
                    gameConfigv5.write(filepath);
                    break;
                }
                case PALTYPE_STAGECONFIGv5: {
                    for (int r = 0; r < 16; ++r) {
                        if (true) {
                            for (int c = 0; c < 16; ++c) {
                                for (int c = 0; c < 16; ++c) {
                                    stageConfigv5.palettes[0].colours[r][c] =
                                        QColor(palette[c].r, palette[c].g, palette[c].b);
                                }
                            }
                        }
                        else {
                            for (int c = 0; c < 16; ++c) {
                                stageConfigv5.palettes[0].colours[r][c] = QColor(0xFF, 0x00, 0xFF);
                            }
                        }
                    }
                    stageConfigv5.write(filepath);
                    break;
                }
                case PALTYPE_GAMECONFIGv4: {
                    gameConfigv4.palette.colours.clear();
                    for (int c = 0; c < 96; ++c) {
                        if (c < palette.count())
                            gameConfigv4.palette.colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            gameConfigv4.palette.colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    gameConfigv4.write(filepath);
                    break;
                }
                case PALTYPE_STAGECONFIGv4: {
                    stageConfigv4.palette.colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < palette.count())
                            stageConfigv4.palette.colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            stageConfigv4.palette.colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    stageConfigv4.write(filepath);
                    break;
                }
                case PALTYPE_STAGECONFIGv3: {
                    stageConfigv3.palette.colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < palette.count())
                            stageConfigv3.palette.colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            stageConfigv3.palette.colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    stageConfigv3.write(filepath);
                    break;
                }
                case PALTYPE_STAGECONFIGv2: {
                    stageConfigv2.palette.colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < palette.count())
                            stageConfigv2.palette.colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            stageConfigv2.palette.colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    stageConfigv2.write(filepath);
                    break;
                }
                case PALTYPE_STAGECONFIGv1: {
                    stageConfigv1.palette.colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < palette.count())
                            stageConfigv1.palette.colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            stageConfigv1.palette.colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    stageConfigv1.write(filepath);
                    break;
                }
            }

            appConfig.addRecentFile(palType, TOOL_PALETTEDITOR, filepath, QList<QString>{});
            return true;
        }
    }
    if (event->type() == (QEvent::Type)RE_EVENT_SAVE_AS) {
        QList<QString> typesList = { "Adobe Colour Table Palettes (*.act)",
                                     "rev02 (plus) RSDKv5 GameConfig Palettes (*GameConfig*.bin)",
                                     "rev01 (pre-plus) RSDKv5 GameConfig Palettes (*GameConfig*.bin)",
                                     "RSDKv5 StageConfig Palettes (*StageConfig*.bin)",
                                     "RSDKv4 GameConfig Palettes (*GameConfig*.bin)",
                                     "RSDKv4 StageConfig Palettes (*StageConfig*.bin)",
                                     "RSDKv3 StageConfig Palettes (*StageConfig*.bin)",
                                     "RSDKv2 StageConfig Palettes (*StageConfig*.bin)",
                                     "RSDKv1 StageConfig Palettes (*Zone*.zcf)" };
        QFileDialog filedialog(this, tr("Save Palette"),
                               QFile::exists(filePath) ? QFileInfo(filePath).absolutePath() : "",
                               tr(typesList[palType].toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            setStatus("Saving palette...");

            QString filepath = filedialog.selectedFiles()[0];
            switch (palType) {
                case 0: { //.act
                    Writer writer(filepath);
                    for (auto &c : palette) {
                        c.write(writer);
                    }
                    writer.flush();
                    break;
                }
                case PALTYPE_GAMECONFIGv5:
                case PALTYPE_GAMECONFIGv5_rev01: {
                    for (int r = 0; r < 16; ++r) {
                        if (true) {
                            for (int c = 0; c < 16; ++c) {
                                for (int c = 0; c < 16; ++c) {
                                    gameConfigv5.palettes[0].colours[r][c] =
                                        QColor(palette[c].r, palette[c].g, palette[c].b);
                                }
                            }
                        }
                        else {
                            for (int c = 0; c < 16; ++c) {
                                gameConfigv5.palettes[0].colours[r][c] = QColor(0xFF, 0x00, 0xFF);
                            }
                        }
                    }
                    gameConfigv5.readMode = palType != PALTYPE_GAMECONFIGv5_rev01;
                    gameConfigv5.write(filepath);
                    break;
                }
                case PALTYPE_STAGECONFIGv5: {
                    for (int r = 0; r < 16; ++r) {
                        if (true) {
                            for (int c = 0; c < 16; ++c) {
                                for (int c = 0; c < 16; ++c) {
                                    stageConfigv5.palettes[0].colours[r][c] =
                                        QColor(palette[c].r, palette[c].g, palette[c].b);
                                }
                            }
                        }
                        else {
                            for (int c = 0; c < 16; ++c) {
                                stageConfigv5.palettes[0].colours[r][c] = QColor(0xFF, 0x00, 0xFF);
                            }
                        }
                    }
                    stageConfigv5.write(filepath);
                    break;
                }
                case PALTYPE_GAMECONFIGv4: {
                    gameConfigv4.palette.colours.clear();
                    for (int c = 0; c < 96; ++c) {
                        if (c < palette.count())
                            gameConfigv4.palette.colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            gameConfigv4.palette.colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    gameConfigv4.write(filepath);
                    break;
                }
                case PALTYPE_STAGECONFIGv4: {
                    stageConfigv4.palette.colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < palette.count())
                            stageConfigv4.palette.colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            stageConfigv4.palette.colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    stageConfigv4.write(filepath);
                    break;
                }
                case PALTYPE_STAGECONFIGv3: {
                    stageConfigv3.palette.colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < palette.count())
                            stageConfigv3.palette.colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            stageConfigv3.palette.colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    stageConfigv3.write(filepath);
                    break;
                }
                case PALTYPE_STAGECONFIGv2: {
                    stageConfigv2.palette.colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < palette.count())
                            stageConfigv2.palette.colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            stageConfigv2.palette.colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    stageConfigv2.write(filepath);
                    break;
                }
                case PALTYPE_STAGECONFIGv1: {
                    stageConfigv1.palette.colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < palette.count())
                            stageConfigv1.palette.colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            stageConfigv1.palette.colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    stageConfigv1.write(filepath);
                    break;
                }
            }

            appConfig.addRecentFile(palType, TOOL_PALETTEDITOR, filepath, QList<QString>{});
            return true;
        }
    }
    return QWidget::event(event);
}

#include "moc_paletteeditor.cpp"
