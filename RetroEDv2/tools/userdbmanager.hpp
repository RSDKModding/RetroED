#ifndef USERDBMANAGER_HPP
#define USERDBMANAGER_HPP

#include <QDialog>

namespace Ui
{
class UserDBManager;
}

class UserDBManager : public QDialog
{
    Q_OBJECT

public:
    explicit UserDBManager(QWidget *parent = nullptr);
    ~UserDBManager();

private:
    char *strptime(const char *s, const char *f, struct tm *tm);

    void convertDBToCSV(QString dbPath, QString csvPath);
    void convertCSVToDB(QString dbPath, QString csvPath);

    Ui::UserDBManager *ui;
};

#endif // USERDBMANAGER_HPP
