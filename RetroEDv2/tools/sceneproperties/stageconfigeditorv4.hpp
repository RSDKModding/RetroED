#ifndef STAGECONFIGEDITOR_V4_H
#define STAGECONFIGEDITOR_V4_H

#include <QWidget>

namespace Ui
{
class StageconfigEditorv4;
}

class StageconfigEditorv4 : public QDialog
{
    Q_OBJECT

public:
    explicit StageconfigEditorv4(FormatHelpers::Stageconfig *scf, int globalObjCount,
                                 int globalSfxCount, QWidget *parent = nullptr);
    ~StageconfigEditorv4();

    void setupUI();

private:
    Ui::StageconfigEditorv4 *ui;

    FormatHelpers::Stageconfig *stageConfig;

    QStandardItemModel *m_sceneModel = nullptr;

    int globalObjectCount = 0;
    int globalSFXCount    = 0;
};

#endif // STAGECONFIGEDITOR_V4_H
