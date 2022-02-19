#include "includes.hpp"
#include "ui_modelmanager.h"

ModelManager::ModelManager(QString filePath, bool usev5Format, QWidget *parent)
    : QWidget(parent), ui(new Ui::ModelManager)
{
    ui->setupUi(this);

    viewer = new ModelViewer(this);
    viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->viewerFrame->layout()->addWidget(viewer);
    viewer->show();

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, QOverload<>::of(&ModelManager::processAnimation));

    connect(ui->loopIndex, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { viewer->loopIndex = v; });

    connect(ui->animSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](double v) { viewer->animSpeed = v; });

    connect(ui->useWireframe, &QCheckBox::toggled, [this](bool c) { viewer->setWireframe(c); });

    connect(ui->loadTexture, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Open Texture"), "", tr("PNG Files (*.png)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            viewer->loadTexture(filedialog.selectedFiles()[0]);
            ui->texPath->setText(filedialog.selectedFiles()[0]);
        }
    });

    ui->frameList->setCurrentRow(0);
    connect(ui->frameList, &QListWidget::currentRowChanged, [this](int r) {
        ui->animSpeed->setDisabled(r == -1);
        ui->loopIndex->setDisabled(r == -1);

        ui->upFrame->setDisabled(r == -1 || r - 1 < 0 || viewer->model.frames.count() <= 1);
        ui->downFrame->setDisabled(r == -1 || r + 1 >= viewer->model.frames.count()
                                   || viewer->model.frames.count() <= 1);
        ui->rmFrame->setDisabled(r == -1 || !viewer->model.frames.count());

        currentFrame = r;
        viewer->setFrame(r);
    });
    ui->frameList->setCurrentRow(-1);

    connect(ui->addFrame, &QToolButton::clicked, [this] {
        ui->frameList->blockSignals(true);
        uint c = ui->frameList->currentRow() + 1;
        int n  = ui->frameList->currentRow() == ui->frameList->count() - 1 ? c - 1 : c;

        switch (mdlFormat) {
            case 0: modelv5.frames.insert(c, RSDKv5::Model::Frame()); break;
            case 1: modelv4.frames.insert(c, RSDKv4::Model::Frame()); break;
        }

        viewer->model.frames.insert(c, RSDKv5::Model::Frame());

        setupUI(false);

        ui->frameList->blockSignals(true);
        ui->frameList->setCurrentRow(n);
        ui->frameList->blockSignals(false);

        viewer->repaint();
        // doAction("Added animation", true);
    });

    connect(ui->rmFrame, &QToolButton::clicked, [this] {
        int c = ui->frameList->currentRow();
        int n = ui->frameList->currentRow() == ui->frameList->count() - 1 ? c - 1 : c;
        delete ui->frameList->item(c);

        switch (mdlFormat) {
            case 0: modelv5.frames.removeAt(c); break;
            case 1: modelv4.frames.removeAt(c); break;
        }

        viewer->model.frames.removeAt(c);
        setupUI(false);

        ui->rmFrame->setDisabled(viewer->model.frames.count() <= 0);

        viewer->repaint();
        // doAction("Removed frame", true);

        ui->frameList->blockSignals(true);
        ui->frameList->setCurrentRow(n);
        ui->frameList->blockSignals(false);
    });

    auto moveFrame = [this](char translation) {
        uint c = ui->frameList->currentRow();
        uint n = ui->frameList->currentRow() + translation;
        if (n >= (uint)viewer->model.frames.count())
            return;

        switch (mdlFormat) {
            case 0: modelv5.frames.move(c, n); break;
            case 1: modelv4.frames.move(c, n); break;
        }

        viewer->model.frames.move(c, n);

        setupUI(false);

        viewer->repaint();
        // doAction("Moved frame", true);

        ui->frameList->setCurrentRow(n);
    };

    connect(ui->upFrame, &QToolButton::clicked, [moveFrame] { moveFrame(-1); });

    connect(ui->downFrame, &QToolButton::clicked, [moveFrame] { moveFrame(1); });

    connect(ui->copyFrame, &QToolButton::clicked, [this] {
        int c = ui->frameList->currentRow() + 1;
        if (currentFrame < viewer->model.frames.count()) {
            ui->frameList->blockSignals(true);

            viewer->model.frames.insert(c, viewer->model.frames[currentFrame]);

            switch (mdlFormat) {
                case 0: modelv5.frames.insert(c, modelv5.frames[currentFrame]); break;
                case 1: modelv4.frames.insert(c, modelv4.frames[currentFrame]); break;
            }

            setupUI(false);

            ui->frameList->blockSignals(true);
            ui->frameList->setCurrentRow(c);
            ui->frameList->blockSignals(false);

            // doAction("Copied frame", true);
        }
    });

    connect(ui->impMDL, &QPushButton::pressed, [this] {
        QFileDialog filedialog(this, tr("Load Model Frame"), "",
                               tr(QString("OBJ Models (*.obj)").toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            QString selFile = filedialog.selectedFiles()[0];

            // Load Obj file :smile:
        }
    });

    connect(ui->expMDL, &QPushButton::pressed, [this] {
        QFileDialog filedialog(this, tr("Save Model Frames"), "",
                               tr(QString("OBJ Models (*.obj)").toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            QString selFile = filedialog.selectedFiles()[0];

            switch (mdlFormat) {
                default: break;
                case 0: modelv5.writeAsOBJ(selFile); break;
                case 1: modelv4.writeAsOBJ(selFile); break;
            }
        }
    });

    /*connect(ui->exportCurFrame, &QPushButton::pressed, [this] {
        QFileDialog filedialog(this, tr("Save Model Frame"), "",
                               tr(QString("OBJ Models (*.obj)").toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            QString selFile = filedialog.selectedFiles()[0];

            switch (mdlFormat) {
                default: break;
                case 0: modelv5.writeAsOBJ(selFile, -1); break;
                case 1: modelv4.writeAsOBJ(selFile, -1); break;
            }
        }
    });*/

    ui->play->setIcon(playPauseIco[0]);
    connect(ui->play, &QToolButton::clicked, [this] {
        if (!playingAnim) {
            startAnim();
            ui->play->setIcon(playPauseIco[1]);
        }
        else {
            stopAnim();
            ui->frameList->setCurrentRow(currentFrame);
            ui->play->setIcon(playPauseIco[0]);
        }

        if (currentFrame >= viewer->model.frames.count()) {
            currentFrame = 0;
        }
    });

    connect(ui->prevFrame, &QToolButton::clicked, [this] {
        if (currentFrame > 0) {
            --currentFrame;
        }
        else {
            currentFrame = viewer->model.frames.count() - 1;
        }
        ui->frameList->setCurrentRow(currentFrame);
    });

    connect(ui->nextFrame, &QToolButton::clicked, [this] {
        if (currentFrame < viewer->model.frames.count() - 1) {
            ++currentFrame;
        }
        else {
            currentFrame = 0;
        }
        ui->frameList->setCurrentRow(currentFrame);
    });

    connect(ui->skipStart, &QToolButton::clicked, [this] {
        currentFrame = 0;
        ui->frameList->setCurrentRow(currentFrame);
    });

    connect(ui->skipEnd, &QToolButton::clicked, [this] {
        currentFrame = viewer->model.frames.count() - 1;
        ui->frameList->setCurrentRow(currentFrame);
    });

    mdlClrSel = new color_widgets::ColorPreview(this);
    ui->mdlClrFrame->layout()->addWidget(mdlClrSel);

    mdlClrSel->setColor(viewer->modelColour);
    connect(mdlClrSel, &color_widgets::ColorPreview::clicked, [this] {
        ColourDialog dlg(viewer->modelColour);
        if (dlg.exec() == QDialog::Accepted) {
            viewer->modelColour = dlg.colour().toQColor();
            mdlClrSel->setColor(viewer->modelColour);
        }
    });

    for (QWidget *w : findChildren<QWidget *>()) {
        w->installEventFilter(this);
    }

    if (QFile::exists(filePath))
        loadModel(filePath, usev5Format);
}

ModelManager::~ModelManager()
{
    delete ui;

    if (mdlClrSel)
        delete mdlClrSel;
    mdlClrSel = nullptr;

    if (updateTimer) {
        updateTimer->stop();
        delete updateTimer;
    }
    updateTimer = nullptr;
}

bool ModelManager::event(QEvent *event)
{
    switch ((int)event->type()) {
        case RE_EVENT_NEW:
            modelv4  = RSDKv4::Model();
            modelv5  = RSDKv5::Model();
            tabTitle = "Model Manager";
            setupUI();
            return true;

        case RE_EVENT_OPEN: {
            QString filters = { "RSDKv5 Model Files (*.bin);;RSDKv4 Model Files (*.bin)" };

            QFileDialog filedialog(this, tr("Open RSDK Model"), "", tr(filters.toStdString().c_str()));
            filedialog.setAcceptMode(QFileDialog::AcceptOpen);
            if (filedialog.exec() == QDialog::Accepted) {
                loadModel(filedialog.selectedFiles()[0],
                          filedialog.selectedNameFilter() == "RSDKv5 Model Files (*.bin)");
                return true;
            }
            break;
        }

        case RE_EVENT_SAVE:
            if (saveModel())
                return true;
            break;

        case RE_EVENT_SAVE_AS:
            if (saveModel(true))
                return true;
            break;

        case QEvent::Close:
            if (modified) {
                bool cancelled = false;
                if (MainWindow::showCloseWarning(this, &cancelled)) {
                    if (saveModel())
                        return true;
                }
                else if (cancelled) {
                    event->ignore();
                    return true;
                }
            }
            break;

        case QEvent::Wheel: {
            QWheelEvent *wEvent = static_cast<QWheelEvent *>(event);

            if (wEvent->angleDelta().y() > 0 && viewer->zoom < 20)
                viewer->zoom *= 2;
            else if (wEvent->angleDelta().y() < 0 && viewer->zoom > 0.5)
                viewer->zoom /= 2;

            viewer->setZoom(viewer->zoom);
            return true;
        }
    }

    return QWidget::event(event);
}

bool ModelManager::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FocusIn || event->type() == QEvent::FocusOut) {
        QApplication::sendEvent(this, event);
        return false;
    }

    if (object != viewer)
        return false;

    switch ((int)event->type()) {
        default: break;
    }

    return false;
}

