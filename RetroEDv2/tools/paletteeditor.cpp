#include "includes.hpp"
#include "ui_paletteeditor.h"

PaletteEditor::PaletteEditor(QString filepath, byte type, QWidget *parent)
    : QWidget(parent), widget(new PaletteWidget(this)), ui(new Ui::PaletteEditor)
{
    if (QFile::exists(filepath)) {
        m_palType = type;
        switch (type) {
            case 0: { //.act
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
            case 1: { // RSDKv4 GameConfig
                m_gameconfigv4 = RSDKv4::Gameconfig(filepath);
                palette.clear();
                for (auto &c : m_gameconfigv4.m_masterPalette.m_colours) {
                    palette.append(PaletteColour(c.r, c.g, c.b));
                }
                break;
            }
            case 2: { // RSDKv4 StageConfig
                stageConfigv4 = RSDKv4::Stageconfig(filepath);
                palette.clear();
                for (auto &c : stageConfigv4.m_stagePalette.m_colours) {
                    palette.append(PaletteColour(c.r, c.g, c.b));
                }
                break;
            }
            case 3: { // RSDKv3 StageConfig
                stageConfigv3 = RSDKv3::Stageconfig(filepath);
                palette.clear();
                for (auto &c : stageConfigv3.m_stagePalette.m_colours) {
                    palette.append(PaletteColour(c.r, c.g, c.b));
                }
                break;
            }
            case 4: { // RSDKv2 StageConfig
                stageConfigv2 = RSDKv2::Stageconfig(filepath);
                palette.clear();
                for (auto &c : stageConfigv2.m_stagePalette.m_colours) {
                    palette.append(PaletteColour(c.r, c.g, c.b));
                }
                break;
            }
            case 5: { // RSDKv1 StageConfig
                stageConfigv1 = RSDKv1::Stageconfig(filepath);
                palette.clear();
                for (auto &c : stageConfigv1.palette.m_colours) {
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
                              "RSDKv4 GameConfig Palettes (*GameConfig*.bin)",
                              "RSDKv4 StageConfig Palettes (*StageConfig*.bin)",
                              "RSDKv3 StageConfig Palettes (*StageConfig*.bin)",
                              "RSDKv2 StageConfig Palettes (*StageConfig*.bin)",
                              "RSDKv1 StageConfig Palettes (*Zone*.zcf)" };
        QList<QString> typesList = { "Adobe Colour Table Palettes (*.act)",
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
                case 0: { //.act
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
                case 1: { // RSDKv4 GameConfig
                    RSDKv4::Gameconfig config(fileName);
                    palette.clear();
                    for (auto &c : config.m_masterPalette.m_colours) {
                        palette.append(PaletteColour(c.r, c.g, c.b));
                    }
                    break;
                }
                case 2: { // RSDKv4 StageConfig
                    RSDKv4::Stageconfig config(fileName);
                    palette.clear();
                    for (auto &c : config.m_stagePalette.m_colours) {
                        palette.append(PaletteColour(c.r, c.g, c.b));
                    }
                    break;
                }
                case 3: { // RSDKv3 StageConfig
                    RSDKv3::Stageconfig config(fileName);
                    palette.clear();
                    for (auto &c : config.m_stagePalette.m_colours) {
                        palette.append(PaletteColour(c.r, c.g, c.b));
                    }
                    break;
                }
                case 4: { // RSDKv2 StageConfig
                    RSDKv2::Stageconfig config(fileName);
                    palette.clear();
                    for (auto &c : config.m_stagePalette.m_colours) {
                        palette.append(PaletteColour(c.r, c.g, c.b));
                    }
                    break;
                }
                case 5: { // RSDKv1 StageConfig
                    RSDKv1::Stageconfig config(fileName);
                    palette.clear();
                    for (auto &c : config.palette.m_colours) {
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
    m_selection = highlight;
    m_enabling  = (event->button() & Qt::LeftButton) == Qt::LeftButton;
    m_pressed   = true;
}

void PaletteWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    ColourDialog dlg(palette->at(m_selection));
    if (dlg.exec() == QDialog::Accepted) {
        PaletteColour clr;
        clr.r = dlg.colour().r;
        clr.g = dlg.colour().g;
        clr.b = dlg.colour().b;
        palette->replace(m_selection, clr);
    }
}

void PaletteWidget::mouseReleaseEvent(QMouseEvent *) { m_pressed = false; }

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
    if (event->type() == (QEvent::Type)RE_EVENT_NEW) {
        palette.clear();
        reinit();
        return true;
    }

    if (event->type() == (QEvent::Type)RE_EVENT_OPEN) {
        QStringList types        = { "Adobe Colour Table Palettes (*.act)",
                              "RSDKv4 GameConfig Palettes (*GameConfig*.bin)",
                              "RSDKv4 StageConfig Palettes (*StageConfig*.bin)",
                              "RSDKv3 StageConfig Palettes (*StageConfig*.bin)",
                              "RSDKv2 StageConfig Palettes (*StageConfig*.bin)",
                              "RSDKv1 StageConfig Palettes (*Zone*.zcf)" };
        QList<QString> typesList = { "Adobe Colour Table Palettes (*.act)",
                                     "RSDKv4 GameConfig Palettes (*GameConfig*.bin)",
                                     "RSDKv4 StageConfig Palettes (*StageConfig*.bin)",
                                     "RSDKv3 StageConfig Palettes (*StageConfig*.bin)",
                                     "RSDKv2 StageConfig Palettes (*StageConfig*.bin)",
                                     "RSDKv1 StageConfig Palettes (*Zone*.zcf)" };
        QFileDialog filedialog(this, tr("Open Palette"), "",
                               tr(types.join(";;").toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            setStatus("Opening palette \"" + Utils::getFilenameAndFolder(filedialog.selectedFiles()[0])
                      + "\"");
            reinit();

            QString filepath = filedialog.selectedFiles()[0];
            int type         = typesList.indexOf(filedialog.selectedNameFilter());
            m_palType        = type;
            switch (type) {
                case 0: { //.act
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
                case 1: { // RSDKv4 GameConfig
                    m_gameconfigv4 = RSDKv4::Gameconfig(filepath);
                    palette.clear();
                    for (auto &c : m_gameconfigv4.m_masterPalette.m_colours) {
                        palette.append(PaletteColour(c.r, c.g, c.b));
                    }
                    break;
                }
                case 2: { // RSDKv4 StageConfig
                    stageConfigv4 = RSDKv4::Stageconfig(filepath);
                    palette.clear();
                    for (auto &c : stageConfigv4.m_stagePalette.m_colours) {
                        palette.append(PaletteColour(c.r, c.g, c.b));
                    }
                    break;
                }
                case 3: { // RSDKv3 StageConfig
                    stageConfigv3 = RSDKv3::Stageconfig(filepath);
                    palette.clear();
                    for (auto &c : stageConfigv3.m_stagePalette.m_colours) {
                        palette.append(PaletteColour(c.r, c.g, c.b));
                    }
                    break;
                }
                case 4: { // RSDKv2 StageConfig
                    stageConfigv2 = RSDKv2::Stageconfig(filepath);
                    palette.clear();
                    for (auto &c : stageConfigv2.m_stagePalette.m_colours) {
                        palette.append(PaletteColour(c.r, c.g, c.b));
                    }
                    break;
                }
                case 5: { // RSDKv1 StageConfig
                    stageConfigv1 = RSDKv1::Stageconfig(filepath);
                    palette.clear();
                    for (auto &c : stageConfigv1.palette.m_colours) {
                        palette.append(PaletteColour(c.r, c.g, c.b));
                    }
                    break;
                }
            }

            appConfig.addRecentFile(m_palType, TOOL_PALETTEDITOR, filepath, QList<QString>{});
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE) {
        if (!QFile::exists(m_filename)) {
            QList<QString> typesList = { "Adobe Colour Table Palettes (*.act)",
                                         "RSDKv4 GameConfig Palettes (*GameConfig*.bin)",
                                         "RSDKv4 StageConfig Palettes (*StageConfig*.bin)",
                                         "RSDKv3 StageConfig Palettes (*StageConfig*.bin)",
                                         "RSDKv2 StageConfig Palettes (*StageConfig*.bin)",
                                         "RSDKv1 StageConfig Palettes (*Zone*.zcf)" };
            QFileDialog filedialog(this, tr("Open Palette"), "",
                                   tr(typesList[m_palType].toStdString().c_str()));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {
                setStatus("Saving palette...");

                QString filepath = filedialog.selectedFiles()[0];
                switch (m_palType) {
                    case 0: { //.act
                        Writer writer(filepath);
                        for (auto &c : palette) {
                            c.write(writer);
                        }
                        writer.flush();
                        break;
                    }
                    case 1: { // RSDKv4 GameConfig
                        m_gameconfigv4.m_masterPalette.m_colours.clear();
                        for (int c = 0; c < 96; ++c) {
                            if (c < palette.count())
                                m_gameconfigv4.m_masterPalette.m_colours.append(
                                    Colour(palette[c].r, palette[c].g, palette[c].b));
                            else
                                m_gameconfigv4.m_masterPalette.m_colours.append(
                                    Colour(0xFF, 0x00, 0xFF));
                        }
                        m_gameconfigv4.write(filepath);
                        break;
                    }
                    case 2: { // RSDKv4 StageConfig
                        stageConfigv4.m_stagePalette.m_colours.clear();
                        for (int c = 0; c < 32; ++c) {
                            if (c < palette.count())
                                stageConfigv4.m_stagePalette.m_colours.append(
                                    Colour(palette[c].r, palette[c].g, palette[c].b));
                            else
                                stageConfigv4.m_stagePalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                        }
                        stageConfigv4.write(filepath);
                        break;
                    }
                    case 3: { // RSDKv3 StageConfig
                        stageConfigv3.m_stagePalette.m_colours.clear();
                        for (int c = 0; c < 32; ++c) {
                            if (c < palette.count())
                                stageConfigv3.m_stagePalette.m_colours.append(
                                    Colour(palette[c].r, palette[c].g, palette[c].b));
                            else
                                stageConfigv3.m_stagePalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                        }
                        stageConfigv3.write(filepath);
                        break;
                    }
                    case 4: { // RSDKv2 StageConfig
                        stageConfigv2.m_stagePalette.m_colours.clear();
                        for (int c = 0; c < 32; ++c) {
                            if (c < palette.count())
                                stageConfigv2.m_stagePalette.m_colours.append(
                                    Colour(palette[c].r, palette[c].g, palette[c].b));
                            else
                                stageConfigv2.m_stagePalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                        }
                        stageConfigv2.write(filepath);
                        break;
                    }
                    case 5: { // RSDKv1 StageConfig
                        stageConfigv1.palette.m_colours.clear();
                        for (int c = 0; c < 32; ++c) {
                            if (c < palette.count())
                                stageConfigv1.palette.m_colours.append(
                                    Colour(palette[c].r, palette[c].g, palette[c].b));
                            else
                                stageConfigv1.palette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                        }
                        stageConfigv1.write(filepath);
                        break;
                    }
                }

                appConfig.addRecentFile(m_palType, TOOL_PALETTEDITOR, filepath, QList<QString>{});
                return true;
            }
        }
        else {
            setStatus("Saving palette...");

            QString filepath = m_filename;
            switch (m_palType) {
                case 0: { //.act
                    Writer writer(filepath);
                    for (auto &c : palette) {
                        c.write(writer);
                    }
                    writer.flush();
                    break;
                }
                case 1: { // RSDKv4 GameConfig
                    m_gameconfigv4.m_masterPalette.m_colours.clear();
                    for (int c = 0; c < 96; ++c) {
                        if (c < palette.count())
                            m_gameconfigv4.m_masterPalette.m_colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            m_gameconfigv4.m_masterPalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    m_gameconfigv4.write(filepath);
                    break;
                }
                case 2: { // RSDKv4 StageConfig
                    stageConfigv4.m_stagePalette.m_colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < palette.count())
                            stageConfigv4.m_stagePalette.m_colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            stageConfigv4.m_stagePalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    stageConfigv4.write(filepath);
                    break;
                }
                case 3: { // RSDKv3 StageConfig
                    stageConfigv3.m_stagePalette.m_colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < palette.count())
                            stageConfigv3.m_stagePalette.m_colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            stageConfigv3.m_stagePalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    stageConfigv3.write(filepath);
                    break;
                }
                case 4: { // RSDKv2 StageConfig
                    stageConfigv2.m_stagePalette.m_colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < palette.count())
                            stageConfigv2.m_stagePalette.m_colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            stageConfigv2.m_stagePalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    stageConfigv2.write(filepath);
                    break;
                }
                case 5: { // RSDKv1 StageConfig
                    stageConfigv1.palette.m_colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < palette.count())
                            stageConfigv1.palette.m_colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            stageConfigv1.palette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    stageConfigv1.write(filepath);
                    break;
                }
            }

            appConfig.addRecentFile(m_palType, TOOL_PALETTEDITOR, filepath, QList<QString>{});
            return true;
        }
    }
    if (event->type() == (QEvent::Type)RE_EVENT_SAVE_AS) {
        QList<QString> typesList = { "Adobe Colour Table Palettes (*.act)",
                                     "RSDKv4 GameConfig Palettes (*GameConfig*.bin)",
                                     "RSDKv4 StageConfig Palettes (*StageConfig*.bin)",
                                     "RSDKv3 StageConfig Palettes (*StageConfig*.bin)",
                                     "RSDKv2 StageConfig Palettes (*StageConfig*.bin)",
                                     "RSDKv1 StageConfig Palettes (*Zone*.zcf)" };
        QFileDialog filedialog(this, tr("Save Palette"),
                               QFile::exists(m_filename) ? QFileInfo(m_filename).absolutePath() : "",
                               tr(typesList[m_palType].toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            setStatus("Saving palette...");

            QString filepath = filedialog.selectedFiles()[0];
            switch (m_palType) {
                case 0: { //.act
                    Writer writer(filepath);
                    for (auto &c : palette) {
                        c.write(writer);
                    }
                    writer.flush();
                    break;
                }
                case 1: { // RSDKv4 GameConfig
                    m_gameconfigv4.m_masterPalette.m_colours.clear();
                    for (int c = 0; c < 96; ++c) {
                        if (c < palette.count())
                            m_gameconfigv4.m_masterPalette.m_colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            m_gameconfigv4.m_masterPalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    m_gameconfigv4.write(filepath);
                    break;
                }
                case 2: { // RSDKv4 StageConfig
                    stageConfigv4.m_stagePalette.m_colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < palette.count())
                            stageConfigv4.m_stagePalette.m_colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            stageConfigv4.m_stagePalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    stageConfigv4.write(filepath);
                    break;
                }
                case 3: { // RSDKv3 StageConfig
                    stageConfigv3.m_stagePalette.m_colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < palette.count())
                            stageConfigv3.m_stagePalette.m_colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            stageConfigv3.m_stagePalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    stageConfigv3.write(filepath);
                    break;
                }
                case 4: { // RSDKv2 StageConfig
                    stageConfigv2.m_stagePalette.m_colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < palette.count())
                            stageConfigv2.m_stagePalette.m_colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            stageConfigv2.m_stagePalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    stageConfigv2.write(filepath);
                    break;
                }
                case 5: { // RSDKv1 StageConfig
                    stageConfigv1.palette.m_colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < palette.count())
                            stageConfigv1.palette.m_colours.append(
                                Colour(palette[c].r, palette[c].g, palette[c].b));
                        else
                            stageConfigv1.palette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    stageConfigv1.write(filepath);
                    break;
                }
            }

            appConfig.addRecentFile(m_palType, TOOL_PALETTEDITOR, filepath, QList<QString>{});
            return true;
        }
    }
    return QWidget::event(event);
}

#include "moc_paletteeditor.cpp"
