#include "includes.hpp"
#include "ui_paletteeditor.h"

#include "paletteeditor.hpp"

#include "paletteeditor/colourdialog.hpp"
#include "paletteeditor/paletteimport.hpp"

#include "qgifimage.h"

PaletteEditor::PaletteEditor(QString path, byte type, bool external, QWidget *parent)
    : QDialog(parent), widget(new PaletteWidget(this)), ui(new Ui::PaletteEditor)
{
    ui->setupUi(this);

    externalWindow = external;
    InitEditor();

    if (QFile::exists(path) || external) {
        LoadPalette(path, type);
        filePath = path;
    }
}

PaletteEditor::PaletteEditor(Palette *stagePal, QWidget *parent)
    : QDialog(parent), widget(new PaletteWidget(this)), ui(new Ui::PaletteEditor)
{
    ui->setupUi(this);

    externalWindow = true;
    InitEditor();
    LoadScnEditorPal(stagePal);
}

PaletteEditor::PaletteEditor(RSDKv5::StageConfig *stagePal, QWidget *parent)
    : QDialog(parent), widget(new PaletteWidget(this)), ui(new Ui::PaletteEditor)
{
    ui->setupUi(this);

    externalWindow = true;
    InitEditor();
    Loadv5ScnEditorPal(stagePal);
}

PaletteEditor::~PaletteEditor() {}

void PaletteEditor::InitEditor()
{
    setWindowTitle("Palette Editor");
    widget->palette = &palette;

    if (firstInit) {
        QStringList types        = { "Adobe Color Table Palettes (*.act)",
                              "Image Files (*.gif *.png *.bmp)",
                              "rev02 (plus) RSDKv5 GameConfig Palettes (*GameConfig*.bin)",
                              "rev01 (pre-plus) RSDKv5 GameConfig Palettes (*GameConfig*.bin)",
                              "RSDKv5 StageConfig Palettes (*StageConfig*.bin)",
                              "RSDKv4 GameConfig Palettes (*GameConfig*.bin)",
                              "RSDKv4 StageConfig Palettes (*StageConfig*.bin)",
                              "RSDKv3 StageConfig Palettes (*StageConfig*.bin)",
                              "RSDKv2 StageConfig Palettes (*StageConfig*.bin)",
                              "RSDKv1 StageConfig Palettes (*Zone*.zcf)" };
        QList<QString> typesList = {
            "Adobe Color Table Palettes (*.act)",
            "rev02 (plus) RSDKv5 GameConfig Palettes (*GameConfig*.bin)",
            "rev01 (pre-plus) RSDKv5 GameConfig Palettes (*GameConfig*.bin)",
            "RSDKv5 StageConfig Palettes (*StageConfig*.bin)",
            "RSDKv4 GameConfig Palettes (*GameConfig*.bin)",
            "RSDKv4 StageConfig Palettes (*StageConfig*.bin)",
            "RSDKv3 StageConfig Palettes (*StageConfig*.bin)",
            "RSDKv2 StageConfig Palettes (*StageConfig*.bin)",
            "RSDKv1 StageConfig Palettes (*Zone*.zcf)",
            "Image Files (*.gif *.png *.bmp)"
        };

        connect(ui->importPal, &QPushButton::clicked, [=] {
            QFileDialog filedialog(this, tr("Import Palette"), "",
                                   tr(types.join(";;").toStdString().c_str()));
            filedialog.setAcceptMode(QFileDialog::AcceptOpen);
            if (filedialog.exec() == QDialog::Accepted) {
                int type         = typesList.indexOf(filedialog.selectedNameFilter());
                QString fileName = filedialog.selectedFiles()[0];

                // disconnect(ui->importPal, nullptr, nullptr, nullptr);
                ImportPalette(fileName, type);
                InitEditor();
            }
        });

        connect(ui->exportPal, &QPushButton::clicked, [=] {
            QFileDialog filedialog(this, tr("Export Palette"), "",
                                   tr("Adobe Color Table Palettes (*.act);;Replace GIF Palette (*.gif)"));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {
                QString filepath = filedialog.selectedFiles()[0];

                if (filepath.endsWith(".gif"))
                {
                    if (!QFile::exists(filepath))
                    {
                        QMessageBox::critical(this, "Palette Export Error", "File not found!\nPlease select the existing GIF sheet you wish to save the palette to.");
                        return;
                    }

                    FormatHelpers::Gif gif;
                    gif.read(filepath);

                    for (int i = 0; i < palette.count() && i < 256; i++)
                        gif.palette[i] = palette[i].toQColor().rgb();

                    gif.write(filepath);
                }
                else
                {
                    if (!CheckOverwrite(filepath, ".act", this))
                        return;

                    Writer writer(filepath);
                    for (auto &c : palette) c.write(writer);
                    writer.flush();
                }
            }
        });

        QToolButton *bankSwitches[] = { ui->bank1, ui->bank2, ui->bank3, ui->bank4,
                                        ui->bank5, ui->bank6, ui->bank7, ui->bank8 };

        for (int b = 0; b < 8; ++b) {
            connect(bankSwitches[b], &QToolButton::clicked, [this, b] { SwitchBank(b); });
        }
        connect(ui->palRows, QOverload<int>::of(&QSpinBox::valueChanged), [this](int r){ UpdatePaletteRows(r * 0x10); });

        ui->widgetLayout->addWidget(widget, 1);
    }

    firstInit = false;
}

