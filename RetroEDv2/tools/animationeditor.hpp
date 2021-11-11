#ifndef ANIMATIONEDITOR_H
#define ANIMATIONEDITOR_H

#include <QWidget>

namespace Ui
{
class AnimationEditor;
}

class AnimationEditor : public QWidget
{
    Q_OBJECT

public:
    explicit AnimationEditor(QString filepath = "", byte type = 0xFF, QWidget *parent = nullptr);
    ~AnimationEditor();

    void setupUI();
    void updateView();

    bool event(QEvent *event) override;

    inline int animCount() { return animFile.animations.count(); }
    inline int frameCount()
    {
        if (currentAnim < animFile.animations.count()) {
            return animFile.animations[currentAnim].frames.count();
        }
        return 0;
    }

private:
    Ui::AnimationEditor *ui;

    FormatHelpers::Animation animFile;
    int aniType = 0xFF;

    ushort currentAnim   = -1;
    ushort currentFrame  = -1;
    ushort currentHitbox = -1;

    inline void stopAnim()
    {
        playingAnim  = false;
        animTimer    = 0;
        animFinished = false;
        prevFrame    = 0;
    }

    bool showHitbox         = false;
    bool showTransparentClr = true;
    QColor bgColour         = QColor(0xA0, 0xA0, 0xA0, 0xFF);

    bool playingAnim  = false;
    int animTimer     = 0;
    bool animFinished = false;
    int prevFrame     = 0;

    bool mouseDownL = false;
    bool mouseDownM = false;
    bool mouseDownR = false;

    void loadSheet(QString filepath, int index, bool addSource = true);
    void removeSheet(int index, bool removeSource = true);
    void moveSheet(int from, int to);

    void loadAnim(QString filepath, int aniType);

    QVector<QImage> sheets;

    inline void updateTitle(bool modified)
    {
        this->modified = modified;
        // if (modified)
        //     emit titleChanged(tabTitle + " *");
        // else
        //     emit titleChanged(tabTitle);
    }

    bool modified    = false;
    QString tabTitle = "";
};

#endif // ANIMATIONEDITOR_H
