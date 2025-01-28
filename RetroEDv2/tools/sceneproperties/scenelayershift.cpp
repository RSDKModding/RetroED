#include "scenelayershift.hpp"
#include "ui_scenelayershift.h"

SceneLayerShift::SceneLayerShift(SceneHelpers::TileLayer layer, byte shiftSize, QWidget *parent) :
    QDialog(parent), ui(new Ui::SceneLayerShift)
{
    ui->setupUi(this);
    size = shiftSize;
    this->setWindowTitle("Shift Scene Layer");
    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->layerName->setText(QString("Layer Name: %1").arg(layer.name));

    ui->shiftX->setMinimum(~layer.width + 1);
    ui->shiftY->setMinimum(~layer.height + 1);

    ui->shiftX->setMaximum((size == 16 ? 1024 : 255) - (keepDimensions == true ? 0 : layer.width));
    ui->shiftY->setMaximum((size == 16 ? 1024 : 255) - (keepDimensions == true ? 0 : layer.height));

    ui->entShift->setText(QString("Entity Shift: %1 %2").arg(shiftX * size).arg(shiftY * size));
    ui->layerDimension->setText(QString("Layer Dimensions: %1 %2").arg(layer.width + shiftX).arg(layer.height + shiftY));

    connect(ui->AcceptCancel, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->AcceptCancel, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(ui->shiftX, QOverload<int>::of(&QSpinBox::valueChanged), [this, layer](int x){
        shiftX = x;
        ui->entShift->setText(QString("Entity Shift: %1 %2").arg(shiftX * size).arg(shiftY * size));
        if (!keepDimensions)
            ui->layerDimension->setText(QString("Layer Dimensions: %1 %2").arg(layer.width + shiftX).arg(layer.height + shiftY));
    });

    connect(ui->shiftY, QOverload<int>::of(&QSpinBox::valueChanged), [this, layer](int y){
        shiftY = y;
        ui->entShift->setText(QString("Entity Shift: %1 %2").arg(shiftX * size).arg(shiftY * size));
        if (!keepDimensions)
            ui->layerDimension->setText(QString("Layer Dimensions: %1 %2").arg(layer.width + shiftX).arg(layer.height + shiftY));
    });

    connect(ui->keepDimensionsBtn, &QCheckBox::clicked, [this, layer](bool c){
        keepDimensions = c;
        ui->layerDimension->setText(QString("Layer Dimensions: %1 %2").arg(layer.width + (keepDimensions ? 0 : shiftX))
                                                                      .arg(layer.height + (keepDimensions ? 0 : shiftY)));

        ui->shiftX->setMaximum((size == 16 ? 1024 : 255) - (keepDimensions == true ? 0 : layer.width));
        ui->shiftY->setMaximum((size == 16 ? 1024 : 255) - (keepDimensions == true ? 0 : layer.height));
    });

    connect(ui->shiftEntBtn, &QCheckBox::clicked, [this](bool c){
        shiftEnt = c;
        ui->entShift->setEnabled(shiftEnt);
    });

}

SceneLayerShift::~SceneLayerShift() { delete ui; }