void PaletteEditor::ReinitEditor() { palette.clear(); }

void PaletteEditor::LoadPalette(QString path, byte type)
{
    QToolButton *bankSwitches[] = { ui->bank1, ui->bank2, ui->bank3, ui->bank4,
                                    ui->bank5, ui->bank6, ui->bank7, ui->bank8 };
    for (int b = 0; b < 8; ++b) {
        bankSwitches[b]->setDown(false);
        bankSwitches[b]->setDisabled(true);
    }


    ui->palRows->setDisabled(true);
    palType = type;
    bankID  = 0;
    switch (type) {
        default: break;
        case PALTYPE_ACT: { //.act
            QList<PaletteColor> pal;
            Reader reader(path);
            pal.clear();
            while (!reader.isEOF()) {
                PaletteColor clr;
                clr.read(reader);
                pal.append(clr);
            }
            palette = pal;
            ui->palRows->setDisabled(false);
            ui->palRows->setValue(pal.count() / 16);
            break;
        }

        case PALTYPE_GAMECONFIGv5:
        case PALTYPE_GAMECONFIGv5_rev01:
        case PALTYPE_STAGECONFIGv5: {
            if (palType == PALTYPE_STAGECONFIGv5){
                stageConfigv5 = RSDKv5::StageConfig(path);
                configPalv5 = &stageConfigv5.palettes[bankID];
            }
            else{
                gameConfigv5 = RSDKv5::GameConfig(path, type == PALTYPE_GAMECONFIGv5_rev01);
                configPalv5 = &gameConfigv5.palettes[bankID];
            }

            SwitchBank(0);
            bankSwitches[0]->setDown(true);
            for (int b = 0; b < 8; ++b) bankSwitches[b]->setDisabled(false);
            ui->palRows->setValue(16);
            break;
        }

        case PALTYPE_GAMECONFIGv4:
        case PALTYPE_STAGECONFIGv4:
        case PALTYPE_STAGECONFIGv3:
        case PALTYPE_STAGECONFIGv2:
        case PALTYPE_STAGECONFIGv1: {
            Palette *configPal = nullptr;
            switch (palType) {
                case PALTYPE_GAMECONFIGv4:
                    gameConfigv4 = RSDKv4::GameConfig(path);
                    configPal    = &gameConfigv4.palette;
                    break;
                case PALTYPE_STAGECONFIGv4:
                    stageConfigv4 = RSDKv4::StageConfig(path);
                    configPal     = &stageConfigv4.palette;
                    break;
                case PALTYPE_STAGECONFIGv3:
                    stageConfigv3 = RSDKv3::StageConfig(path);
                    configPal     = &stageConfigv3.palette;
                    break;
                case PALTYPE_STAGECONFIGv2:
                    stageConfigv2 = RSDKv2::StageConfig(path);
                    configPal     = &stageConfigv2.palette;
                    break;
                case PALTYPE_STAGECONFIGv1:
                    stageConfigv1 = RSDKv1::StageConfig(path);
                    configPal     = &stageConfigv1.palette;
                    break;
            }

            palette.clear();
            for (auto &c : configPal->colors) {
                palette.append(PaletteColor(c.r, c.g, c.b));
            }
            ui->palRows->setValue(palette.count() / 16);
            break;
        }
    }

    ClearActions();
}


