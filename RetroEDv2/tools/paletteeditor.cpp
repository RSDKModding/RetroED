#include "includes.hpp"
#include "ui_paletteeditor.h"

PaletteEditor::PaletteEditor(QString filepath, byte type, QWidget *parent)
    : QWidget(parent), m_widget(new PaletteWidget(this)), ui(new Ui::PaletteEditor)
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
                m_palette = pal;
                break;
            }
            case 1: { // RSDKv4 GameConfig
                m_gameconfigv4 = RSDKv4::Gameconfig(filepath);
                m_palette.clear();
                for (auto &c : m_gameconfigv4.m_masterPalette.m_colours) {
                    m_palette.append(PaletteColour(c.m_r, c.m_g, c.m_b));
                }
                break;
            }
            case 2: { // RSDKv4 StageConfig
                m_stageconfigv4 = RSDKv4::Stageconfig(filepath);
                m_palette.clear();
                for (auto &c : m_stageconfigv4.m_stagePalette.m_colours) {
                    m_palette.append(PaletteColour(c.m_r, c.m_g, c.m_b));
                }
                break;
            }
            case 3: { // RSDKv3 StageConfig
                m_stageconfigv3 = RSDKv3::Stageconfig(filepath);
                m_palette.clear();
                for (auto &c : m_stageconfigv3.m_stagePalette.m_colours) {
                    m_palette.append(PaletteColour(c.m_r, c.m_g, c.m_b));
                }
                break;
            }
            case 4: { // RSDKv2 StageConfig
                m_stageconfigv2 = RSDKv2::Stageconfig(filepath);
                m_palette.clear();
                for (auto &c : m_stageconfigv2.m_stagePalette.m_colours) {
                    m_palette.append(PaletteColour(c.m_r, c.m_g, c.m_b));
                }
                break;
            }
            case 5: { // RSDKv1 StageConfig
                m_stageconfigv1 = RSDKv1::Stageconfig(filepath);
                m_palette.clear();
                for (auto &c : m_stageconfigv1.m_stagePalette.m_colours) {
                    m_palette.append(PaletteColour(c.m_r, c.m_g, c.m_b));
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
    m_widget->m_palette = &m_palette;

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
                    m_palette = pal;
                    break;
                }
                case 1: { // RSDKv4 GameConfig
                    RSDKv4::Gameconfig config(fileName);
                    m_palette.clear();
                    for (auto &c : config.m_masterPalette.m_colours) {
                        m_palette.append(PaletteColour(c.m_r, c.m_g, c.m_b));
                    }
                    break;
                }
                case 2: { // RSDKv4 StageConfig
                    RSDKv4::Stageconfig config(fileName);
                    m_palette.clear();
                    for (auto &c : config.m_stagePalette.m_colours) {
                        m_palette.append(PaletteColour(c.m_r, c.m_g, c.m_b));
                    }
                    break;
                }
                case 3: { // RSDKv3 StageConfig
                    RSDKv3::Stageconfig config(fileName);
                    m_palette.clear();
                    for (auto &c : config.m_stagePalette.m_colours) {
                        m_palette.append(PaletteColour(c.m_r, c.m_g, c.m_b));
                    }
                    break;
                }
                case 4: { // RSDKv2 StageConfig
                    RSDKv2::Stageconfig config(fileName);
                    m_palette.clear();
                    for (auto &c : config.m_stagePalette.m_colours) {
                        m_palette.append(PaletteColour(c.m_r, c.m_g, c.m_b));
                    }
                    break;
                }
                case 5: { // RSDKv1 StageConfig
                    RSDKv1::Stageconfig config(fileName);
                    m_palette.clear();
                    for (auto &c : config.m_stagePalette.m_colours) {
                        m_palette.append(PaletteColour(c.m_r, c.m_g, c.m_b));
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
            for (auto &c : m_palette) c.write(writer);
            writer.flush();
        }
    });

    if (m_firstInit) {
        ui->widgetLayout->addWidget(m_widget, 1);
    }

    m_firstInit = false;
}

