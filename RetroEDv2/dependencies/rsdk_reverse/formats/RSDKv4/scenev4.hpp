#ifndef SCENE_V4_H
#define SCENE_V4_H

namespace RSDKv4
{

extern QList<QString> objectAttributeNames;
extern QList<QString> objectAttributeTypes;

class Scene
{
public:
    class Object
    {
    public:
        struct AttributeInfo {
            AttributeInfo() {}

            int value   = 0;
            bool active = false;
        };

        enum AttributesIDs {
            ATTRIBUTE_STATE,
            ATTRIBUTE_DIRECTION,
            ATTRIBUTE_SCALE,
            ATTRIBUTE_ROTATION,
            ATTRIBUTE_DRAWORDER,
            ATTRIBUTE_PRIORITY,
            ATTRIBUTE_ALPHA,
            ATTRIBUTE_ANIMATION,
            ATTRIBUTE_ANIMATIONSPEED,
            ATTRIBUTE_FRAME,
            ATTRIBUTE_INKEFFECT,
            ATTRIBUTE_VALUE0,
            ATTRIBUTE_VALUE1,
            ATTRIBUTE_VALUE2,
            ATTRIBUTE_VALUE3,
        };

        Object() {}
        Object(Reader &reader, int id = 0) { read(reader, id); }

        void read(Reader &reader, int id = 0);
        void write(Writer &writer);

        inline float getX() { return posX / 65536.0f; }
        inline float getY() { return posY / 65536.0f; }

        inline void setX(float x) { posX = x * (1 << 0x10); }
        inline void setY(float y) { posY = y * (1 << 0x10); }

        byte type    = 0;
        byte propertyValue = 0;
        int posX     = 0 << 0x10;
        int posY     = 0 << 0x10;
        AttributeInfo variables[0x0F];

        short slotID = 0;
    };

    Scene() {}
    Scene(QString filename) { read(filename); }
    Scene(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    void read(Reader &reader);

    inline void write(QString filename)
    {
        if (filename == "")
            filename = m_filename;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    QString title = "Stage";

    QList<QList<ushort>> layout;

    byte activeLayers[4];
    byte midpoint = 3;

    QList<Object> objects;

    byte width;
    byte height;

    QString m_filename = "";
};

} // namespace RSDKv4

#endif // SCENE_V4_H