void PaletteEditor::LoadScnEditorPal(Palette *stagePal)
{
    QToolButton *bankSwitches[] = { ui->bank1, ui->bank2, ui->bank3, ui->bank4,
                                    ui->bank5, ui->bank6, ui->bank7, ui->bank8 };
    for (int b = 0; b < 8; ++b) {
        bankSwitches[b]->setDown(false);
        bankSwitches[b]->setDisabled(true);
    }

    ui->palRows->setDisabled(true);
    bankID  = 0;
    palType = PALTYPE_STAGECONFIGv4;
    palette.clear();
    for (auto &c : stagePal->colors) {
        palette.append(PaletteColor(c.r, c.g, c.b));
    }
    ui->palRows->setValue(palette.count() / 16);

    ClearActions();
}

void PaletteEditor::Loadv5ScnEditorPal(RSDKv5::StageConfig *stagePal)
{
    QToolButton *bankSwitches[] = { ui->bank1, ui->bank2, ui->bank3, ui->bank4,
                                    ui->bank5, ui->bank6, ui->bank7, ui->bank8 };
    for (int b = 0; b < 8; ++b) {
        bankSwitches[b]->setDown(false);
        bankSwitches[b]->setDisabled(true);
    }

    ui->palRows->setDisabled(true);
    palType = PALTYPE_STAGECONFIGv5;
    bankID  = 0;
    palette.clear();
    stageConfigv5 = *stagePal;
    configPalv5 = &stageConfigv5.palettes[bankID];

    SwitchBank(0);
    bankSwitches[0]->setDown(true);
    for (int b = 0; b < 8; ++b) bankSwitches[b]->setDisabled(false);
    ui->palRows->setValue(16);

    ClearActions();
}

