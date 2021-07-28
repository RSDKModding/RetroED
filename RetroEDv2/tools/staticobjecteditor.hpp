#ifndef STATICOBJECTEDITOR_H
#define STATICOBJECTEDITOR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class StaticObjectEditor;
}
QT_END_NAMESPACE

class StaticObjectEditor : public QWidget
{
    Q_OBJECT

public:
    StaticObjectEditor(QString filePath = "", QWidget *parent = nullptr);
    ~StaticObjectEditor();

    void setupInfo(QString selFile);
    void setupUI();

protected:
    bool event(QEvent *event);

private:
    RSDKv5::StaticObject staticObject;
    StaticObjectInfo info;


    QStandardItemModel *m_arrayModel = nullptr;

private:
    Ui::StaticObjectEditor *ui;
};
#endif // STATICOBJECTEDITOR_H
