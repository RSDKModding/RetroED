#include "includes.hpp"
#include "ui_savefileeditorv3.h"

SaveFileEditorv3::SaveFileEditorv3(QString filePath, QWidget *parent)
    : QWidget(parent), ui(new Ui::SaveFileEditorv3)
{
    ui->setupUi(this);

    if (QFile::exists(filePath)) {
        savefile.read(filePath);
        appConfig.addRecentFile(ENGINE_v3, TOOL_SAVEEDITOR, filePath, QList<QString>{});
    }

    setupUI();
}

SaveFileEditorv3::~SaveFileEditorv3() { delete ui; }

void SaveFileEditorv3::setupUI()
{
    QCheckBox *tsBoxes[7] = { ui->e1, ui->e2, ui->e3, ui->e4, ui->e5, ui->e6, ui->e7 };

    // CharacterID:         SaveRAM[0]
    // Lives: 				SaveRAM[1]
    // Score: 				SaveRAM[2]
    // ZoneID:              SaveRAM[3]
    // TimeStones:          SaveRAM[4]
    // SpecialZoneID:       SaveRAM[5]
    // Score Bonus:  		SaveRAM[6]
    // FuturesSaved:  		SaveRAM[7] & 0xFFFF
    // MSDestroyed:         SaveRAM[7] >> 16

    // NewSave:             SaveRAM[32]
    // MusVolume: 			SaveRAM[33]
    // SFXVolume: 			SaveRAM[34]
    // SpindashStyle: 		SaveRAM[35]
    // TailsUnlocked: 		SaveRAM[36]
    // Filter:              SaveRAM[37]
    // Soundtrack:          SaveRAM[38]
    // TotalTAttackTime: 	SaveRAM[39]
    // HapticsEnabled:      SaveRAM[40]

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
    disconnect(ui->futureList, nullptr, nullptr, nullptr);
    disconnect(ui->msList, nullptr, nullptr, nullptr);

    disconnect(ui->activeFile, nullptr, nullptr, nullptr);
    disconnect(ui->musVol, nullptr, nullptr, nullptr);
    disconnect(ui->sfxVol, nullptr, nullptr, nullptr);
    disconnect(ui->ctrlStyle, nullptr, nullptr, nullptr);
    disconnect(ui->tailsUnlocked, nullptr, nullptr, nullptr);
    disconnect(ui->filter, nullptr, nullptr, nullptr);
    disconnect(ui->soundtrack, nullptr, nullptr, nullptr);
    // disconnect(ui->totalTAttackTime, nullptr, nullptr, nullptr);
    disconnect(ui->hapticsEnabled, nullptr, nullptr, nullptr);

    ui->futureList->clear();
    ushort futureList = savefile.saveRAM[m_saveID + 7] & 0xFFFF;
    for (int s = 0; s < stageNames.count(); ++s) {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(stageNames[s]);
        item->setFlags(item->flags() | Qt::ItemFlag::ItemIsUserCheckable);
        item->setCheckState(Utils::getBit(futureList, s) ? Qt::CheckState::Checked
                                                         : Qt::CheckState::Unchecked);
        ui->futureList->addItem(item);
    }

    ui->msList->clear();
    ushort msList = (savefile.saveRAM[m_saveID + 7] >> 16) & 0xFFFF;
    for (int s = 0; s < stageNames.count(); ++s) {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(stageNames[s]);
        item->setFlags(item->flags() | Qt::ItemFlag::ItemIsUserCheckable);
        item->setCheckState(Utils::getBit(msList, s) ? Qt::CheckState::Checked
                                                     : Qt::CheckState::Unchecked);
        ui->msList->addItem(item);
    }

    m_saveID = 0 << 3;
    ui->curSave->setCurrentIndex(0);

    int stageID = savefile.saveRAM[m_saveID + 3];

    ui->characterID->setCurrentIndex(savefile.saveRAM[m_saveID + 0]);
    ui->lives->setValue(savefile.saveRAM[m_saveID + 1]);
    ui->score->setValue(savefile.saveRAM[m_saveID + 2]);
    ui->curStageSpecial->setChecked(stageID >= 80);

    if (stageID >= 80) {
        stageID -= 80;
    }

    if (stageID >= 1) {
        int zonePos  = ((stageID - 1) / 10);
        int stagePos = (stageID - 1) - (zonePos * 10);
        int listID   = (zonePos * 3) + (stagePos / 4);
        ui->curStage->setCurrentIndex(listID + 1);
        ui->curSpecialStage->setCurrentIndex(savefile.saveRAM[m_saveID + 5]);
    }
    else {
        ui->curStage->setCurrentIndex(0);
        ui->curSpecialStage->setCurrentIndex(stageID >= 80 ? savefile.saveRAM[m_saveID + 5] : 0);
    }

    for (int t = 0; t < 7; ++t) {
        tsBoxes[t]->setChecked(Utils::getBit(savefile.saveRAM[m_saveID + 4], t));
    }
    ui->scoreBonus->setValue(savefile.saveRAM[m_saveID + 6]);

    ui->activeFile->setChecked(savefile.saveRAM[32]);
    ui->musVol->setValue(savefile.saveRAM[33]);
    ui->sfxVol->setValue(savefile.saveRAM[34]);
    ui->ctrlStyle->setCurrentIndex(savefile.saveRAM[35]);
    ui->tailsUnlocked->setChecked(savefile.saveRAM[36]);
    ui->filter->setCurrentIndex(savefile.saveRAM[37]);
    ui->soundtrack->setCurrentIndex(savefile.saveRAM[38]);
    ui->hapticsEnabled->setChecked(savefile.saveRAM[40]);

    connect(ui->curSave, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this, tsBoxes, stageNames](int c) {
                if (c == -1)
                    return;

                m_saveID = c << 3;

                ui->characterID->blockSignals(true);
                ui->lives->blockSignals(true);
                ui->score->blockSignals(true);
                ui->curStage->blockSignals(true);
                ui->curSpecialStage->blockSignals(true);
                for (int t = 0; t < 7; ++t) tsBoxes[t]->blockSignals(true);
                ui->scoreBonus->blockSignals(true);
                ui->futureList->blockSignals(true);
                ui->msList->blockSignals(true);

                int stageID = savefile.saveRAM[m_saveID + 3];

                ui->characterID->setCurrentIndex(savefile.saveRAM[m_saveID + 0]);
                ui->lives->setValue(savefile.saveRAM[m_saveID + 1]);
                ui->score->setValue(savefile.saveRAM[m_saveID + 2]);
                ui->curStageSpecial->setChecked(stageID >= 80);

                if (stageID >= 80) {
                    stageID -= 80;
                }

                if (stageID >= 1) {
                    int zonePos  = ((stageID - 1) / 10);
                    int stagePos = (stageID - 1) - (zonePos * 10);
                    int listID   = (zonePos * 3) + (stagePos / 4);
                    ui->curStage->setCurrentIndex(listID + 1);
                    ui->curSpecialStage->setCurrentIndex(savefile.saveRAM[m_saveID + 5]);
                }
                else {
                    ui->curStage->setCurrentIndex(0);
                    ui->curSpecialStage->setCurrentIndex(stageID >= 80 ? savefile.saveRAM[m_saveID + 5]
                                                                       : 0);
                }

                for (int t = 0; t < 7; ++t) {
                    tsBoxes[t]->setChecked(Utils::getBit(savefile.saveRAM[m_saveID + 4], t));
                }
                // ui->nextSpecialStage->setCurrentIndex(m_savefile.m_saveRAM[m_saveID + 5]);
                ui->scoreBonus->setValue(savefile.saveRAM[m_saveID + 6]);

                ui->futureList->clear();
                ushort futureList = savefile.saveRAM[m_saveID + 7] & 0xFFFF;
                for (int s = 0; s < stageNames.count(); ++s) {
                    QListWidgetItem *item = new QListWidgetItem();
                    item->setText(stageNames[s]);
                    item->setFlags(item->flags() | Qt::ItemFlag::ItemIsUserCheckable);
                    item->setCheckState(Utils::getBit(futureList, s) ? Qt::CheckState::Checked
                                                                     : Qt::CheckState::Unchecked);
                    ui->futureList->addItem(item);
                }

                ui->msList->clear();
                ushort msList = (savefile.saveRAM[m_saveID + 7] >> 16) & 0xFFFF;
                for (int s = 0; s < stageNames.count(); ++s) {
                    QListWidgetItem *item = new QListWidgetItem();
                    item->setText(stageNames[s]);
                    item->setFlags(item->flags() | Qt::ItemFlag::ItemIsUserCheckable);
                    item->setCheckState(Utils::getBit(msList, s) ? Qt::CheckState::Checked
                                                                 : Qt::CheckState::Unchecked);
                    ui->msList->addItem(item);
                }

                ui->characterID->blockSignals(false);
                ui->lives->blockSignals(false);
                ui->score->blockSignals(false);
                ui->curStage->blockSignals(false);
                ui->curSpecialStage->blockSignals(false);
                for (int t = 0; t < 7; ++t) tsBoxes[t]->blockSignals(false);
                ui->scoreBonus->blockSignals(false);
                ui->futureList->blockSignals(false);
                ui->msList->blockSignals(false);
            });

    connect(ui->characterID, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int c) {
        if (c == -1)
            return;
        savefile.saveRAM[m_saveID + 0] = c;
    });

    connect(ui->lives, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { savefile.saveRAM[m_saveID + 1] = v; });

    connect(ui->score, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { savefile.saveRAM[m_saveID + 2] = v; });

    connect(ui->curStage, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int c) {
        if (c == -1)
            return;

        int zonePos  = (c - 1) / 3;
        int stagePos = (c - 1) - (zonePos * 3);
        int listPos  = (zonePos * 10) + (stagePos * 4);
        if (savefile.saveRAM[m_saveID + 3] >= 80)
            savefile.saveRAM[m_saveID + 3] = listPos + 81;
        else
            savefile.saveRAM[m_saveID + 3] = listPos + 1;
    });

    connect(ui->curStageSpecial, &QCheckBox::toggled, [this](bool c) {
        if (c) {
            if (savefile.saveRAM[m_saveID + 3] < 80)
                savefile.saveRAM[m_saveID + 3] += 80;
        }
        else {
            if (savefile.saveRAM[m_saveID + 3] >= 80)
                savefile.saveRAM[m_saveID + 3] -= 80;
        }
    });

    connect(ui->curSpecialStage, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int c) {
        if (c == -1)
            return;
        savefile.saveRAM[m_saveID + 5] = 0;
    });

    for (int t = 0; t < 7; ++t) {
        connect(tsBoxes[t], &QCheckBox::toggled,
                [this, t](bool c) { Utils::setBit(savefile.saveRAM[m_saveID + 4], c, t); });
    }

    connect(ui->scoreBonus, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { savefile.saveRAM[m_saveID + 6] = v; });

    connect(ui->futureList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        int row     = ui->futureList->row(item);
        bool active = item->checkState() != Qt::Unchecked;
        Utils::setBit(savefile.saveRAM[m_saveID + 7], active, row);
    });

    connect(ui->msList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        int row     = ui->msList->row(item);
        bool active = item->checkState() != Qt::Unchecked;
        Utils::setBit(savefile.saveRAM[m_saveID + 7], active, row + 0x10);
    });

    connect(ui->activeFile, &QCheckBox::toggled, [this](bool c) { savefile.saveRAM[32] = c; });
    connect(ui->musVol, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { savefile.saveRAM[33] = v; });
    connect(ui->sfxVol, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { savefile.saveRAM[34] = v; });
    connect(ui->ctrlStyle, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int c) { savefile.saveRAM[35] = c; });
    connect(ui->tailsUnlocked, &QCheckBox::toggled, [this](bool c) { savefile.saveRAM[36] = c; });
    connect(ui->filter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int c) { savefile.saveRAM[37] = c; });
    connect(ui->soundtrack, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int c) { savefile.saveRAM[38] = c; });
    connect(ui->hapticsEnabled, &QCheckBox::toggled, [this](bool c) { savefile.saveRAM[40] = c; });
}

