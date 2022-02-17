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
    explicit ModelManager(QWidget *parent = nullptr);
    ~ModelManager();

    void setupUI();

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
    int mdlFormat = -1;
    RSDKv5::Model modelv5;
    RSDKv4::Model modelv4;

    ModelViewer *viewer = nullptr;

    Ui::ModelManager *ui;

    bool modified    = false;
    QString tabTitle = "Model Manager";
};

#endif // MODELMANAGER_H
