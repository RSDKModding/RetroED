#pragma once

namespace FormatHelpers
{

class Scene
{
public:
    class Object
    {
    public:
        class VariableInfo
        {
        public:
            VariableInfo() {}

            int value   = 0;
            bool active = false;
        };

        Object() {}

        inline float getX() { return pos.x / 65536.0f; }
        inline float getY() { return pos.y / 65536.0f; }

        inline void setX(float x) { pos.x = x * (1 << 0x10); }
        inline void setY(float y) { pos.y = y * (1 << 0x10); }

        byte type          = 0;
        byte propertyValue = 0;
        Vector2<int> pos   = Vector2<int>(0 << 0x10, 0 << 0x10);
        VariableInfo variables[0x0F];

        short slotID = 0;
    };

    Scene() {}
    Scene(byte ver, QString filepath) { read(ver, filepath); }

    void read(byte ver, QString filename);
    void write(byte ver, QString filename);

    QString title = "ACT";

    byte activeLayer[4];
    byte midpoint = 3;

    byte musicID      = 0;
    byte backgroundID = 0;

    short playerX = 0;
    short playerY = 0;

    QList<QList<ushort>> layout;

    QList<Object> objects;
    QList<QString> objectTypeNames;

    ushort width  = 0;
    ushort height = 0;

    QString filepath = "";

    static const int entityLimit = 0x400;
};

} // namespace FormatHelpers


