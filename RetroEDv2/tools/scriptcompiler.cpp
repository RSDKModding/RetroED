#include "includes.hpp"
#include "ui_scriptcompiler.h"

ScriptCompiler::ScriptCompiler(QWidget *parent) : QWidget(parent), ui(new Ui::ScriptCompiler)
{
    ui->setupUi(this);

    auto updateButtons = [this] {
        bool isGlobal = ui->isGlobal->isChecked();
        ui->setStageConfig->setDisabled(isGlobal);

        bool compileValid = true;
        if (engineType == ENGINE_v1) {
            if (!QFile::exists(ui->trScript->text()))
                compileValid = false;
        }
        else {
            if (!QFile::exists(ui->gameConfigPath->text()))
                compileValid = false;
            if (!isGlobal && !QFile::exists(ui->stageConfigPath->text()))
                compileValid = false;
            if (!QDir().exists(ui->scriptsPath->text()))
                compileValid = false;
            if (!QDir().exists(ui->outputPath->text()))
                compileValid = false;
        }

        ui->compileScr->setDisabled(!compileValid);
    };

    connect(ui->selEngineType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this, updateButtons](int i) {
                switch (i) {
                    default: break;
                    case 0: engineType = ENGINE_v3; break;
                    case 1: engineType = ENGINE_v4; break;
                    case 2: engineType = ENGINE_v1; break;
                }

                if (i >= 0)
                    ui->stackedWidget->setCurrentIndex(i == 2);

                updateButtons();
            });

    connect(ui->isGlobal, &QCheckBox::toggled, [updateButtons] { updateButtons(); });

    connect(ui->setGameConfig, &QPushButton::clicked, [this, updateButtons] {
        QFileDialog filedialog(this, tr("Open GameConfig"), "",
                               QString(engineType == ENGINE_v4 ? "RSDKv4" : "RSDKv3")
                                   + " GameConfig files (GameConfig*.bin)");
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            QString gameConfigPath = filedialog.selectedFiles()[0];
            ui->gameConfigPath->setText(gameConfigPath);

            updateButtons();
        }
    });

    connect(ui->setStageConfig, &QPushButton::clicked, [this, updateButtons] {
        QFileDialog filedialog(this, tr("Open StageConfig"), "",
                               QString(engineType == ENGINE_v4 ? "RSDKv4" : "RSDKv3")
                                   + " StageConfig files (StageConfig*.bin)");
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            QString stageConfigPath = filedialog.selectedFiles()[0];
            ui->stageConfigPath->setText(stageConfigPath);

            updateButtons();
        }
    });

    connect(ui->setScriptsDir, &QPushButton::clicked, [this, updateButtons] {
        QFileDialog filedialog(this, tr("Open Scripts Folder"), "", tr("Folder"));
        filedialog.setFileMode(QFileDialog::Directory);
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            QString scriptsPath = filedialog.selectedFiles()[0];
            ui->scriptsPath->setText(scriptsPath);

            updateButtons();
        }
    });

    connect(ui->setOutputDir, &QPushButton::clicked, [this, updateButtons] {
        QFileDialog filedialog(this, tr("Open Output Folder"), "", tr("Folder"));
        filedialog.setFileMode(QFileDialog::Directory);
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            QString outPath = filedialog.selectedFiles()[0];
            ui->outputPath->setText(outPath);

            updateButtons();
        }
    });

    connect(ui->selectTRScript, &QPushButton::clicked, [this, updateButtons] {
        QFileDialog filedialog(this, tr("Open Script"), "", QString("Retro-Sonic Scripts (*.txt)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            QString scrPath = filedialog.selectedFiles()[0];
            ui->trScript->setText(scrPath);

            updateButtons();
        }
    });

    connect(ui->setTRBytecodePath, &QPushButton::clicked, [this, updateButtons] {
        QFileDialog filedialog(this, tr("Save Bytecode"), "",
                               QString("Retro-Sonic Bytecode files (*.rsf)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            QString bcPath = filedialog.selectedFiles()[0];
            ui->trBytecodePath->setText(bcPath);

            updateButtons();
        }
    });

    connect(ui->compileScr, &QPushButton::clicked, [this] {
        // lol
        switch (engineType) {
            default: break;
            case ENGINE_v4: {
                RSDKv4::GameConfig gameConfig(ui->gameConfigPath->text());
                RSDKv4::StageConfig stageConfig;
                if (!ui->isGlobal->isChecked())
                    stageConfig = RSDKv4::StageConfig(ui->stageConfigPath->text());
                QString scriptsFolder = ui->scriptsPath->text() + "/";
                QString outputFolder  = ui->outputPath->text() + "/";

                QString bytecodeName = "GlobalCode";
                if (!ui->isGlobal->isChecked()) {
                    QFileInfo info(stageConfig.filePath);
                    QDir dir(info.dir());
                    bytecodeName = dir.dirName();
                }

                setStatus("Compiling " + bytecodeName + ".bin...", true);

                compilerv4.clearScriptData();
                compilerv4.gamePlatform      = "STANDARD";
                compilerv4.gameRenderType    = "SW_RENDERING";
                compilerv4.gameHapticSetting = "NO_F_FEEDBACK";

                if (ui->platformType->currentIndex() == 1)
                    compilerv4.gameHapticSetting = "MOBILE";

                if (ui->renderType->currentIndex() == 1)
                    compilerv4.gameHapticSetting = "HW_RENDERING";

                if (ui->useHaptics->isChecked())
                    compilerv4.gameHapticSetting = "USE_F_FEEDBACK";

                compilerv4.gameConfig = gameConfig;

                int id                     = 0;
                compilerv4.typeNames[id++] = "BlankObject";
                for (auto &obj : gameConfig.objects) {
                    compilerv4.typeNames[id] = obj.name;
                    compilerv4.typeNames[id].replace(" ", "");

                    id++;
                }
                for (auto &obj : stageConfig.objects) {
                    compilerv4.typeNames[id] = obj.name;
                    compilerv4.typeNames[id].replace(" ", "");

                    id++;
                }

                id = 0;
                for (auto &sfx : gameConfig.soundFX) {
                    compilerv4.sfxNames[id] = sfx.name;
                    compilerv4.sfxNames[id].replace(" ", "");

                    id++;
                }

                for (auto &sfx : stageConfig.soundFX) {
                    compilerv4.sfxNames[id] = sfx.name;
                    compilerv4.sfxNames[id].replace(" ", "");

                    id++;
                }

                for (auto &var : gameConfig.globalVariables)
                    compilerv4.globalVariables.append(var.name);

                int scrID     = 1;
                bool hasError = false;
                if (stageConfig.loadGlobalScripts || ui->isGlobal->isChecked()) {
                    for (int i = 0; !hasError && i < gameConfig.objects.count(); ++i) {
                        QString scriptPath = scriptsFolder + gameConfig.objects[i].script;
                        scriptPath =
                            WorkingDirManager::GetPath(gameConfig.objects[i].script, scriptPath);
                        compilerv4.parseScriptFile(scriptPath, scrID++, false);

                        if (compilerv4.scriptError) {
                            printLog(compilerv4.errorMsg);
                            printLog(compilerv4.errorPos);
                            printLog(QString::number(compilerv4.errorLine));

                            QFileInfo info(compilerv4.errorScr);
                            QDir dir(info.dir());
                            dir.cdUp();
                            QString dirFile = dir.relativeFilePath(compilerv4.errorScr);

                            QMessageBox msgBox(QMessageBox::Information, "RetroED",
                                               QString("Failed to compile script: %1\n"
                                                       "Error: %2\n"
                                                       "\n"
                                                       "Pos: %3\n"
                                                       "Line: %4\n")
                                                   .arg(gameConfig.objects[i].script)
                                                   .arg(compilerv4.errorMsg)
                                                   .arg(compilerv4.errorPos)
                                                   .arg(compilerv4.errorLine),
                                               QMessageBox::Ok);
                            msgBox.exec();

                            setStatus("Failed to compile script: " + dirFile);
                            compilerv4.objectScriptList[scrID - 1].eventMain.scriptCodePtr =
                                SCRIPTDATA_COUNT_v4 - 1;
                            compilerv4.objectScriptList[scrID - 1].eventMain.jumpTablePtr =
                                JUMPTABLE_COUNT_v4 - 1;
                            compilerv4.objectScriptList[scrID - 1].eventDraw.scriptCodePtr =
                                SCRIPTDATA_COUNT_v4 - 1;
                            compilerv4.objectScriptList[scrID - 1].eventDraw.jumpTablePtr =
                                JUMPTABLE_COUNT_v4 - 1;
                            compilerv4.objectScriptList[scrID - 1].eventStartup.scriptCodePtr =
                                SCRIPTDATA_COUNT_v4 - 1;
                            compilerv4.objectScriptList[scrID - 1].eventStartup.jumpTablePtr =
                                JUMPTABLE_COUNT_v4 - 1;

                            hasError = true;
                            break;
                        }
                        setStatusProgress(((float)i + 1 / gameConfig.objects.count()) / 2);
                    }
                }
                int globalScriptCodePos = compilerv4.scriptDataPos;
                int globalJumpTablePos  = compilerv4.jumpTableDataPos;

                for (int i = 0;
                     !hasError && !ui->isGlobal->isChecked() && i < stageConfig.objects.count(); ++i) {
                    QString scriptPath = scriptsFolder + stageConfig.objects[i].script;
                    scriptPath = WorkingDirManager::GetPath(stageConfig.objects[i].script, scriptPath);
                    compilerv4.parseScriptFile(scriptPath, scrID++, false);

                    if (compilerv4.scriptError) {
                        printLog(compilerv4.errorMsg);
                        printLog(compilerv4.errorPos);
                        printLog(QString::number(compilerv4.errorLine));

                        QFileInfo info(compilerv4.errorScr);
                        QDir dir(info.dir());
                        dir.cdUp();
                        QString dirFile = dir.relativeFilePath(compilerv4.errorScr);

                        setStatus("Failed to compile script: " + dirFile);

                        QMessageBox msgBox(QMessageBox::Information, "RetroED",
                                           QString("Failed to compile script: %1\n"
                                                   "Error: %2\n"
                                                   "\n"
                                                   "Pos: %3\n"
                                                   "Line: %4\n")
                                               .arg(stageConfig.objects[i].script)
                                               .arg(compilerv4.errorMsg)
                                               .arg(compilerv4.errorPos)
                                               .arg(compilerv4.errorLine),
                                           QMessageBox::Ok);
                        msgBox.exec();

                        compilerv4.objectScriptList[scrID - 1].eventMain.scriptCodePtr =
                            SCRIPTDATA_COUNT_v4 - 1;
                        compilerv4.objectScriptList[scrID - 1].eventMain.jumpTablePtr =
                            JUMPTABLE_COUNT_v4 - 1;
                        compilerv4.objectScriptList[scrID - 1].eventDraw.scriptCodePtr =
                            SCRIPTDATA_COUNT_v4 - 1;
                        compilerv4.objectScriptList[scrID - 1].eventDraw.jumpTablePtr =
                            JUMPTABLE_COUNT_v4 - 1;
                        compilerv4.objectScriptList[scrID - 1].eventStartup.scriptCodePtr =
                            SCRIPTDATA_COUNT_v4 - 1;
                        compilerv4.objectScriptList[scrID - 1].eventStartup.jumpTablePtr =
                            JUMPTABLE_COUNT_v4 - 1;
                        hasError = true;
                        break;
                    }
                    setStatusProgress(((float)i + 1 / stageConfig.objects.count()) / 2 + .5);
                }
                int scriptCodePos = compilerv4.scriptDataPos;
                int jumpTablePos  = compilerv4.jumpTableDataPos;

                if (!hasError) {
                    RSDKv4::Bytecode bytecode;

                    int offset           = 1;
                    int count            = gameConfig.objects.count();
                    int scriptCodeOffset = 0;
                    int jumpTableOffset  = 0;
                    if (!ui->isGlobal->isChecked()) {
                        offset += gameConfig.objects.count();
                        count            = stageConfig.objects.count();
                        scriptCodeOffset = globalScriptCodePos;
                        jumpTableOffset  = globalJumpTablePos;
                    }

                    for (int c = scriptCodeOffset; c < scriptCodePos; ++c)
                        bytecode.scriptCode.append(compilerv4.scriptData[c]);

                    for (int j = jumpTableOffset; j < jumpTablePos; ++j)
                        bytecode.jumpTable.append(compilerv4.jumpTableData[j]);

                    for (int s = 0; s < count; ++s) {
                        RSDKv4::Bytecode::ScriptInfo script;
                        script.main.scriptCodePos =
                            compilerv4.objectScriptList[offset + s].eventMain.scriptCodePtr;
                        script.main.jumpTablePos =
                            compilerv4.objectScriptList[offset + s].eventMain.jumpTablePtr;
                        script.draw.scriptCodePos =
                            compilerv4.objectScriptList[offset + s].eventDraw.scriptCodePtr;
                        script.draw.jumpTablePos =
                            compilerv4.objectScriptList[offset + s].eventDraw.jumpTablePtr;
                        script.startup.scriptCodePos =
                            compilerv4.objectScriptList[offset + s].eventStartup.scriptCodePtr;
                        script.startup.jumpTablePos =
                            compilerv4.objectScriptList[offset + s].eventStartup.jumpTablePtr;

                        bytecode.scriptList.append(script);
                    }

                    for (int f = 0; f < compilerv4.functionCount; ++f) {
                        RSDKv4::Bytecode::FunctionInfo func;
                        func.scriptCodePos = compilerv4.functionList[f].scriptCodePtr;
                        func.jumpTablePos  = compilerv4.functionList[f].jumpTablePtr;

                        bytecode.functionList.append(func);
                    }

                    bytecode.write(outputFolder + bytecodeName + ".bin");

                    setStatus(bytecodeName + ".bin compiled successfully!");
                    QMessageBox msgBox(QMessageBox::Information, "RetroED",
                                       QString(bytecodeName + ".bin compiled successfully!"),
                                       QMessageBox::Ok);
                    msgBox.exec();
                }
                else {
                    setStatus(bytecodeName + ".bin compilation failed...");
                }

                break;
            }
            case ENGINE_v3: {

                RSDKv3::GameConfig gameConfig(ui->gameConfigPath->text());
                RSDKv3::StageConfig stageConfig;
                if (!ui->isGlobal->isChecked())
                    stageConfig = RSDKv3::StageConfig(ui->stageConfigPath->text());
                QString scriptsFolder = ui->scriptsPath->text() + "/";
                QString outputFolder  = ui->outputPath->text() + "/";

                QString bytecodeName = "GlobalCode";
                if (!ui->isGlobal->isChecked()) {
                    QFileInfo info(stageConfig.filePath);
                    QDir dir(info.dir());
                    bytecodeName = dir.dirName();
                }

                setStatus("Compiling " + bytecodeName + ".bin...", true);

                compilerv3.clearScriptData();
                compilerv3.gamePlatform      = "Standard";
                compilerv3.gameRenderType    = "SW_Rendering";
                compilerv3.gameHapticSetting = "No_Haptics";

                if (ui->platformType->currentIndex() == 1)
                    compilerv3.gameHapticSetting = "Mobile";

                if (ui->renderType->currentIndex() == 1)
                    compilerv3.gameHapticSetting = "HW_Rendering";

                if (ui->useHaptics->isChecked())
                    compilerv3.gameHapticSetting = "Use_Haptics";

                compilerv3.gameConfig = gameConfig;

                int id                     = 0;
                compilerv3.typeNames[id++] = "BlankObject";
                for (auto &obj : gameConfig.objects) {
                    compilerv3.typeNames[id] = obj.name;
                    compilerv3.typeNames[id].replace(" ", "");

                    id++;
                }
                for (auto &obj : stageConfig.objects) {
                    compilerv3.typeNames[id] = obj.name;
                    compilerv3.typeNames[id].replace(" ", "");

                    id++;
                }

                for (auto &sfx : gameConfig.soundFX)
                    compilerv3.globalSfxNames.append(QFileInfo(sfx).baseName().replace(" ", ""));

                for (auto &sfx : stageConfig.soundFX)
                    compilerv3.stageSfxNames.append(QFileInfo(sfx).baseName().replace(" ", ""));

                for (auto &var : gameConfig.globalVariables)
                    compilerv3.globalVariables.append(var.name);

                int scrID     = 1;
                bool hasError = false;
                if (stageConfig.loadGlobalScripts || ui->isGlobal->isChecked()) {
                    for (int i = 0; !hasError && i < gameConfig.objects.count(); ++i) {
                        QString scriptPath = scriptsFolder + gameConfig.objects[i].script;
                        scriptPath =
                            WorkingDirManager::GetPath(gameConfig.objects[i].script, scriptPath);
                        compilerv3.parseScriptFile(scriptPath, scrID++, false);

                        if (compilerv3.scriptError) {
                            printLog(compilerv3.errorMsg);
                            printLog(compilerv3.errorPos);
                            printLog(QString::number(compilerv3.errorLine));

                            QFileInfo info(compilerv3.errorScr);
                            QDir dir(info.dir());
                            dir.cdUp();
                            QString dirFile = dir.relativeFilePath(compilerv3.errorScr);

                            QMessageBox msgBox(QMessageBox::Information, "RetroED",
                                               QString("Failed to compile script: %1\n"
                                                       "Error: %2\n"
                                                       "\n"
                                                       "Pos: %3\n"
                                                       "Line: %4\n")
                                                   .arg(gameConfig.objects[i].script)
                                                   .arg(compilerv3.errorMsg)
                                                   .arg(compilerv3.errorPos)
                                                   .arg(compilerv3.errorLine),
                                               QMessageBox::Ok);
                            msgBox.exec();

                            setStatus("Failed to compile script: " + dirFile);

                            hasError = true;
                            break;
                        }
                        setStatusProgress(((float)i + 1 / gameConfig.objects.count()) / 2);
                    }
                }
                int globalScriptCodePos = compilerv3.scriptDataPos;
                int globalJumpTablePos  = compilerv3.jumpTableDataPos;

                for (int i = 0;
                     !hasError && !ui->isGlobal->isChecked() && i < stageConfig.objects.count(); ++i) {
                    QString scriptPath = scriptsFolder + stageConfig.objects[i].script;
                    scriptPath = WorkingDirManager::GetPath(stageConfig.objects[i].script, scriptPath);
                    compilerv3.parseScriptFile(scriptPath, scrID++, false);

                    if (compilerv3.scriptError) {
                        printLog(compilerv3.errorMsg);
                        printLog(compilerv3.errorPos);
                        printLog(QString::number(compilerv3.errorLine));

                        QFileInfo info(compilerv3.errorScr);
                        QDir dir(info.dir());
                        dir.cdUp();
                        QString dirFile = dir.relativeFilePath(compilerv3.errorScr);

                        QMessageBox msgBox(QMessageBox::Information, "RetroED",
                                           QString("Failed to compile script: %1\n"
                                                   "Error: %2\n"
                                                   "\n"
                                                   "Pos: %3\n"
                                                   "Line: %4\n")
                                               .arg(stageConfig.objects[i].script)
                                               .arg(compilerv3.errorMsg)
                                               .arg(compilerv3.errorPos)
                                               .arg(compilerv3.errorLine),
                                           QMessageBox::Ok);
                        msgBox.exec();

                        setStatus("Failed to compile script: " + dirFile);

                        hasError = true;
                        break;
                    }
                    setStatusProgress(((float)i + 1 / stageConfig.objects.count()) / 2 + .5);
                }
                int scriptCodePos = compilerv3.scriptDataPos;
                int jumpTablePos  = compilerv3.jumpTableDataPos;

                if (!hasError) {
                    RSDKv3::Bytecode bytecode;

                    int offset           = 1;
                    int count            = gameConfig.objects.count();
                    int scriptCodeOffset = 0;
                    int jumpTableOffset  = 0;
                    if (!ui->isGlobal->isChecked()) {
                        offset += gameConfig.objects.count();
                        count            = stageConfig.objects.count();
                        scriptCodeOffset = globalScriptCodePos;
                        jumpTableOffset  = globalJumpTablePos;
                    }

                    for (int c = scriptCodeOffset; c < scriptCodePos; ++c)
                        bytecode.scriptCode.append(compilerv3.scriptData[c]);

                    for (int j = jumpTableOffset; j < jumpTablePos; ++j)
                        bytecode.jumpTable.append(compilerv3.jumpTableData[j]);

                    for (int s = 0; s < count; ++s) {
                        RSDKv3::Bytecode::ScriptInfo script;
                        script.main.scriptCodePos =
                            compilerv3.objectScriptList[offset + s].subMain.scriptCodePtr;
                        script.main.jumpTablePos =
                            compilerv3.objectScriptList[offset + s].subMain.jumpTablePtr;

                        script.playerInteraction.scriptCodePos =
                            compilerv3.objectScriptList[offset + s].subPlayerInteraction.scriptCodePtr;
                        script.playerInteraction.jumpTablePos =
                            compilerv3.objectScriptList[offset + s].subPlayerInteraction.jumpTablePtr;

                        script.draw.scriptCodePos =
                            compilerv3.objectScriptList[offset + s].subDraw.scriptCodePtr;
                        script.draw.jumpTablePos =
                            compilerv3.objectScriptList[offset + s].subDraw.jumpTablePtr;

                        script.startup.scriptCodePos =
                            compilerv3.objectScriptList[offset + s].subStartup.scriptCodePtr;
                        script.startup.jumpTablePos =
                            compilerv3.objectScriptList[offset + s].subStartup.jumpTablePtr;

                        bytecode.scriptList.append(script);
                    }

                    for (int f = 0; f < compilerv3.functionCount; ++f) {
                        RSDKv3::Bytecode::FunctionInfo func;
                        func.scriptCodePos = compilerv3.functionList[f].scriptCodePtr;
                        func.jumpTablePos  = compilerv3.functionList[f].jumpTablePtr;

                        bytecode.functionList.append(func);
                    }

                    bytecode.write(outputFolder + bytecodeName + ".bin");

                    setStatus(bytecodeName + ".bin compiled successfully!");
                    QMessageBox msgBox(QMessageBox::Information, "RetroED",
                                       QString(bytecodeName + ".bin compiled successfully!"),
                                       QMessageBox::Ok);
                    msgBox.exec();
                }
                else {
                    setStatus(bytecodeName + ".bin compilation failed...");
                }
                break;
            }
            case ENGINE_v1: {
                QString scrName = Utils::getFilenameAndFolder(ui->trScript->text());
                setStatus("Compiling " + scrName + "...", true);
                compilerv1.parseScriptFile(ui->trScript->text());

                if (compilerv1.scriptError) {
                    printLog(compilerv1.errorMsg);
                    printLog(compilerv1.errorPos);
                    printLog(QString::number(compilerv1.errorLine));

                    QFileInfo info(compilerv1.errorScr);
                    QDir dir(info.dir());
                    dir.cdUp();
                    QString dirFile = dir.relativeFilePath(compilerv1.errorScr);

                    QMessageBox msgBox(QMessageBox::Information, "RetroED",
                                       QString("Failed to compile script: %1\n"
                                               "Error: %2\n"
                                               "\n"
                                               "Pos: %3\n"
                                               "Line: %4\n")
                                           .arg(scrName)
                                           .arg(compilerv1.errorMsg)
                                           .arg(compilerv1.errorPos)
                                           .arg(compilerv1.errorLine),
                                       QMessageBox::Ok);
                    msgBox.exec();

                    setStatus("Failed to compile script: " + dirFile);
                    break;
                }
                else {
                    setStatus(scrName + " compiled successfully");
                    compilerv1.script.write(ui->trBytecodePath->text());
                }

                break;
            }
        }
    });

    updateButtons();
}

ScriptCompiler::~ScriptCompiler() { delete ui; }

#include "moc_scriptcompiler.cpp"
