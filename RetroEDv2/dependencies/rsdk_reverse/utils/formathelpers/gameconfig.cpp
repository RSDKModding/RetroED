#include "include.hpp"

void FormatHelpers::Gameconfig::read(byte ver, QString filename)
{
    filePath = filename;
    Reader reader(filename);

    gameWindowText      = "Retro-Engine";
    gameDescriptionText = "About";
    unknown             = "Data";
    objects.clear();
    soundFX.clear();
    globalVariables.clear();
    players.clear();
    palette = Palette();
    stageLists.clear();
    for (int i = 0; i < 4; ++i) stageLists.append(Category());

    switch (ver) {
        default: break;
        case ENGINE_v1: break;
        case ENGINE_v2: {
            RSDKv2::Gameconfig gameconfig(reader);

            gameWindowText      = gameconfig.gameWindowText;
            gameDescriptionText = gameconfig.gameDescriptionText;
            unknown             = gameconfig.unknown;

            for (QString &obj : gameconfig.scripts) {
                ObjectInfo o;
                o.m_name = QFileInfo(obj).baseName();
                o.script = obj;

                objects.append(o);
            }

            for (QString &sfx : gameconfig.soundFX) {
                SoundInfo s;
                s.m_name = QFileInfo(sfx).baseName();
                s.path   = sfx;

                soundFX.append(s);
            }

            for (RSDKv2::Gameconfig::GlobalVariable &var : gameconfig.globalVariables) {
                GlobalVariable v;
                v.name = var.name;
                v.value  = var.value;

                globalVariables.append(v);
            }

            for (RSDKv2::Gameconfig::PlayerInfo &plr : gameconfig.players) {
                PlayerInfo p;
                p.m_name = plr.name;
                p.anim   = plr.anim;
                p.script = plr.script;

                players.append(p);
            }

            for (int c = 0; c < 4; ++c) {
                for (RSDKv2::Gameconfig::SceneInfo &scn : gameconfig.categories[c].scenes) {
                    SceneInfo s;
                    s.m_name      = scn.name;
                    s.folder      = scn.folder;
                    s.id          = scn.id;
                    s.highlighted = scn.highlighted;

                    stageLists[c].scenes.append(s);
                }
            }
        } break;
        case ENGINE_v3: {
            RSDKv3::Gameconfig gameconfig(reader);

            gameWindowText      = gameconfig.gameWindowText;
            gameDescriptionText = gameconfig.gameDescriptionText;
            unknown             = gameconfig.unknown;

            for (RSDKv3::Gameconfig::ObjectInfo &obj : gameconfig.objects) {
                ObjectInfo o;
                o.m_name = obj.name;
                o.script = obj.script;

                objects.append(o);
            }

            for (QString &sfx : gameconfig.soundFX) {
                SoundInfo s;
                s.m_name = QFileInfo(sfx).baseName();
                s.path   = sfx;

                soundFX.append(s);
            }

            for (RSDKv3::Gameconfig::GlobalVariable &var : gameconfig.globalVariables) {
                GlobalVariable v;
                v.name = var.name;
                v.value  = var.value;

                globalVariables.append(v);
            }

            for (QString &plr : gameconfig.players) {
                PlayerInfo p;
                p.m_name = plr;
                p.anim   = plr + ".ani";
                p.script = "Players/" + plr + ".txt";

                players.append(p);
            }

            for (int c = 0; c < 4; ++c) {
                for (RSDKv3::Gameconfig::SceneInfo &scn : gameconfig.categories[c].scenes) {
                    SceneInfo s;
                    s.m_name      = scn.name;
                    s.folder      = scn.folder;
                    s.id          = scn.id;
                    s.highlighted = scn.highlighted;

                    stageLists[c].scenes.append(s);
                }
            }
        } break;
        case ENGINE_v4: {
            RSDKv4::GameConfig gameconfig(reader);

            gameWindowText      = gameconfig.gameWindowText;
            gameDescriptionText = gameconfig.gameDescriptionText;
            palette             = gameconfig.palette;

            for (RSDKv4::GameConfig::ObjectInfo &obj : gameconfig.objects) {
                ObjectInfo o;
                o.m_name = obj.name;
                o.script = obj.script;

                objects.append(o);
            }

            for (RSDKv4::GameConfig::SoundInfo &sfx : gameconfig.soundFX) {
                SoundInfo s;
                s.m_name = sfx.name;
                s.path   = sfx.path;

                soundFX.append(s);
            }

            for (RSDKv4::GameConfig::GlobalVariable &var : gameconfig.globalVariables) {
                GlobalVariable v;
                v.name = var.name;
                v.value  = var.value;

                globalVariables.append(v);
            }

            for (QString &plr : gameconfig.players) {
                PlayerInfo p;
                p.m_name = plr;
                p.anim   = plr + ".ani";
                p.script = "Players/" + plr + ".txt";

                players.append(p);
            }

            for (int c = 0; c < 4; ++c) {
                for (RSDKv4::GameConfig::SceneInfo &scn : gameconfig.categories[c].scenes) {
                    SceneInfo s;
                    s.m_name      = scn.name;
                    s.folder      = scn.folder;
                    s.id          = scn.id;
                    s.highlighted = scn.highlighted;

                    stageLists[c].scenes.append(s);
                }
            }
        } break;
    }
}

