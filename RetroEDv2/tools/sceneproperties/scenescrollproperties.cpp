#include "includes.hpp"
#include "ui_scenescrollproperties.h"
#include "scenescrollproperties.hpp"

#include "sceneincludesv5.hpp"
#include "tools/sceneeditor.hpp"

SceneScrollProperties::SceneScrollProperties(QWidget *parent)
    : QWidget(parent), ui(new Ui::SceneScrollProperties)
{
    ui->setupUi(this);
}

SceneScrollProperties::~SceneScrollProperties() { delete ui; }

void SceneScrollProperties::setupUI(SceneHelpers::TileLayer::ScrollIndexInfo *info)
{
    unsetUI();

    ui->parallaxFactor->setValue(info->parallaxFactor);
    ui->scrollSpeed->setValue(info->scrollSpeed);
    ui->useDeform->setChecked(info->deform);

    connect(ui->parallaxFactor, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [info](double v) {
        info->parallaxFactor = v;

        if (scnEditor)
            scnEditor->CreateScrollList(true);
    });
    connect(ui->scrollSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [info](double v) {
        info->scrollSpeed = v;

        if (scnEditor)
            scnEditor->CreateScrollList(true);
    });
    connect(ui->useDeform, &QCheckBox::toggled, [info](bool c) { info->deform = c; });

    ui->instanceList->clear();
    for (auto &instance : info->instances) {
        ui->instanceList->addItem(QString("Start: %1, End: %2, Length: %3, Layer: %4")
                                      .arg(instance.startLine)
                                      .arg(instance.startLine + instance.length)
                                      .arg(instance.length)
                                      .arg(instance.layerID));
    }

    ui->rmInst->setDisabled(ui->instanceList->currentRow() == -1);
    ui->startLine->setDisabled(ui->instanceList->currentRow() == -1);
    ui->length->setDisabled(ui->instanceList->currentRow() == -1);
    ui->layerID->setDisabled(ui->instanceList->currentRow() == -1);

    connect(ui->instanceList, &QListWidget::currentRowChanged, [this, info](int c) {
        ui->rmInst->setDisabled(c == -1);
        ui->startLine->setDisabled(c == -1);
        ui->length->setDisabled(c == -1);
        ui->layerID->setDisabled(c == -1);

        if (c == -1)
            return;

        ui->startLine->blockSignals(true);
        ui->startLine->setValue(info->instances[c].startLine);
        ui->startLine->blockSignals(false);

        ui->length->blockSignals(true);
        ui->length->setValue(info->instances[c].length);
        ui->length->blockSignals(false);

        ui->layerID->blockSignals(true);
        ui->layerID->setCurrentIndex(info->instances[c].layerID - 1);
        ui->layerID->blockSignals(false);
    });

    connect(ui->addInst, &QToolButton::clicked, [this, info] {
        uint c = ui->instanceList->currentRow() + 1;

        SceneHelpers::TileLayer::ScrollInstance instance;

        instance.startLine = 0;
        instance.length    = 1;
        instance.layerID   = 0xFF;
        info->instances.insert(c, instance);

        ui->instanceList->blockSignals(true);
        ui->instanceList->insertItem(c, QString("Start: %1, End: %2, Length: %3, Layer: %4")
                                            .arg(instance.startLine)
                                            .arg(instance.startLine + instance.length)
                                            .arg(instance.length)
                                            .arg(instance.layerID));
        ui->instanceList->blockSignals(false);

        // DoAction("Add Instance: " + QString::number(info->instances.count() - 1));
    });

    connect(ui->rmInst, &QToolButton::clicked, [this, info] {
        int c = ui->instanceList->currentRow();
        int n = ui->instanceList->currentRow() == ui->instanceList->count() - 1 ? c - 1 : c;

        info->instances.removeAt(c);

        ui->instanceList->blockSignals(true);
        delete ui->instanceList->item(c);
        ui->instanceList->setCurrentRow(n);
        ui->instanceList->blockSignals(false);

        // DoAction("Remove Instance: " + QString::number(c));
    });

    connect(ui->startLine, QOverload<int>::of(&QSpinBox::valueChanged), [this, info](int v) {
        int c = ui->instanceList->currentRow();

        info->instances[c].startLine = v;

        ui->instanceList->blockSignals(true);
        ui->instanceList->item(c)->setText(
            QString("Start: %1, End: %2, Length: %3, Layer: %4")
                .arg(info->instances[c].startLine)
                .arg(info->instances[c].startLine + info->instances[c].length)
                .arg(info->instances[c].length)
                .arg(info->instances[c].layerID));
        ui->instanceList->blockSignals(false);

        // DoAction("Changed Instance Start Line");
    });
    connect(ui->length, QOverload<int>::of(&QSpinBox::valueChanged), [this, info](int v) {
        int c = ui->instanceList->currentRow();

        info->instances[c].length = v;

        ui->instanceList->blockSignals(true);
        ui->instanceList->item(c)->setText(
            QString("Start: %1, End: %2, Length: %3, Layer: %4")
                .arg(info->instances[c].startLine)
                .arg(info->instances[c].startLine + info->instances[c].length)
                .arg(info->instances[c].length)
                .arg(info->instances[c].layerID));
        ui->instanceList->blockSignals(false);

        // DoAction("Changed Instance Length");
    });

    connect(ui->layerID, QOverload<int>::of(&QComboBox::currentIndexChanged), [this, info](int v) {
        int c = ui->instanceList->currentRow();

        ui->instanceList->blockSignals(true);
        if (v < 0)
            info->instances[c].layerID = 0xFF;
        else
            info->instances[c].layerID = v + 1;
        ui->instanceList->item(c)->setText(
            QString("Start: %1, End: %2, Length: %3, Layer: %4")
                .arg(info->instances[c].startLine)
                .arg(info->instances[c].startLine + info->instances[c].length)
                .arg(info->instances[c].length)
                .arg(info->instances[c].layerID));
        ui->instanceList->blockSignals(false);

        // DoAction("Changed LayerID");
    });
}

void SceneScrollProperties::unsetUI()
{
    disconnect(ui->parallaxFactor, nullptr, nullptr, nullptr);
    disconnect(ui->scrollSpeed, nullptr, nullptr, nullptr);
    disconnect(ui->useDeform, nullptr, nullptr, nullptr);

    disconnect(ui->instanceList, nullptr, nullptr, nullptr);
    disconnect(ui->addInst, nullptr, nullptr, nullptr);
    disconnect(ui->rmInst, nullptr, nullptr, nullptr);
    disconnect(ui->startLine, nullptr, nullptr, nullptr);
    disconnect(ui->length, nullptr, nullptr, nullptr);
    disconnect(ui->layerID, nullptr, nullptr, nullptr);
}

#include "moc_scenescrollproperties.cpp"
