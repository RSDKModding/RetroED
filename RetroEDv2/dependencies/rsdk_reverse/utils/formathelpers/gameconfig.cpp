#include "include.hpp"

void FormatHelpers::Gameconfig::read(byte ver, QString filename)
{
    m_filename = filename;
    Reader reader(filename);

    m_gameWindowText      = "Game";
    m_gameDescriptionText = "About";
    m_unknown             = "Data";
    objects.clear();
    m_soundFX.clear();
    m_globalVariables.clear();
    m_players.clear();
    m_masterPalette = Palette();
    m_categories.clear();
    for (int i = 0; i < 4; ++i) m_categories.append(Category());

    switch (ver) {
        default: break;
        case ENGINE_v1: break;
        case ENGINE_v2: {
            RSDKv2::Gameconfig gameconfig(reader);

            m_gameWindowText      = gameconfig.m_gameWindowText;
            m_gameDescriptionText = gameconfig.m_gameDescriptionText;
            m_unknown             = gameconfig.m_unknown;

            for (QString &obj : gameconfig.m_scripts) {
                ObjectInfo o;
                o.m_name   = QFileInfo(obj).baseName();
                o.m_script = obj;

                objects.append(o);
            }

            for (QString &sfx : gameconfig.m_soundFX) {
                SoundInfo s;
                s.m_name = QFileInfo(sfx).baseName();
                s.m_path = sfx;

                m_soundFX.append(s);
            }

            for (RSDKv2::Gameconfig::GlobalVariable &var : gameconfig.m_globalVariables) {
                GlobalVariable v;
                v.m_name  = var.m_name;
                v.m_value = var.m_value;

                m_globalVariables.append(v);
            }

            for (RSDKv2::Gameconfig::PlayerInfo &plr : gameconfig.m_players) {
                PlayerInfo p;
                p.m_name   = plr.m_name;
                p.m_anim   = plr.m_anim;
                p.m_script = plr.m_script;

                m_players.append(p);
            }

            for (int c = 0; c < 4; ++c) {
                for (RSDKv2::Gameconfig::SceneInfo &scn : gameconfig.m_categories[c].m_scenes) {
                    SceneInfo s;
                    s.m_name        = scn.m_name;
                    s.m_folder      = scn.m_folder;
                    s.m_actID       = scn.m_actID;
                    s.m_highlighted = scn.m_highlighted;

                    m_categories[c].m_scenes.append(s);
                }
            }
        } break;
        case ENGINE_v3: {
            RSDKv3::Gameconfig gameconfig(reader);

            m_gameWindowText      = gameconfig.m_gameWindowText;
            m_gameDescriptionText = gameconfig.m_gameDescriptionText;
            m_unknown             = gameconfig.m_unknown;

            for (RSDKv3::Gameconfig::ObjectInfo &obj : gameconfig.m_objects) {
                ObjectInfo o;
                o.m_name   = obj.m_name;
                o.m_script = obj.m_script;

                objects.append(o);
            }

            for (QString &sfx : gameconfig.m_soundFX) {
                SoundInfo s;
                s.m_name = QFileInfo(sfx).baseName();
                s.m_path = sfx;

                m_soundFX.append(s);
            }

            for (RSDKv3::Gameconfig::GlobalVariable &var : gameconfig.globalVariables) {
                GlobalVariable v;
                v.m_name  = var.m_name;
                v.m_value = var.m_value;

                m_globalVariables.append(v);
            }

            for (QString &plr : gameconfig.m_players) {
                PlayerInfo p;
                p.m_name   = plr;
                p.m_anim   = plr + ".ani";
                p.m_script = "Players/" + plr + ".txt";

                m_players.append(p);
            }

            for (int c = 0; c < 4; ++c) {
                for (RSDKv3::Gameconfig::SceneInfo &scn : gameconfig.categories[c].scenes) {
                    SceneInfo s;
                    s.m_name        = scn.name;
                    s.m_folder      = scn.folder;
                    s.m_actID       = scn.m_actID;
                    s.m_highlighted = scn.m_highlighted;

                    m_categories[c].m_scenes.append(s);
                }
            }
        } break;
        case ENGINE_v4: {
            RSDKv4::Gameconfig gameconfig(reader);

            m_gameWindowText      = gameconfig.m_gameWindowText;
            m_gameDescriptionText = gameconfig.m_gameDescriptionText;
            m_masterPalette       = gameconfig.m_masterPalette;

            for (RSDKv4::Gameconfig::ObjectInfo &obj : gameconfig.objects) {
                ObjectInfo o;
                o.m_name   = obj.name;
                o.m_script = obj.script;

                objects.append(o);
            }

            for (RSDKv4::Gameconfig::SoundInfo &sfx : gameconfig.soundFX) {
                SoundInfo s;
                s.m_name = sfx.m_name;
                s.m_path = sfx.m_path;

                m_soundFX.append(s);
            }

            for (RSDKv4::Gameconfig::GlobalVariable &var : gameconfig.globalVariables) {
                GlobalVariable v;
                v.m_name  = var.m_name;
                v.m_value = var.m_value;

                m_globalVariables.append(v);
            }

            for (QString &plr : gameconfig.m_players) {
                PlayerInfo p;
                p.m_name   = plr;
                p.m_anim   = plr + ".ani";
                p.m_script = "Players/" + plr + ".txt";

                m_players.append(p);
            }

            for (int c = 0; c < 4; ++c) {
                for (RSDKv4::Gameconfig::SceneInfo &scn : gameconfig.categories[c].scenes) {
                    SceneInfo s;
                    s.m_name        = scn.name;
                    s.m_folder      = scn.folder;
                    s.m_actID       = scn.m_actID;
                    s.m_highlighted = scn.m_highlighted;

                    m_categories[c].m_scenes.append(s);
                }
            }
        } break;
    }
}

