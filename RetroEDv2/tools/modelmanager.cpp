#include "includes.hpp"
#include "ui_modelmanager.h"

ModelManager::ModelManager(QWidget *parent) : QWidget(parent), ui(new Ui::ModelManager)
{
    ui->setupUi(this);

    viewer = new ModelViewer(this);
    viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->viewerFrame->layout()->addWidget(viewer);
    viewer->show();

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

    connect(ui->loopIndex, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { viewer->loopIndex = v; });

    connect(ui->animSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](double v) { viewer->animSpeed = v; });

    ui->frameList->setCurrentRow(0);
    connect(ui->frameList, &QListWidget::currentRowChanged, [this](int r) {
        ui->animSpeed->setDisabled(r == -1);
        ui->loopIndex->setDisabled(r == -1);

        viewer->setFrame(r);
    });
    ui->frameList->setCurrentRow(-1);

    for (QWidget *w : findChildren<QWidget *>()) {
        w->installEventFilter(this);
    }
}

ModelManager::~ModelManager() { delete ui; }

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
                if (filedialog.selectedNameFilter() == "RSDKv5 Model Files (*.bin)") {
                    setStatus("Loading model...", true);
                    mdlFormat = 0;
                    modelv5.read(filedialog.selectedFiles()[0]);
                    tabTitle = Utils::getFilenameAndFolder(modelv5.filePath);
                    setStatus("Loaded model " + tabTitle);
                }
                else {
                    mdlFormat = 1;
                    modelv4.read(filedialog.selectedFiles()[0]);
                    tabTitle = Utils::getFilenameAndFolder(modelv4.filePath);
                }
                setupUI();
            }
            return true;
        }

        case RE_EVENT_SAVE:
            switch (mdlFormat) {
                default: break;
                case 0: {
                    if (modelv5.filePath.isEmpty()) {
                        QFileDialog filedialog(this, tr("Save RSDK Model"), "",
                                               tr("RSDKv5 Model Files (*.bin)"));
                        filedialog.setAcceptMode(QFileDialog::AcceptSave);
                        if (filedialog.exec() == QDialog::Accepted) {
                            setStatus("Saving model...", true);

                            QString filepath = filedialog.selectedFiles()[0];

                            modelv5.write(filepath);
                            setStatus("Saved model to " + filepath);
                            updateTitle(false);
                            return true;
                        }
                    }
                    else {
                        setStatus("Saving model...", true);

                        modelv5.write();
                        setStatus("Saved model to " + modelv5.filePath);

                        updateTitle(false);
                        return true;
                    }
                    break;
                }
                case 1: {
                    if (modelv4.filePath.isEmpty()) {
                        QFileDialog filedialog(this, tr("Save RSDK Model"), "",
                                               tr("RSDKv4 Model Files (*.bin)"));
                        filedialog.setAcceptMode(QFileDialog::AcceptSave);
                        if (filedialog.exec() == QDialog::Accepted) {
                            setStatus("Saving model...", true);

                            QString filepath = filedialog.selectedFiles()[0];

                            modelv4.write(filepath);
                            setStatus("Saved model to " + filepath);

                            updateTitle(false);
                            return true;
                        }
                    }
                    else {
                        setStatus("Saving model...", true);

                        modelv4.write();
                        updateTitle(false);
                        setStatus("Saved model to " + modelv5.filePath);

                        return true;
                    }
                    break;
                }
            }
            break;

        case RE_EVENT_SAVE_AS:
            switch (mdlFormat) {
                default: break;
                case 0: {
                    QFileDialog filedialog(this, tr("Save Model"), "",
                                           tr("RSDKv5 Model Files (*.bin)"));
                    filedialog.setAcceptMode(QFileDialog::AcceptSave);
                    if (filedialog.exec() == QDialog::Accepted) {
                        setStatus("Saving model...", true);

                        QString filepath = filedialog.selectedFiles()[0];

                        modelv5.write(filepath);
                        setStatus("Saved model to " + filepath);

                        updateTitle(false);
                        return true;
                    }
                    break;
                }
                case 1: {
                    QFileDialog filedialog(this, tr("Save Model"), "",
                                           tr("RSDKv4 Model Files (*.bin)"));
                    filedialog.setAcceptMode(QFileDialog::AcceptSave);
                    if (filedialog.exec() == QDialog::Accepted) {
                        setStatus("Saving model...", true);

                        QString filepath = filedialog.selectedFiles()[0];

                        modelv4.write(filepath);
                        setStatus("Saved model to " + filepath);

                        updateTitle(false);
                        return true;
                    }
                    break;
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
    }

    return false;
}

void ModelManager::setupUI()
{
    ui->frameList->blockSignals(true);

    ui->frameList->clear();
    switch (mdlFormat) {
        default: break;
        case 0: {
            for (int f = 0; f < modelv5.frames.count(); ++f)
                ui->frameList->addItem("Frame " + QString::number(f));

            ui->animSpeed->blockSignals(true);
            ui->animSpeed->setValue(0);
            viewer->animSpeed = 0;
            ui->animSpeed->blockSignals(false);

            ui->loopIndex->blockSignals(true);
            ui->loopIndex->setValue(0);
            viewer->loopIndex = 0;
            ui->loopIndex->blockSignals(false);

            ui->frameList->setCurrentRow(-1);

            viewer->setModel(modelv5);
            break;
        }

        case 1: {
            for (int f = 0; f < modelv4.frames.count(); ++f)
                ui->frameList->addItem("Frame " + QString::number(f));

            ui->animSpeed->blockSignals(true);
            ui->animSpeed->setValue(0);
            viewer->animSpeed = 0;
            ui->animSpeed->blockSignals(false);

            ui->loopIndex->blockSignals(true);
            ui->loopIndex->setValue(0);
            viewer->loopIndex = 0;
            ui->loopIndex->blockSignals(false);

            ui->frameList->setCurrentRow(-1);

            viewer->setModel(modelv4);
            break;
        }
    }

    ui->frameList->blockSignals(false);
}

#include "moc_modelmanager.cpp"
