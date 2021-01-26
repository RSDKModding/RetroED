#ifndef GAMECONFIGEDITOR_V5_H
#define GAMECONFIGEDITOR_V5_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class GameconfigEditorv5;
}
QT_END_NAMESPACE

class GameconfigEditorv5 : public QWidget
{
    Q_OBJECT

public:
    GameconfigEditorv5(QString configPath, byte type, QWidget *parent = nullptr);
    ~GameconfigEditorv5();

    void setupUI();

protected:
    bool event(QEvent *event);

private:
    RSDKv5::GameConfig m_gameconfig;
    RSDKv5::RSDKConfig m_rsdkconfig;

    RSDKv5::StageConfig m_stageconfig;

    QStandardItemModel *m_sceneModel = nullptr;

private:
    Ui::GameconfigEditorv5 *ui;
};
#endif // GAMECONFIGEDITOR_V5_H