void ModelManager::startAnim()
{
    playingAnim       = true;
    animFinished      = false;
    viewer->animTimer = 0;
    prevFrame         = 0;
    updateTimer->start(1000.0f / 60.0f);
}

void ModelManager::stopAnim()
{
    playingAnim       = false;
    animFinished      = false;
    viewer->animTimer = 0;
    prevFrame         = 0;

    updateTimer->stop();
}

void ModelManager::processAnimation()
{
    bool changed = false;

    if (currentFrame < viewer->model.frames.count()) {
        viewer->animTimer += viewer->animSpeed;
        while (viewer->animTimer > 1.0) {
            viewer->animTimer -= 1.0;
            ++currentFrame;

            if (currentFrame >= viewer->model.frames.count())
                currentFrame = viewer->loopIndex;

            changed = true;
        }
    }

    viewer->setFrame(currentFrame);
    if (changed)
        viewer->repaint();
}

void ModelManager::setupUI(bool initialSetup)
{
    ui->frameList->blockSignals(true);

    ui->frameList->clear();
    switch (mdlFormat) {
        default: break;
        case 0: {
            for (int f = 0; f < modelv5.frames.count(); ++f)
                ui->frameList->addItem("Frame " + QString::number(f));

            if (initialSetup) {
                ui->animSpeed->blockSignals(true);
                ui->animSpeed->setValue(0.1);
                viewer->animSpeed = 0.1f;
                ui->animSpeed->blockSignals(false);

                ui->loopIndex->blockSignals(true);
                ui->loopIndex->setValue(0);
                viewer->loopIndex = 0;
                ui->loopIndex->blockSignals(false);

                ui->frameList->setCurrentRow(-1);

                viewer->setModel(modelv5);
            }
            break;
        }

        case 1: {
            for (int f = 0; f < modelv4.frames.count(); ++f)
                ui->frameList->addItem("Frame " + QString::number(f));

            if (initialSetup) {
                ui->animSpeed->blockSignals(true);
                ui->animSpeed->setValue(0.1);
                viewer->animSpeed = 0.1f;
                ui->animSpeed->blockSignals(false);

                ui->loopIndex->blockSignals(true);
                ui->loopIndex->setValue(0);
                viewer->loopIndex = 0;
                ui->loopIndex->blockSignals(false);

                ui->frameList->setCurrentRow(-1);

                viewer->setModel(modelv4);
            }
            break;
        }
    }

    ui->frameList->blockSignals(false);
}

