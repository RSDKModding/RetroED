#ifndef SCENEPROPERTIES_V5_H
#define SCENEPROPERTIES_V5_H

#include <QWidget>

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

    QPushButton *editTIL  = nullptr;
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

    QPushButton *syncGC = nullptr;
    QPushButton *syncSC = nullptr;

private:
    Ui::ScenePropertiesv5 *ui;
};

#endif // SCENEPROPERTIES_V5_H
