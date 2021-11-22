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
    void processAnimation();

    bool event(QEvent *event) override;

    inline int animCount() { return animFile.animations.count(); }
    inline int frameCount()
    {
        if (currentAnim < animFile.animations.count()) {
            return animFile.animations[currentAnim].frames.count();
        }
        return 0;
    }

    inline void updateTitle(bool modified)
    {
        this->modified = modified;
        if (modified)
            emit titleChanged(tabTitle + " *");
        else
            emit titleChanged(tabTitle);
    }

signals:
    void titleChanged(QString title);

private:
    Ui::AnimationEditor *ui;

    FormatHelpers::Animation animFile;
    QList<bool> hitboxVisible;
    int aniType = 0xFF;

    ushort currentAnim   = -1;
    ushort currentFrame  = -1;
    ushort currentHitbox = -1;

    inline void startAnim()
    {
        playingAnim  = true;
        animFinished = false;
        animTimer    = 0;
        prevFrame    = 0;
        updateTimer->start(1000.0f / 60.0f);
    }

    inline void stopAnim()
    {
        playingAnim  = false;
        animFinished = false;
        animTimer    = 0;
        prevFrame    = 0;

        updateTimer->stop();
    }

    bool showTransparentClr = true;
    QColor bgColour         = QColor(0xA0, 0xA0, 0xA0, 0xFF);
    float zoom              = 1.0;

    bool playingAnim  = false;
    int animTimer     = 0;
    bool animFinished = false;
    int prevFrame     = 0;

    bool mouseDownL = false;
    bool mouseDownM = false;
    bool mouseDownR = false;

    Vector2<int> offset = Vector2<int>(0, 0);

    QTimer *updateTimer = nullptr;

    void loadSheet(QString filepath, int index, bool addSource = true);
    void removeSheet(int index, bool removeSource = true);
    void moveSheet(int from, int to);

    void loadAnim(QString filepath, int aniType);

    QVector<QImage> sheets;

    bool modified    = false;
    QString tabTitle = "Animation Editor";
};

#endif // ANIMATIONEDITOR_H
