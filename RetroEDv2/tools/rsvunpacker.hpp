#ifndef RSVUNPACKER_H
#define RSVUNPACKER_H

#include <QWidget>

namespace Ui
{
class RSVUnpacker;
}

class RSVUnpacker : public QWidget
{
    Q_OBJECT

public:
    explicit RSVUnpacker(QWidget *parent = nullptr);
    ~RSVUnpacker();

private:
    Ui::RSVUnpacker *ui;
};

#endif // RSVUNPACKER_H
