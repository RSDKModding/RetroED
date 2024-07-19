#pragma once

#include <QDialog>
#include <RSDKv5/stageconfigv5.hpp>
#include "sceneincludesv5.hpp"

namespace Ui
{
class StageConfigEditorv5;
}

class StageConfigEditorv5 : public QDialog
{
    Q_OBJECT

public:
    class ActionState
    {
    public:
        QString name = "Action";

        RSDKv5::StageConfig stageConfig;
    };

    explicit StageConfigEditorv5(RSDKv5::StageConfig *scf, QList<SceneObject> &objList, QList<GameObjectInfo> gamelinkObjs, bool linkState, QWidget *parent = nullptr);
    ~StageConfigEditorv5();
    QString oldObj = "";
    QString replaceObj = "";
    void setupUI(bool allowRowChange = true);

protected:
    bool event(QEvent *event);

private:
    void UndoAction();
    void RedoAction();
    void ResetAction();
    void DoAction(QString name = "Action", bool setModified = true);
    void ClearActions();

    void copyConfig(ActionState *stateDst, ActionState *stateSrc);

    QList<ActionState> actions;
    int actionIndex = 0;
    bool linkLoaded = false;

    Ui::StageConfigEditorv5 *ui;

    RSDKv5::StageConfig *stageConfig;
};


