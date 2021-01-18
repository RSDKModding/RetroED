#ifndef STAGECONFIGEDITOR_V1_H
#define STAGECONFIGEDITOR_V1_H

#include <QWidget>

namespace Ui
{
class StageconfigEditorv2;
}

class StageconfigEditorv2 : public QDialog
{
    Q_OBJECT

public:
    explicit StageconfigEditorv2(FormatHelpers::Stageconfig *scf, int globalObjCount,
                                 QWidget *parent = nullptr);
    ~StageconfigEditorv2();

    void setupUI();

private:
    Ui::StageconfigEditorv2 *ui;

    FormatHelpers::Stageconfig *m_stageconfig;

    QStandardItemModel *m_sceneModel = nullptr;
};

#endif // STAGECONFIGEDITOR_V1_H
