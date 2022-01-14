#ifndef GAMECONFIGEDITOR_V1_H
#define GAMECONFIGEDITOR_V1_H

#include <QWidget>

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
};

#endif // GAMECONFIGEDITOR_V1_H
