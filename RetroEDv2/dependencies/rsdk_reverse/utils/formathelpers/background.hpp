#ifndef RSDK_BACKGROUNDS_H
#define RSDK_BACKGROUNDS_H

namespace FormatHelpers
{

class Background
{
public:
    class ScrollInfo
    {
    public:
        ScrollInfo() {}

        float m_relativeSpeed = 1.0f;
        float m_constantSpeed = 0.0f;
        byte m_behaviour      = 0;
    };

    class ScrollIndexInfo
    {
    public:
        int m_startLine = 0;
        int m_length    = 1;

        float m_scrollPos     = 0.0f; // not written, for scene viewer only
        float m_relativeSpeed = 1.0f;
        float m_constantSpeed = 0.0f;
        byte m_behaviour      = 0;

        bool operator==(const ScrollIndexInfo &other) const
        {
            return m_startLine == other.m_startLine && m_length == other.m_length
                   && m_scrollPos == other.m_scrollPos && m_relativeSpeed == other.m_relativeSpeed
                   && m_constantSpeed == other.m_constantSpeed && m_behaviour == other.m_behaviour;
        }
    };

    class Layer
    {
    public:
        Layer() {}

        QList<QList<ushort>> m_layout;

        byte m_width          = 0;
        byte m_height         = 0;
        byte m_behaviour      = 0;
        float m_relativeSpeed = 1.0f;
        float m_constantSpeed = 0.0f;
        QByteArray m_lineIndexes;

        QList<ScrollIndexInfo> m_scrollInfos;
    };

    Background() {}
    Background(byte ver, QString filepath) { read(ver, filepath); }

    void read(byte ver, QString filename);
    void write(byte ver, QString filename);

    void scrollInfoFromIndices();
    void scrollIndicesFromInfo();

    QList<Layer> m_layers;
    QList<ScrollInfo> m_hScroll;
    QList<ScrollInfo> m_vScroll;

    QString m_filename = "";
};

} // namespace FormatHelpers

#endif // RSDK_BACKGROUNDS_H
