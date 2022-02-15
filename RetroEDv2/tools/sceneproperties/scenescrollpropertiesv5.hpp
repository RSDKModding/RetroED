#ifndef SCENESCROLLPROPERTIES_V5_H
#define SCENESCROLLPROPERTIES_V5_H

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

    void setupUI(SceneHelpers::TileLayer::ScrollIndexInfo *info,
                 QList<SceneHelpers::TileLayer::ScrollIndexInfo> &infos);
    void unsetUI();

private:
    Ui::SceneScrollPropertiesv5 *ui;
};

#endif // SCENESCROLLPROPERTIES_V5_H
