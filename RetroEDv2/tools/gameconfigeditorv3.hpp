#ifndef GAMECONFIGEDITOR_V3_H
#define GAMECONFIGEDITOR_V3_H

#include <QWidget>

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

    Ui::GameConfigEditorv3 *ui;

    RSDKv3::GameConfig gameConfig;

    QStandardItemModel *sceneModel = nullptr;

    QList<ActionState> actions;
    int actionIndex = 0;

    bool modified    = false;
    QString tabTitle = "GameConfig Editor";
};

#endif // GAMECONFIGEDITOR_V3_H
