#include "includes.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    statusLabel = ui->status;

    icon_up   = QIcon(Utils::getColouredIcon(":/icons/ic_arrow_upward_48px.svg"));
    icon_down = QIcon(Utils::getColouredIcon(":/icons/ic_arrow_downward_48px.svg"));
    icon_add  = QIcon(Utils::getColouredIcon(":/icons/ic_add_circle_48px.svg"));
    icon_rm   = QIcon(Utils::getColouredIcon(":/icons/ic_cancel_48px.svg"));

    QShortcut *closeTab = new QShortcut(ui->toolTabs);
    closeTab->setKey(Qt::CTRL + Qt::Key_W);

    auto removeTab = [this](int t) {
        QWidget *w = ui->toolTabs->widget(t);
        ui->toolTabs->removeTab(t);
        w->close();
    };

    connect(closeTab, &QShortcut::activated,
            [this, removeTab] { removeTab(ui->toolTabs->currentIndex()); });

    connect(ui->toolTabs, &QTabWidget::tabCloseRequested, removeTab);

    QMenu *file    = new QMenu("File");
    auto newAction = [this] {
        if (!ui->toolTabs->currentWidget())
            return;

        RENewEvent e = RENewEvent();
        QApplication::sendEvent(ui->toolTabs->currentWidget(), &e);
    };
    file->addAction("New", newAction);

    auto openAction = [this] {
        if (!ui->toolTabs->currentWidget())
            return;

        REOpenEvent e = REOpenEvent();
        QApplication::sendEvent(ui->toolTabs->currentWidget(), &e);
    };
    file->addAction("Open", openAction);

    auto saveAction = [this] {
        if (!ui->toolTabs->currentWidget())
            return;

        RESaveEvent e = RESaveEvent();
        QApplication::sendEvent(ui->toolTabs->currentWidget(), &e);
    };
    file->addAction("Save", saveAction);

    auto saveAsAction = [this] {
        if (!ui->toolTabs->currentWidget())
            return;

        RESaveAsEvent e = RESaveAsEvent();
        QApplication::sendEvent(ui->toolTabs->currentWidget(), &e);
    };
    file->addAction("Save As", saveAsAction);

    // SHORTCUTS
    QShortcut *shortcut_new = new QShortcut(this);
    shortcut_new->setKey(Qt::CTRL + Qt::Key_N);
    connect(shortcut_new, &QShortcut::activated, [newAction] { newAction(); });
    QShortcut *shortcut_open = new QShortcut(this);
    shortcut_open->setKey(Qt::CTRL + Qt::Key_O);
    connect(shortcut_open, &QShortcut::activated, [openAction] { openAction(); });
    QShortcut *shortcut_save = new QShortcut(this);
    shortcut_save->setKey(Qt::CTRL + Qt::Key_S);
    connect(shortcut_save, &QShortcut::activated, [saveAction] { saveAction(); });
    QShortcut *shortcut_saveAs = new QShortcut(this);
    shortcut_saveAs->setKey(Qt::CTRL + Qt::ALT + Qt::Key_S);
    connect(shortcut_saveAs, &QShortcut::activated, [saveAsAction] { saveAsAction(); });

    file->addSeparator();

    QMenu *rfMenu = new QMenu("Recent Files");
    for (auto &r : appConfig.m_recentFiles) {
        rfMenu->addAction(r.m_path, [this, r] {
            switch (r.m_tool) {
                case TOOL_SCENEEDITOR: {
                    SceneEditor *tool = new SceneEditor();
                    tool->installEventFilter(this);
                    ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "Scene Editor"));

                    if (QFile::exists(r.m_path)
                        && (QFile::exists(r.m_extra[0]) || r.m_gameVer == ENGINE_v1))
                        tool->loadScene(r.m_path, r.m_extra[0], r.m_gameVer);

                    break;
                }
                case TOOL_SCRIPTUNPACKER: break;
                case TOOL_RSDKUNPACKER: break;
                case TOOL_PALETTEDITOR: {
                    if (QFile::exists(r.m_path)) {
                        PaletteEditor *tool = new PaletteEditor(r.m_path, r.m_gameVer);
                        tool->installEventFilter(this);
                        ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "Palette Editor"));
                    }
                    break;
                }
                case TOOL_GFXTOOL: break;
                case TOOL_GAMECONFIGEDITOR:
                    switch (r.m_gameVer) {
                        case ENGINE_v2: {
                            GameconfigEditorv2 *tool = new GameconfigEditorv2(r.m_path);
                            ui->toolTabs->setCurrentIndex(
                                ui->toolTabs->addTab(tool, "Gameconfig Editor"));
                            break;
                        }
                        case ENGINE_v3: {
                            GameconfigEditorv3 *tool = new GameconfigEditorv3(r.m_path);
                            ui->toolTabs->setCurrentIndex(
                                ui->toolTabs->addTab(tool, "Gameconfig Editor"));
                            break;
                        }
                        case ENGINE_v4: {
                            GameconfigEditorv4 *tool = new GameconfigEditorv4(r.m_path);
                            ui->toolTabs->setCurrentIndex(
                                ui->toolTabs->addTab(tool, "Gameconfig Editor"));
                            break;
                        }
                        case ENGINE_v5: {
                            GameconfigEditorv5 *tool =
                                new GameconfigEditorv5(r.m_path, r.m_extra[0] == "StageConfig");
                            ui->toolTabs->setCurrentIndex(
                                ui->toolTabs->addTab(tool, "Gameconfig Editor"));
                            break;
                        }
                    }

                    break;
                case TOOL_RSVUNPACKER: break;
                case TOOL_STATICOBJECTEDITOR: {
                    if (QFile::exists(r.m_path)) {
                        StaticObjectEditor *tool = new StaticObjectEditor(r.m_path);
                        tool->installEventFilter(this);
                        ui->toolTabs->setCurrentIndex(
                            ui->toolTabs->addTab(tool, "Static Object Editor"));
                    }
                    break;
                }
            }
        });
    }
    file->addMenu(rfMenu);

    file->addSeparator();
    file->addAction("Close", [this] {
        if (ui->toolTabs->currentWidget() != nullptr)
            ui->toolTabs->removeTab(ui->toolTabs->indexOf(ui->toolTabs->currentWidget()));
        else
            QApplication::closeAllWindows();
    });

    ui->menubar->addMenu(file);

    QMenu *tools = new QMenu("Tools");
    tools->addAction("RSDK Unpacker", [this] {
        setStatus("Opening RSDK Unpacker...");
        RSDKUnpacker *tool = new RSDKUnpacker();
        ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "RSDK Unpacker"));
    });
    tools->addSeparator();

    QMenu *scn = new QMenu("Scene Editor");
    scn->addAction("v1, v2, v3, v4", [this] {
        setStatus("Opening Scene Editor");
        SceneEditor *tool = new SceneEditor();
        tool->installEventFilter(this);
        ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "Scene Editor"));
    });
