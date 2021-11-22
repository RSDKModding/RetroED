#ifndef GAMECONFIGEDITOR_V1_H
#define GAMECONFIGEDITOR_V1_H

#include <QWidget>

namespace Ui
{
class GameconfigEditorv2;
}

class GameconfigEditorv2 : public QWidget
{
    Q_OBJECT

public:
    explicit GameconfigEditorv2(QString path = "", QWidget *parent = nullptr);
    ~GameconfigEditorv2();

    void load(QString filename);

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

private:
    Ui::GameconfigEditorv2 *ui;

    RSDKv2::Gameconfig gameConfig;

    QStandardItemModel *sceneModel = nullptr;

    bool modified    = false;
    QString tabTitle = "GameConfig Editor";
};

#endif // GAMECONFIGEDITOR_V1_H
