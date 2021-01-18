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
                                 QWidget *parent = nullptr);
    ~StageconfigEditorv4();

    void setupUI();

private:
    Ui::StageconfigEditorv4 *ui;

    FormatHelpers::Stageconfig *m_stageconfig;

    QStandardItemModel *m_sceneModel = nullptr;
};

#endif // STAGECONFIGEDITOR_V4_H
