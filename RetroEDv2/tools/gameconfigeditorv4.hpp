#ifndef GAMECONFIGEDITOR_V4_H
#define GAMECONFIGEDITOR_V4_H

#include <QWidget>

namespace Ui
{
class GameconfigEditorv4;
}

class GameconfigEditorv4 : public QWidget
{
    Q_OBJECT

public:
    explicit GameconfigEditorv4(QString path = "", QWidget *parent = nullptr);
    ~GameconfigEditorv4();

    void load(QString filename);

protected:
    bool event(QEvent *event);

private:
    Ui::GameconfigEditorv4 *ui;

    RSDKv4::Gameconfig gameConfig;

    QStandardItemModel *m_sceneModel = nullptr;
};

#endif // GAMECONFIGEDITOR_V4_H
