#include "rsdkreverse.hpp"

void FormatHelpers::Scene::read(byte ver, QString filename)
{
    Reader reader(filename);
    filepath = filename;

    title = "Stage";

    activeLayer[0] = 9;
    activeLayer[1] = 9;
    activeLayer[2] = 9;
    activeLayer[3] = 9;
    midpoint       = 3;

    musicID      = 0;
    backgroundID = 0;

    playerX = 0;
    playerY = 0;

    layout.clear();

    objects.clear();
    objectTypeNames.clear();

    width  = 0;
    height = 0;

    switch (ver) {
        default: break;
        case ENGINE_v1: {
            RSDKv1::Scene scn(reader);

            title = scn.title;

            musicID      = scn.musicID;
            backgroundID = scn.backgroundID;

            playerX = scn.playerXPos;
            playerY = scn.playerYPos;

            width  = scn.width;
            height = scn.height;

            for (int y = 0; y < height; ++y) {
                layout.append(QList<ushort>());
                for (int x = 0; x < width; ++x) {
                    layout[y].append(scn.layout[y][x]);
                }
            }

            for (RSDKv1::Scene::Entity &object : scn.entities) {
                Object obj;

                obj.type          = object.type;
                obj.propertyValue = object.propertyValue;
                obj.pos.x         = object.posX << 0x10;
                obj.pos.y         = object.posY << 0x10;

                obj.slotID = object.slotID;
                objects.append(obj);
            }
            break;
        }
        case ENGINE_v2: {
            RSDKv2::Scene scn(reader);

            title = scn.title;

            for (int i = 0; i < 4; ++i) activeLayer[i] = scn.activeLayer[i];
            midpoint = scn.midpoint;

            width  = scn.width;
            height = scn.height;

            for (int y = 0; y < height; ++y) {
                layout.append(QList<ushort>());
                for (int x = 0; x < width; ++x) {
                    layout[y].append(scn.layout[y][x]);
                }
            }

            for (QString name : scn.typeNames) objectTypeNames.append(name);

            for (RSDKv2::Scene::Entity &object : scn.entities) {
                Object obj;

                obj.type          = object.type;
                obj.propertyValue = object.propertyValue;
                obj.pos.x         = object.posX << 0x10;
                obj.pos.y         = object.posY << 0x10;

                obj.slotID = object.slotID;
                objects.append(obj);
            }
            break;
        }
        case ENGINE_v3: {
            RSDKv3::Scene scn(reader);

            title = scn.title;

            for (int i = 0; i < 4; ++i) activeLayer[i] = scn.activeLayer[i];
            midpoint = scn.midpoint;

            width  = scn.width;
            height = scn.height;

            for (int y = 0; y < height; ++y) {
                layout.append(QList<ushort>());
                for (int x = 0; x < width; ++x) {
                    layout[y].append(scn.layout[y][x]);
                }
            }

            for (QString name : scn.typeNames) objectTypeNames.append(name);

            for (RSDKv3::Scene::Entity &object : scn.entities) {
                Object obj;

                obj.type          = object.type;
                obj.propertyValue = object.propertyValue;
                obj.pos.x         = object.posX << 0x10;
                obj.pos.y         = object.posY << 0x10;

                obj.slotID = object.slotID;
                objects.append(obj);
            }
            break;
        }
        case ENGINE_v4: {
            RSDKv4::Scene scn(reader);

            title = scn.title;

            for (int i = 0; i < 4; ++i) activeLayer[i] = scn.activeLayers[i];
            midpoint = scn.midpoint;

            width  = scn.width;
            height = scn.height;

            for (int y = 0; y < height; ++y) {
                layout.append(QList<ushort>());
                for (int x = 0; x < width; ++x) {
                    layout[y].append(scn.layout[y][x]);
                }
            }

            for (RSDKv4::Scene::Entity &object : scn.entities) {
                Object obj;

                obj.type          = object.type;
                obj.propertyValue = object.propertyValue;
                obj.pos.x         = object.posX;
                obj.pos.y         = object.posY;

                obj.slotID = object.slotID;

                for (int a = 0; a < 0x0F; ++a) {
                    obj.variables[a].value  = object.variables[a].value;
                    obj.variables[a].active = object.variables[a].active;
                }

                objects.append(obj);
            }
            break;
        }
    }
}

