#pragma once

#include <QWidget>

#include <RSDKv1/characterlistv1.hpp>
#include <RSDKv1/zonelistv1.hpp>

namespace Ui
{
class GameConfigEditorv1;
}

class GameConfigEditorv1 : public QWidget
{
    Q_OBJECT

public:
    class ActionState
    {
    public:
        QString name = "Action";

        RSDKv1::CharacterList characters;
        RSDKv1::StageList stageList[4];
    };

    explicit GameConfigEditorv1(QString path = "", QWidget *parent = nullptr);
    ~GameConfigEditorv1();

    void load(QString filename);

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

    Ui::GameConfigEditorv1 *ui;

    QList<QString> stageListNames = {
        "Regular Stages",
        "Custom Stages",
        "Special Stages",
        "Bonus Stages",
    };

    RSDKv1::CharacterList characters;
    RSDKv1::StageList stageList[4];

    QStandardItemModel *sceneModel = nullptr;

    QList<ActionState> actions;
    int actionIndex = 0;

    bool modified    = false;
    QString tabTitle = "GameConfig Editor";
    QString tabPath  = "";
};