#if RETROED_VER >= 0x110 || RETROED_DEBUG
    scn->addAction("v5 (Sonic Mania)", [this] {
        setStatus("Opening Scene Editor");
        // SceneEditorv5 *tool = new SceneEditorv5();
        // tool->installEventFilter(this);
        // ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "Scene Editor"));
    });
#endif
    tools->addMenu(scn);

#if RETROED_VER >= 0x110 || RETROED_DEBUG
    tools->addAction("Animation Editor", [this] {
        setStatus("Opening Animation Editor...");
        // AnimationEditor *tool = new AnimationEditor();
        // ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "Animation Editor"));
    });
#endif

    QMenu *gc = new QMenu("Gameconfig Editor");
    gc->addAction("v2 (Sonic Nexus)", [this] {
        setStatus("Opening Gameconfig Editor...");
        GameconfigEditorv2 *tool = new GameconfigEditorv2();
        ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "Gameconfig Editor"));
    });
    gc->addAction("v3 (Sonic CD)", [this] {
        setStatus("Opening Gameconfig Editor...");
        GameconfigEditorv3 *tool = new GameconfigEditorv3();
        ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "Gameconfig Editor"));
    });
    gc->addAction("v4 (Sonic 1/Sonic 2)", [this] {
        setStatus("Opening Gameconfig Editor...");
        GameconfigEditorv4 *tool = new GameconfigEditorv4();
        ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "Gameconfig Editor"));
    });
    gc->addAction("v5 (Sonic Mania)", [this] {
        setStatus("Opening Gameconfig Editor...");
        GameconfigEditorv5 *tool = new GameconfigEditorv5("", 0);
        ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "Gameconfig Editor"));
    });
    tools->addMenu(gc);

    tools->addAction("Palette Editor", [this] {
        setStatus("Opening Palette Editor...");
        PaletteEditor *tool = new PaletteEditor();
        ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "Palette Editor"));
    });

    /*QMenu *se = new QMenu("SaveFile Editor");
    se->addAction("Sonic CD", [this] {
        setStatus("Opening SaveFile Editor...");
        SaveFileEditorv3 *tool = new SaveFileEditorv3();
        ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "SaveFile Editor"));
    });
    se->addAction("Sonic 1/Sonic 2", [this] {
        setStatus("Opening SaveFile Editor...");
        SaveFileEditorv4 *tool = new SaveFileEditorv4();
        ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "SaveFile Editor"));
    });
    tools->addMenu(se);*/

    tools->addAction("Static Object Editor", [this] {
        setStatus("Opening Static Object Editor...");
        StaticObjectEditor *tool = new StaticObjectEditor();
        ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "Static Object Editor"));
    });

