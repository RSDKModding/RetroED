#include "include.hpp"

void FormatHelpers::Scene::read(byte ver, QString filename)
{
    Reader reader(filename);
    filepath = filename;

    m_title = "Stage";

    m_activeLayer[0] = 9;
    m_activeLayer[1] = 9;
    m_activeLayer[2] = 9;
    m_activeLayer[3] = 9;
    m_midpoint       = 3;

    m_music      = 0;
    m_background = 0;

    m_playerXPos = 0;
    m_playerYPos = 0;

    layout.clear();

    objects.clear();
    objectTypeNames.clear();

    width  = 0;
    height = 0;

    switch (ver) {
        default: break;
        case ENGINE_v1: {
            RSDKv1::Scene scn(reader);

            m_title = scn.title;

            m_music      = scn.musicID;
            m_background = scn.backgroundID;

            m_playerXPos = scn.playerXPos;
            m_playerYPos = scn.playerYPos;

            width  = scn.width;
            height = scn.height;

            for (int y = 0; y < height; ++y) {
                layout.append(QList<ushort>());
                for (int x = 0; x < width; ++x) {
                    layout[y].append(scn.layout[y][x]);
                }
            }

            for (RSDKv1::Scene::Object &object : scn.objects) {
                Object obj;

                obj.type          = object.type;
                obj.m_propertyValue = object.propertyValue;
                obj.m_position.x    = object.XPos << 0x10;
                obj.m_position.y    = object.YPos << 0x10;

                obj.m_id = object.slotID;
                objects.append(obj);
            }
            break;
        }
        case ENGINE_v2: {
            RSDKv2::Scene scn(reader);

            m_title = scn.m_title;

            for (int i = 0; i < 4; ++i) m_activeLayer[i] = scn.m_activeLayer[i];
            m_midpoint = scn.m_midpoint;

            width  = scn.m_width;
            height = scn.m_height;

            for (int y = 0; y < height; ++y) {
                layout.append(QList<ushort>());
                for (int x = 0; x < width; ++x) {
                    layout[y].append(scn.m_layout[y][x]);
                }
            }

            for (QString name : scn.m_objectTypeNames) objectTypeNames.append(name);

            for (RSDKv2::Scene::Object &object : scn.m_objects) {
                Object obj;

                obj.type          = object.m_type;
                obj.m_propertyValue = object.m_propertyValue;
                obj.m_position.x    = object.m_xPos << 0x10;
                obj.m_position.y    = object.m_yPos << 0x10;

                obj.m_id = object.m_id;
                objects.append(obj);
            }
            break;
        }
        case ENGINE_v3: {
            RSDKv3::Scene scn(reader);

            m_title = scn.m_title;

            for (int i = 0; i < 4; ++i) m_activeLayer[i] = scn.m_activeLayer[i];
            m_midpoint = scn.m_midpoint;

            width  = scn.m_width;
            height = scn.m_height;

            for (int y = 0; y < height; ++y) {
                layout.append(QList<ushort>());
                for (int x = 0; x < width; ++x) {
                    layout[y].append(scn.m_layout[y][x]);
                }
            }

            for (QString name : scn.m_objectTypeNames) objectTypeNames.append(name);

            for (RSDKv3::Scene::Object &object : scn.m_objects) {
                Object obj;

                obj.type          = object.m_type;
                obj.m_propertyValue = object.m_propertyValue;
                obj.m_position.x    = object.m_xPos << 0x10;
                obj.m_position.y    = object.m_yPos << 0x10;

                obj.m_id = object.m_id;
                objects.append(obj);
            }
            break;
        }
        case ENGINE_v4: {
            RSDKv4::Scene scn(reader);

            m_title = scn.m_title;

            for (int i = 0; i < 4; ++i) m_activeLayer[i] = scn.m_activeLayer[i];
            m_midpoint = scn.m_midpoint;

            width  = scn.m_width;
            height = scn.m_height;

            for (int y = 0; y < height; ++y) {
                layout.append(QList<ushort>());
                for (int x = 0; x < width; ++x) {
                    layout[y].append(scn.m_layout[y][x]);
                }
            }

            for (RSDKv4::Scene::Object &object : scn.m_objects) {
                Object obj;

                obj.type          = object.m_type;
                obj.m_propertyValue = object.m_subtype;
                obj.m_position.x    = object.m_xPos;
                obj.m_position.y    = object.m_yPos;

                obj.m_id = object.m_id;

                for (int a = 0; a < 0x0F; ++a) {
                    obj.m_attributes[a].m_value  = object.m_attributes[a].m_value;
                    obj.m_attributes[a].m_active = object.m_attributes[a].m_active;
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

            scn.title = m_title;

            scn.musicID      = m_music;
            scn.backgroundID = m_background;

            scn.playerXPos = m_playerXPos;
            scn.playerYPos = m_playerYPos;

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
                RSDKv1::Scene::Object obj;

                obj.type          = object.type;
                obj.propertyValue = object.m_propertyValue;
                obj.XPos          = object.m_position.x >> 0x10;
                obj.YPos          = object.m_position.y >> 0x10;

                obj.slotID = object.m_id;
                scn.objects.append(obj);
            }

            scn.write(writer);
            break;
        }
        case ENGINE_v2: {
            RSDKv2::Scene scn;

            scn.m_title = m_title;

            for (int i = 0; i < 4; ++i) scn.m_activeLayer[i] = m_activeLayer[i];
            scn.m_midpoint = m_midpoint;

            scn.m_width  = width;
            scn.m_height = height;

            scn.m_layout.clear();
            for (int y = 0; y < scn.m_height; ++y) {
                scn.m_layout.append(QList<ushort>());
                for (int x = 0; x < scn.m_width; ++x) {
                    scn.m_layout[y].append(layout[y][x]);
                }
            }

            for (QString name : objectTypeNames) scn.m_objectTypeNames.append(name);

            for (Object &object : objects) {
                RSDKv2::Scene::Object obj;

                obj.m_type          = object.type;
                obj.m_propertyValue = object.m_propertyValue;
                obj.m_xPos          = object.m_position.x >> 0x10;
                obj.m_yPos          = object.m_position.y >> 0x10;

                obj.m_id = object.m_id;
                scn.m_objects.append(obj);
            }

            scn.write(writer);
            break;
        }
        case ENGINE_v3: {
            RSDKv3::Scene scn;

            scn.m_title = m_title;

            for (int i = 0; i < 4; ++i) scn.m_activeLayer[i] = m_activeLayer[i];
            scn.m_midpoint = m_midpoint;

            scn.m_width  = width;
            scn.m_height = height;

            for (int y = 0; y < scn.m_height; ++y) {
                scn.m_layout.append(QList<ushort>());
                for (int x = 0; x < scn.m_width; ++x) {
                    scn.m_layout[y].append(layout[y][x]);
                }
            }

            for (QString name : objectTypeNames) scn.m_objectTypeNames.append(name);

            for (Object &object : objects) {
                RSDKv3::Scene::Object obj;

                obj.m_type          = object.type;
                obj.m_propertyValue = object.m_propertyValue;
                obj.m_xPos          = object.m_position.x >> 0x10;
                obj.m_yPos          = object.m_position.y >> 0x10;

                obj.m_id = object.m_id;
                scn.m_objects.append(obj);
            }

            scn.write(writer);
            break;
        }
        case ENGINE_v4: {
            RSDKv4::Scene scn;

            scn.m_title = m_title;

            for (int i = 0; i < 4; ++i) scn.m_activeLayer[i] = m_activeLayer[i];
            scn.m_midpoint = m_midpoint;

            scn.m_width  = width;
            scn.m_height = height;

            for (int y = 0; y < scn.m_height; ++y) {
                scn.m_layout.append(QList<ushort>());
                for (int x = 0; x < scn.m_width; ++x) {
                    scn.m_layout[y].append(layout[y][x]);
                }
            }

            for (Object &object : objects) {
                RSDKv4::Scene::Object obj;

                obj.m_type    = object.type;
                obj.m_subtype = object.m_propertyValue;
                obj.m_xPos    = object.m_position.x;
                obj.m_yPos    = object.m_position.x;

                obj.m_id = object.m_id;

                for (int a = 0; a < 0x0F; ++a) {
                    obj.m_attributes[a].m_value  = object.m_attributes[a].m_value;
                    obj.m_attributes[a].m_active = object.m_attributes[a].m_active;
                }

                scn.m_objects.append(obj);
            }

            scn.write(writer);
            break;
        }
    }
}
