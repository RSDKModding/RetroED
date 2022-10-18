#pragma once

#include <QWidget>

#include <RSDKv3/gameconfigv3.hpp>

namespace Ui
{
class GameConfigEditorv3;
}

class GameConfigEditorv3 : public QWidget
{
    Q_OBJECT

public:
    class ActionState
    {
    public:
        QString name = "Action";

        RSDKv3::GameConfig gameConfig;
    };

    explicit GameConfigEditorv3(QString path = "", QWidget *parent = nullptr);
    ~GameConfigEditorv3();

    void load(QString filename);

    void setupUI(bool allowRowChange = true);

    inline void UpdateTitle(bool modified)
    {
        this->modified = modified;
        if (modified)
            emit TitleChanged(tabTitle + " *");
        else
            emit TitleChanged(tabTitle);
    }

signals:
    void TitleChanged(QString title);

protected:
    bool event(QEvent *event);

private:
    void UndoAction();
    void RedoAction();
    void ResetAction();
    void DoAction(QString name = "Action", bool setModified = true);
    void ClearActions();

    void copyConfig(ActionState *stateDst, ActionState *stateSr);

    Ui::GameConfigEditorv3 *ui;

    RSDKv3::GameConfig gameConfig;

    QStandardItemModel *sceneModel = nullptr;

    QList<ActionState> actions;
    int actionIndex = 0;

    bool modified    = false;
    QString tabTitle = "GameConfig Editor";
};


