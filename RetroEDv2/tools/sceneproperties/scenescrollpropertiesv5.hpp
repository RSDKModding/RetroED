#ifndef SCENESCROLLPROPERTIES_V5_HPP
#define SCENESCROLLPROPERTIES_V5_HPP

#include <QWidget>

namespace Ui
{
class SceneScrollPropertiesv5;
}

class SceneScrollPropertiesv5 : public QWidget
{
    Q_OBJECT

public:
    explicit SceneScrollPropertiesv5(QWidget *parent = nullptr);
    ~SceneScrollPropertiesv5();

    void setupUI(RSDKv5::Scene::ScrollIndexInfo *info, QList<RSDKv5::Scene::ScrollIndexInfo> &infos);
    void unsetUI();

private:
    Ui::SceneScrollPropertiesv5 *ui;
};

#endif // SCENESCROLLPROPERTIES_V5_HPP
