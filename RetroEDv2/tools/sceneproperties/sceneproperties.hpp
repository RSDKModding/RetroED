#pragma once

#include <QWidget>

namespace Ui
{
class SceneProperties;
}

class SceneProperties : public QWidget
{
    Q_OBJECT

public:
    explicit SceneProperties(QWidget *parent = nullptr);
    ~SceneProperties();

    void setupUI(FormatHelpers::Scene *scn, byte ver);
    void unsetUI();

    QComboBox *musBox = nullptr;

    QCheckBox *loadGlobalCB = nullptr;

    QPushButton *editCHK  = nullptr;
    QPushButton *editSCF  = nullptr;
    QPushButton *editPAL  = nullptr;
    QPushButton *editTSet = nullptr;
    QPushButton *copyPlane = nullptr;
    QLabel *objCnt    = nullptr;
    QLabel *entCntTot = nullptr;

    QSpinBox *gridX = nullptr;
    QSpinBox *gridY = nullptr;

    QPushButton *syncGC     = nullptr;
    QPushButton *syncSC     = nullptr;
    QPushButton *reloadLink = nullptr;

    QPushButton *replaceChunk = nullptr;

    color_widgets::ColorPreview *bgSel    = nullptr;
    color_widgets::ColorPreview *altBGSel = nullptr;

    QGroupBox *v1PropertiesBox = nullptr;
    QWidget *layers            = nullptr;

private:
    Ui::SceneProperties *ui;

    void *sceneEditor = nullptr;
};


