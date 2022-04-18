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
        float scrollSpeed    = 0.0f;
        byte deform          = 0;
    };

    struct ScrollInstance {
        int startLine = 0;
        int length    = 1;

        byte layerID = 0;
    };

    struct ScrollIndexInfo {
        float scrollPos      = 0.0f; // not written, for scene viewer only
        float parallaxFactor = 1.0f;
        float scrollSpeed    = 0.0f;
        bool deform          = false;

        QList<ScrollInstance> instances;

        bool operator==(const ScrollIndexInfo &other) const
        {
            if (instances.count() != other.instances.count())
                return false;

            bool instanceMatch = false;
            for (int i = 0; i < instances.count(); ++i) {
                instanceMatch = instanceMatch && instances[i].startLine == other.instances[i].startLine
                                && instances[i].length == other.instances[i].length
                                && instances[i].layerID == other.instances[i].layerID;
            }

            return instanceMatch && scrollPos == other.scrollPos
                   && parallaxFactor == other.parallaxFactor && scrollSpeed == other.scrollSpeed
                   && deform == other.deform;
        }
    };

    class Layer
    {
    public:
        Layer() {}

        QList<QList<ushort>> layout;

        byte width           = 0;
        byte height          = 0;
        byte type            = 0;
        float parallaxFactor = 1.0f;
        float scrollSpeed    = 0.0f;
        QByteArray lineScroll;
    };

    Background() {}
    Background(byte ver, QString filepath) { read(ver, filepath); }

    void read(byte ver, QString filename);
    void write(byte ver, QString filename);

    void scrollInfoFromIndices();
    void scrollIndicesFromInfo();

    QList<ScrollIndexInfo> hScrollInfo;
    QList<ScrollIndexInfo> vScrollInfo;

    QList<Layer> layers;
    QList<ScrollInfo> hScroll;
    QList<ScrollInfo> vScroll;

    QString filePath = "";
};

} // namespace FormatHelpers

#endif // RSDK_BACKGROUNDS_H
