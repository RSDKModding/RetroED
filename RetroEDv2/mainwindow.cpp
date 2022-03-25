#include "includes.hpp"
#include "ui_mainwindow.h"

SceneEditor *scnEditor  = nullptr;
SceneEditorv5 *v5Editor = nullptr;

QIcon playPauseIco[2];

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    statusLabel    = ui->status;
    statusProgress = ui->statusProgress;
    toolTabs       = ui->toolTabs;

    statusProgress->hide();

    playPauseIco[0] = QIcon(":/icons/ic_play_arrow_48px.svg");
    playPauseIco[1] = QIcon(":/icons/ic_stop_48px.svg");

    connect(ui->toolTabs, &QTabWidget::currentChanged, [this](int) {
        scnEditor = qobject_cast<SceneEditor *>(ui->toolTabs->currentWidget());
        v5Editor  = qobject_cast<SceneEditorv5 *>(ui->toolTabs->currentWidget());
    });

    QShortcut *closeTab = new QShortcut(ui->toolTabs);
    closeTab->setKey(Qt::CTRL + Qt::Key_W);

    auto removeTab = [this](int t) {
        QWidget *w  = ui->toolTabs->widget(t);
        bool closed = w->close();
        if (closed) {
            ui->toolTabs->removeTab(t);
            delete w;
        }
    };

    connect(closeTab, &QShortcut::activated,
            [this, removeTab] { removeTab(ui->toolTabs->currentIndex()); });

    connect(ui->toolTabs, &QTabWidget::tabCloseRequested, removeTab);

    auto focusChangeEvent = [this](int index) {
        for (int w = 0; w < ui->toolTabs->count(); ++w) {
            QWidget *widget = ui->toolTabs->widget(w);

            REAppEvent e = REAppEvent(RE_EVENT_TAB_LOSE_FOCUS);
            QApplication::sendEvent(widget, &e);
        }

        if (index != -1) {
            QWidget *widget = ui->toolTabs->widget(index);

            REAppEvent e = REAppEvent(RE_EVENT_TAB_GAIN_FOCUS);
            QApplication::sendEvent(widget, &e);
        }
    };
    connect(ui->toolTabs, &QTabWidget::currentChanged, focusChangeEvent);

    QMenu *file    = new QMenu("File");
    auto newAction = [this] {
        if (!ui->toolTabs->currentWidget())
            return;

        REAppEvent e = REAppEvent(RE_EVENT_NEW);
        QApplication::sendEvent(ui->toolTabs->currentWidget(), &e);
    };
    file->addAction("New", newAction);

    auto openAction = [this] {
        if (!ui->toolTabs->currentWidget())
            return;

        REAppEvent e = REAppEvent(RE_EVENT_OPEN);
        QApplication::sendEvent(ui->toolTabs->currentWidget(), &e);
    };
    file->addAction("Open", openAction);

    auto saveAction = [this] {
        if (!ui->toolTabs->currentWidget())
            return;

        REAppEvent e = REAppEvent(RE_EVENT_SAVE);
        QApplication::sendEvent(ui->toolTabs->currentWidget(), &e);
    };
    file->addAction("Save", saveAction);

    auto saveAsAction = [this] {
        if (!ui->toolTabs->currentWidget())
            return;

        REAppEvent e = REAppEvent(RE_EVENT_SAVE_AS);
        QApplication::sendEvent(ui->toolTabs->currentWidget(), &e);
    };
    file->addAction("Save As", saveAsAction);

    auto undoAction = [this] {
        if (!ui->toolTabs->currentWidget())
            return;

        REAppEvent e = REAppEvent(RE_EVENT_UNDO);
        QApplication::sendEvent(ui->toolTabs->currentWidget(), &e);
    };
    // file->addAction("Undo", undoAction);

    auto redoAction = [this] {
        if (!ui->toolTabs->currentWidget())
            return;

        REAppEvent e = REAppEvent(RE_EVENT_REDO);
        QApplication::sendEvent(ui->toolTabs->currentWidget(), &e);
    };
    // file->addAction("Redo", redoAction);

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

    QShortcut *shortcut_undo = new QShortcut(this);
    shortcut_undo->setKey(Qt::CTRL + Qt::Key_Z);
    connect(shortcut_undo, &QShortcut::activated, [undoAction] { undoAction(); });

    QShortcut *shortcut_redo = new QShortcut(this);
    shortcut_redo->setKey(Qt::CTRL + Qt::Key_Y);
    connect(shortcut_redo, &QShortcut::activated, [redoAction] { redoAction(); });

    file->addSeparator();

    QMenu *rfMenu = new QMenu("Recent Files");
    connect(rfMenu, &QMenu::aboutToShow, [rfMenu, this] {
        rfMenu->clear();
        for (auto &r : appConfig.recentFiles) {
            if (!QFile(r.path).exists()) {
                appConfig.recentFiles.removeOne(r);
            }
            else {
                rfMenu->addAction(r.path, [this, r] {
                    switch (r.tool) {
                        case TOOL_SCENEEDITOR: {
                            if (r.gameVer == ENGINE_v5) {
                                if (QFile::exists(r.path) && QFile::exists(r.extra[0])) {
                                    SceneEditorv5 *tool = new SceneEditorv5();
                                    tool->installEventFilter(this);
                                    addTab(tool, "Scene Editor");
                                    tool->loadScene(r.path, r.extra[0],
                                                    r.extra.count() >= 2 && r.extra[1] == "1");
                                }
                                else
                                    appConfig.recentFiles.removeOne(r);
                            }
                            else {
                                if (QFile::exists(r.path)
                                    && (QFile::exists(r.extra[0]) || r.gameVer == ENGINE_v1)) {
                                    SceneEditor *tool = new SceneEditor();
                                    tool->installEventFilter(this);
                                    addTab(tool, "Scene Editor");
                                    tool->loadScene(r.path, r.extra[0], r.gameVer);
                                }
                                else
                                    appConfig.recentFiles.removeOne(r);
                            }

                            break;
                        }
                        case TOOL_ANIMATIONEDITOR: {
                            AnimationEditor *tool = new AnimationEditor(r.path, r.gameVer);
                            addTab(tool, "Animation Editor");
                            break;
                        }
                        case TOOL_RSDKUNPACKER: break;
                        case TOOL_PALETTEDITOR: {
                            PaletteEditor *tool = new PaletteEditor(r.path, r.gameVer);
                            addTab(tool, "Palette Editor");
                            break;
                        }
                        case TOOL_MODELMANAGER: {
                            ModelManager *tool = new ModelManager(r.path, r.gameVer == ENGINE_v5);
                            addTab(tool, "Model Manager");
                            break;
                        }
                        case TOOL_GAMECONFIGEDITOR:
                            switch (r.gameVer) {
                                case ENGINE_v1: {
                                    GameConfigEditorv1 *tool = new GameConfigEditorv1(r.path);
                                    addTab(tool, "GameConfig Editor");
                                    break;
                                }
                                case ENGINE_v2: {
                                    GameConfigEditorv2 *tool = new GameConfigEditorv2(r.path);
                                    addTab(tool, "GameConfig Editor");
                                    break;
                                }
                                case ENGINE_v3: {
                                    GameConfigEditorv3 *tool = new GameConfigEditorv3(r.path);
                                    addTab(tool, "GameConfig Editor");
                                    break;
                                }
                                case ENGINE_v4: {
                                    GameConfigEditorv4 *tool = new GameConfigEditorv4(r.path);
                                    addTab(tool, "GameConfig Editor");
                                    break;
                                }
                                case ENGINE_v5: {
                                    GameConfigEditorv5 *tool = new GameConfigEditorv5(
                                        r.path, r.extra[0] == "StageConfig", r.extra[1] == "rev01");
                                    addTab(tool, "GameConfig Editor");
                                    break;
                                }
                            }

                            break;
                    }
                });
            }
        }
    });
    file->addMenu(rfMenu);

    file->addSeparator();
    file->addAction("Close", [this, removeTab] {
        if (ui->toolTabs->currentWidget() != nullptr)
            removeTab(ui->toolTabs->currentIndex());
        else
            QApplication::closeAllWindows();
    });

    ui->menubar->addMenu(file);

    QMenu *tools = new QMenu("Tools");
    tools->addAction("RSDK Unpacker", [this] {
        setStatus("Opening RSDK unpacker...");
        RSDKUnpacker *tool = new RSDKUnpacker;
        ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "RSDK Unpacker"));
        setStatus("Opened RSDK unpacker!");
    });
    tools->addSeparator();

    QMenu *scn = new QMenu("Scene Editor");
    scn->addAction("v1, v2, v3, v4", [this] {
        setStatus("Opening Scene Editor...");
        SceneEditor *tool = new SceneEditor();
        tool->installEventFilter(this);
        tool->createNewScene();
        addTab(tool, "Scene Editor");
        setStatus("Opened Scene Editor!");
    });

    scn->addAction("v5 (Sonic Mania)", [this] {
        setStatus("Opening Scene Editor...");
        SceneEditorv5 *tool = new SceneEditorv5();
        tool->installEventFilter(this);
        tool->createNewScene();
        addTab(tool, "Scene Editor");
        setStatus("Opened Scene Editor!");
    });
    tools->addMenu(scn);

    tools->addAction("Animation Editor", [this] {
        setStatus("Opening Animation Editor...");
        AnimationEditor *tool = new AnimationEditor;
        addTab(tool, "Animation Editor");
        setStatus("Opened Animation Editor!");
    });

    QMenu *gc = new QMenu("GameConfig Editor");
    gc->addAction("v1 (Retro-Sonic)", [this] {
        setStatus("Opening Game Configuration Editor...");
        GameConfigEditorv1 *tool = new GameConfigEditorv1;
        addTab(tool, "GameConfig Editor");
        setStatus("Opened Game Configuration Editor!");
    });
    gc->addAction("v2 (Sonic Nexus)", [this] {
        setStatus("Opening GameConfig Editor...");
        GameConfigEditorv2 *tool = new GameConfigEditorv2;
        addTab(tool, "GameConfig Editor");
        setStatus("Opened GameConfig Editor!");
    });
    gc->addAction("v3 (Sonic CD)", [this] {
        setStatus("Opening GameConfig Editor...");
        GameConfigEditorv3 *tool = new GameConfigEditorv3;
        addTab(tool, "GameConfig Editor");
        setStatus("Opened GameConfig Editor!");
    });
    gc->addAction("v4 (Sonic 1/Sonic 2)", [this] {
        setStatus("Opening GameConfig Editor...");
        GameConfigEditorv4 *tool = new GameConfigEditorv4;
        addTab(tool, "GameConfig Editor");
        setStatus("Opened GameConfig Editor!");
    });
    gc->addAction("v5 (Sonic Mania)", [this] {
        setStatus("Opening GameConfig Editor...");
        GameConfigEditorv5 *tool = new GameConfigEditorv5("", 0, false);
        addTab(tool, "GameConfig Editor");
        setStatus("Opened GameConfig Editor!");
    });
    tools->addMenu(gc);

    tools->addAction("Palette Editor", [this] {
        setStatus("Opening Palette Editor...");
        PaletteEditor *tool = new PaletteEditor;
        addTab(tool, "Palette Editor");
        setStatus("Opened Palette Editor!");
    });

    tools->addAction("Model Manager", [this] {
        setStatus("Opening Model Manager...");
        ModelManager *tool = new ModelManager;
        tool->installEventFilter(this);
        addTab(tool, "Model Manager");
        setStatus("Opened Model Manager!");
    });

