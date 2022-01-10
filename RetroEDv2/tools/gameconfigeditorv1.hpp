#ifndef GAMECONFIGEDITOR_V1_H
#define GAMECONFIGEDITOR_V1_H

#include <QWidget>

namespace Ui
{
class GameconfigEditorv1;
}

class GameconfigEditorv1 : public QWidget
{
    Q_OBJECT

public:
    explicit GameconfigEditorv1(QString path = "", QWidget *parent = nullptr);
    ~GameconfigEditorv1();

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
    Ui::GameconfigEditorv1 *ui;

    QList<QString> stageListNames = {
        "Regular Stages",
        "Custom Stages",
        "Special Stages",
        "Bonus Stages",
    };

    RSDKv1::CharacterList characters;
    RSDKv1::StageList stageList[4];

    QStandardItemModel *sceneModel = nullptr;

    bool modified    = false;
    QString tabTitle = "GameConfig Editor";
};

#endif // GAMECONFIGEDITOR_V1_H
