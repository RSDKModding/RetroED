#include "includes.hpp"
#include "ui_scriptunpacker.h"

ScriptUnpacker::ScriptUnpacker(QWidget *parent) : QWidget(parent), ui(new Ui::ScriptUnpacker)
{
    ui->setupUi(this);

    connect(ui->selEngineType, &QComboBox::currentIndexChanged, [this](int i) {
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
            m_dataPath = filedialog.selectedFiles()[0];

            if (ui->selEngineType->currentIndex() == 0) {
                QString globalPath = m_dataPath + "/Scripts/Bytecode/GS000.bin";
                m_mobileVer        = false;
                if (!QFile::exists(globalPath)) {
                    globalPath  = m_dataPath + "/Scripts/Bytecode/GlobalCode.bin";
                    m_mobileVer = true;
                    if (!QFile::exists(globalPath)) {
                        setStatus("no global bytecode found in folder! aborting extraction!");
                        return;
                    }
                }

                m_bytecodeList.clear();
                m_variableNames.clear();

                if (!QFile::exists(m_dataPath + "/Game/GameConfig.bin")) {
                    setStatus("no gameconfig found in folder! aborting extraction!");
                    return;
                }
                else {
                    RSDKv3::Gameconfig gcf;
                    gcf.read(m_dataPath + "/Game/GameConfig.bin");

                    BytecodeInfo globalInfo;

                    globalInfo.m_sourceNames.clear();
                    globalInfo.m_typeNames.clear();
                    globalInfo.m_sfxNames.clear();

                    globalInfo.m_sourceNames.append("Blank Object");
                    globalInfo.m_typeNames.append("Blank Object");

                    for (auto &s : gcf.m_objects) {
                        globalInfo.m_typeNames.append(s.m_name);
                        globalInfo.m_sourceNames.append(s.m_script);
                    }
                    m_globalScriptCount = gcf.m_objects.count() + 1;

                    for (auto &s : gcf.m_soundFX) globalInfo.m_sfxNames.append(QFileInfo(s).baseName());
                    m_globalSFXCount = gcf.m_soundFX.count();

                    globalInfo.m_path = globalPath;
                    m_bytecodeList.append(globalInfo);

                    for (auto &v : gcf.m_globalVariables) m_variableNames.append(v.m_name);

                    qDebug() << "Loading Global Bytecode: " << globalPath;

                    QList<QString> categoryChars = { "PS", "RS", "SS", "BS" };
                    for (int cat = 0; cat < 4; ++cat) {
                        auto &c = gcf.m_categories[cat];
                        for (int scn = 0; scn < c.m_scenes.count(); ++scn) {
                            auto &s = c.m_scenes[scn];

                            QString dirPath = m_dataPath + "/Stages/" + s.m_folder + "/";
                            RSDKv3::Stageconfig scf;
                            scf.read(dirPath + "StageConfig.bin");
                            BytecodeInfo stageInfo;

                            for (auto &s : scf.m_objects) {
                                stageInfo.m_typeNames.append(s.m_name);
                                stageInfo.m_sourceNames.append(s.m_script);
                            }

                            for (auto &s : scf.m_soundFX)
                                stageInfo.m_sfxNames.append(QFileInfo(s).baseName());

                            if (m_mobileVer) {
                                stageInfo.m_path =
                                    m_dataPath + "/Scripts/Bytecode/" + s.m_folder + ".bin";
                            }
                            else {
                                char stageNo1 = ((scn / 100) + '0');
                                char stageNo2 = (((scn % 100) / 10) + '0');
                                char stageNo3 = ((scn % 10) + '0');

                                stageInfo.m_path = m_dataPath + "/Scripts/Bytecode/"
                                                   + categoryChars[cat] + stageNo1 + stageNo2 + stageNo3
                                                   + ".bin";
                            }
                            stageInfo.m_loadGlobals = scf.m_loadGlobalScripts;
                            m_bytecodeList.append(stageInfo);

                            qDebug() << "Loading Stage Bytecode: " + s.m_name + " (" + stageInfo.m_path
                                            + ")";
                        }
                    }

                    appConfig.addRecentFile(ENGINE_v3, TOOL_SCRIPTUNPACKER, m_dataPath,
                                            QList<QString>{});
                    setStatus(QString("loaded %1 bytecode files!").arg(m_bytecodeList.count()));
                }
            }
            else if (ui->selEngineType->currentIndex() == 1) {
                QString globalPath = m_dataPath + "/../Bytecode/GlobalCode.bin";
                m_mobileVer        = true;
                if (!QFile::exists(globalPath)) {
                    setStatus("no global bytecode found in folder! aborting extraction!");
                    return;
                }

                m_bytecodeList.clear();
                m_variableNames.clear();

                if (!QFile::exists(m_dataPath + "/Game/GameConfig.bin")) {
                    setStatus("no gameconfig found in folder! aborting extraction!");
                    return;
                }
                else {
                    RSDKv4::Gameconfig gcf;
                    gcf.read(m_dataPath + "/Game/GameConfig.bin");

                    BytecodeInfo globalInfo;

                    globalInfo.m_sourceNames.clear();
                    globalInfo.m_typeNames.clear();
                    globalInfo.m_sfxNames.clear();

                    globalInfo.m_sourceNames.append("Blank Object");
                    globalInfo.m_typeNames.append("Blank Object");

                    for (auto &s : gcf.m_objects) {
                        globalInfo.m_typeNames.append(s.m_name);
                        globalInfo.m_sourceNames.append(s.m_script);
                    }
                    m_globalScriptCount = gcf.m_objects.count() + 1;

                    for (auto &s : gcf.m_soundFX) globalInfo.m_sfxNames.append(s.m_name);
                    m_globalSFXCount = gcf.m_soundFX.count();

                    globalInfo.m_path = globalPath;
                    m_bytecodeList.append(globalInfo);

                    for (auto &v : gcf.m_globalVariables) m_variableNames.append(v.m_name);

                    qDebug() << "Loading Global Bytecode: " << globalPath;

                    for (int cat = 0; cat < 4; ++cat) {
                        auto &c = gcf.m_categories[cat];
                        for (int scn = 0; scn < c.m_scenes.count(); ++scn) {
                            auto &s = c.m_scenes[scn];

                            QString dirPath = m_dataPath + "/Stages/" + s.m_folder + "/";
                            RSDKv4::Stageconfig scf;
                            scf.read(dirPath + "StageConfig.bin");
                            BytecodeInfo stageInfo;

                            for (auto &s : scf.m_objects) {
                                stageInfo.m_typeNames.append(s.m_name);
                                stageInfo.m_sourceNames.append(s.m_script);
                            }

                            for (auto &s : scf.m_soundFX) stageInfo.m_sfxNames.append(s.m_name);

                            stageInfo.m_path = m_dataPath + "/../Bytecode/" + s.m_folder + ".bin";
                            stageInfo.m_loadGlobals = scf.m_loadGlobalScripts;
                            m_bytecodeList.append(stageInfo);

                            qDebug() << "Loading Stage Bytecode: " + s.m_name + " (" + stageInfo.m_path
                                            + ")";
                        }
                    }

                    appConfig.addRecentFile(ENGINE_v4, TOOL_SCRIPTUNPACKER, m_dataPath,
                                            QList<QString>{});
                    setStatus(QString("loaded %1 bytecode files!").arg(m_bytecodeList.count()));
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
                m_decompilerv3.m_seperateFolders  = ui->sepFolders->isChecked();
                m_decompilerv3.m_useCustomAliases = ui->useCustomAliases->isChecked();
                if (m_decompilerv3.m_useCustomAliases)
                    m_decompilerv3.m_seperateFolders = true;
                m_decompilerv3.m_useHex    = ui->useHex->isChecked();
                m_decompilerv3.m_mobileVer = m_mobileVer;

                m_decompilerv3.m_globalScriptCount = m_globalScriptCount;
                m_decompilerv3.m_globalSFXCount    = m_globalSFXCount;
                QList<QString> m_globalFunctionScripts;
                QList<QString> m_globalFunctionNames;

                m_decompilerv3.m_variableNames = m_variableNames;

                for (int b = 0; b < m_bytecodeList.count(); ++b) {
                    m_decompilerv3.m_sourceNames.clear();
                    m_decompilerv3.m_typeNames.clear();
                    m_decompilerv3.m_sfxNames.clear();
                    m_decompilerv3.m_functionNames.clear();

                    m_decompilerv3.m_globalScriptCount = b ? m_globalScriptCount : 0;

                    // Add Global Info
                    if (b && m_bytecodeList[b].m_loadGlobals) {
                        for (auto &n : m_bytecodeList[0].m_sourceNames)
                            m_decompilerv3.m_sourceNames.append(n);
                        for (auto &n : m_bytecodeList[0].m_typeNames)
                            m_decompilerv3.m_typeNames.append(n);
                        for (auto &n : m_bytecodeList[0].m_sfxNames)
                            m_decompilerv3.m_sfxNames.append(n);
                        for (auto &n : m_globalFunctionNames) m_decompilerv3.m_functionNames.append(n);

                        m_decompilerv3.m_globalFunctionCount = m_globalFunctionCount;
                        m_decompilerv3.m_globalScriptCount   = m_globalScriptCount;
                        m_decompilerv3.m_globalSFXCount      = m_globalSFXCount;
                    }
                    else {
                        if (!m_bytecodeList[b].m_loadGlobals && b) {
                            m_decompilerv3.m_sourceNames.append("Blank Object");
                            m_decompilerv3.m_typeNames.append("Blank Object");

                            for (auto &n : m_bytecodeList[0].m_sfxNames)
                                m_decompilerv3.m_sfxNames.append(n);
                        }

                        m_decompilerv3.m_globalFunctionCount = 0;
                        m_decompilerv3.m_globalScriptCount   = 0;
                        m_decompilerv3.m_globalSFXCount      = m_globalSFXCount;
                    }

                    for (auto &n : m_bytecodeList[b].m_sourceNames)
                        m_decompilerv3.m_sourceNames.append(n);
                    for (auto &n : m_bytecodeList[b].m_typeNames) m_decompilerv3.m_typeNames.append(n);
                    for (auto &n : m_bytecodeList[b].m_sfxNames) m_decompilerv3.m_sfxNames.append(n);

                    RSDKv3::Bytecode bc;
                    if (b && m_bytecodeList[b].m_loadGlobals) {
                        bc.read(m_bytecodeList[0].m_path, 1);
                        bc.read(m_bytecodeList[b].m_path, m_globalScriptCount, false);
                    }
                    else {
                        bc.read(m_bytecodeList[b].m_path, 1);
                    }
                    m_decompilerv3.decompile(bc, outputPath);

                    if (!b) {
                        m_globalFunctionCount = bc.m_functionList.count();

                        m_globalFunctionNames.clear();
                        m_globalFunctionScripts.clear();
                        for (auto &n : m_decompilerv3.m_functionNames) m_globalFunctionNames.append(n);
                    }
                }
                setStatus("finished decompiling scripts!");
            }
            else if (ui->selEngineType->currentIndex() == 1) {
                m_decompilerv4.m_seperateFolders  = ui->sepFolders->isChecked();
                m_decompilerv4.m_useCustomAliases = ui->useCustomAliases->isChecked();
                if (m_decompilerv4.m_useCustomAliases)
                    m_decompilerv4.m_seperateFolders = true;
                m_decompilerv4.m_useHex    = ui->useHex->isChecked();
                m_decompilerv4.m_mobileVer = m_mobileVer;

                m_decompilerv4.m_globalScriptCount = m_globalScriptCount;
                m_decompilerv4.m_globalSFXCount    = m_globalSFXCount;
                QList<QString> m_globalFunctionScripts;
                QList<QString> m_globalFunctionNames;

                m_decompilerv4.m_variableNames = m_variableNames;
                QList<RSDKv4::Decompiler::StaticVarInfo> m_globalConstants;
                QList<RSDKv4::Decompiler::StaticVarInfo> m_globalArrays;

                for (int b = 0; b < m_bytecodeList.count(); ++b) {
                    m_decompilerv4.m_sourceNames.clear();
                    m_decompilerv4.m_typeNames.clear();
                    m_decompilerv4.m_sfxNames.clear();
                    m_decompilerv4.m_functionNames.clear();
                    m_decompilerv4.m_staticVars.clear();
                    m_decompilerv4.m_tables.clear();

                    m_decompilerv4.m_globalScriptCount = b ? m_globalScriptCount : 0;

                    // Add Global Info
                    if (b && m_bytecodeList[b].m_loadGlobals) {
                        for (auto &n : m_bytecodeList[0].m_sourceNames)
                            m_decompilerv4.m_sourceNames.append(n);
                        for (auto &n : m_bytecodeList[0].m_typeNames)
                            m_decompilerv4.m_typeNames.append(n);
                        for (auto &n : m_bytecodeList[0].m_sfxNames)
                            m_decompilerv4.m_sfxNames.append(n);
                        for (auto &n : m_globalFunctionNames) m_decompilerv4.m_functionNames.append(n);
                        for (auto &n : m_globalConstants) m_decompilerv4.m_staticVars.append(n);
                        for (auto &n : m_globalArrays) m_decompilerv4.m_tables.append(n);

                        m_decompilerv4.m_globalFunctionCount = m_globalFunctionCount;
                        m_decompilerv4.m_globalStaticCount   = m_globalArrayCount;
                        m_decompilerv4.m_globalArrayCount    = m_globalConstantCount;
                        m_decompilerv4.m_globalScriptCount   = m_globalScriptCount;
                        m_decompilerv4.m_globalSFXCount      = m_globalSFXCount;

                        m_decompilerv4.m_lastOffset = m_globalOffset;
                    }
                    else {
                        if (!m_bytecodeList[b].m_loadGlobals && b) {
                            m_decompilerv4.m_sourceNames.append("Blank Object");
                            m_decompilerv4.m_typeNames.append("Blank Object");

                            for (auto &n : m_bytecodeList[0].m_sfxNames)
                                m_decompilerv4.m_sfxNames.append(n);
                        }

                        m_decompilerv4.m_globalFunctionCount = 0;
                        m_decompilerv4.m_globalStaticCount   = 0;
                        m_decompilerv4.m_globalArrayCount    = 0;
                        m_decompilerv4.m_globalScriptCount   = 0;
                        m_decompilerv4.m_globalSFXCount      = m_globalSFXCount;

                        m_decompilerv4.m_lastOffset = 0;
                    }

                    for (auto &n : m_bytecodeList[b].m_sourceNames)
                        m_decompilerv4.m_sourceNames.append(n);
                    for (auto &n : m_bytecodeList[b].m_typeNames) m_decompilerv4.m_typeNames.append(n);
                    for (auto &n : m_bytecodeList[b].m_sfxNames) m_decompilerv4.m_sfxNames.append(n);

                    RSDKv4::Bytecode bc;
                    if (b && m_bytecodeList[b].m_loadGlobals) {
                        bc.read(m_bytecodeList[0].m_path, 1);
                        bc.read(m_bytecodeList[b].m_path, m_globalScriptCount, false);
                    }
                    else {
                        bc.read(m_bytecodeList[b].m_path, 1);
                    }
                    m_decompilerv4.decompile(bc, outputPath);

                    if (!b) {
                        m_globalFunctionCount = bc.m_functionList.count();
                        m_globalConstantCount = m_decompilerv4.m_staticVars.count();
                        m_globalArrayCount    = m_decompilerv4.m_tables.count();
                        m_globalOffset        = m_decompilerv4.m_lastOffset;

                        m_globalFunctionNames.clear();
                        m_globalFunctionScripts.clear();
                        m_globalConstants.clear();
                        m_globalArrays.clear();
                        for (auto &n : m_decompilerv4.m_functionNames) m_globalFunctionNames.append(n);
                        for (auto &n : m_decompilerv4.m_staticVars) m_globalConstants.append(n);
                        for (auto &n : m_decompilerv4.m_tables) m_globalArrays.append(n);
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
