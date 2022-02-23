#ifndef SCENESCROLLPROPERTIES_HPP
#define SCENESCROLLPROPERTIES_HPP

#include <QWidget>

namespace Ui
{
class SceneScrollProperties;
}

class SceneScrollProperties : public QWidget
{
    Q_OBJECT

public:
    explicit SceneScrollProperties(QWidget *parent = nullptr);
    ~SceneScrollProperties();

    void setupUI(SceneHelpers::TileLayer::ScrollIndexInfo *info,
                 QList<SceneHelpers::TileLayer::ScrollIndexInfo> &infos);
    void unsetUI();

private:
    Ui::SceneScrollProperties *ui;
};

#endif // SCENESCROLLPROPERTIES_HPP
