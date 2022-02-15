#include "rsdkreverse.hpp"

void FormatHelpers::GameConfig::read(byte ver, QString filename)
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
            RSDKv2::GameConfig gameconfig(reader);

            gameWindowText      = gameconfig.gameWindowText;
            gameDescriptionText = gameconfig.gameDescriptionText;
            unknown             = gameconfig.unknown;

            for (QString &obj : gameconfig.objects) {
                ObjectInfo o;
                o.name = QFileInfo(obj).baseName();
                o.script = obj;

                objects.append(o);
            }

            for (QString &sfx : gameconfig.soundFX) {
                SoundInfo s;
                s.m_name = QFileInfo(sfx).baseName();
                s.path   = sfx;

                soundFX.append(s);
            }

            for (RSDKv2::GameConfig::GlobalVariable &var : gameconfig.globalVariables) {
                GlobalVariable v;
                v.name  = var.name;
                v.value = var.value;

                globalVariables.append(v);
            }

            for (RSDKv2::GameConfig::PlayerInfo &plr : gameconfig.players) {
                PlayerInfo p;
                p.m_name = plr.name;
                p.anim   = plr.anim;
                p.script = plr.script;

                players.append(p);
            }

            for (int c = 0; c < 4; ++c) {
                for (RSDKv2::GameConfig::SceneInfo &scn : gameconfig.stageLists[c].scenes) {
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
            RSDKv3::GameConfig gameconfig(reader);

            gameWindowText      = gameconfig.gameWindowText;
            gameDescriptionText = gameconfig.gameDescriptionText;
            unknown             = gameconfig.unknown;

            for (RSDKv3::GameConfig::ObjectInfo &obj : gameconfig.objects) {
                ObjectInfo o;
                o.name = obj.name;
                o.script = obj.script;

                objects.append(o);
            }

            for (QString &sfx : gameconfig.soundFX) {
                SoundInfo s;
                s.m_name = QFileInfo(sfx).baseName();
                s.path   = sfx;

                soundFX.append(s);
            }

            for (RSDKv3::GameConfig::GlobalVariable &var : gameconfig.globalVariables) {
                GlobalVariable v;
                v.name  = var.name;
                v.value = var.value;

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
                for (RSDKv3::GameConfig::SceneInfo &scn : gameconfig.stageLists[c].scenes) {
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
                o.name = obj.name;
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
                v.name  = var.name;
                v.value = var.value;

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
                for (RSDKv4::GameConfig::SceneInfo &scn : gameconfig.stageLists[c].scenes) {
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

void FormatHelpers::GameConfig::write(byte ver, QString filename)
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
            RSDKv2::GameConfig gameconfig;

            gameconfig.gameWindowText      = gameWindowText;
            gameconfig.gameDescriptionText = gameDescriptionText;
            gameconfig.unknown             = unknown;

            for (ObjectInfo &obj : objects) gameconfig.objects.append(obj.script);

            for (SoundInfo &sfx : soundFX) gameconfig.soundFX.append(sfx.path);

            for (GlobalVariable &var : globalVariables) {
                RSDKv2::GameConfig::GlobalVariable v;
                v.name  = var.name;
                v.value = var.value;

                gameconfig.globalVariables.append(v);
            }

            for (PlayerInfo &plr : players) {
                RSDKv2::GameConfig::PlayerInfo p;
                p.name   = plr.m_name;
                p.anim   = plr.anim;
                p.script = plr.script;

                gameconfig.players.append(p);
            }

            for (int c = 0; c < 4; ++c) {
                for (SceneInfo &scn : stageLists[c].scenes) {
                    RSDKv2::GameConfig::SceneInfo s;
                    s.name        = scn.m_name;
                    s.folder      = scn.folder;
                    s.id          = scn.id;
                    s.highlighted = scn.highlighted;

                    gameconfig.stageLists[c].scenes.append(s);
                }
            }
        } break;
        case ENGINE_v3: {
            RSDKv3::GameConfig gameconfig;

            gameconfig.gameWindowText      = gameWindowText;
            gameconfig.gameDescriptionText = gameDescriptionText;
            gameconfig.unknown             = unknown;

            for (ObjectInfo &obj : objects) {
                RSDKv3::GameConfig::ObjectInfo o;
                o.name   = obj.name;
                o.script = obj.script;

                gameconfig.objects.append(o);
            }

            for (SoundInfo &sfx : soundFX) gameconfig.soundFX.append(sfx.path);

            for (GlobalVariable &var : globalVariables) {
                RSDKv3::GameConfig::GlobalVariable v;
                v.name  = var.name;
                v.value = var.value;

                gameconfig.globalVariables.append(v);
            }

            for (PlayerInfo &plr : players) gameconfig.players.append(plr.m_name);

            for (int c = 0; c < 4; ++c) {
                for (SceneInfo &scn : stageLists[c].scenes) {
                    RSDKv3::GameConfig::SceneInfo s;
                    s.name        = scn.m_name;
                    s.folder      = scn.folder;
                    s.id          = scn.id;
                    s.highlighted = scn.highlighted;

                    gameconfig.stageLists[c].scenes.append(s);
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
                o.name   = obj.name;
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

                    gameconfig.stageLists[c].scenes.append(s);
                }
            }
        } break;
    }
}
