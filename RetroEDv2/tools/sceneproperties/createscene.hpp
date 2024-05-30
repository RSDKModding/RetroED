#pragma once

#include <QDialog>

namespace Ui
{
    class CreateScene;
}

class CreateScene : public QDialog
{
    Q_OBJECT
public:
    explicit CreateScene(QWidget *parent = nullptr);
    ~CreateScene();

    void LoadSceneVer(QList<QString> list);

    int  sceneVer = 0;
    bool loadGC   = false;
    QString scenePath = "";
    QString gcPath = "";

private:
    Ui::CreateScene *ui;
};
