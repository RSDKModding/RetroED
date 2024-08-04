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

    static inline bool ShowCloseWarning(QWidget *parent = nullptr, bool *cancelled = nullptr)
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

    QMenu *file = nullptr;
    void SetMenuStates(bool newEnabled = false, bool openEnabled = false, bool saveEnabled = false, bool saveAsEnabled = false);


protected:
    bool event(QEvent *event);

private:
    Ui::MainWindow *ui;

    QTabWidget *toolTabs = nullptr;
    int tabClicked = -1;

    inline int GetTab(QWidget *tab)
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

    template <typename T> inline void AddTab(T *tab, QString name)
    {
        if (!toolTabs)
            return;

        connect(tab, &T::TitleChanged, [this, tab](QString t, QString p) {
            tab->setWindowTitle(t);
            int i = GetTab(tab);
            if (i >= 0){
                toolTabs->setTabText(i, t);
                toolTabs->setTabToolTip(toolTabs->currentIndex(), p);
            }

            if (t != "Scene Editor" && t != "Scene Editor (v5)" && t != "Animation Editor" &&
                t != "Palette Editor" && t != "Model Manager")
                { SetMenuStates(true,true,true,true); }
            else { SetMenuStates(true,true,false,false); }
        });

        tab->setWindowTitle(name);
        toolTabs->setCurrentIndex(toolTabs->addTab(tab, name));
        tab->UpdateTitle(false);
    }

signals:
    void MiddleClickPressed();
};
