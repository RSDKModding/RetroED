#ifndef SCENELAYERPROPERTIES_V5_H
#define SCENELAYERPROPERTIES_V5_H

#include <QWidget>

namespace Ui
{
class SceneLayerPropertiesv5;
}

class SceneLayerPropertiesv5 : public QWidget
{
    Q_OBJECT

public:
    explicit SceneLayerPropertiesv5(QWidget *parent = nullptr);
    ~SceneLayerPropertiesv5();

    void setupUI(RSDKv5::Scene *scn, byte lID);
    void unsetUI();

private:
    Ui::SceneLayerPropertiesv5 *ui;
};

#endif // SCENELAYERPROPERTIES_V5_H
