#include "includes.hpp"
#include "ui_scriptunpacker.h"

ScriptUnpacker::ScriptUnpacker(QWidget *parent) : QWidget(parent), ui(new Ui::ScriptUnpacker)
{
    ui->setupUi(this);

    connect(ui->selEngineType, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int i) {
        if (i >= 0)
            ui->stackedWidget->setCurrentIndex(i == 2);
        variableNames.clear();
        bytecodeList.clear();
    });

    connect(ui->useCustomAliases, &QCheckBox::toggled,
            [this](bool c) { ui->sepFolders->setDisabled(c); });

    connect(ui->selDataFolder, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Open Data Folder"), "", tr("Folder"));
        filedialog.setFileMode(QFileDialog::Directory);
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            dataPath = filedialog.selectedFiles()[0];

            if (ui->selEngineType->currentIndex() == 0) {
                QString globalPath = dataPath + "/Scripts/Bytecode/GS000.bin";
                mobileVer          = false;
                if (!QFile::exists(globalPath)) {
                    globalPath = dataPath + "/Scripts/Bytecode/GlobalCode.bin";
                    mobileVer  = true;
                    if (!QFile::exists(globalPath)) {
                        setStatus("no global bytecode found in folder! aborting extraction!");
                        return;
                    }
                }

                bytecodeList.clear();
                variableNames.clear();

                if (!QFile::exists(dataPath + "/Game/GameConfig.bin")) {
                    setStatus("no gameconfig found in folder! aborting extraction!");
                    return;
                }
                else {
                    RSDKv3::GameConfig gcf;
                    gcf.read(dataPath + "/Game/GameConfig.bin");

                    BytecodeInfo globalInfo;

                    globalInfo.sourceNames.clear();
                    globalInfo.typeNames.clear();
                    globalInfo.sfxNames.clear();

                    globalInfo.sourceNames.append("Blank Object");
                    globalInfo.typeNames.append("Blank Object");

                    for (auto &s : gcf.objects) {
                        globalInfo.typeNames.append(s.name);
                        globalInfo.sourceNames.append(s.script);
                    }
                    globalScriptCount = gcf.objects.count() + 1;

                    for (auto &s : gcf.soundFX) globalInfo.sfxNames.append(QFileInfo(s).baseName());
                    globalSFXCount = gcf.soundFX.count();

                    globalInfo.path = globalPath;
                    bytecodeList.append(globalInfo);

                    for (auto &v : gcf.globalVariables) variableNames.append(v.name);

                    printLog("Loading Global Bytecode: " + globalPath);

                    QList<QString> categoryChars = { "PS", "RS", "SS", "BS" };
                    for (int cat = 0; cat < 4; ++cat) {
                        auto &c = gcf.stageLists[cat];
                        for (int scn = 0; scn < c.scenes.count(); ++scn) {
                            auto &s = c.scenes[scn];

                            QString dirPath = dataPath + "/Stages/" + s.folder + "/";
                            RSDKv3::StageConfig scf;
                            scf.read(dirPath + "StageConfig.bin");
                            BytecodeInfo stageInfo;

                            for (auto &s : scf.objects) {
                                stageInfo.typeNames.append(s.name);
                                stageInfo.sourceNames.append(s.script);
                            }

                            for (auto &s : scf.soundFX)
                                stageInfo.sfxNames.append(QFileInfo(s).baseName());

                            if (mobileVer) {
                                stageInfo.path = dataPath + "/Scripts/Bytecode/" + s.folder + ".bin";
                            }
                            else {
                                char stageNo1 = ((scn / 100) + '0');
                                char stageNo2 = (((scn % 100) / 10) + '0');
                                char stageNo3 = ((scn % 10) + '0');

                                stageInfo.path = dataPath + "/Scripts/Bytecode/" + categoryChars[cat]
                                                 + stageNo1 + stageNo2 + stageNo3 + ".bin";
                            }
                            stageInfo.loadGlobals = scf.loadGlobalScripts;
                            bytecodeList.append(stageInfo);

                            printLog("Loading Stage Bytecode: " + s.name + " (" + stageInfo.path + ")");
                        }
                    }

                    setStatus(QString("loaded %1 bytecode files!").arg(bytecodeList.count()));
                }
            }
            else if (ui->selEngineType->currentIndex() == 1) {
                QString globalPath = dataPath + "/../Bytecode/GlobalCode.bin";
                mobileVer          = true;
                if (!QFile::exists(globalPath)) {
                    setStatus("no global bytecode found in folder! aborting extraction!");
                    return;
                }

                bytecodeList.clear();
                variableNames.clear();

                if (!QFile::exists(dataPath + "/Game/GameConfig.bin")) {
                    setStatus("no gameconfig found in folder! aborting extraction!");
                    return;
                }
                else {
                    RSDKv4::GameConfig gcf;
                    gcf.read(dataPath + "/Game/GameConfig.bin");

                    BytecodeInfo globalInfo;

                    globalInfo.sourceNames.clear();
                    globalInfo.typeNames.clear();
                    globalInfo.sfxNames.clear();

                    globalInfo.sourceNames.append("Blank Object");
                    globalInfo.typeNames.append("Blank Object");

                    for (auto &s : gcf.objects) {
                        globalInfo.typeNames.append(s.name);
                        globalInfo.sourceNames.append(s.script);
                    }
                    globalScriptCount = gcf.objects.count() + 1;

                    for (auto &s : gcf.soundFX) globalInfo.sfxNames.append(s.name);
                    globalSFXCount = gcf.soundFX.count();

                    globalInfo.path = globalPath;
                    bytecodeList.append(globalInfo);

                    for (auto &v : gcf.globalVariables) variableNames.append(v.name);

                    printLog("Loading Global Bytecode: " + globalPath);

                    for (int cat = 0; cat < 4; ++cat) {
                        auto &c = gcf.categories[cat];
                        for (int scn = 0; scn < c.scenes.count(); ++scn) {
                            auto &s = c.scenes[scn];

                            QString dirPath = dataPath + "/Stages/" + s.folder + "/";
                            RSDKv4::StageConfig scf;
                            scf.read(dirPath + "StageConfig.bin");
                            BytecodeInfo stageInfo;

                            for (auto &s : scf.objects) {
                                stageInfo.typeNames.append(s.name);
                                stageInfo.sourceNames.append(s.script);
                            }

                            for (auto &s : scf.soundFX) stageInfo.sfxNames.append(s.name);

                            stageInfo.path        = dataPath + "/../Bytecode/" + s.folder + ".bin";
                            stageInfo.loadGlobals = scf.loadGlobalScripts;
                            bytecodeList.append(stageInfo);

                            printLog("Loading Stage Bytecode: " + s.name + " (" + stageInfo.path + ")");
                        }
                    }

                    setStatus(QString("loaded %1 bytecode files!").arg(bytecodeList.count()));
                }
            }
        }
    });

    connect(ui->selExtractFolder, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Open Output Folder"), "", tr("Folder"));
        filedialog.setFileMode(QFileDialog::Directory);
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            QString outputPath = filedialog.selectedFiles()[0];

            if (ui->selEngineType->currentIndex() == 0) {
                decompilerv3.seperateFolders  = ui->sepFolders->isChecked();
                decompilerv3.useCustomAliases = ui->useCustomAliases->isChecked();
                if (decompilerv3.useCustomAliases)
                    decompilerv3.seperateFolders = true;
                decompilerv3.useHex    = ui->useHex->isChecked();
                decompilerv3.mobileVer = mobileVer;

                decompilerv3.globalScriptCount = globalScriptCount;
                decompilerv3.globalSFXCount    = globalSFXCount;
                QList<QString> globalFunctionScripts;
                QList<QString> globalFunctionNames;

                decompilerv3.variableNames = variableNames;

                for (int b = 0; b < bytecodeList.count(); ++b) {
                    decompilerv3.sourceNames.clear();
                    decompilerv3.typeNames.clear();
                    decompilerv3.sfxNames.clear();
                    decompilerv3.functionNames.clear();

                    decompilerv3.globalScriptCount = b ? globalScriptCount : 0;

                    // Add Global Info
                    if (b && bytecodeList[b].loadGlobals) {
                        for (auto &n : bytecodeList[0].sourceNames) decompilerv3.sourceNames.append(n);
                        for (auto &n : bytecodeList[0].typeNames) decompilerv3.typeNames.append(n);
                        for (auto &n : bytecodeList[0].sfxNames) decompilerv3.sfxNames.append(n);
                        for (auto &n : globalFunctionNames) decompilerv3.functionNames.append(n);

                        decompilerv3.globalFunctionCount = globalFunctionCount;
                        decompilerv3.globalScriptCount   = globalScriptCount;
                        decompilerv3.globalSFXCount      = globalSFXCount;
                    }
                    else {
                        if (!bytecodeList[b].loadGlobals && b) {
                            decompilerv3.sourceNames.append("Blank Object");
                            decompilerv3.typeNames.append("Blank Object");

                            for (auto &n : bytecodeList[0].sfxNames) decompilerv3.sfxNames.append(n);
                        }

                        decompilerv3.globalFunctionCount = 0;
                        decompilerv3.globalScriptCount   = 0;
                        decompilerv3.globalSFXCount      = globalSFXCount;
                    }

                    for (auto &n : bytecodeList[b].sourceNames) decompilerv3.sourceNames.append(n);
                    for (auto &n : bytecodeList[b].typeNames) decompilerv3.typeNames.append(n);
                    for (auto &n : bytecodeList[b].sfxNames) decompilerv3.sfxNames.append(n);

                    RSDKv3::Bytecode bc;
                    decompilerv3.scriptCode.clear();
                    decompilerv3.jumpTable.clear();
                    decompilerv3.scriptList.clear();
                    decompilerv3.functionList.clear();
                    decompilerv3.bytecodePath = "";
                    if (b && bytecodeList[b].loadGlobals) {
                        // Load Global Bytecode
                        bc.read(bytecodeList[0].path);
                        decompilerv3.scriptCode = bc.scriptCode;
                        decompilerv3.jumpTable  = bc.jumpTable;
                        decompilerv3.scriptList = bc.scriptList;

                        // Load Stage-Specific Bytecode
                        bc.read(bytecodeList[b].path);
                        decompilerv3.scriptCode.append(bc.scriptCode);
                        decompilerv3.jumpTable.append(bc.jumpTable);
                        decompilerv3.scriptList.append(bc.scriptList);
                        decompilerv3.functionList = bc.functionList;
                        decompilerv3.bytecodePath = bc.filePath;
                    }
                    else {
                        // Load Stage-Specific Bytecode
                        bc.read(bytecodeList[b].path);
                        decompilerv3.scriptCode   = bc.scriptCode;
                        decompilerv3.jumpTable    = bc.jumpTable;
                        decompilerv3.scriptList   = bc.scriptList;
                        decompilerv3.functionList = bc.functionList;
                        decompilerv3.bytecodePath = bc.filePath;
                    }
                    decompilerv3.decompile(outputPath);

                    if (!b) {
                        globalFunctionCount = bc.functionList.count();

                        globalFunctionNames.clear();
                        globalFunctionScripts.clear();
                        for (auto &n : decompilerv3.functionNames) globalFunctionNames.append(n);
                    }
                }
                setStatus("finished decompiling scripts!");
            }
            else if (ui->selEngineType->currentIndex() == 1) {
                decompilerv4.seperateFolders  = ui->sepFolders->isChecked();
                decompilerv4.useCustomAliases = ui->useCustomAliases->isChecked();
                if (decompilerv4.useCustomAliases)
                    decompilerv4.seperateFolders = true;
                decompilerv4.useHex    = ui->useHex->isChecked();
                decompilerv4.useOldOps = ui->useOldOps->isChecked();

                decompilerv4.globalScriptCount = globalScriptCount;
                decompilerv4.globalSFXCount    = globalSFXCount;
                QList<QString> globalFunctionScripts;
                QList<QString> globalFunctionNames;

                decompilerv4.variableNames = variableNames;
                QList<RSDKv4::Decompiler::StaticVarInfo> globalConstants;
                QList<RSDKv4::Decompiler::StaticVarInfo> globalArrays;

                for (int b = 0; b < bytecodeList.count(); ++b) {
                    decompilerv4.sourceNames.clear();
                    decompilerv4.typeNames.clear();
                    decompilerv4.sfxNames.clear();
                    decompilerv4.functionNames.clear();
                    decompilerv4.staticVars.clear();
                    decompilerv4.tables.clear();

                    decompilerv4.globalScriptCount = b ? globalScriptCount : 0;

                    // Add Global Info
                    if (b && bytecodeList[b].loadGlobals) {
                        for (auto &n : bytecodeList[0].sourceNames) decompilerv4.sourceNames.append(n);
                        for (auto &n : bytecodeList[0].typeNames) decompilerv4.typeNames.append(n);
                        for (auto &n : bytecodeList[0].sfxNames) decompilerv4.sfxNames.append(n);
                        for (auto &n : globalFunctionNames) decompilerv4.functionNames.append(n);
                        for (auto &n : globalConstants) decompilerv4.staticVars.append(n);
                        for (auto &n : globalArrays) decompilerv4.tables.append(n);

                        decompilerv4.globalFunctionCount = globalFunctionCount;
                        decompilerv4.globalStaticCount   = globalArrayCount;
                        decompilerv4.globalArrayCount    = globalConstantCount;
                        decompilerv4.globalScriptCount   = globalScriptCount;
                        decompilerv4.globalSFXCount      = globalSFXCount;

                        decompilerv4.lastOffset = globalOffset;
                    }
                    else {
                        if (!bytecodeList[b].loadGlobals && b) {
                            decompilerv4.sourceNames.append("Blank Object");
                            decompilerv4.typeNames.append("Blank Object");

                            for (auto &n : bytecodeList[0].sfxNames) decompilerv4.sfxNames.append(n);
                        }

                        decompilerv4.globalFunctionCount = 0;
                        decompilerv4.globalStaticCount   = 0;
                        decompilerv4.globalArrayCount    = 0;
                        decompilerv4.globalScriptCount   = 0;
                        decompilerv4.globalSFXCount      = globalSFXCount;

                        decompilerv4.lastOffset = 0;
                    }

                    for (auto &n : bytecodeList[b].sourceNames) decompilerv4.sourceNames.append(n);
                    for (auto &n : bytecodeList[b].typeNames) decompilerv4.typeNames.append(n);
                    for (auto &n : bytecodeList[b].sfxNames) decompilerv4.sfxNames.append(n);

                    RSDKv4::Bytecode bc;
                    decompilerv4.scriptCode.clear();
                    decompilerv4.jumpTable.clear();
                    decompilerv4.scriptList.clear();
                    decompilerv4.functionList.clear();
                    decompilerv4.bytecodePath = "";
                    if (b && bytecodeList[b].loadGlobals) {
                        // Load Global Bytecode
                        bc.read(bytecodeList[0].path);
                        decompilerv4.scriptCode = bc.scriptCode;
                        decompilerv4.jumpTable  = bc.jumpTable;
                        decompilerv4.scriptList = bc.scriptList;

                        // Load Stage-Specific Bytecode
                        bc.read(bytecodeList[b].path);
                        decompilerv4.scriptCode.append(bc.scriptCode);
                        decompilerv4.jumpTable.append(bc.jumpTable);
                        decompilerv4.scriptList.append(bc.scriptList);
                        decompilerv4.functionList = bc.functionList;
                        decompilerv4.bytecodePath = bc.filePath;
                    }
                    else {
                        // Load Stage-Specific Bytecode
                        bc.read(bytecodeList[b].path);
                        decompilerv4.scriptCode   = bc.scriptCode;
                        decompilerv4.jumpTable    = bc.jumpTable;
                        decompilerv4.scriptList   = bc.scriptList;
                        decompilerv4.functionList = bc.functionList;
                        decompilerv4.bytecodePath = bc.filePath;
                    }
                    decompilerv4.decompile(outputPath);

                    if (!b) {
                        globalFunctionCount = bc.functionList.count();
                        globalConstantCount = decompilerv4.staticVars.count();
                        globalArrayCount    = decompilerv4.tables.count();
                        globalOffset        = decompilerv4.lastOffset;

                        globalFunctionNames.clear();
                        globalFunctionScripts.clear();
                        globalConstants.clear();
                        globalArrays.clear();
                        for (auto &n : decompilerv4.functionNames) globalFunctionNames.append(n);
                        for (auto &n : decompilerv4.staticVars) globalConstants.append(n);
                        for (auto &n : decompilerv4.tables) globalArrays.append(n);
                    }
                }
                setStatus("finished decompiling scripts!");
            }
        }
    });

    connect(ui->loadTRBytecode, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Open Retro-Sonic Bytecode"), "",
                               tr("Retro-Sonic Bytecode files (*.rsf)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            variableNames.clear(); // hack :]
            variableNames.append(filedialog.selectedFiles()[0]);
        }
    });
    connect(ui->saveTRScript, &QPushButton::clicked, [this] {
        if (!variableNames.count())
            return;

        QFileDialog filedialog(this, tr("Save Script"), "", tr("Text Files (*.txt)"));
        filedialog.setFileMode(QFileDialog::Directory);
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            QString scriptPath = filedialog.selectedFiles()[0];

            RSDKv1::Script script(variableNames[0]);
            decompilerv1.decompile(script, scriptPath);
        }
    });
}

ScriptUnpacker::~ScriptUnpacker() { delete ui; }

#include "moc_scriptunpacker.cpp"
