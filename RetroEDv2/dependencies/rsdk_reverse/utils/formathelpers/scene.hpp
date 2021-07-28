#ifndef RSDK_SCENE_H
#define RSDK_SCENE_H

namespace FormatHelpers
{

class Scene
{
public:
    class Object
    {
    public:
        class AttributeInfo
        {
        public:
            AttributeInfo() {}

            int m_value   = 0;
            bool m_active = false;
        };

        Object() {}

        inline float getX() { return m_position.x / 65536.0f; }
        inline float getY() { return m_position.y / 65536.0f; }

        inline void setX(float x) { m_position.x = x * (1 << 0x10); }
        inline void setY(float y) { m_position.y = y * (1 << 0x10); }

        byte type             = 0;
        byte m_propertyValue    = 0;
        Vector2<int> m_position = Vector2<int>(0 << 0x10, 0 << 0x10);
        AttributeInfo m_attributes[0x0F];

        short m_id = 0;
    };

    Scene() {}
    Scene(byte ver, QString filepath) { read(ver, filepath); }

    void read(byte ver, QString filename);
    void write(byte ver, QString filename);

    QString m_title = "ACT";

    byte activeLayer[4];
    byte midpoint = 3;

    byte m_music      = 0;
    byte m_background = 0;

    short m_playerXPos = 0;
    short m_playerYPos = 0;

    QList<QList<ushort>> layout;

    QList<Object> objects;
    QList<QString> objectTypeNames;

    ushort width  = 0;
    ushort height = 0;

    QString filepath = "";

    static const int maxObjectCount = 1056;
};

} // namespace FormatHelpers

#endif // RSDK_SCENE_H
