#ifndef STAGECONFIGEDITOR_V2_H
#define STAGECONFIGEDITOR_V2_H

#include <QWidget>

namespace Ui
{
class StageConfigEditorv2;
}

class StageConfigEditorv2 : public QDialog
{
    Q_OBJECT

public:
    class ActionState
    {
    public:
        QString name = "Action";

        FormatHelpers::StageConfig stageConfig;
    };

    explicit StageConfigEditorv2(FormatHelpers::StageConfig *scf, int globalObjCount,
                                 int globalSfxCount, QWidget *parent = nullptr);
    ~StageConfigEditorv2();

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

    Ui::StageConfigEditorv2 *ui;

    FormatHelpers::StageConfig *stageConfig;

    int globalObjectCount = 0;
    int globalSFXCount    = 0;
};

#endif // STAGECONFIGEDITOR_V2_H
