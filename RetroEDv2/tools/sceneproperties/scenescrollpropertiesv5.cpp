#include "includes.hpp"
#include "ui_scenescrollpropertiesv5.h"

SceneScrollPropertiesv5::SceneScrollPropertiesv5(QWidget *parent)
    : QWidget(parent), ui(new Ui::SceneScrollPropertiesv5)
{
    ui->setupUi(this);
}

SceneScrollPropertiesv5::~SceneScrollPropertiesv5() { delete ui; }

void SceneScrollPropertiesv5::setupUI(SceneHelpers::TileLayer::ScrollIndexInfo *info,
                                      QList<SceneHelpers::TileLayer::ScrollIndexInfo> &infos)
{
    unsetUI();

    ui->startLine->setValue(info->startLine);
    ui->length->setValue(info->length);
    ui->parallaxFactor->setValue(info->parallaxFactor);
    ui->scrollSpeed->setValue(info->scrollSpeed);
    ui->useDeform->setChecked(info->deform);

    connect(ui->startLine, QOverload<int>::of(&QSpinBox::valueChanged), [info, &infos](int v) {
        int move        = info->startLine + info->length;
        info->startLine = v;
        move            = (info->startLine + info->length) - move;
        int index       = infos.indexOf(*info);
        for (int i = index + 1; i < infos.count(); ++i) infos[i].startLine += move;
    });
    connect(ui->length, QOverload<int>::of(&QSpinBox::valueChanged), [info, &infos](int v) {
        int move     = info->startLine + info->length;
        info->length = v;
        move         = (info->startLine + info->length) - move;
        int index    = infos.indexOf(*info);
        for (int i = index + 1; i < infos.count(); ++i) infos[i].startLine += move;
    });
    connect(ui->parallaxFactor, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [info](double v) { info->parallaxFactor = v; });
    connect(ui->scrollSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [info](double v) { info->scrollSpeed = v; });
    connect(ui->useDeform, &QCheckBox::toggled, [info](bool c) { info->deform = c; });
}

void SceneScrollPropertiesv5::unsetUI()
{
    disconnect(ui->startLine, nullptr, nullptr, nullptr);
    disconnect(ui->length, nullptr, nullptr, nullptr);
    disconnect(ui->parallaxFactor, nullptr, nullptr, nullptr);
    disconnect(ui->scrollSpeed, nullptr, nullptr, nullptr);
    disconnect(ui->useDeform, nullptr, nullptr, nullptr);
}

#include "moc_scenescrollpropertiesv5.cpp"
