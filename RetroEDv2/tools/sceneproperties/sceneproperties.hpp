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

    QComboBox *m_musBox = nullptr;

    QCheckBox *loadGlobalCB = nullptr;

    QPushButton *m_editTIL = nullptr;
    QPushButton *m_editSCF = nullptr;
    QPushButton *m_editPAL = nullptr;

private:
    Ui::SceneProperties *ui;
};

#endif // SCENEPROPERTIES_H
