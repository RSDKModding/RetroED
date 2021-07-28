#include "includes.hpp"
#include "ui_savefileeditorv4.h"

SaveFileEditorv4::SaveFileEditorv4(QString filePath, QWidget *parent)
    : QWidget(parent), ui(new Ui::SaveFileEditorv4)
{
    ui->setupUi(this);

    if (QFile::exists(filePath)) {
        savefile.read(filePath);
        appConfig.addRecentFile(ENGINE_v4, TOOL_SAVEEDITOR, filePath, QList<QString>{});
    }

    setupUI();
}

SaveFileEditorv4::~SaveFileEditorv4() { delete ui; }

void SaveFileEditorv4::setupUI()
{
    QCheckBox *tsBoxes[7] = { ui->e1, ui->e2, ui->e3, ui->e4, ui->e5, ui->e6, ui->e7 };

    // Save Slots (x4) (0-8, 8-15, etc)
    // CharacterID:         SaveRAM[0]
    // Lives: 				SaveRAM[1]
    // Score: 				SaveRAM[2]
    // Score Bonus:  		SaveRAM[3]
    // Continues:           SaveRAM[4]
    // ZoneID:              SaveRAM[5]
    // Emeralds:            SaveRAM[6]
    // SpecialZoneID:       SaveRAM[7]

    // NewSave:             SaveRAM[32]
    // MusVolume: 			SaveRAM[33]
    // SFXVolume: 			SaveRAM[34]
    // Spindash Enabled:    SaveRAM[35]
    // Box Region: 			SaveRAM[36]
    // VDPadSize:           SaveRAM[37]
    // VDPadOpacity:        SaveRAM[38]
    // VDPadX               SaveRAM[39]
    // VDPadY               SaveRAM[40]
    // VDPadX2              SaveRAM[41]
    // VDPadY2              SaveRAM[42]
    // Tails Unlocked:      SaveRAM[43]
    // Knux Unlocked:       SaveRAM[44]
    // ???:                 SaveRAM[45]

    //[Unused]:             SaveRAM[46-63]

    // TA Records:          SaveRAM[64 - 138]

    QList<QString> stageNames = {
        "Palmtree Panic 1",   "Palmtree Panic 2",   "Collision Chaos 1",   "Collision Chaos 2",
        "Tidal Tempest 1",    "Tidal Tempest 2",    "Quartz Quadrant 1",   "Quartz Quadrant 2",
        "Wacky Workbench 1",  "Wacky Workbench 2",  "Stardust Speedway 1", "Stardust Speedway 2",
        "Metallic Madness 1", "Metallic Madness 2",
    };

    disconnect(ui->curSave, nullptr, nullptr, nullptr);
    disconnect(ui->characterID, nullptr, nullptr, nullptr);
    disconnect(ui->lives, nullptr, nullptr, nullptr);
    disconnect(ui->score, nullptr, nullptr, nullptr);
    disconnect(ui->curStage, nullptr, nullptr, nullptr);
    disconnect(ui->curSpecialStage, nullptr, nullptr, nullptr);
    for (int t = 0; t < 7; ++t) disconnect(tsBoxes[t], nullptr, nullptr, nullptr);
    disconnect(ui->scoreBonus, nullptr, nullptr, nullptr);

    disconnect(ui->activeFile, nullptr, nullptr, nullptr);
    disconnect(ui->musVol, nullptr, nullptr, nullptr);
    disconnect(ui->sfxVol, nullptr, nullptr, nullptr);
    disconnect(ui->ogCtrls, nullptr, nullptr, nullptr);
    disconnect(ui->tailsUnlocked, nullptr, nullptr, nullptr);
    disconnect(ui->knuxUnlocked, nullptr, nullptr, nullptr);
    disconnect(ui->boxArtRegion, nullptr, nullptr, nullptr);
    disconnect(ui->vDpadSize, nullptr, nullptr, nullptr);
    disconnect(ui->vDpadOpacity, nullptr, nullptr, nullptr);
    disconnect(ui->vDPadX1, nullptr, nullptr, nullptr);
    disconnect(ui->vDPadY1, nullptr, nullptr, nullptr);
    disconnect(ui->vDPadX2, nullptr, nullptr, nullptr);
    disconnect(ui->vDPadY2, nullptr, nullptr, nullptr);

    saveID = 0 << 3;
    ui->curSave->setCurrentIndex(0);

    int stageID = savefile.saveRAM[saveID + 5];

    ui->characterID->setCurrentIndex(savefile.saveRAM[saveID + 0]);
    ui->lives->setValue(savefile.saveRAM[saveID + 1]);
    ui->score->setValue(savefile.saveRAM[saveID + 2]);
    ui->scoreBonus->setValue(savefile.saveRAM[saveID + 3]);
    ui->continues->setValue(savefile.saveRAM[saveID + 4]);
    ui->curStageSpecial->setChecked(stageID >= 0x80);

    if (stageID >= 0x80) {
        stageID -= 0x80;
    }

    if (stageID >= 1) {
        ui->curStage->setCurrentIndex(((stageID - 1) / 2) + 1);
        ui->curSpecialStage->setCurrentIndex(savefile.saveRAM[saveID + 7]);
    }
    else {
        ui->curStage->setCurrentIndex(0);
        ui->curSpecialStage->setCurrentIndex(stageID >= 0x80 ? savefile.saveRAM[saveID + 7] : 0);
    }

    for (int t = 0; t < 7; ++t) {
        tsBoxes[t]->setChecked(Utils::getBit(savefile.saveRAM[saveID + 6], t));
    }

    ui->activeFile->setChecked(savefile.saveRAM[32]);
    ui->musVol->setValue(savefile.saveRAM[33]);
    ui->sfxVol->setValue(savefile.saveRAM[34]);
    ui->ogCtrls->setChecked(savefile.saveRAM[35]);
    ui->tailsUnlocked->setChecked(savefile.saveRAM[43]);
    ui->knuxUnlocked->setChecked(savefile.saveRAM[44]);
    ui->boxArtRegion->setCurrentIndex(savefile.saveRAM[36]);
    ui->vDpadSize->setValue(savefile.saveRAM[37] / 64.0f);
    ui->vDpadOpacity->setValue(savefile.saveRAM[38] / 255.0f);
    ui->vDPadX1->setValue(savefile.saveRAM[39]);
    ui->vDPadY1->setValue(savefile.saveRAM[40]);
    ui->vDPadX2->setValue(savefile.saveRAM[41]);
    ui->vDPadY2->setValue(savefile.saveRAM[42]);

    connect(ui->curSave, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this, tsBoxes, stageNames](int c) {
                if (c == -1)
                    return;

                saveID = c << 3;

                ui->characterID->blockSignals(true);
                ui->lives->blockSignals(true);
                ui->score->blockSignals(true);
                ui->scoreBonus->blockSignals(true);
                ui->continues->blockSignals(true);
                ui->curStage->blockSignals(true);
                ui->curSpecialStage->blockSignals(true);
                for (int t = 0; t < 7; ++t) tsBoxes[t]->blockSignals(true);

                int stageID = savefile.saveRAM[saveID + 3];

                ui->characterID->setCurrentIndex(savefile.saveRAM[saveID + 0]);
                ui->lives->setValue(savefile.saveRAM[saveID + 1]);
                ui->score->setValue(savefile.saveRAM[saveID + 2]);
                ui->scoreBonus->setValue(savefile.saveRAM[saveID + 3]);
                ui->continues->setValue(savefile.saveRAM[saveID + 4]);
                ui->curStageSpecial->setChecked(stageID >= 0x80);

                if (stageID >= 0x80) {
                    stageID -= 0x80;
                }

                if (stageID >= 1) {
                    ui->curStage->setCurrentIndex(((stageID - 1) / 2) + 1);
                    ui->curSpecialStage->setCurrentIndex(savefile.saveRAM[saveID + 5]);
                }
                else {
                    ui->curStage->setCurrentIndex(0);
                    ui->curSpecialStage->setCurrentIndex(stageID >= 0x80 ? savefile.saveRAM[saveID + 5]
                                                                         : 0);
                }

                for (int t = 0; t < 7; ++t) {
                    tsBoxes[t]->setChecked(Utils::getBit(savefile.saveRAM[saveID + 4], t));
                }

                ui->characterID->blockSignals(false);
                ui->lives->blockSignals(false);
                ui->score->blockSignals(false);
                ui->scoreBonus->blockSignals(false);
                ui->continues->blockSignals(false);
                ui->curStage->blockSignals(false);
                ui->curSpecialStage->blockSignals(false);
                for (int t = 0; t < 7; ++t) tsBoxes[t]->blockSignals(false);
            });

    connect(ui->characterID, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int c) {
        if (c == -1)
            return;
        savefile.saveRAM[saveID + 0] = c;
    });

    connect(ui->lives, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { savefile.saveRAM[saveID + 1] = v; });

    connect(ui->score, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { savefile.saveRAM[saveID + 2] = v; });

    connect(ui->scoreBonus, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { savefile.saveRAM[saveID + 3] = v; });

    connect(ui->continues, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { savefile.saveRAM[saveID + 4] = v; });

    connect(ui->curStage, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int c) {
        if (c == -1)
            return;

        int zonePos  = (c - 1) / 3;
        int stagePos = (c - 1) - (zonePos * 3);
        int listPos  = (zonePos * 10) + (stagePos * 4);
        if (savefile.saveRAM[saveID + 5] >= 0x80)
            savefile.saveRAM[saveID + 5] = listPos + 0x81;
        else
            savefile.saveRAM[saveID + 5] = listPos + 1;
    });

    connect(ui->curStageSpecial, &QCheckBox::toggled, [this](bool c) {
        if (c) {
            if (savefile.saveRAM[saveID + 5] < 0x80)
                savefile.saveRAM[saveID + 5] += 0x80;
        }
        else {
            if (savefile.saveRAM[saveID + 5] >= 0x80)
                savefile.saveRAM[saveID + 5] -= 0x80;
        }
    });

    connect(ui->curSpecialStage, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int c) {
        if (c == -1)
            return;
        savefile.saveRAM[saveID + 7] = 0;
    });

    for (int t = 0; t < 7; ++t) {
        connect(tsBoxes[t], &QCheckBox::toggled,
                [this, t](bool c) { Utils::setBit(savefile.saveRAM[saveID + 6], c, t); });
    }

    connect(ui->activeFile, &QCheckBox::toggled, [this](bool c) { savefile.saveRAM[32] = c; });
    connect(ui->musVol, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](int v) { savefile.saveRAM[33] = v * 100; });
    connect(ui->sfxVol, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](int v) { savefile.saveRAM[34] = v * 100; });
    connect(ui->ogCtrls, &QCheckBox::toggled, [this](int c) { savefile.saveRAM[35] = c; });
    connect(ui->tailsUnlocked, &QCheckBox::toggled, [this](bool c) { savefile.saveRAM[36] = c; });
    connect(ui->knuxUnlocked, &QCheckBox::toggled, [this](bool c) { savefile.saveRAM[36] = c; });
    connect(ui->boxArtRegion, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int c) { savefile.saveRAM[36] = c; });
    connect(ui->vDpadSize, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](double v) { savefile.saveRAM[37] = v * 64; });
    connect(ui->vDpadOpacity, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](double v) { savefile.saveRAM[38] = v * 255; });
    connect(ui->vDPadX1, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { savefile.saveRAM[39] = v; });
    connect(ui->vDPadY1, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { savefile.saveRAM[40] = v; });
    connect(ui->vDPadX2, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { savefile.saveRAM[41] = v; });
    connect(ui->vDPadY2, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { savefile.saveRAM[42] = v; });
}

