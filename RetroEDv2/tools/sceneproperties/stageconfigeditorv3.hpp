#ifndef STAGECONFIGEDITOR_V3_H
#define STAGECONFIGEDITOR_V3_H

#include <QWidget>

namespace Ui
{
class StageconfigEditorv3;
}

class StageconfigEditorv3 : public QDialog
{
    Q_OBJECT

public:
    explicit StageconfigEditorv3(FormatHelpers::Stageconfig *scf, int globalObjCount,
                                 int globalSfxCount, QWidget *parent = nullptr);
    ~StageconfigEditorv3();

    void setupUI();

private:
    Ui::StageconfigEditorv3 *ui;

    FormatHelpers::Stageconfig *stageConfig;

    QStandardItemModel *m_sceneModel = nullptr;

    int globalObjectCount = 0;
    int globalSFXCount    = 0;
};

#endif // STAGECONFIGEDITOR_V3_H
