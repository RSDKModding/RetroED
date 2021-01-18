#ifndef RSDK_ANIMATION_H
#define RSDK_ANIMATION_H

namespace FormatHelpers
{

class Animation
{
public:
    class Hitbox
    {
    public:
        struct HitboxInfo {
        public:
            HitboxInfo() {}

            char m_left   = 0;
            char m_top    = 0;
            char m_right  = 0;
            char m_bottom = 0;
        };

        Hitbox() {}

        HitboxInfo m_hitboxes[8];
    };

    class Frame
    {
    public:
        Frame() {}

        byte m_sheet        = 0;
        byte m_collisionBox = 0;
        Hitbox::HitboxInfo m_hitbox;
        byte m_sprX   = 0;
        byte m_sprY   = 0;
        byte m_width  = 0;
        byte m_height = 0;
        char m_pivotX = 0;
        char m_pivotY = 0;
    };

    class AnimationEntry
    {
    public:
        AnimationEntry() {}

        QString m_name = "Animation";
        QList<Frame> m_frames;
        byte m_loopIndex       = 0;
        byte m_speedMultiplyer = 0;
        byte m_rotationFlags   = 0;
    };

    Animation() {}
    Animation(byte ver, QString filepath) { read(ver, filepath); }

    void read(byte ver, QString filename);
    void write(byte ver, QString filename);

    QString m_filename = "";

    byte m_unknown    = 0;
    byte m_playerType = 0;

    QList<QString> m_sheets;
    QList<AnimationEntry> m_animations;
    QList<Hitbox> m_hitboxes;
};

} // namespace FormatHelpers

#endif // RSDK_ANIMATION_H
