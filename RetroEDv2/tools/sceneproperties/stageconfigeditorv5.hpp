#ifndef STAGECONFIGEDITOR_V5_H
#define STAGECONFIGEDITOR_V5_H

#include <QWidget>

namespace Ui
{
class StageconfigEditorv5;
}

class StageconfigEditorv5 : public QDialog
{
    Q_OBJECT

public:
    explicit StageconfigEditorv5(RSDKv5::StageConfig *scf, QWidget *parent = nullptr);
    ~StageconfigEditorv5();

    void setupUI();

private:
    Ui::StageconfigEditorv5 *ui;

    RSDKv5::StageConfig *stageConfig;
};

#endif // STAGECONFIGEDITOR_V5_H
