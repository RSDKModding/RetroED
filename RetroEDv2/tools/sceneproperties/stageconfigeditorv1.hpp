#ifndef STAGECONFIGEDITOR_VRS_H
#define STAGECONFIGEDITOR_VRS_H

#include <QWidget>

namespace Ui
{
class StageconfigEditorv1;
}

class StageconfigEditorv1 : public QDialog
{
    Q_OBJECT

public:
    explicit StageconfigEditorv1(FormatHelpers::Stageconfig *scf, int globalSfxCount,
                                 QWidget *parent = nullptr);
    ~StageconfigEditorv1();

    void setupUI();

private:
    Ui::StageconfigEditorv1 *ui;

    FormatHelpers::Stageconfig *stageConfig;

    QStandardItemModel *m_sceneModel = nullptr;

    int globalSFXCount = 0;
};

#endif // STAGECONFIGEDITOR_VRS_H
