#pragma once

namespace RSDKv4
{

extern QList<QString> objectVariableNames;
extern QList<QString> objectVariableTypes;

class Scene
{
public:
    class Entity
    {
    public:
        struct VariableInfo {
            VariableInfo() {}

            int value   = 0;
            bool active = false;
        };

        enum VariableIDs {
            VAR_STATE,
            VAR_DIRECTION,
            VAR_SCALE,
            VAR_ROTATION,
            VAR_DRAWORDER,
            VAR_PRIORITY,
            VAR_ALPHA,
            VAR_ANIMATION,
            VAR_ANIMATIONSPEED,
            VAR_FRAME,
            VAR_INKEFFECT,
            VAR_VALUE0,
            VAR_VALUE1,
            VAR_VALUE2,
            VAR_VALUE3,
        };

        Entity() {}
        Entity(Reader &reader, int id = 0) { read(reader, id); }

        void read(Reader &reader, int id = 0);
        void write(Writer &writer);

        inline float getX() { return posX / 65536.0f; }
        inline float getY() { return posY / 65536.0f; }

        inline void setX(float x) { posX = x * (1 << 0x10); }
        inline void setY(float y) { posY = y * (1 << 0x10); }

        byte type          = 0;
        byte propertyValue = 0;
        int posX           = 0 << 0x10;
        int posY           = 0 << 0x10;
        VariableInfo variables[0x0F];

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
            filename = filePath;
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

    QList<Entity> entities;

    byte width;
    byte height;

    QString filePath = "";
};

} // namespace RSDKv4


