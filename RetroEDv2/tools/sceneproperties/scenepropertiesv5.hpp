#pragma once

#include <QWidget>

#include <RSDKv5/scenev5.hpp>
#include <RSDKv5/stageconfigv5.hpp>

namespace Ui
{
class ScenePropertiesv5;
}

class ScenePropertiesv5 : public QWidget
{
    Q_OBJECT

public:
    explicit ScenePropertiesv5(QWidget *parent = nullptr);
    ~ScenePropertiesv5();

    void setupUI(RSDKv5::Scene *scn, RSDKv5::StageConfig *config);
    void unsetUI();

    QCheckBox *loadGlobalCB  = nullptr;
    QLineEdit *stampNameEdit = nullptr;

    QPushButton *loadStamps = nullptr;
    QPushButton *saveStamps = nullptr;

    QPushButton *editSCF  = nullptr;
    QPushButton *editPAL  = nullptr;
    QPushButton *editTSet = nullptr;

    QLabel *layerCnt   = nullptr;
    QLabel *objCnt     = nullptr;
    QLabel *entCntTot  = nullptr;
    QLabel *entCntAct  = nullptr;
    QLabel *entCntInac = nullptr;

    QSpinBox *gridX = nullptr;
    QSpinBox *gridY = nullptr;

    QPushButton *syncGC     = nullptr;
    QPushButton *syncSC     = nullptr;
    QPushButton *reloadLink = nullptr;

    QPushButton *replaceTile = nullptr;

    color_widgets::ColorPreview *bgSel    = nullptr;
    color_widgets::ColorPreview *altBGSel = nullptr;

    QSpinBox *unknown1 = nullptr;
    QSpinBox *unknown2 = nullptr;
    QSpinBox *unknown3 = nullptr;
    QSpinBox *unknown4 = nullptr;
    QSpinBox *unknown5 = nullptr;
    QSpinBox *unknown6 = nullptr;
    QSpinBox *unknown7 = nullptr;
    QSpinBox *unknown8 = nullptr;
    QSpinBox *unknown9 = nullptr;

private:
    Ui::ScenePropertiesv5 *ui;
};


