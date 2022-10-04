#ifndef STAGECONFIGEDITOR_V3_H
#define STAGECONFIGEDITOR_V3_H

#include <QWidget>

namespace Ui
{
class StageConfigEditorv3;
}

class StageConfigEditorv3 : public QDialog
{
    Q_OBJECT

public:
    class ActionState
    {
    public:
        QString name = "Action";

        FormatHelpers::StageConfig stageConfig;
    };

    explicit StageConfigEditorv3(FormatHelpers::StageConfig *scf, int globalObjCount,
                                 int globalSfxCount, QWidget *parent = nullptr);
    ~StageConfigEditorv3();

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

    Ui::StageConfigEditorv3 *ui;

    FormatHelpers::StageConfig *stageConfig;

    int globalObjectCount = 0;
    int globalSFXCount    = 0;
};

#endif // STAGECONFIGEDITOR_V3_H
