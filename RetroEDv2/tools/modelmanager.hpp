#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QWidget>

namespace Ui
{
class ModelManager;
}

class ModelManager : public QWidget
{
    Q_OBJECT

public:
    explicit ModelManager(QString filePath = "", bool usev5Format = true, QWidget *parent = nullptr);
    ~ModelManager();

    void setupUI(bool initialSetup = true);
    ModelViewer *viewer = nullptr;

    void loadModel(QString filePath, bool usev5Format);
    bool saveModel(bool forceSaveAs = false);

    inline void updateTitle(bool modified)
    {
        this->modified = modified;
        if (modified)
            emit titleChanged(tabTitle + " *");
        else
            emit titleChanged(tabTitle);
    }

signals:
    void titleChanged(QString title);

protected:
    bool event(QEvent *event);
    bool eventFilter(QObject *object, QEvent *event);

private:
    Ui::ModelManager *ui;

    color_widgets::ColorPreview *mdlClrSel = nullptr;

    void startAnim();
    void stopAnim();

    void processAnimation();

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

    bool modified    = false;
    QString tabTitle = "Model Manager";
};

#endif // MODELMANAGER_H