void PaletteEditor::ImportPalette(QString path, byte type)
{
    QList<PaletteColor> backup = palette;
    switch (type) {
        case PALTYPE_ACT: { //.act
            QList<PaletteColor> pal;
            Reader reader(path);
            pal.clear();
            while (!reader.isEOF() && pal.count() < 256) {
                PaletteColor clr;
                clr.read(reader);
                pal.append(clr);
            }
            importFile = new PaletteImport(pal, palette, false);
            if (importFile->exec() != QDialog::Accepted) {
                palette = backup;
            } else {
                DoAction("Imported Palette", !externalWindow);
            };
            importFile = nullptr;
            break;
        }

        case PALTYPE_IMAGE: { // .gif, .png, .bmp
            QImage img;
            if (path.endsWith(".gif"))
            {
                QGifImage gif(path);
                img = gif.frame(0);
            }
            else
                img.load(path);

            if (img.format() != QImage::Format_Indexed8)
            {
                QMessageBox::critical(this, "Palette Import Error", "No palette found in image!\nPalettes can only be imported from indexed images.");
                return;
            }

            QList<PaletteColor> pal;
            pal.clear();
            for (auto& c : img.colorTable())
                pal.append(PaletteColor(c));

            importFile = new PaletteImport(pal, palette, false);
            if (importFile->exec() != QDialog::Accepted) {
                palette = backup;
            } else {
                DoAction("Imported Palette", !externalWindow);
            };
            importFile = nullptr;
            break;
        }
        case PALTYPE_GAMECONFIGv5:
        case PALTYPE_GAMECONFIGv5_rev01:
        case PALTYPE_STAGECONFIGv5: {
            RSDKv5::Palette *importPal = nullptr;
            RSDKv5::StageConfig importSC;
            RSDKv5::GameConfig importGC;
            if (type == PALTYPE_STAGECONFIGv5){
                importSC  = RSDKv5::StageConfig(path);
                importPal = &importSC.palettes[0];
            }
            else{
                importGC  = RSDKv5::GameConfig(path, type == PALTYPE_GAMECONFIGv5_rev01);
                importPal = &importGC.palettes[0];
            }


            QList<PaletteColor> pal;
            for (int r = 0; r < 16; ++r) {
                for (int c = 0; c < 16; ++c) {
                    pal.append(PaletteColor(importPal->colors[r][c].red(),
                                            importPal->colors[r][c].green(),
                                            importPal->colors[r][c].blue()));
                }
            }

            importFile = new PaletteImport(pal, palette, true);
            for (int b = 0; b < 8; b++){
                if (type == PALTYPE_STAGECONFIGv5){
                    importPal = &importSC.palettes[b];
                }
                else{
                    importPal = &importGC.palettes[b];
                }
                importFile->banks[b] = *importPal;
            }

            if (importFile->exec() != QDialog::Accepted) {
                palette = backup;
            } else {
                DoAction("Imported Palette", !externalWindow);
            };
            importFile = nullptr;
            break;
        }

        case PALTYPE_GAMECONFIGv4:
        case PALTYPE_STAGECONFIGv4:
        case PALTYPE_STAGECONFIGv3:
        case PALTYPE_STAGECONFIGv2:
        case PALTYPE_STAGECONFIGv1: {
            Palette *configPal = nullptr;
            RSDKv4::GameConfig importGCv4;
            RSDKv4::StageConfig importSCv4;
            RSDKv3::StageConfig importSCv3;
            RSDKv2::StageConfig importSCv2;
            RSDKv1::StageConfig importSCv1;

            switch (type) {
                case PALTYPE_GAMECONFIGv4: {
                    importGCv4 = RSDKv4::GameConfig(path);
                    configPal    = &importGCv4.palette;
                    break;
                }
                case PALTYPE_STAGECONFIGv4: {
                    importSCv4 = RSDKv4::StageConfig(path);
                    configPal     = &importSCv4.palette;
                    break;
                }
                case PALTYPE_STAGECONFIGv3: {
                    importSCv3 = RSDKv3::StageConfig(path);
                    configPal     = &importSCv3.palette;
                    break;
                }
                case PALTYPE_STAGECONFIGv2: {
                    importSCv2 = RSDKv2::StageConfig(path);
                    configPal     = &importSCv2.palette;
                    break;
                }
                case PALTYPE_STAGECONFIGv1: {
                    importSCv1 = RSDKv1::StageConfig(path);
                    configPal     = &importSCv1.palette;
                    break;
                }
            }

            QList<PaletteColor> pal;
            pal.clear();
            for (auto &c : configPal->colors) {
                pal.append(PaletteColor(c.r, c.g, c.b));
            }
            importFile = new PaletteImport(pal, palette, false);
            if (importFile->exec() != QDialog::Accepted) {
                palette = backup;
            } else {
                DoAction("Imported Palette", !externalWindow);
            };
            importFile = nullptr;
            break;
        }
    }
}

void PaletteEditor::SwitchBank(int id)
{
    bankID = id;

    QToolButton *bankSwitches[] = { ui->bank1, ui->bank2, ui->bank3, ui->bank4,
                                    ui->bank5, ui->bank6, ui->bank7, ui->bank8 };
    for (int b = 0; b < 8; ++b) {
        bankSwitches[b]->setDown(b == bankID);
    }

    switch (palType) {
        default: bankID = 0; break;
        case PALTYPE_GAMECONFIGv5:
        case PALTYPE_GAMECONFIGv5_rev01:
        case PALTYPE_STAGECONFIGv5: {
            // Store changes from the previous bank
            if (palette.count() > 0){
                for (int r = 0; r < 16; ++r) {
                    for (int c = 0; c < 16; ++c) {
                        PaletteColor pal = palette.takeAt(0);
                        configPalv5->colors[r][c] = QColor(pal.r, pal.g, pal.b);
                    }
                }
            }

            palette.clear();

            if (palType == PALTYPE_STAGECONFIGv5)
                configPalv5 = &stageConfigv5.palettes[bankID];
            else
                configPalv5 = &gameConfigv5.palettes[bankID];

            for (int r = 0; r < 16; ++r) {
                    for (int c = 0; c < 16; ++c) {
                        palette.append(PaletteColor(configPalv5->colors[r][c].red(),
                                                    configPalv5->colors[r][c].green(),
                                                    configPalv5->colors[r][c].blue()));
                    }
            }
            break;
        }
    }

    update();
}

