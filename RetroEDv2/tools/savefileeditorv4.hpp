#ifndef SAVEFILEEDITOR_V4_H
#define SAVEFILEEDITOR_V4_H

#include <QWidget>

namespace Ui
{
class SaveFileEditorv4;
}

class SaveFileEditorv4 : public QWidget
{
    Q_OBJECT

public:
    explicit SaveFileEditorv4(QString filePath = "", QWidget *parent = nullptr);
    ~SaveFileEditorv4();

    RSDKv4::SaveFile m_savefile;

    void setupUI();

protected:
    bool event(QEvent *event);

private:
    Ui::SaveFileEditorv4 *ui;

    int m_saveID = 0;
};

#endif // SAVEFILEEDITOR_V4_H
