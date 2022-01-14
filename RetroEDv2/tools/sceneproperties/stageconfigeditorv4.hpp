#ifndef STAGECONFIGEDITOR_V4_H
#define STAGECONFIGEDITOR_V4_H

#include <QWidget>

namespace Ui
{
class StageConfigEditorv4;
}

class StageConfigEditorv4 : public QDialog
{
    Q_OBJECT

public:
    class ActionState
    {
    public:
        QString name = "Action";

        FormatHelpers::StageConfig stageConfig;
    };

    explicit StageConfigEditorv4(FormatHelpers::StageConfig *scf, int globalObjCount,
                                 int globalSfxCount, QWidget *parent = nullptr);
    ~StageConfigEditorv4();

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

    Ui::StageConfigEditorv4 *ui;

    FormatHelpers::StageConfig *stageConfig;

    int globalObjectCount = 0;
    int globalSFXCount    = 0;
};

#endif // STAGECONFIGEDITOR_V4_H