bool SaveFileEditorv3::event(QEvent *event)
{
    if (event->type() == (QEvent::Type)RE_EVENT_NEW) {
        savefile = RSDKv3::SaveFile();
        setupUI();
        return true;
    }
    if (event->type() == (QEvent::Type)RE_EVENT_OPEN) {
        QFileDialog filedialog(this, tr("Save RSDKv3 Save File"), "",
                               tr("RSDKv3 Save Files (*SData*.bin);;RSDKv3 Save Files (*SGame*.bin)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            savefile.read(filedialog.selectedFiles()[0]);
            appConfig.addRecentFile(ENGINE_v3, TOOL_SAVEEDITOR, filedialog.selectedFiles()[0],
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

        QFileDialog filedialog(this, tr("Save RSDKv3 Save File"), "",
                               tr("RSDKv3 Save Files (*SData*.bin);;RSDKv3 Save Files (*SGame*.bin)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            savefile.write(filedialog.selectedFiles()[0]);
            appConfig.addRecentFile(ENGINE_v3, TOOL_SAVEEDITOR, filedialog.selectedFiles()[0],
                                    QList<QString>{});
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE_AS) {
        QFileDialog filedialog(this, tr("Save RSDKv3 Save File;"), "",
                               tr("RSDKv3 Save Files (*SData*.bin);;RSDKv3 Save Files (*SGame*.bin)"));
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

#include "moc_savefileeditorv3.cpp"
