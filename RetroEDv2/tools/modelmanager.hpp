#pragma once

#include <QWidget>
#include "tools/utils/modelviewer.hpp"

namespace Ui
{
class ModelManager;
}

class ModelManager : public QWidget
{
    Q_OBJECT

public:
    class ActionState
    {
    public:
        QString name = "Action";

        FormatHelpers::Animation animFile;
    };

    explicit ModelManager(QString filePath = "", bool usev5Format = true, QWidget *parent = nullptr);
    ~ModelManager();

    void SetupUI(bool initialSetup = true);
    ModelViewer *viewer = nullptr;

    void LoadModel(QString filePath, bool usev5Format);
    bool SaveModel(bool forceSaveAs = false);

    inline void UpdateTitle(bool modified)
    {
        this->modified = modified;
        if (modified)
            emit TitleChanged(tabTitle + " *", tabPath);
        else
            emit TitleChanged(tabTitle, tabPath);
    }

signals:
    void TitleChanged(QString title, QString tabFullPath);

protected:
    bool event(QEvent *event);
    bool eventFilter(QObject *object, QEvent *event);

private:
    Ui::ModelManager *ui;

    color_widgets::ColorPreview *mdlClrSel = nullptr;

    void StartAnim();
    void StopAnim();

    void ProcessAnimation();

    void DoAction(QString name = "Action", bool setModified = true);
    void ClearActions();

    ushort currentFrame = -1;
    bool playingAnim    = false;
    bool animFinished   = false;
    int prevFrame       = 0;
    QTimer *updateTimer = nullptr;

    QPointF lastMousePos;
    bool ctrlDownL  = false;
    bool mouseDownL = false;
    bool mouseDownM = false;
    bool mouseDownR = false;

    QList<ActionState> actions;
    int actionIndex = 0;

    bool modified    = false;
    QString tabTitle = "Model Manager";
    QString tabPath  = "";
};