void FormatHelpers::Gameconfig::write(byte ver, QString filename)
{
    if (filename == "")
        filename = filePath;
    if (filename == "")
        return;
    filePath = filename;
    Writer writer(filename);

    switch (ver) {
        default: break;
        case ENGINE_v1: break;
        case ENGINE_v2: {
            RSDKv2::Gameconfig gameconfig;

            gameconfig.gameWindowText      = gameWindowText;
            gameconfig.gameDescriptionText = gameDescriptionText;
            gameconfig.unknown             = unknown;

            for (ObjectInfo &obj : objects) gameconfig.scripts.append(obj.script);

            for (SoundInfo &sfx : soundFX) gameconfig.soundFX.append(sfx.path);

            for (GlobalVariable &var : globalVariables) {
                RSDKv2::Gameconfig::GlobalVariable v;
                v.name  = var.name;
                v.value = var.value;

                gameconfig.globalVariables.append(v);
            }

            for (PlayerInfo &plr : players) {
                RSDKv2::Gameconfig::PlayerInfo p;
                p.name   = plr.m_name;
                p.anim   = plr.anim;
                p.script = plr.script;

                gameconfig.players.append(p);
            }

            for (int c = 0; c < 4; ++c) {
                for (SceneInfo &scn : stageLists[c].scenes) {
                    RSDKv2::Gameconfig::SceneInfo s;
                    s.name        = scn.m_name;
                    s.folder      = scn.folder;
                    s.id          = scn.id;
                    s.highlighted = scn.highlighted;

                    gameconfig.categories[c].scenes.append(s);
                }
            }
        } break;
        case ENGINE_v3: {
            RSDKv3::Gameconfig gameconfig;

            gameconfig.gameWindowText      = gameWindowText;
            gameconfig.gameDescriptionText = gameDescriptionText;
            gameconfig.unknown             = unknown;

            for (ObjectInfo &obj : objects) {
                RSDKv3::Gameconfig::ObjectInfo o;
                o.name   = obj.m_name;
                o.script = obj.script;

                gameconfig.objects.append(o);
            }

            for (SoundInfo &sfx : soundFX) gameconfig.soundFX.append(sfx.path);

            for (GlobalVariable &var : globalVariables) {
                RSDKv3::Gameconfig::GlobalVariable v;
                v.name  = var.name;
                v.value = var.value;

                gameconfig.globalVariables.append(v);
            }

            for (PlayerInfo &plr : players) gameconfig.players.append(plr.m_name);

            for (int c = 0; c < 4; ++c) {
                for (SceneInfo &scn : stageLists[c].scenes) {
                    RSDKv3::Gameconfig::SceneInfo s;
                    s.name        = scn.m_name;
                    s.folder      = scn.folder;
                    s.id          = scn.id;
                    s.highlighted = scn.highlighted;

                    gameconfig.categories[c].scenes.append(s);
                }
            }
        } break;
        case ENGINE_v4: {
            RSDKv4::GameConfig gameconfig;

            gameconfig.gameWindowText      = gameWindowText;
            gameconfig.gameDescriptionText = gameDescriptionText;
            gameconfig.palette             = palette;

            for (ObjectInfo &obj : objects) {
                RSDKv4::GameConfig::ObjectInfo o;
                o.name   = obj.m_name;
                o.script = obj.script;

                gameconfig.objects.append(o);
            }

            for (SoundInfo &sfx : soundFX) {
                RSDKv4::GameConfig::SoundInfo s;
                s.name = sfx.m_name;
                s.path = sfx.path;

                gameconfig.soundFX.append(s);
            }

            for (GlobalVariable &var : globalVariables) {
                RSDKv4::GameConfig::GlobalVariable v;
                v.name  = var.name;
                v.value = var.value;

                gameconfig.globalVariables.append(v);
            }

            for (PlayerInfo &plr : players) gameconfig.players.append(plr.m_name);

            for (int c = 0; c < 4; ++c) {
                for (SceneInfo &scn : stageLists[c].scenes) {
                    RSDKv4::GameConfig::SceneInfo s;
                    s.name        = scn.m_name;
                    s.folder      = scn.folder;
                    s.id          = scn.id;
                    s.highlighted = scn.highlighted;

                    gameconfig.categories[c].scenes.append(s);
                }
            }
        } break;
    }
}
