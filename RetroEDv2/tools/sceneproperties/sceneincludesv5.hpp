#ifndef SCENEINCLUDES_V5_H
#define SCENEINCLUDES_V5_H

namespace SceneHelpers
{

class TileLayer
{
public:
    TileLayer() {}

    class ScrollIndexInfo
    {
    public:
        int startLine = 0;
        int length    = 1;

        float scrollPos      = 0.0f; // not written, for scene viewer only
        float parallaxFactor = 1.0f;
        float scrollSpeed    = 0.0f;
        bool deform          = false;
        byte unknown         = 0;

        bool operator==(const ScrollIndexInfo &other) const
        {
            return startLine == other.startLine && length == other.length
                   && scrollPos == other.scrollPos && parallaxFactor == other.parallaxFactor
                   && scrollSpeed == other.scrollSpeed && deform == other.deform
                   && unknown == other.unknown;
        }
    };

    enum LayerTypes {
        LAYER_NONE,
        LAYER_HSCROLL,
        LAYER_VSCROLL,
        LAYER_ROTOZOOM,
        LAYER_BASIC,
        LAYER_SKY3D,
        LAYER_FLOOR3D
    };

    QString name = "TileLayer";

    byte type            = LAYER_NONE;
    byte drawOrder       = 2;
    ushort width         = 0;
    ushort height        = 0;
    bool visible         = true;
    float parallaxFactor = 1.0f;
    float scrollSpeed    = 0.0f;

    QList<ScrollIndexInfo> scrollInfos;

    QList<QList<ushort>> layout;
};

} // namespace SceneHelpers

#endif // SCENEINCLUDES_V5_H
