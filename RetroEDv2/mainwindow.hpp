#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static bool showCloseWarning(QWidget *parent = nullptr, bool *cancelled = nullptr)
    {
        QMessageBox::StandardButton reply =
            QMessageBox::question(parent, "RetroED",
                                  QString("You have unsaved changes!\n"
                                          "Would you like to save?\n"),
                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (cancelled)
            *cancelled = reply == QMessageBox::Cancel;

        return reply == QMessageBox::Yes;
    }

protected:
    bool event(QEvent *event);

private:
    Ui::MainWindow *ui;

    QTabWidget *toolTabs = nullptr;

    int getTab(QWidget *tab)
    {
        if (!toolTabs)
            return -1;

        for (int t = 0; t < toolTabs->count(); ++t) {
            if (toolTabs->widget(t) == tab) {
                return t;
            }
        }
        return -1;
    }

    template <typename T> void addTab(T *tab, QString name)
    {
        if (!toolTabs)
            return;

        connect(tab, &T::TitleChanged, [this, tab](QString t) {
            tab->setWindowTitle(t);
            int i = getTab(tab);
            if (i >= 0)
                toolTabs->setTabText(i, t);
        });

        tab->setWindowTitle(name);
        toolTabs->setCurrentIndex(toolTabs->addTab(tab, name));
        tab->UpdateTitle(false);
    }

    QString gamePath = "";
};