#if RETROED_DEBUG
    tools->addAction("Script Unpacker", [this] {
        setStatus("Opening Script Unpacker...");
        ScriptUnpacker *tool = new ScriptUnpacker();
        ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "Script Unpacker"));
    });
#endif

    tools->addAction("GFX Tool", [this] {
        setStatus("Opening GFX Tool...");
        GFXTool *tool = new GFXTool();
        ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "GFX Tool"));
    });

    tools->addAction("RSV Tool", [this] {
        setStatus("Opening RSV Tool...");
        RSVTool *tool = new RSVTool();
        ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "RSV Tool"));
    });

    ui->menubar->addMenu(tools);

#ifndef Q_NO_PROCESS
    QMenu *gameManager = new QMenu("Game Manager");
    gameManager->addAction("Change executable", [this] {
        QFileDialog filedialog(this, tr("Open Executable"), "",
                               "Windows Executables (*.exe)"); // TODO: multiplatform support
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted)
            m_exePath = filedialog.selectedFiles()[0];
    });

    auto runGame = [this] {
        if (QFile::exists(m_exePath)) {
            QStringList args;
            args << m_exePath;
            QProcess proc;
            proc.setProgram(m_exePath);
            proc.setWorkingDirectory(QFileInfo(m_exePath).absolutePath());
            proc.setArguments(args);
            proc.startDetached();
            proc.waitForStarted();
        }
    };

    gameManager->addAction("Run executable (F5)", runGame);
    ui->menubar->addMenu(gameManager);

    QShortcut *rgShort = new QShortcut(this);
    rgShort->setKey(Qt::Key_F5);
    connect(rgShort, &QShortcut::activated, runGame);
#endif

    QMenu *about = new QMenu("About");
    about->addAction("About RetroED", [] {
        QMessageBox msgBox(
            QMessageBox::Information, "RetroED",
            QString("RetroED - Retro Engine Editor v2.0\nGeneral Purpose Editor for RSDK "
                    "Files\n\nCreated by: Rubberduckycooly"),
            QMessageBox::NoButton);
        msgBox.exec();
    });
    ui->menubar->addMenu(about);
}

MainWindow::~MainWindow()
{
    statusLabel = nullptr;
    delete ui;
}

#include "moc_mainwindow.cpp"
