#ifndef STAGECONFIGEDITOR_V1_H
#define STAGECONFIGEDITOR_V1_H

#include <QWidget>

namespace Ui
{
class StageConfigEditorv1;
}

class StageConfigEditorv1 : public QDialog
{
    Q_OBJECT

public:
    class ActionState
    {
    public:
        QString name = "Action";

        FormatHelpers::StageConfig stageConfig;
    };

    explicit StageConfigEditorv1(FormatHelpers::StageConfig *scf, QWidget *parent = nullptr);
    ~StageConfigEditorv1();

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

    Ui::StageConfigEditorv1 *ui;

    FormatHelpers::StageConfig *stageConfig;
};

#endif // STAGECONFIGEDITOR_V1_H
