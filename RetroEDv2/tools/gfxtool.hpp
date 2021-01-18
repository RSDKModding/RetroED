#ifndef GFXTOOL_H
#define GFXTOOL_H

#include <QWidget>

namespace Ui
{
class GFXTool;
}

class GFXTool : public QWidget
{
    Q_OBJECT

public:
    explicit GFXTool(QWidget *parent = nullptr);
    ~GFXTool();

    QString m_gfxPath = "";
    QString m_imgPath = "";

    byte m_type = 0;

private:
    Ui::GFXTool *ui;
};

#endif // GFXTOOL_H