void ModelManager::loadModel(QString filePath, bool usev5Format)
{
    setStatus("Loading model...", true);
    if (usev5Format) {
        mdlFormat = 0;
        modelv5.read(filePath);
        tabTitle = Utils::getFilenameAndFolder(modelv5.filePath);
    }
    else {
        mdlFormat = 1;
        modelv4.read(filePath);
        tabTitle = Utils::getFilenameAndFolder(modelv4.filePath);
    }
    updateTitle(false);
    setStatus("Loaded model " + tabTitle);

    appConfig.addRecentFile(mdlFormat == 0 ? ENGINE_v5 : ENGINE_v4, TOOL_MODELMANAGER, filePath,
                            QList<QString>{});
    setupUI();
}

bool ModelManager::saveModel(bool forceSaveAs)
{
    switch (mdlFormat) {
        default: break;
        case 0: {
            if (forceSaveAs || modelv5.filePath.isEmpty()) {
                QFileDialog filedialog(this, tr("Save RSDK Model"), "",
                                       tr("RSDKv5 Model Files (*.bin)"));
                filedialog.setAcceptMode(QFileDialog::AcceptSave);
                if (filedialog.exec() == QDialog::Accepted) {
                    QString filepath = filedialog.selectedFiles()[0];

                    setStatus("Saving model...", true);
                    modelv5.write(filepath);
                    setStatus("Saved model to " + filepath);

                    appConfig.addRecentFile(mdlFormat == 0 ? ENGINE_v5 : ENGINE_v4, TOOL_MODELMANAGER,
                                            filepath, QList<QString>{});
                    updateTitle(false);
                    return true;
                }
            }
            else {
                setStatus("Saving model...", true);
                modelv5.write();
                setStatus("Saved model to " + modelv5.filePath);

                appConfig.addRecentFile(mdlFormat == 0 ? ENGINE_v5 : ENGINE_v4, TOOL_MODELMANAGER,
                                        modelv5.filePath, QList<QString>{});
                updateTitle(false);
                return true;
            }
            break;
        }

        case 1: {
            if (forceSaveAs || modelv4.filePath.isEmpty()) {
                QFileDialog filedialog(this, tr("Save RSDK Model"), "",
                                       tr("RSDKv4 Model Files (*.bin)"));
                filedialog.setAcceptMode(QFileDialog::AcceptSave);
                if (filedialog.exec() == QDialog::Accepted) {
                    QString filepath = filedialog.selectedFiles()[0];

                    setStatus("Saving model...", true);
                    modelv4.write(filepath);
                    setStatus("Saved model to " + filepath);

                    appConfig.addRecentFile(mdlFormat == 0 ? ENGINE_v5 : ENGINE_v4, TOOL_MODELMANAGER,
                                            filepath, QList<QString>{});
                    updateTitle(false);
                    return true;
                }
            }
            else {
                setStatus("Saving model...", true);
                modelv4.write();
                setStatus("Saved model to " + modelv5.filePath);

                appConfig.addRecentFile(mdlFormat == 0 ? ENGINE_v5 : ENGINE_v4, TOOL_MODELMANAGER,
                                        modelv4.filePath, QList<QString>{});
                updateTitle(false);

                return true;
            }
            break;
        }
    }

    return false;
}

#include "moc_modelmanager.cpp"
