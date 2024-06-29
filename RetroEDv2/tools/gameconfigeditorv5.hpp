#pragma once

#include <QWidget>

#include <RSDKv5/gameconfigv5.hpp>
#include <RSDKv5/rsdkconfigv5.hpp>
#include <RSDKv5/stageconfigv5.hpp>

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

    inline void UpdateTitle(bool modified)
    {
        this->modified = modified;
        if (modified)
            emit TitleChanged(tabTitle + " *", tabPath);
        else
            emit TitleChanged(tabTitle, tabPath);
    }

signals:
    void TitleChanged(QString title, QString tabFullPath);

protected:
    bool event(QEvent *event);

private:
    void UndoAction();
    void RedoAction();
    void ResetAction();
    void DoAction(QString name = "Action", bool setModified = true);
    void ClearActions();

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
    QString tabPath  = "";

private:
    Ui::GameConfigEditorv5 *ui;
};

