#ifndef GAMECONFIGEDITOR_V5_H
#define GAMECONFIGEDITOR_V5_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class GameConfigEditorv5;
}
QT_END_NAMESPACE

class GameConfigEditorv5 : public QWidget
{
    Q_OBJECT

public:
    class ActionState
    {
    public:
        QString name = "Action";

        RSDKv5::GameConfig gameConfig;
        RSDKv5::RSDKConfig rsdkconfig;

        RSDKv5::StageConfig stageconfig;
    };

    GameConfigEditorv5(QString configPath, byte type, bool oldVer, QWidget *parent = nullptr);
    ~GameConfigEditorv5();

    void setupUI(bool allowRowChange = true);

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
    void undoAction();
    void redoAction();
    void resetAction();
    void doAction(QString name = "Action", bool setModified = true);
    void clearActions();

    void copyConfig(ActionState *stateDst, ActionState *stateSr);

    RSDKv5::GameConfig gameConfig;
    RSDKv5::RSDKConfig rsdkConfig;

    RSDKv5::StageConfig stageConfig;

    bool oldVer = false;

    QStandardItemModel *sceneModel = nullptr;

    QList<ActionState> actions;
    int actionIndex = 0;

    bool modified    = false;
    QString tabTitle = "GameConfig Editor";

private:
    Ui::GameConfigEditorv5 *ui;
};
#endif // GAMECONFIGEDITOR_V5_H
