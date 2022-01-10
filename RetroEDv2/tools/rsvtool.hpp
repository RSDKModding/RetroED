#ifndef RSVUNPACKER_H
#define RSVUNPACKER_H

#include <QWidget>

namespace Ui
{
class RSVTool;
}

class RSVTool : public QWidget
{
    Q_OBJECT

public:
    explicit RSVTool(QWidget *parent = nullptr);
    ~RSVTool();

private:
    Ui::RSVTool *ui;

    QString rsvPath    = "";
    QString framesPath = "";
};

#endif // RSVUNPACKER_H
