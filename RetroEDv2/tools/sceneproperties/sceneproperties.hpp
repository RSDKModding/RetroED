#ifndef SCENEPROPERTIES_H
#define SCENEPROPERTIES_H

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

    QPushButton *editTIL  = nullptr;
    QPushButton *editSCF  = nullptr;
    QPushButton *editPAL  = nullptr;
    QPushButton *editTSet = nullptr;

    QLabel *objCnt    = nullptr;
    QLabel *entCntTot = nullptr;

    QSpinBox *gridX = nullptr;
    QSpinBox *gridY = nullptr;

    QPushButton *syncGC     = nullptr;
    QPushButton *syncSC     = nullptr;
    QPushButton *reloadLink = nullptr;

private:
    Ui::SceneProperties *ui;
};

#endif // SCENEPROPERTIES_H
