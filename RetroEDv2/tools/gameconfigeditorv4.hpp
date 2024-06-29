#pragma once

#include <QWidget>

#include <RSDKv4/gameconfigv4.hpp>

namespace Ui
{
class GameConfigEditorv4;
}

class GameConfigEditorv4 : public QWidget
{
    Q_OBJECT

public:
    class ActionState
    {
    public:
        QString name = "Action";

        RSDKv4::GameConfig gameConfig;
    };

    explicit GameConfigEditorv4(QString path = "", QWidget *parent = nullptr);
    ~GameConfigEditorv4();

    void load(QString filename);
    bool saveGameConfig(bool forceSaveAs = false);

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

    Ui::GameConfigEditorv4 *ui;

    RSDKv4::GameConfig gameConfig;

    QStandardItemModel *sceneModel = nullptr;

    QList<ActionState> actions;
    int actionIndex = 0;

    bool modified    = false;
    QString tabTitle = "GameConfig Editor";
    QString tabPath  = "";
};