void PaletteEditor::reinit() { m_palette.clear(); }

PaletteWidget::PaletteWidget(QWidget *parent) : QWidget(parent) { setMouseTracking(true); }

void PaletteWidget::leaveEvent(QEvent *)
{
    m_highlight = -1;
    update();
}

void PaletteWidget::mousePressEvent(QMouseEvent *event)
{
    m_selection = m_highlight;
    m_enabling  = (event->button() & Qt::LeftButton) == Qt::LeftButton;
    m_pressed   = true;
}

void PaletteWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    ColourDialog dlg(m_palette->at(m_selection));
    if (dlg.exec() == QDialog::Accepted) {
        PaletteColour clr;
        clr.r = dlg.colour().r;
        clr.g = dlg.colour().g;
        clr.b = dlg.colour().b;
        m_palette->replace(m_selection, clr);
    }
}

void PaletteWidget::mouseReleaseEvent(QMouseEvent *) { m_pressed = false; }

void PaletteWidget::mouseMoveEvent(QMouseEvent *event)
{
    byte x = floor((float)(event->x() / ((qreal)width() / 16)));
    byte y = floor((float)(event->y() / ((qreal)height() / 16)));

    if (y > 15)
        y = 15;

    m_highlight = x % 16 + y * 16;

    update();
}

void PaletteWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    QRectF rect(0, 0, (qreal)width() / 16, (qreal)height() / 16);
    short index = -1;
    if (!m_palette)
        return;

    for (byte y = 0; y < m_palette->count() / 0x10; ++y) {
        for (byte x = 0; x < 0x10; ++x) {
            ++index;
            if (index >= m_palette->count())
                return;
            // if (index >= m_palette->m_pal.count()) continue;
            QPen pen(qApp->palette().base(), 2);
            p.setPen(pen);
            PaletteColour clr = m_palette->at(index);
            p.setBrush(clr.toQColor());

            p.drawRect(rect.translated(x * (qreal)width() / 16, y * (qreal)height() / 16));

            if (m_highlight == index) {
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
        m_palette.clear();
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
                    m_palette = pal;
                    break;
                }
                case 1: { // RSDKv4 GameConfig
                    m_gameconfigv4 = RSDKv4::Gameconfig(filepath);
                    m_palette.clear();
                    for (auto &c : m_gameconfigv4.m_masterPalette.m_colours) {
                        m_palette.append(PaletteColour(c.m_r, c.m_g, c.m_b));
                    }
                    break;
                }
                case 2: { // RSDKv4 StageConfig
                    m_stageconfigv4 = RSDKv4::Stageconfig(filepath);
                    m_palette.clear();
                    for (auto &c : m_stageconfigv4.m_stagePalette.m_colours) {
                        m_palette.append(PaletteColour(c.m_r, c.m_g, c.m_b));
                    }
                    break;
                }
                case 3: { // RSDKv3 StageConfig
                    m_stageconfigv3 = RSDKv3::Stageconfig(filepath);
                    m_palette.clear();
                    for (auto &c : m_stageconfigv3.m_stagePalette.m_colours) {
                        m_palette.append(PaletteColour(c.m_r, c.m_g, c.m_b));
                    }
                    break;
                }
                case 4: { // RSDKv2 StageConfig
                    m_stageconfigv2 = RSDKv2::Stageconfig(filepath);
                    m_palette.clear();
                    for (auto &c : m_stageconfigv2.m_stagePalette.m_colours) {
                        m_palette.append(PaletteColour(c.m_r, c.m_g, c.m_b));
                    }
                    break;
                }
                case 5: { // RSDKv1 StageConfig
                    m_stageconfigv1 = RSDKv1::Stageconfig(filepath);
                    m_palette.clear();
                    for (auto &c : m_stageconfigv1.m_stagePalette.m_colours) {
                        m_palette.append(PaletteColour(c.m_r, c.m_g, c.m_b));
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
                        for (auto &c : m_palette) {
                            c.write(writer);
                        }
                        writer.flush();
                        break;
                    }
                    case 1: { // RSDKv4 GameConfig
                        m_gameconfigv4.m_masterPalette.m_colours.clear();
                        for (int c = 0; c < 96; ++c) {
                            if (c < m_palette.count())
                                m_gameconfigv4.m_masterPalette.m_colours.append(
                                    Colour(m_palette[c].r, m_palette[c].g, m_palette[c].b));
                            else
                                m_gameconfigv4.m_masterPalette.m_colours.append(
                                    Colour(0xFF, 0x00, 0xFF));
                        }
                        m_gameconfigv4.write(filepath);
                        break;
                    }
                    case 2: { // RSDKv4 StageConfig
                        m_stageconfigv4.m_stagePalette.m_colours.clear();
                        for (int c = 0; c < 32; ++c) {
                            if (c < m_palette.count())
                                m_stageconfigv4.m_stagePalette.m_colours.append(
                                    Colour(m_palette[c].r, m_palette[c].g, m_palette[c].b));
                            else
                                m_stageconfigv4.m_stagePalette.m_colours.append(
                                    Colour(0xFF, 0x00, 0xFF));
                        }
                        m_stageconfigv4.write(filepath);
                        break;
                    }
                    case 3: { // RSDKv3 StageConfig
                        m_stageconfigv3.m_stagePalette.m_colours.clear();
                        for (int c = 0; c < 32; ++c) {
                            if (c < m_palette.count())
                                m_stageconfigv3.m_stagePalette.m_colours.append(
                                    Colour(m_palette[c].r, m_palette[c].g, m_palette[c].b));
                            else
                                m_stageconfigv3.m_stagePalette.m_colours.append(
                                    Colour(0xFF, 0x00, 0xFF));
                        }
                        m_stageconfigv3.write(filepath);
                        break;
                    }
                    case 4: { // RSDKv2 StageConfig
                        m_stageconfigv2.m_stagePalette.m_colours.clear();
                        for (int c = 0; c < 32; ++c) {
                            if (c < m_palette.count())
                                m_stageconfigv2.m_stagePalette.m_colours.append(
                                    Colour(m_palette[c].r, m_palette[c].g, m_palette[c].b));
                            else
                                m_stageconfigv2.m_stagePalette.m_colours.append(
                                    Colour(0xFF, 0x00, 0xFF));
                        }
                        m_stageconfigv2.write(filepath);
                        break;
                    }
                    case 5: { // RSDKv1 StageConfig
                        m_stageconfigv1.m_stagePalette.m_colours.clear();
                        for (int c = 0; c < 32; ++c) {
                            if (c < m_palette.count())
                                m_stageconfigv1.m_stagePalette.m_colours.append(
                                    Colour(m_palette[c].r, m_palette[c].g, m_palette[c].b));
                            else
                                m_stageconfigv1.m_stagePalette.m_colours.append(
                                    Colour(0xFF, 0x00, 0xFF));
                        }
                        m_stageconfigv1.write(filepath);
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
                    for (auto &c : m_palette) {
                        c.write(writer);
                    }
                    writer.flush();
                    break;
                }
                case 1: { // RSDKv4 GameConfig
                    m_gameconfigv4.m_masterPalette.m_colours.clear();
                    for (int c = 0; c < 96; ++c) {
                        if (c < m_palette.count())
                            m_gameconfigv4.m_masterPalette.m_colours.append(
                                Colour(m_palette[c].r, m_palette[c].g, m_palette[c].b));
                        else
                            m_gameconfigv4.m_masterPalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    m_gameconfigv4.write(filepath);
                    break;
                }
                case 2: { // RSDKv4 StageConfig
                    m_stageconfigv4.m_stagePalette.m_colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < m_palette.count())
                            m_stageconfigv4.m_stagePalette.m_colours.append(
                                Colour(m_palette[c].r, m_palette[c].g, m_palette[c].b));
                        else
                            m_stageconfigv4.m_stagePalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    m_stageconfigv4.write(filepath);
                    break;
                }
                case 3: { // RSDKv3 StageConfig
                    m_stageconfigv3.m_stagePalette.m_colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < m_palette.count())
                            m_stageconfigv3.m_stagePalette.m_colours.append(
                                Colour(m_palette[c].r, m_palette[c].g, m_palette[c].b));
                        else
                            m_stageconfigv3.m_stagePalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    m_stageconfigv3.write(filepath);
                    break;
                }
                case 4: { // RSDKv2 StageConfig
                    m_stageconfigv2.m_stagePalette.m_colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < m_palette.count())
                            m_stageconfigv2.m_stagePalette.m_colours.append(
                                Colour(m_palette[c].r, m_palette[c].g, m_palette[c].b));
                        else
                            m_stageconfigv2.m_stagePalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    m_stageconfigv2.write(filepath);
                    break;
                }
                case 5: { // RSDKv1 StageConfig
                    m_stageconfigv1.m_stagePalette.m_colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < m_palette.count())
                            m_stageconfigv1.m_stagePalette.m_colours.append(
                                Colour(m_palette[c].r, m_palette[c].g, m_palette[c].b));
                        else
                            m_stageconfigv1.m_stagePalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    m_stageconfigv1.write(filepath);
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
                    for (auto &c : m_palette) {
                        c.write(writer);
                    }
                    writer.flush();
                    break;
                }
                case 1: { // RSDKv4 GameConfig
                    m_gameconfigv4.m_masterPalette.m_colours.clear();
                    for (int c = 0; c < 96; ++c) {
                        if (c < m_palette.count())
                            m_gameconfigv4.m_masterPalette.m_colours.append(
                                Colour(m_palette[c].r, m_palette[c].g, m_palette[c].b));
                        else
                            m_gameconfigv4.m_masterPalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    m_gameconfigv4.write(filepath);
                    break;
                }
                case 2: { // RSDKv4 StageConfig
                    m_stageconfigv4.m_stagePalette.m_colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < m_palette.count())
                            m_stageconfigv4.m_stagePalette.m_colours.append(
                                Colour(m_palette[c].r, m_palette[c].g, m_palette[c].b));
                        else
                            m_stageconfigv4.m_stagePalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    m_stageconfigv4.write(filepath);
                    break;
                }
                case 3: { // RSDKv3 StageConfig
                    m_stageconfigv3.m_stagePalette.m_colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < m_palette.count())
                            m_stageconfigv3.m_stagePalette.m_colours.append(
                                Colour(m_palette[c].r, m_palette[c].g, m_palette[c].b));
                        else
                            m_stageconfigv3.m_stagePalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    m_stageconfigv3.write(filepath);
                    break;
                }
                case 4: { // RSDKv2 StageConfig
                    m_stageconfigv2.m_stagePalette.m_colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < m_palette.count())
                            m_stageconfigv2.m_stagePalette.m_colours.append(
                                Colour(m_palette[c].r, m_palette[c].g, m_palette[c].b));
                        else
                            m_stageconfigv2.m_stagePalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    m_stageconfigv2.write(filepath);
                    break;
                }
                case 5: { // RSDKv1 StageConfig
                    m_stageconfigv1.m_stagePalette.m_colours.clear();
                    for (int c = 0; c < 32; ++c) {
                        if (c < m_palette.count())
                            m_stageconfigv1.m_stagePalette.m_colours.append(
                                Colour(m_palette[c].r, m_palette[c].g, m_palette[c].b));
                        else
                            m_stageconfigv1.m_stagePalette.m_colours.append(Colour(0xFF, 0x00, 0xFF));
                    }
                    m_stageconfigv1.write(filepath);
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