void FormatHelpers::Gameconfig::write(byte ver, QString filename)
{
    if (filename == "")
        filename = m_filename;
    if (filename == "")
        return;
    m_filename = filename;
    Writer writer(filename);

    switch (ver) {
        default: break;
        case ENGINE_v1: break;
        case ENGINE_v2: {
            RSDKv2::Gameconfig gameconfig;

            gameconfig.m_gameWindowText      = m_gameWindowText;
            gameconfig.m_gameDescriptionText = m_gameDescriptionText;
            gameconfig.m_unknown             = m_unknown;

            for (ObjectInfo &obj : objects) gameconfig.m_scripts.append(obj.m_script);

            for (SoundInfo &sfx : m_soundFX) gameconfig.m_soundFX.append(sfx.m_path);

            for (GlobalVariable &var : m_globalVariables) {
                RSDKv2::Gameconfig::GlobalVariable v;
                v.m_name  = var.m_name;
                v.m_value = var.m_value;

                gameconfig.m_globalVariables.append(v);
            }

            for (PlayerInfo &plr : m_players) {
                RSDKv2::Gameconfig::PlayerInfo p;
                p.m_name   = plr.m_name;
                p.m_anim   = plr.m_anim;
                p.m_script = plr.m_script;

                gameconfig.m_players.append(p);
            }

            for (int c = 0; c < 4; ++c) {
                for (SceneInfo &scn : m_categories[c].m_scenes) {
                    RSDKv2::Gameconfig::SceneInfo s;
                    s.m_name        = scn.m_name;
                    s.m_folder      = scn.m_folder;
                    s.m_actID       = scn.m_actID;
                    s.m_highlighted = scn.m_highlighted;

                    gameconfig.m_categories[c].m_scenes.append(s);
                }
            }
        } break;
        case ENGINE_v3: {
            RSDKv3::Gameconfig gameconfig;

            gameconfig.m_gameWindowText      = m_gameWindowText;
            gameconfig.m_gameDescriptionText = m_gameDescriptionText;
            gameconfig.m_unknown             = m_unknown;

            for (ObjectInfo &obj : objects) {
                RSDKv3::Gameconfig::ObjectInfo o;
                o.m_name   = obj.m_name;
                o.m_script = obj.m_script;

                gameconfig.m_objects.append(o);
            }

            for (SoundInfo &sfx : m_soundFX) gameconfig.m_soundFX.append(sfx.m_path);

            for (GlobalVariable &var : m_globalVariables) {
                RSDKv3::Gameconfig::GlobalVariable v;
                v.m_name  = var.m_name;
                v.m_value = var.m_value;

                gameconfig.globalVariables.append(v);
            }

            for (PlayerInfo &plr : m_players) gameconfig.m_players.append(plr.m_name);

            for (int c = 0; c < 4; ++c) {
                for (SceneInfo &scn : m_categories[c].m_scenes) {
                    RSDKv3::Gameconfig::SceneInfo s;
                    s.name        = scn.m_name;
                    s.folder      = scn.m_folder;
                    s.m_actID       = scn.m_actID;
                    s.m_highlighted = scn.m_highlighted;

                    gameconfig.categories[c].scenes.append(s);
                }
            }
        } break;
        case ENGINE_v4: {
            RSDKv4::Gameconfig gameconfig;

            gameconfig.m_gameWindowText      = m_gameWindowText;
            gameconfig.m_gameDescriptionText = m_gameDescriptionText;
            gameconfig.m_masterPalette       = m_masterPalette;

            for (ObjectInfo &obj : objects) {
                RSDKv4::Gameconfig::ObjectInfo o;
                o.name   = obj.m_name;
                o.script = obj.m_script;

                gameconfig.objects.append(o);
            }

            for (SoundInfo &sfx : m_soundFX) {
                RSDKv4::Gameconfig::SoundInfo s;
                s.m_name = sfx.m_name;
                s.m_path = sfx.m_path;

                gameconfig.soundFX.append(s);
            }

            for (GlobalVariable &var : m_globalVariables) {
                RSDKv4::Gameconfig::GlobalVariable v;
                v.m_name  = var.m_name;
                v.m_value = var.m_value;

                gameconfig.globalVariables.append(v);
            }

            for (PlayerInfo &plr : m_players) gameconfig.m_players.append(plr.m_name);

            for (int c = 0; c < 4; ++c) {
                for (SceneInfo &scn : m_categories[c].m_scenes) {
                    RSDKv4::Gameconfig::SceneInfo s;
                    s.name        = scn.m_name;
                    s.folder      = scn.m_folder;
                    s.m_actID       = scn.m_actID;
                    s.m_highlighted = scn.m_highlighted;

                    gameconfig.categories[c].scenes.append(s);
                }
            }
        } break;
    }
}