void PaletteEditor::UpdatePaletteRows(int rows)
{
    if (rows > palette.count()){
        for (int c = palette.count(); c < rows; c++){
            palette.append(PaletteColor(0xFF, 0x00, 0xFF));
        }
    } else if (rows < palette.count()) {
        for (int c = palette.count(); c > rows; c--){
            palette.removeAt(c - 1);
        }

    }
    update();
}

void PaletteEditor::UndoAction()
{
    if (actionIndex > 0) {
        // setStatus("Undid Action: " + actions[actionIndex].name);
        actionIndex--;
        ResetAction();
    }
}
void PaletteEditor::RedoAction()
{
    if (actionIndex + 1 < actions.count()) {
        // setStatus("Redid Action: " + actions[actionIndex].name);
        actionIndex++;
        ResetAction();
    }
}
void PaletteEditor::ResetAction()
{
#if RE_USE_UNSTABLE
    palette       = actions[actionIndex].palette;
    stageConfigv1 = actions[actionIndex].stageConfigv1;
    stageConfigv2 = actions[actionIndex].stageConfigv2;
    stageConfigv3 = actions[actionIndex].stageConfigv3;
    stageConfigv4 = actions[actionIndex].stageConfigv4;
    stageConfigv5 = actions[actionIndex].stageConfigv5;

    gameConfigv4 = actions[actionIndex].gameConfigv4;
    gameConfigv5 = actions[actionIndex].gameConfigv5;

    update();
#endif

    UpdateTitle(actionIndex > 0);
}
void PaletteEditor::DoAction(QString name, bool setModified)
{
    ActionState action;

    action.name = name;

#if RE_USE_UNSTABLE
    action.palette       = palette;
    action.stageConfigv1 = stageConfigv1;
    action.stageConfigv2 = stageConfigv2;
    action.stageConfigv3 = stageConfigv3;
    action.stageConfigv4 = stageConfigv4;
    action.stageConfigv5 = stageConfigv5;

    action.gameConfigv4 = gameConfigv4;
    action.gameConfigv5 = gameConfigv5;
#endif

    // Actions
    for (int i = actions.count() - 1; i > actionIndex; --i) {
        actions.removeAt(i);
    }

    actions.append(action);
    actionIndex = actions.count() - 1;

    UpdateTitle(setModified);

    // setStatus("Did Action: " + name);
}
void PaletteEditor::ClearActions()
{
    actions.clear();
    actionIndex = 0;
    DoAction("Action Setup", false); // first action, cant be undone
}

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
    if (selection >= palette->count())
        return;

    PaletteEditor *editor = (PaletteEditor *)parent();

    PaletteColor prev    = palette->at(selection);
    RSDKColorDialog *dlg = new RSDKColorDialog(palette->at(selection));
    if (dlg->exec() == QDialog::Accepted) {
        PaletteColor clr;
        clr.r = dlg->color().r;
        clr.g = dlg->color().g;
        clr.b = dlg->color().b;
        palette->replace(selection, clr);

        if (prev.r != clr.r || prev.g != clr.g || prev.b != clr.b)
            editor->DoAction("Changed color", !editor->externalWindow);
    }
    delete dlg;

    // QColorDialog *dlg = new QColorDialog(
    //     QColor(palette->at(selection).r, palette->at(selection).g, palette->at(selection).b));
    // if (dlg->exec() == QDialog::Accepted) {
    //     PaletteColor clr;
    //     clr.r = dlg->currentColor().red();
    //     clr.g = dlg->currentColor().green();
    //     clr.b = dlg->currentColor().blue();
    //     palette->replace(selection, clr);
    //
    //     if (prev.r != clr.r && prev.g != clr.g && prev.b != clr.b)
    //         editor->DoAction("Changed color", true);
    // }
    // delete dlg;
}

