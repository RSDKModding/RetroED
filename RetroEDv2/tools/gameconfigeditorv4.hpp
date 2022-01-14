#ifndef GAMECONFIGEDITOR_V4_H
#define GAMECONFIGEDITOR_V4_H

#include <QWidget>

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

    Ui::GameConfigEditorv4 *ui;

    RSDKv4::GameConfig gameConfig;

    QStandardItemModel *sceneModel = nullptr;

    QList<ActionState> actions;
    int actionIndex = 0;

    bool modified    = false;
    QString tabTitle = "GameConfig Editor";
};

#endif // GAMECONFIGEDITOR_V4_H
