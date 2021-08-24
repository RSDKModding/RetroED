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

protected:
    bool event(QEvent *event);

private:
    int mdlFormat = -1;
    RSDKv5::Model modelv5;
    RSDKv4::Model modelv4;

    Ui::ModelManager *ui;
};

#endif // MODELMANAGER_H