bool SaveFileEditorv4::event(QEvent *event)
{
    if (event->type() == (QEvent::Type)RE_EVENT_NEW) {
        savefile = RSDKv4::SaveFile();
        setupUI();
        return true;
    }
    if (event->type() == (QEvent::Type)RE_EVENT_OPEN) {
        QFileDialog filedialog(this, tr("Save RSDKv4 Save File"), "",
                               tr("RSDKv4 Save Files (*SData*.bin);;RSDKv4 Save Files (*SGame*.bin)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            savefile.read(filedialog.selectedFiles()[0]);
            appConfig.addRecentFile(ENGINE_v4, TOOL_SAVEEDITOR, filedialog.selectedFiles()[0],
                                    QList<QString>{});
            setupUI();
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE) {
        if (savefile.filepath != "") {
            savefile.write("");
            return true;
        }

        QFileDialog filedialog(this, tr("Save RSDKv4 Save File"), "",
                               tr("RSDKv4 Save Files (*SData*.bin);;RSDKv4 Save Files (*SGame*.bin)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            savefile.write(filedialog.selectedFiles()[0]);
            appConfig.addRecentFile(ENGINE_v4, TOOL_SAVEEDITOR, filedialog.selectedFiles()[0],
                                    QList<QString>{});
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE_AS) {
        QFileDialog filedialog(this, tr("Save RSDKv4 Save File;"), "",
                               tr("RSDKv4 Save Files (*SData*.bin);;RSDKv4 Save Files (*SGame*.bin)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            savefile.write(filedialog.selectedFiles()[0]);
            appConfig.addRecentFile(ENGINE_v3, TOOL_SAVEEDITOR, filedialog.selectedFiles()[0],
                                    QList<QString>{});
            return true;
        }
    }
    return QWidget::event(event);
}

#include "moc_savefileeditorv4.cpp"