#if !RE_BUILD_TYPE
    tools->addAction("Script Decompiler", [] {
        setStatus("Opening Script Decompiler...");
        ScriptUnpacker *tool = new ScriptUnpacker();
        tool->exec();
    });
#endif

    tools->addAction("Script Compiler", [this] {
        setStatus("Opening Script Compiler...");
        ScriptCompiler *tool = new ScriptCompiler();
        ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "Script Compiler"));
        setStatus("Opened Script Compiler!");
    });

    tools->addAction("GFX Manager", [] {
        setStatus("Opening GFX Manager...");
        GFXTool *tool = new GFXTool();
        tool->exec();
    });

    tools->addAction("UserDB Manager", [] {
        setStatus("Opening UserDB Manager...");
        UserDBManager *tool = new UserDBManager();
        tool->exec();
    });

    tools->addAction("RSV Manager", [] {
        setStatus("Opening RSV Manager...");
        RSVTool *tool = new RSVTool();
        tool->exec();
    });

    ui->menubar->addMenu(tools);

    QMenu *options                = new QMenu("Options");
    WorkingDirManager::workingDir = "";
    options->addAction("Set Base Data Folder", [this] {
        QFileDialog filedialog(this, tr("Open Directory"), "", "");
        filedialog.setFileMode(QFileDialog::FileMode::Directory);
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            WorkingDirManager::workingDir = filedialog.selectedFiles()[0] + "/";
        }
    });

    ui->menubar->addMenu(options);

