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
    Ui::GameconfigEditorv4 *ui;

    RSDKv4::GameConfig gameConfig;

    QStandardItemModel *m_sceneModel = nullptr;

    bool modified    = false;
    QString tabTitle = "GameConfig Editor";
};

#endif // GAMECONFIGEDITOR_V4_H
