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

    bool exportFG[2];
    bool exportBG[8][2];
    bool exportObjects = true;
    bool exportObjInfo = true;

private:
    Ui::SceneExportImgOptions *ui;
};

#endif // SCENEEXPORTIMGOPTIONS_H
