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
    GameconfigEditorv5(QString configPath, byte type, bool oldVer, QWidget *parent = nullptr);
    ~GameconfigEditorv5();

    void setupUI();

    inline void updateTitle(bool modified)
    {
        this->modified = modified;
        if (modified)
            emit titleChanged(tabTitle + " *");
        else
            emit titleChanged(tabTitle);
    }

signals:
    void titleChanged(QString title);

protected:
    bool event(QEvent *event);

private:
    RSDKv5::GameConfig gameConfig;
    RSDKv5::RSDKConfig rsdkconfig;

    RSDKv5::StageConfig stageconfig;

    bool oldVer = false;

    QStandardItemModel *sceneModel = nullptr;

    bool modified    = false;
    QString tabTitle = "GameConfig Editor";

private:
    Ui::GameconfigEditorv5 *ui;
};
#endif // GAMECONFIGEDITOR_V5_H
