#ifndef GAMECONFIGEDITOR_V1_H
#define GAMECONFIGEDITOR_V1_H

#include <QWidget>

namespace Ui
{
class GameconfigEditorv2;
}

class GameconfigEditorv2 : public QWidget
{
    Q_OBJECT

public:
    explicit GameconfigEditorv2(QString path = "", QWidget *parent = nullptr);
    ~GameconfigEditorv2();

    void load(QString filename);

protected:
    bool event(QEvent *event);

private:
    Ui::GameconfigEditorv2 *ui;

    RSDKv2::Gameconfig gameConfig;

    QStandardItemModel *m_sceneModel = nullptr;
};

#endif // GAMECONFIGEDITOR_V1_H