void PaletteWidget::mouseReleaseEvent(QMouseEvent *) { pressed = false; }

void PaletteWidget::mouseMoveEvent(QMouseEvent *event)
{
    byte x = floor((float)(event->x() / ((qreal)width() / 16)));
    byte y = floor((float)(event->y() / ((qreal)height() / 16)));

    if (y > 15)
        y = 15;

    highlight = x % 16 + y * 16;

    PaletteEditor *editor = (PaletteEditor *)parent();
    if (editor->palType == PALTYPE_GAMECONFIGv5 || editor->palType == PALTYPE_GAMECONFIGv5_rev01) {
        if (pressed) {
            bool prev = editor->gameConfigv5.palettes[editor->bankID].activeRows[y];
            editor->gameConfigv5.palettes[editor->bankID].activeRows[y] = enabling;

            if (prev != enabling)
                editor->DoAction("Changed row active", !editor->externalWindow);
        }
    }
    else if (editor->palType == PALTYPE_STAGECONFIGv5) {
        if (pressed) {
            bool prev = editor->stageConfigv5.palettes[editor->bankID].activeRows[y];
            editor->stageConfigv5.palettes[editor->bankID].activeRows[y] = enabling;

            if (prev != enabling)
                editor->DoAction("Changed row active", !editor->externalWindow);
        }
    }

    update();
}

void PaletteWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    PaletteEditor *editor = (PaletteEditor *)parent();

    QRectF rect(0, 0, (qreal)width() / 16, (qreal)height() / 16);
    short index = -1;
    if (!palette)
        return;

    for (byte y = 0; y < palette->count() / 0x10; ++y) {
        for (byte x = 0; x < 0x10; ++x) {
            ++index;
            if (index >= palette->count())
                return;
            QPen pen(qApp->palette().base(), 2);
            p.setPen(pen);
            PaletteColor clr = palette->at(index);
            p.setBrush(clr.toQColor());

            if (editor->palType == PALTYPE_GAMECONFIGv5
                || editor->palType == PALTYPE_GAMECONFIGv5_rev01) {
                if (!editor->gameConfigv5.palettes[editor->bankID].activeRows[y])
                    p.setBrush(p.brush().color().darker(255));
            }
            else if (editor->palType == PALTYPE_STAGECONFIGv5) {
                if (!editor->stageConfigv5.palettes[editor->bankID].activeRows[y])
                    p.setBrush(p.brush().color().darker(255));
            }

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
    QStringList types        = { "Adobe Color Table Palettes (*.act)",
                          "rev02 (plus) RSDKv5 GameConfig Palettes (*GameConfig*.bin)",
                          "rev01 (pre-plus) RSDKv5 GameConfig Palettes (*GameConfig*.bin)",
                          "RSDKv5 StageConfig Palettes (*StageConfig*.bin)",
                          "RSDKv4 GameConfig Palettes (*GameConfig*.bin)",
                          "RSDKv4 StageConfig Palettes (*StageConfig*.bin)",
                          "RSDKv3 StageConfig Palettes (*StageConfig*.bin)",
                          "RSDKv2 StageConfig Palettes (*StageConfig*.bin)",
                          "RSDKv1 StageConfig Palettes (*Zone*.zcf)" };
    QList<QString> typesList = { "Adobe Color Table Palettes (*.act)",
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
        ReinitEditor();

        QToolButton *bankSwitches[] = { ui->bank1, ui->bank2, ui->bank3, ui->bank4,
                                        ui->bank5, ui->bank6, ui->bank7, ui->bank8 };
        for (int b = 0; b < 8; ++b) {
            bankSwitches[b]->setDown(false);
            bankSwitches[b]->setDisabled(true);
        }
        palType = PALTYPE_ACT;
        QList<PaletteColor> pal;
        PaletteColor clr(0xFF, 0, 0xFF);
        for (int s = 0; s < 256; ++s)
            pal.append(clr);
        palette = pal;
        ui->palRows->setDisabled(false);
        ui->palRows->setValue(16);
        tabTitle = "New Palette";
        tabPath  = "";
        filePath = "";

        ClearActions();
        return true;
    }

    if (event->type() == (QEvent::Type)RE_EVENT_OPEN) {
        QFileDialog filedialog(this, tr("Open Palette"), "",
                               tr(types.join(";;").toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            ReinitEditor();

            SetStatus("Loading palette...", true);
            filePath         = filedialog.selectedFiles()[0];
            tabTitle         = Utils::getFilenameAndFolder(filePath);
            tabPath          = filedialog.selectedFiles()[0];
            int type         = typesList.indexOf(filedialog.selectedNameFilter());
            LoadPalette(filePath, type);
            SetStatus("Loaded palette from " + tabTitle);

            appConfig.addRecentFile(palType, TOOL_PALETTEDITOR, filePath, QList<QString>{});
            ClearActions();
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE) {
        if (!QFile::exists(filePath)) {
            QFileDialog filedialog(this, tr("Save Palette"), "",
                                   tr(typesList[palType].toStdString().c_str()));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {
                int type         = typesList.indexOf(filedialog.selectedNameFilter());
                QString filepath = filedialog.selectedFiles()[0];
                QString extension;
                switch (type){
                    default: extension = ".bin"; break;
                    case 0: extension  = ".act"; break;
                    case 8: extension  = ".zcf"; break;
                }

                if (!CheckOverwrite(filepath, extension, this))
                    return false;

                SavePalette(filepath);
                tabPath = filepath;

                ClearActions();
                appConfig.addRecentFile(palType, TOOL_PALETTEDITOR, filepath, QList<QString>{});
                return true;
            }
        }
        else {
            QString filepath = filePath;
            SavePalette(filepath);
            tabPath = filepath;

            ClearActions();
            appConfig.addRecentFile(palType, TOOL_PALETTEDITOR, filepath, QList<QString>{});
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE_AS) {
        QList<QString> typesList = { "Adobe Color Table Palettes (*.act)",
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
            int type         = typesList.indexOf(filedialog.selectedNameFilter());

            QString filepath = filedialog.selectedFiles()[0];
            QString extension;
            switch (type){
                default: extension = ".bin"; break;
                case 0: extension  = ".act"; break;
                case 8: extension  = ".zcf"; break;
            }

            if (!CheckOverwrite(filepath, extension, this))
                return false;
            SavePalette(filepath);
            tabPath = filepath;

            ClearActions();
            appConfig.addRecentFile(palType, TOOL_PALETTEDITOR, filepath, QList<QString>{});
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_UNDO) {
        UndoAction();
        return true;
    }
    if (event->type() == (QEvent::Type)RE_EVENT_REDO) {
        RedoAction();
        return true;
    }

    switch (event->type()) {
        default: break;
        case QEvent::Close:
            if (modified) {
                bool cancelled = false;
                if (MainWindow::ShowCloseWarning(this, &cancelled)) {
                    if (!QFile::exists(filePath)) {
                        QFileDialog filedialog(this, tr("Save Palette"), "",
                                               tr(typesList[palType].toStdString().c_str()));
                        filedialog.setAcceptMode(QFileDialog::AcceptSave);
                        if (filedialog.exec() == QDialog::Accepted) {
                            int type         = typesList.indexOf(filedialog.selectedNameFilter());

                            QString filepath = filedialog.selectedFiles()[0];
                            QString extension;
                            switch (type){
                            default: extension = ".bin"; break;
                            case 0: extension  = ".act"; break;
                            case 8: extension  = ".zcf"; break;
                            }

                            if (!CheckOverwrite(filepath, extension, this))
                                return false;

                            SavePalette(filepath);

                            ClearActions();
                            appConfig.addRecentFile(palType, TOOL_PALETTEDITOR, filepath,
                                                    QList<QString>{});
                            return true;
                        }
                    }
                    else {
                        SavePalette(filePath);

                        ClearActions();
                        appConfig.addRecentFile(palType, TOOL_PALETTEDITOR, filePath, QList<QString>{});
                        return true;
                    }
                }
                else if (cancelled) {
                    event->ignore();
                    return true;
                }
            }
            break;
    }

    return QWidget::event(event);
}

void PaletteEditor::SavePalette(QString filepath)
{
    SetStatus("Saving palette...", true);
    switch (palType) {
        case PALTYPE_ACT: {
            float total = palette.count();

            Writer writer(filepath);
            for (auto &c : palette) {
                c.write(writer);

                AddStatusProgress(1.0 / total);
            }

            writer.flush();
            break;
        }

        case PALTYPE_GAMECONFIGv5:
        case PALTYPE_GAMECONFIGv5_rev01:
        case PALTYPE_STAGECONFIGv5: {
            if (palType == PALTYPE_STAGECONFIGv5)
                configPalv5 = &stageConfigv5.palettes[bankID];
            else
                configPalv5 = &gameConfigv5.palettes[bankID];

            for (int r = 0; r < 16; ++r) {
                if (configPalv5->activeRows[r]) {
                    for (int c = 0; c < 16; ++c) {
                        for (int c = 0; c < 16; ++c) {
                            configPalv5->colors[r][c] =
                                QColor(palette[(r << 4) + c].r, palette[(r << 4) + c].g,
                                       palette[(r << 4) + c].b);

                            AddStatusProgress(1.0 / 256);
                        }
                    }
                }
                else {
                    for (int c = 0; c < 16; ++c) configPalv5->colors[r][c] = QColor(0xFF, 0x00, 0xFF);

                    AddStatusProgress(16.0 / 256);
                }
            }

            if (palType == PALTYPE_STAGECONFIGv5) {
                stageConfigv5.write(filepath);
            }
            else {
                gameConfigv5.readFilter = palType != PALTYPE_GAMECONFIGv5_rev01;
                gameConfigv5.write(filepath);
            }
            break;
        }

        case PALTYPE_GAMECONFIGv4:
        case PALTYPE_STAGECONFIGv4:
        case PALTYPE_STAGECONFIGv3:
        case PALTYPE_STAGECONFIGv2:
        case PALTYPE_STAGECONFIGv1: {
            Palette *configPal = nullptr;

            switch (palType) {
                case PALTYPE_GAMECONFIGv4: configPal = &gameConfigv4.palette; break;
                case PALTYPE_STAGECONFIGv4: configPal = &stageConfigv4.palette; break;
                case PALTYPE_STAGECONFIGv3: configPal = &stageConfigv3.palette; break;
                case PALTYPE_STAGECONFIGv2: configPal = &stageConfigv2.palette; break;
                case PALTYPE_STAGECONFIGv1: configPal = &stageConfigv1.palette; break;
            }

            configPal->colors.clear();
            int colorCount = palType == PALTYPE_GAMECONFIGv4 ? 96 : 32;
            for (int c = 0; c < colorCount; ++c) {
                if (c < palette.count())
                    configPal->colors.append(Color(palette[c].r, palette[c].g, palette[c].b));
                else
                    configPal->colors.append(Color(0xFF, 0x00, 0xFF));

                AddStatusProgress(1.0 / colorCount);
            }

            switch (palType) {
                case PALTYPE_GAMECONFIGv4: gameConfigv4.write(filepath); break;
                case PALTYPE_STAGECONFIGv4: stageConfigv4.write(filepath); break;
                case PALTYPE_STAGECONFIGv3: stageConfigv3.write(filepath); break;
                case PALTYPE_STAGECONFIGv2: stageConfigv2.write(filepath); break;
                case PALTYPE_STAGECONFIGv1: stageConfigv1.write(filepath); break;
            }

            break;
        }
    }

    SetStatus("Saved Palette to " + filepath);
}

#include "moc_paletteeditor.cpp"
