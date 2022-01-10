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
    Ui::GameconfigEditorv3 *ui;

    RSDKv3::GameConfig gameConfig;

    QStandardItemModel *m_sceneModel = nullptr;

    bool modified    = false;
    QString tabTitle = "GameConfig Editor";
};

#endif // GAMECONFIGEDITOR_V3_H
