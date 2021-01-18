#ifndef SCENEEXPORTIMGOPTIONS_H
#define SCENEEXPORTIMGOPTIONS_H

#include <QDialog>

namespace Ui
{
class SceneExportImgOptions;
}

class SceneExportImgOptions : public QDialog
{
    Q_OBJECT

public:
    explicit SceneExportImgOptions(QWidget *parent = nullptr);
    ~SceneExportImgOptions();

    bool m_exportFG[2];
    bool m_exportBG[8][2];
    bool m_exportObjects = true;
    bool m_exportObjInfo = true;

private:
    Ui::SceneExportImgOptions *ui;
};

#endif // SCENEEXPORTIMGOPTIONS_H