#ifndef Q_NO_PROCESS
    QMenu *gameManager = new QMenu("Game Manager");
    gameManager->addAction("Change executable", [this] {
        QFileDialog filedialog(this, tr("Open Executable"), "",
                               "Windows Executables (*.exe);;All Files (*)");
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted)
            gamePath = filedialog.selectedFiles()[0];
    });

    auto runGame = [this] {
        if (QFile::exists(gamePath)) {
            QStringList args;
            args << "console=true;";
            QProcess proc;
            proc.setProgram(gamePath);
            proc.setWorkingDirectory(QFileInfo(gamePath).absolutePath());
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
        AboutWindow *about = new AboutWindow;
        about->exec();
        delete about;
    });
    ui->menubar->addMenu(about);
}

MainWindow::~MainWindow()
{
    statusLabel    = nullptr;
    statusProgress = nullptr;
    delete ui;
}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::Close) {
        if (toolTabs->count()) {
            bool canClose = true;
            for (int t = toolTabs->count() - 1; t >= 0; --t) {
                QWidget *w  = ui->toolTabs->widget(t);
                bool closed = w->close();
                if (closed) {
                    ui->toolTabs->removeTab(t);
                    delete w;
                }
                else {
                    canClose = false;
                }
            }

            if (!canClose) {
                event->ignore();
                return true;
            }
        }
    }

    return QWidget::event(event);
}

#include "moc_mainwindow.cpp"
