#pragma once

#include <QLabel>
#include <QWidget>

//ughh

class FocusLabel : public QLabel
{
    using QLabel::QLabel;
signals:
    void mousePressEvent(QMouseEvent* e) {
        setFocus();
        QLabel::mousePressEvent(e);
    }
};
