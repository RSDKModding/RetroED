#ifndef SCENELAYERPROPERTIES_H
#define SCENELAYERPROPERTIES_H

#include <QWidget>

namespace Ui
{
class SceneLayerProperties;
}

class SceneLayerProperties : public QWidget
{
    Q_OBJECT

public:
    explicit SceneLayerProperties(QWidget *parent = nullptr);
    ~SceneLayerProperties();

    void setupUI(FormatHelpers::Scene *scn, FormatHelpers::Background *bg, byte lID, byte ver);
    void unsetUI();

private:
    Ui::SceneLayerProperties *ui;
};

#endif // SCENELAYERPROPERTIES_H
