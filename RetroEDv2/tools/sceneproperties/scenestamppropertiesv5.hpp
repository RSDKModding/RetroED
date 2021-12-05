#ifndef SCENESTAMPPROPERTIES_V5_H
#define SCENESTAMPPROPERTIES_V5_H

#include <QWidget>
#include <QDialog>

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

private:
    Ui::SceneStampPropertiesv5 *ui;

    RSDKv5::Stamps *stampsPtr = nullptr;
};

#endif // SCENESTAMPPROPERTIES_V5_H
