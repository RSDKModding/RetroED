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

        float parallaxFactor = 1.0f;
        float scrollSpeed = 0.0f;
        byte deform      = 0;
    };

    class ScrollIndexInfo
    {
    public:
        int startLine = 0;
        int length    = 1;

        float m_scrollPos     = 0.0f; // not written, for scene viewer only
        float parallaxFactor = 1.0f;
        float scrollSpeed = 0.0f;
        byte deform      = 0;

        bool operator==(const ScrollIndexInfo &other) const
        {
            return startLine == other.startLine && length == other.length
                   && m_scrollPos == other.m_scrollPos && parallaxFactor == other.parallaxFactor
                   && scrollSpeed == other.scrollSpeed && deform == other.deform;
        }
    };

    class Layer
    {
    public:
        Layer() {}

        QList<QList<ushort>> layout;

        byte width          = 0;
        byte height         = 0;
        byte type      = 0;
        float parallaxFactor = 1.0f;
        float scrollSpeed = 0.0f;
        QByteArray lineIndexes;

        QList<ScrollIndexInfo> scrollInfos;
    };

    Background() {}
    Background(byte ver, QString filepath) { read(ver, filepath); }

    void read(byte ver, QString filename);
    void write(byte ver, QString filename);

    void scrollInfoFromIndices();
    void scrollIndicesFromInfo();

    QList<Layer> layers;
    QList<ScrollInfo> hScroll;
    QList<ScrollInfo> vScroll;

    QString filePath = "";
};

} // namespace FormatHelpers

#endif // RSDK_BACKGROUNDS_H
