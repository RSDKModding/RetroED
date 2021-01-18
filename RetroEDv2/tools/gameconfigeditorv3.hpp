#ifndef GAMECONFIGEDITOR_V3_H
#define GAMECONFIGEDITOR_V3_H

#include <QWidget>

namespace Ui
{
class GameconfigEditorv3;
}

class GameconfigEditorv3 : public QWidget
{
    Q_OBJECT

public:
    explicit GameconfigEditorv3(QString path = "", QWidget *parent = nullptr);
    ~GameconfigEditorv3();

    void load(QString filename);

protected:
    bool event(QEvent *event);

private:
    Ui::GameconfigEditorv3 *ui;

    RSDKv3::Gameconfig m_gameconfig;

    QStandardItemModel *m_sceneModel = nullptr;
};

#endif // GAMECONFIGEDITOR_V3_H