void FormatHelpers::Scene::write(byte ver, QString filename)
{
    if (filename == "")
        filename = filepath;
    if (filename == "")
        return;
    Writer writer(filename);
    filepath = filename;

    switch (ver) {
        default: break;
        case ENGINE_v1: {
            RSDKv1::Scene scn;

            scn.title = title;

            scn.musicID      = musicID;
            scn.backgroundID = backgroundID;

            scn.playerXPos = playerX;
            scn.playerYPos = playerY;

            scn.width  = width;
            scn.height = height;

            scn.layout.clear();
            for (int y = 0; y < height; ++y) {
                scn.layout.append(QList<byte>());
                for (int x = 0; x < width; ++x) {
                    int index = layout[y][x];
                    if (index >= 0x100)
                        index = 0;
                    scn.layout[y].append((byte)index);
                }
            }

            for (Object &object : objects) {
                RSDKv1::Scene::Entity obj;

                obj.type          = object.type;
                obj.propertyValue = object.propertyValue;
                obj.posX          = object.pos.x >> 0x10;
                obj.posY          = object.pos.y >> 0x10;

                obj.slotID = object.slotID;
                scn.entities.append(obj);
            }

            scn.write(writer);
            break;
        }
        case ENGINE_v2: {
            RSDKv2::Scene scn;

            scn.title = title;

            for (int i = 0; i < 4; ++i) scn.activeLayer[i] = activeLayer[i];
            scn.midpoint = midpoint;

            scn.width  = width;
            scn.height = height;

            scn.layout.clear();
            for (int y = 0; y < scn.height; ++y) {
                scn.layout.append(QList<ushort>());
                for (int x = 0; x < scn.width; ++x) {
                    scn.layout[y].append(layout[y][x]);
                }
            }

            for (QString name : objectTypeNames) scn.typeNames.append(name);

            for (Object &object : objects) {
                RSDKv2::Scene::Entity obj;

                obj.type          = object.type;
                obj.propertyValue = object.propertyValue;
                obj.posX          = object.pos.x >> 0x10;
                obj.posY          = object.pos.y >> 0x10;

                obj.slotID = object.slotID;
                scn.entities.append(obj);
            }

            scn.write(writer);
            break;
        }
        case ENGINE_v3: {
            RSDKv3::Scene scn;

            scn.title = title;

            for (int i = 0; i < 4; ++i) scn.activeLayer[i] = activeLayer[i];
            scn.midpoint = midpoint;

            scn.width  = width;
            scn.height = height;

            scn.layout.clear();
            for (int y = 0; y < scn.height; ++y) {
                scn.layout.append(QList<ushort>());
                for (int x = 0; x < scn.width; ++x) {
                    scn.layout[y].append(layout[y][x]);
                }
            }

            for (QString name : objectTypeNames) scn.typeNames.append(name);

            for (Object &object : objects) {
                RSDKv3::Scene::Entity obj;

                obj.type          = object.type;
                obj.propertyValue = object.propertyValue;
                obj.posX          = object.pos.x >> 0x10;
                obj.posY          = object.pos.y >> 0x10;

                obj.slotID = object.slotID;
                scn.entities.append(obj);
            }

            scn.write(writer);
            break;
        }
        case ENGINE_v4: {
            RSDKv4::Scene scn;

            scn.title = title;

            for (int i = 0; i < 4; ++i) scn.activeLayers[i] = activeLayer[i];
            scn.midpoint = midpoint;

            scn.width  = width;
            scn.height = height;

            scn.layout.clear();
            for (int y = 0; y < scn.height; ++y) {
                scn.layout.append(QList<ushort>());
                for (int x = 0; x < scn.width; ++x) {
                    scn.layout[y].append(layout[y][x]);
                }
            }

            for (Object &object : objects) {
                RSDKv4::Scene::Entity obj;

                obj.type          = object.type;
                obj.propertyValue = object.propertyValue;
                obj.posX          = object.pos.x;
                obj.posY          = object.pos.y;

                obj.slotID = object.slotID;

                for (int a = 0; a < 0x0F; ++a) {
                    obj.variables[a].value  = object.variables[a].value;
                    obj.variables[a].active = object.variables[a].active;
                }

                scn.entities.append(obj);
            }

            scn.write(writer);
            break;
        }
    }
}
