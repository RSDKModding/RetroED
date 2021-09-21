#include "includes.hpp"
#include "ui_scriptunpacker.h"

ScriptUnpacker::ScriptUnpacker(QWidget *parent) : QWidget(parent), ui(new Ui::ScriptUnpacker)
{
    ui->setupUi(this);

    connect(ui->selEngineType, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int i) {
        if (i >= 0)
            ui->stackedWidget->setCurrentIndex(i == 2);
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
                    RSDKv3::Gameconfig gcf;
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

                    qDebug() << "Loading Global Bytecode: " << globalPath;

                    QList<QString> categoryChars = { "PS", "RS", "SS", "BS" };
                    for (int cat = 0; cat < 4; ++cat) {
                        auto &c = gcf.categories[cat];
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

                            qDebug()
                                << "Loading Stage Bytecode: " + s.name + " (" + stageInfo.path + ")";
                        }
                    }

                    appConfig.addRecentFile(ENGINE_v3, TOOL_SCRIPTUNPACKER, dataPath, QList<QString>{});
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

                    qDebug() << "Loading Global Bytecode: " << globalPath;

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

                            qDebug()
                                << "Loading Stage Bytecode: " + s.name + " (" + stageInfo.path + ")";
                        }
                    }

                    appConfig.addRecentFile(ENGINE_v4, TOOL_SCRIPTUNPACKER, dataPath, QList<QString>{});
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
                decompilerv3.m_useHex    = ui->useHex->isChecked();
                decompilerv3.m_mobileVer = mobileVer;

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
                    if (b && bytecodeList[b].loadGlobals) {
                        bc.read(bytecodeList[0].path, 1);
                        bc.read(bytecodeList[b].path, globalScriptCount, false);
                    }
                    else {
                        bc.read(bytecodeList[b].path, 1);
                    }
                    decompilerv3.decompile(bc, outputPath);

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
                decompilerv4.m_seperateFolders  = ui->sepFolders->isChecked();
                decompilerv4.m_useCustomAliases = ui->useCustomAliases->isChecked();
                if (decompilerv4.m_useCustomAliases)
                    decompilerv4.m_seperateFolders = true;
                decompilerv4.m_useHex    = ui->useHex->isChecked();
                decompilerv4.m_useOldOps = ui->useOldOps->isChecked();

                decompilerv4.m_globalScriptCount = globalScriptCount;
                decompilerv4.m_globalSFXCount    = globalSFXCount;
                QList<QString> globalFunctionScripts;
                QList<QString> globalFunctionNames;

                decompilerv4.m_variableNames = variableNames;
                QList<RSDKv4::Decompiler::StaticVarInfo> m_globalConstants;
                QList<RSDKv4::Decompiler::StaticVarInfo> m_globalArrays;

                for (int b = 0; b < bytecodeList.count(); ++b) {
                    decompilerv4.m_sourceNames.clear();
                    decompilerv4.m_typeNames.clear();
                    decompilerv4.m_sfxNames.clear();
                    decompilerv4.m_functionNames.clear();
                    decompilerv4.m_staticVars.clear();
                    decompilerv4.m_tables.clear();

                    decompilerv4.m_globalScriptCount = b ? globalScriptCount : 0;

                    // Add Global Info
                    if (b && bytecodeList[b].loadGlobals) {
                        for (auto &n : bytecodeList[0].sourceNames)
                            decompilerv4.m_sourceNames.append(n);
                        for (auto &n : bytecodeList[0].typeNames) decompilerv4.m_typeNames.append(n);
                        for (auto &n : bytecodeList[0].sfxNames) decompilerv4.m_sfxNames.append(n);
                        for (auto &n : globalFunctionNames) decompilerv4.m_functionNames.append(n);
                        for (auto &n : m_globalConstants) decompilerv4.m_staticVars.append(n);
                        for (auto &n : m_globalArrays) decompilerv4.m_tables.append(n);

                        decompilerv4.m_globalFunctionCount = globalFunctionCount;
                        decompilerv4.m_globalStaticCount   = m_globalArrayCount;
                        decompilerv4.m_globalArrayCount    = m_globalConstantCount;
                        decompilerv4.m_globalScriptCount   = globalScriptCount;
                        decompilerv4.m_globalSFXCount      = globalSFXCount;

                        decompilerv4.m_lastOffset = m_globalOffset;
                    }
                    else {
                        if (!bytecodeList[b].loadGlobals && b) {
                            decompilerv4.m_sourceNames.append("Blank Object");
                            decompilerv4.m_typeNames.append("Blank Object");

                            for (auto &n : bytecodeList[0].sfxNames) decompilerv4.m_sfxNames.append(n);
                        }

                        decompilerv4.m_globalFunctionCount = 0;
                        decompilerv4.m_globalStaticCount   = 0;
                        decompilerv4.m_globalArrayCount    = 0;
                        decompilerv4.m_globalScriptCount   = 0;
                        decompilerv4.m_globalSFXCount      = globalSFXCount;

                        decompilerv4.m_lastOffset = 0;
                    }

                    for (auto &n : bytecodeList[b].sourceNames) decompilerv4.m_sourceNames.append(n);
                    for (auto &n : bytecodeList[b].typeNames) decompilerv4.m_typeNames.append(n);
                    for (auto &n : bytecodeList[b].sfxNames) decompilerv4.m_sfxNames.append(n);

                    RSDKv4::Bytecode bc;
                    if (b && bytecodeList[b].loadGlobals) {
                        bc.read(bytecodeList[0].path, 1);
                        bc.read(bytecodeList[b].path, globalScriptCount, false);
                    }
                    else {
                        bc.read(bytecodeList[b].path, 1);
                    }
                    decompilerv4.decompile(bc, outputPath);

                    if (!b) {
                        globalFunctionCount   = bc.functionList.count();
                        m_globalConstantCount = decompilerv4.m_staticVars.count();
                        m_globalArrayCount    = decompilerv4.m_tables.count();
                        m_globalOffset        = decompilerv4.m_lastOffset;

                        globalFunctionNames.clear();
                        globalFunctionScripts.clear();
                        m_globalConstants.clear();
                        m_globalArrays.clear();
                        for (auto &n : decompilerv4.m_functionNames) globalFunctionNames.append(n);
                        for (auto &n : decompilerv4.m_staticVars) m_globalConstants.append(n);
                        for (auto &n : decompilerv4.m_tables) m_globalArrays.append(n);
                    }
                }
                setStatus("finished decompiling scripts!");
            }
        }
    });

    connect(ui->loadTRBytecode, &QPushButton::clicked, [this] {});
    connect(ui->saveTRScript, &QPushButton::clicked, [this] {});
}

ScriptUnpacker::~ScriptUnpacker() { delete ui; }

#include "moc_scriptunpacker.cpp"
