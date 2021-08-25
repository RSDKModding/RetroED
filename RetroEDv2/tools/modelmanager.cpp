#include "includes.hpp"
#include "ui_modelmanager.h"

ModelManager::ModelManager(QWidget *parent) : QWidget(parent), ui(new Ui::ModelManager)
{
    ui->setupUi(this);

    connect(ui->exportFrames, &QPushButton::pressed, [this] {
        QFileDialog filedialog(this, tr("Save Model Frames"), "",
                               tr(QString("OBJ Models (*.obj)").toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            QString selFile = filedialog.selectedFiles()[0];

            switch (mdlFormat) {
                default: break;
                case 0: {
                    modelv5.writeAsOBJ(selFile);
                    break;
                }
                case 1: {
                    modelv4.writeAsOBJ(selFile);
                    break;
                }
            }
        }
    });

    connect(ui->exportCurFrame, &QPushButton::pressed, [this] {
        QFileDialog filedialog(this, tr("Save Model Frame"), "",
                               tr(QString("OBJ Models (*.obj)").toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            QString selFile = filedialog.selectedFiles()[0];

            // TODO: set da frame
            switch (mdlFormat) {
                default: break;
                case 0: {
                    modelv5.writeAsOBJ(selFile, -1);
                    break;
                }
                case 1: {
                    modelv4.writeAsOBJ(selFile, -1);
                    break;
                }
            }
        }
    });
}

ModelManager::~ModelManager() { delete ui; }

bool ModelManager::event(QEvent *event)
{
    if (event->type() == (QEvent::Type)RE_EVENT_NEW) {
        modelv4 = RSDKv4::Model();
        modelv5 = RSDKv5::Model();
        // setupUI();
        return true;
    }
    if (event->type() == (QEvent::Type)RE_EVENT_OPEN) {
        QString filters = { "RSDKv5 Model Files (*.bin);;RSDKv4 Model Files (*.bin)" };

        QFileDialog filedialog(this, tr("Open RSDK Model"), "", tr(filters.toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            if (filedialog.selectedNameFilter() == "RSDKv5 Model Files (*.bin)") {
                mdlFormat = 0;
                modelv5.read(filedialog.selectedFiles()[0]);
            }
            else {
                mdlFormat = 1;
                modelv4.read(filedialog.selectedFiles()[0]);
            }
            // setupUI();
        }
        return true;
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE) {
        switch (mdlFormat) {
            default: break;
            case 0: {
                break;
            }
            case 1: {
                break;
            }
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE_AS) {
        switch (mdlFormat) {
            default: break;
            case 0: {
                break;
            }
            case 1: {
                break;
            }
        }
    }
    return QWidget::event(event);
}

#include "moc_modelmanager.cpp"
