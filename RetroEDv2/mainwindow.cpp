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

    QMenu *file = new QMenu("File");
    file->addAction("Open", [this] {
        if (!ui->toolTabs->currentWidget())
            return;

        REOpenEvent e = REOpenEvent();
        QApplication::sendEvent(ui->toolTabs->currentWidget(), &e);
    });

    file->addAction("Save", [this] {
        if (!ui->toolTabs->currentWidget())
            return;

        RESaveEvent e = RESaveEvent();
        QApplication::sendEvent(ui->toolTabs->currentWidget(), &e);
    });

    file->addAction("Save As", [this] {
        if (!ui->toolTabs->currentWidget())
            return;

        RESaveAsEvent e = RESaveAsEvent();
        QApplication::sendEvent(ui->toolTabs->currentWidget(), &e);
    });

    file->addSeparator();

    QMenu *rfMenu = new QMenu("Recent Files");
    for (auto &r : appConfig.m_recentFiles) {
        rfMenu->addAction(r.m_path, [this, r] {
            switch (r.m_tool) {
                case TOOL_SCENEEDITOR: {
                    SceneEditor *tool = new SceneEditor();
                    tool->installEventFilter(this);
                    ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "Scene Editor"));

                    if (QFile::exists(r.m_path) && QFile::exists(r.m_extra[0]))
                        tool->loadScene(r.m_path, r.m_extra[0], r.m_gameVer);

                    break;
                }
                case TOOL_SCRIPTUNPACKER: break;
                case TOOL_RSDKUNPACKER: break;
                case TOOL_PALETTEDITOR: break;
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
                    }

                    break;
                case TOOL_RSVUNPACKER: break;
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

    tools->addAction("Scene Editor", [this] {
        setStatus("Opening Scene Editor");
        SceneEditor *tool = new SceneEditor();
        tool->installEventFilter(this);
        ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "Scene Editor"));
    });

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
    tools->addMenu(gc);

    tools->addAction("Script Unpacker", [this] {
        setStatus("Opening Script Unpacker...");
        ScriptUnpacker *tool = new ScriptUnpacker();
        ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "Script Unpacker"));
    });

    tools->addAction("GFX Tool", [this] {
        setStatus("Opening GFX Tool...");
        GFXTool *tool = new GFXTool();
        ui->toolTabs->setCurrentIndex(ui->toolTabs->addTab(tool, "GFX Tool"));
    });

    ui->menubar->addMenu(tools);

    QMenu *gameManager = new QMenu("Game Manager");
    gameManager->addAction("Change executable", [this] {
        QFileDialog filedialog(
            this, tr("Open Executable"), "",
            tr(QString("Sonic CD (soniccd*.exe);;Sonic Nexus (nexus*.exe);;Retro-Sonic (rsonic*.exe)")
                   .toStdString()
                   .c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            m_exePath = filedialog.selectedFiles()[0];
        }
    });

    gameManager->addAction("Run executable", [this] {
        QStringList args;
        QProcess::execute(m_exePath, args);
    });
    ui->menubar->addMenu(gameManager);

    QMenu *about = new QMenu("About");
    about->addAction("About RetroED", [this] {
        QMessageBox msgBox(
            QMessageBox::Information, "RetroED",
            QString("RetroED - Retro Engine Editor v2.0\nGeneral Purpose Editor for RSDK "
                    "versions below v5\n\nCreated by: Rubberduckycooly"),
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
