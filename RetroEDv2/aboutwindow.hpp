#pragma once

#include <QDialog>

namespace Ui {
class AboutWindow;
}

class AboutWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AboutWindow(QWidget *parent = nullptr);
    ~AboutWindow();

private:
    Ui::AboutWindow *ui;
};


