#ifndef STAGECONFIGEDITOR_V5_H
#define STAGECONFIGEDITOR_V5_H

#include <QWidget>

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

    explicit StageConfigEditorv5(RSDKv5::StageConfig *scf, QWidget *parent = nullptr);
    ~StageConfigEditorv5();

    void setupUI(bool allowRowChange = true);

protected:
    bool event(QEvent *event);

private:
    void undoAction();
    void redoAction();
    void resetAction();
    void doAction(QString name = "Action", bool setModified = true);
    void clearActions();

    void copyConfig(ActionState *stateDst, ActionState *stateSrc);

    QList<ActionState> actions;
    int actionIndex = 0;

    Ui::StageConfigEditorv5 *ui;

    RSDKv5::StageConfig *stageConfig;
};

#endif // STAGECONFIGEDITOR_V5_H
