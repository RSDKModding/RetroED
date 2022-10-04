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

    ui->vertCnt->setText(QString("Vertex Count: 0 Vertices"));
    ui->idxCnt->setText(QString("Index Count: 0 Indices"));
    ui->fVertCnt->setText(QString("Face Vertex Count: 3 Vertices"));

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, QOverload<>::of(&ModelManager::processAnimation));

    connect(ui->loopIndex, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
        int val = v;
        if (val >= viewer->model.frames.count())
            v = viewer->model.frames.count() - 1;
        if (val < 0)
            v = 0;

        if (val != v)
            ui->loopIndex->setValue(val);
        viewer->loopIndex = v;
    });

    connect(ui->animSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](double v) { viewer->animSpeed = v; });

    connect(ui->useWireframe, &QCheckBox::toggled, [this](bool c) { viewer->setWireframe(c); });
    connect(ui->showNormals, &QCheckBox::toggled, [this](bool c) { viewer->setNormalsVisible(c); });

    connect(ui->hasNormals, &QCheckBox::toggled, [this](bool c) { viewer->model.hasNormals = c; });
    connect(ui->hasColors, &QCheckBox::toggled, [this](bool c) { viewer->model.hasColors = c; });
    connect(ui->hasTextures, &QCheckBox::toggled, [this](bool c) { viewer->model.hasTextures = c; });

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

        viewer->model.frames.insert(c, RSDKv5::Model::Frame());

        if (viewer->model.frames.count() > 1) {
            for (int v = 0; v < viewer->model.frames[0].vertices.count(); ++v) {
                viewer->model.frames.last().vertices.append(RSDKv5::Model::Frame::Vertex());
            }
        }

        setupUI(false);

        ui->frameList->blockSignals(true);
        ui->frameList->setCurrentRow(n);
        ui->frameList->blockSignals(false);

        viewer->setFrame(n);

        viewer->repaint();
        // DoAction("Added animation", true);
    });

    connect(ui->rmFrame, &QToolButton::clicked, [this] {
        int c = ui->frameList->currentRow();
        int n = ui->frameList->currentRow() == ui->frameList->count() - 1 ? c - 1 : c;
        delete ui->frameList->item(c);

        viewer->model.frames.removeAt(c);
        setupUI(false);

        ui->rmFrame->setDisabled(viewer->model.frames.count() <= 0);

        viewer->setFrame(n);

        viewer->repaint();
        // DoAction("Removed frame", true);

        ui->frameList->blockSignals(true);
        ui->frameList->setCurrentRow(n);
        ui->frameList->blockSignals(false);
    });

    auto moveFrame = [this](char translation) {
        uint c = ui->frameList->currentRow();
        uint n = ui->frameList->currentRow() + translation;
        if (n >= (uint)viewer->model.frames.count())
            return;

        viewer->model.frames.move(c, n);

        setupUI(false);

        viewer->setFrame(n);

        viewer->repaint();
        // DoAction("Moved frame", true);

        ui->frameList->setCurrentRow(n);
    };

    connect(ui->upFrame, &QToolButton::clicked, [moveFrame] { moveFrame(-1); });

    connect(ui->downFrame, &QToolButton::clicked, [moveFrame] { moveFrame(1); });

    connect(ui->copyFrame, &QToolButton::clicked, [this] {
        int c = ui->frameList->currentRow() + 1;
        if (currentFrame < viewer->model.frames.count()) {
            ui->frameList->blockSignals(true);

            viewer->model.frames.insert(c, viewer->model.frames[currentFrame]);

            setupUI(false);

            ui->frameList->blockSignals(true);
            ui->frameList->setCurrentRow(c);
            ui->frameList->blockSignals(false);

            // DoAction("Copied frame", true);
        }
    });

    connect(ui->impMDL, &QPushButton::pressed, [this] {
        QFileDialog filedialog(this, tr("Load Model Frame"), "",
                               tr(QString("PLY Models (*.ply)").toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            QString selFile = filedialog.selectedFiles()[0];

            RSDKv5::Model mdl;
            // read
            mdl.loadPLY(selFile);
            mdl.filePath = "Imported Model";

            if (!mdl.frames.count()) {
                QMessageBox msgBox(
                    QMessageBox::Information, "RetroED",
                    QString("No Vertices were loaded from the imported model.\nAborting import."),
                    QMessageBox::NoButton);
                msgBox.exec();
                return;
            }

            if (viewer->model.frames.count() > 1 && mdl.frames.count() >= 1) {
                if (mdl.frames[0].vertices.count() != viewer->model.frames[0].vertices.count()) {
                    QMessageBox msgBox(QMessageBox::Information, "RetroED",
                                       QString("Different number of vertices between the "
                                               "frames.\nExpected %1 vertices, but loaded %2 "
                                               "vertices.\nAborting import.")
                                           .arg(viewer->model.frames[0].vertices.count())
                                           .arg(mdl.frames[0].vertices.count()),
                                       QMessageBox::NoButton);
                    msgBox.exec();
                    return;
                }

                if (mdl.indices.count() != viewer->model.indices.count()) {
                    QMessageBox msgBox(QMessageBox::Information, "RetroED",
                                       QString("Inconsistency in index count.\nExpected %1 "
                                               "indices, but loaded %2 "
                                               "indices.\nAborting import.")
                                           .arg(viewer->model.indices.count())
                                           .arg(mdl.indices.count()),
                                       QMessageBox::NoButton);
                    msgBox.exec();
                    return;
                }
            }

            uint c = ui->frameList->currentRow();
            if (!viewer->model.frames.count()
                || (ui->frameList->currentRow() <= 0 && viewer->model.frames.count() == 1)) {
                viewer->setModel(mdl);
            }
            else {
                if (c >= (uint)viewer->model.frames.count())
                    c = 0;

                viewer->model.frames[c].vertices.clear();
                for (auto &vertex : mdl.frames[0].vertices)
                    viewer->model.frames[c].vertices.append(vertex);

                viewer->setFrame(c);
            }

            setupUI(false);
            ui->frameList->setCurrentRow(c);
            viewer->repaint();
        }
    });

    connect(ui->expMDL, &QPushButton::pressed, [this] {
        QFileDialog filedialog(this, tr("Save Model Frames"), "",
                               tr(QString("PLY Models (*.ply)").toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            QString selFile = filedialog.selectedFiles()[0];
            viewer->model.writeAsPLY(selFile);
        }
    });

    /*connect(ui->exportCurFrame, &QPushButton::pressed, [this] {
        QFileDialog filedialog(this, tr("Save Model Frame"), "",
                               tr(QString("PLY Models (*.ply)").toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            QString selFile = filedialog.selectedFiles()[0];
            viewer->model.writeAsPLY(selFile, -1);
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

    mdlClrSel->setColor(viewer->modelColor);
    connect(mdlClrSel, &color_widgets::ColorPreview::clicked, [this] {
        RSDKColorDialog dlg(viewer->modelColor);
        if (dlg.exec() == QDialog::Accepted) {
            viewer->modelColor = dlg.color().toQColor();
            mdlClrSel->setColor(viewer->modelColor);
            viewer->repaint();
        }
    });

    connect(ui->resetCam, &QPushButton::pressed, [this] {
        viewer->resetCamera();
        viewer->repaint();
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
            viewer->model = RSDKv5::Model();
            tabTitle      = "Model Manager";
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

        case QEvent::Wheel: {
            QWheelEvent *wEvent = static_cast<QWheelEvent *>(event);

            if (wEvent->angleDelta().y() > 0 && viewer->zoom < 20)
                viewer->zoom *= 2;
            else if (wEvent->angleDelta().y() < 0 && viewer->zoom > 0.05)
                viewer->zoom /= 2;

            viewer->setZoom(viewer->zoom);
            return true;
        }

        case QEvent::MouseButtonPress: {
            QMouseEvent *mEvent = static_cast<QMouseEvent *>(event);

            if ((mEvent->button() & Qt::LeftButton) == Qt::LeftButton)
                mouseDownL = true;
            if ((mEvent->button() & Qt::MiddleButton) == Qt::MiddleButton)
                mouseDownM = true;
            if ((mEvent->button() & Qt::RightButton) == Qt::RightButton)
                mouseDownR = true;

            lastMousePos = mEvent->localPos();
            break;
        }

        case QEvent::MouseMove: {
            QMouseEvent *mEvent = static_cast<QMouseEvent *>(event);

            bool shouldRepaint = false;
            if (mouseDownL || mouseDownM) {
                bool useCursorPos = false;

                QPoint cursorPos = viewer->mapFromGlobal(QCursor::pos());
                if (!viewer->rect().contains(cursorPos)) {
                    if (cursorPos.x() < viewer->x()) {
                        cursorPos.setX(cursorPos.x() + (viewer->x() + viewer->width()));
                        lastMousePos.setX(lastMousePos.x() + (viewer->x() + viewer->width()));
                    }

                    if (cursorPos.x() > viewer->x() + viewer->width()) {
                        cursorPos.setX(cursorPos.x() - (viewer->x() + viewer->width()));
                        lastMousePos.setX(lastMousePos.x() - (viewer->x() + viewer->width()));
                    }

                    if (cursorPos.y() < viewer->y()) {
                        cursorPos.setY(cursorPos.y() + (viewer->y() + viewer->height()));
                        lastMousePos.setY(lastMousePos.y() + (viewer->y() + viewer->height()));
                    }

                    if (cursorPos.y() > viewer->y() + viewer->height()) {
                        cursorPos.setY(cursorPos.y() - (viewer->y() + viewer->height()));
                        lastMousePos.setY(lastMousePos.y() - (viewer->y() + viewer->height()));
                    }

                    useCursorPos = true;
                }

                float moveX       = (lastMousePos - cursorPos).x();
                float moveY       = (lastMousePos - cursorPos).y();
                float sensitivity = 0.5f;

                if (ctrlDownL) {
                    viewer->camera.orbit(moveX * sensitivity, moveY * sensitivity, { 0, 0, -128 });
                }
                else {
                    viewer->camera.rotateAxisUp(moveX);
                    viewer->camera.rotateAxisLeft(moveY);
                }
                shouldRepaint = true;

                if (useCursorPos) {
                    lastMousePos = cursorPos;
                    QCursor::setPos(viewer->mapToGlobal(cursorPos));
                }
                else {
                    lastMousePos = mEvent->localPos();
                }
            }

            if (shouldRepaint)
                viewer->repaint();
            break;
        }

        case QEvent::MouseButtonRelease: {
            QMouseEvent *mEvent = static_cast<QMouseEvent *>(event);

            if ((mEvent->button() & Qt::LeftButton) == Qt::LeftButton)
                mouseDownL = false;
            if ((mEvent->button() & Qt::MiddleButton) == Qt::MiddleButton)
                mouseDownM = false;
            if ((mEvent->button() & Qt::RightButton) == Qt::RightButton)
                mouseDownR = false;

            lastMousePos = mEvent->localPos();
            break;
        }

        case QEvent::KeyPress: {
            QKeyEvent *kEvent = static_cast<QKeyEvent *>(event);

            if (kEvent->key() == Qt::Key_Control)
                ctrlDownL = true;
            break;
        }

        case QEvent::KeyRelease: {
            QKeyEvent *kEvent = static_cast<QKeyEvent *>(event);

            if (kEvent->key() == Qt::Key_Control)
                ctrlDownL = false;
            break;
        }
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
    //bool changed = false;

    if (currentFrame < viewer->model.frames.count()) {
        viewer->animTimer += viewer->animSpeed;
        while (viewer->animTimer > 1.0) {
            viewer->animTimer -= 1.0;
            ++currentFrame;

            if (currentFrame >= viewer->model.frames.count())
                currentFrame = viewer->loopIndex;

            //changed = true;
        }
    }

    viewer->setFrame(currentFrame);
    viewer->repaint();
}

void ModelManager::setupUI(bool initialSetup)
{
    ui->frameList->blockSignals(true);

    ui->hasNormals->blockSignals(true);
    ui->hasColors->blockSignals(true);
    ui->hasTextures->blockSignals(true);

    ui->hasNormals->setDisabled(false);
    ui->hasColors->setDisabled(false);
    ui->hasTextures->setDisabled(false);

    ui->hasNormals->setChecked(false);
    ui->hasColors->setChecked(false);
    ui->hasTextures->setChecked(false);

    ui->frameList->clear();

    for (int f = 0; f < viewer->model.frames.count(); ++f)
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
    }

    ui->hasNormals->setChecked(viewer->model.hasNormals);
    ui->hasColors->setChecked(viewer->model.hasColors);
    ui->hasTextures->setChecked(viewer->model.hasTextures);

    int vertCnt = 0;
    if (viewer->model.frames.count())
        vertCnt = viewer->model.frames[0].vertices.count();

    ui->vertCnt->setText(
        QString("Vertex Count: %1 %2").arg(vertCnt).arg(vertCnt == 1 ? "Vertex" : "Vertices"));
    ui->idxCnt->setText(QString("Index Count: %1 %2")
                            .arg(viewer->model.indices.count())
                            .arg(vertCnt == 1 ? "Index" : "Indices"));
    ui->fVertCnt->setText(QString("Face Vertex Count: %1 %2")
                              .arg(viewer->model.faceVerticesCount)
                              .arg(vertCnt == 1 ? "Vertex" : "Vertices"));

    ui->hasNormals->blockSignals(false);
    ui->hasColors->blockSignals(false);
    ui->hasTextures->blockSignals(false);

    ui->frameList->blockSignals(false);
}

void ModelManager::loadModel(QString filePath, bool usev5Format)
{
    SetStatus("Loading model...", true);

    if (usev5Format) {
        RSDKv5::Model mdl;
        if (filePath != "")
            mdl.read(filePath);
        viewer->setModel(mdl);
        tabTitle            = Utils::getFilenameAndFolder(mdl.filePath);
        viewer->modelFormat = 0;
    }
    else {
        RSDKv4::Model mdl;
        if (filePath != "")
            mdl.read(filePath);
        viewer->setModel(mdl);
        tabTitle            = Utils::getFilenameAndFolder(mdl.filePath);
        viewer->modelFormat = 1;
    }

    UpdateTitle(false);
    SetStatus("Loaded model " + tabTitle);

    appConfig.addRecentFile(usev5Format ? ENGINE_v5 : ENGINE_v4, TOOL_MODELMANAGER, filePath,
                            QList<QString>{});
    setupUI();
}

bool ModelManager::saveModel(bool forceSaveAs)
{

    if (forceSaveAs || viewer->model.filePath.isEmpty()) {
        QString filters = { "RSDKv5 Model Files (*.bin);;RSDKv4 Model Files (*.bin)" };

        QFileDialog filedialog(this, tr("Save RSDK Model"), "", tr(filters.toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            bool usev5Format = filedialog.selectedNameFilter() == "RSDKv5 Model Files (*.bin)";
            QString filepath = filedialog.selectedFiles()[0];

            if (usev5Format) {
                SetStatus("Saving model...", true);
                viewer->model.write(filepath);
                SetStatus("Saved model to " + filepath);
                viewer->modelFormat = 0;

                appConfig.addRecentFile(ENGINE_v5, TOOL_MODELMANAGER, filepath, QList<QString>{});
            }
            else {
                RSDKv4::Model mdl = viewer->getModelv4();

                SetStatus("Saving model...", true);
                mdl.write(filepath);
                viewer->model.filePath = mdl.filePath;
                SetStatus("Saved model to " + filepath);
                viewer->modelFormat = 1;

                appConfig.addRecentFile(ENGINE_v4, TOOL_MODELMANAGER, filepath, QList<QString>{});
            }

            UpdateTitle(false);
            return true;
        }
    }
    else {
        if (viewer->modelFormat == 0) {
            SetStatus("Saving model...", true);
            viewer->model.write();
            SetStatus("Saved model to " + viewer->model.filePath);

            appConfig.addRecentFile(ENGINE_v5, TOOL_MODELMANAGER, viewer->model.filePath,
                                    QList<QString>{});
        }
        else {
            RSDKv4::Model mdl = viewer->getModelv4();

            SetStatus("Saving model...", true);
            mdl.write();
            viewer->model.filePath = mdl.filePath;
            SetStatus("Saved model to " + mdl.filePath);

            appConfig.addRecentFile(ENGINE_v5, TOOL_MODELMANAGER, mdl.filePath, QList<QString>{});
        }

        UpdateTitle(false);
        return true;
    }

    return false;
}

#include "moc_modelmanager.cpp"
