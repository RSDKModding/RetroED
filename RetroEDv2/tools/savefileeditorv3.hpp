#ifndef SAVEEDITOR_V3_H
#define SAVEEDITOR_V3_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class SaveFileEditorv3;
}
QT_END_NAMESPACE

class SaveFileEditorv3 : public QWidget
{
    Q_OBJECT

public:
    SaveFileEditorv3(QString filePath = "", QWidget *parent = nullptr);
    ~SaveFileEditorv3();

    RSDKv3::SaveFile savefile;

    void setupUI();

protected:
    bool event(QEvent *event);

private:
    Ui::SaveFileEditorv3 *ui;

    int m_saveID = 0;
};
#endif // SAVEEDITOR_V3_H
