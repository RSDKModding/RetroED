#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <QDialog>

namespace Ui {
class GameManager;
}

class GameManager : public QDialog
{
    Q_OBJECT

public:
    explicit GameManager(QWidget *parent = nullptr);
    ~GameManager();

private:
    Ui::GameManager *ui;
};

#endif // GAMEMANAGER_HPP
