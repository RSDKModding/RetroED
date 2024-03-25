#pragma once

#include <QWidget>
#include <QDialog>

#include <RSDKv5/stamps.hpp>

class SceneViewer;

namespace Ui
{
class SceneStampPropertiesv5;
}

class SceneStampPropertiesv5 : public QWidget
{
    Q_OBJECT

public:
    explicit SceneStampPropertiesv5(QWidget *parent = nullptr);
    ~SceneStampPropertiesv5();

    void setupUI(RSDKv5::Stamps *stamps, int id);
    void unsetUI();

signals:
    void stampNameChanged(QString name);

private:
    Ui::SceneStampPropertiesv5 *ui;

    RSDKv5::Stamps *stampsPtr = nullptr;
};


