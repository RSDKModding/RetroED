#pragma once

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

    void ConvertDBToCSV(QString dbPath, QString csvPath);
    void ConvertCSVToDB(QString dbPath, QString csvPath);

    Ui::UserDBManager *ui;
};
