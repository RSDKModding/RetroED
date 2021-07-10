#include "include.hpp"

void FormatHelpers::Stageconfig::read(byte ver, QString filename)
{
    Reader reader(filename);
    m_filename = filename;

    objects.clear();
    m_soundFX.clear();
    m_music.clear();
    m_objectSpritesheets.clear();
    loadGlobalScripts = false;
    m_stagePalette      = Palette();

    switch (ver) {
        default: break;
        case ENGINE_v1: {
            RSDKv1::Stageconfig stageconfig(reader);

            m_stagePalette = stageconfig.palette;

            for (RSDKv1::Stageconfig::ObjectInfo &obj : stageconfig.objects) {
                ObjectInfo o;
                o.m_name    = QFileInfo(obj.script).baseName();
                o.m_script  = obj.script;
                o.m_sheetID = obj.m_sheetID;

                objects.append(o);
            }

            for (QString &sht : stageconfig.spriteSheets) m_objectSpritesheets.append(sht);

            for (QString &sfx : stageconfig.soundFX) {
                SoundInfo s;
                s.m_name = QFileInfo(sfx).baseName();
                s.m_path = sfx;

                m_soundFX.append(s);
            }

            for (QString &mus : stageconfig.music) m_music.append(mus);
        } break;
        case ENGINE_v2: {
            RSDKv2::Stageconfig stageconfig(reader);

            loadGlobalScripts = stageconfig.m_loadGlobalScripts;
            m_stagePalette      = stageconfig.m_stagePalette;

            for (QString &obj : stageconfig.m_scripts) {
                ObjectInfo o;
                o.m_name   = QFileInfo(obj).baseName();
                o.m_script = obj;

                objects.append(o);
            }

            for (QString &sfx : stageconfig.m_soundFX) {
                SoundInfo s;
                s.m_name = QFileInfo(sfx).baseName();
                s.m_path = sfx;

                m_soundFX.append(s);
            }
        } break;
        case ENGINE_v3: {
            RSDKv3::Stageconfig stageconfig(reader);

            loadGlobalScripts = stageconfig.loadGlobalScripts;
            m_stagePalette      = stageconfig.m_stagePalette;

            for (RSDKv3::Stageconfig::ObjectInfo &obj : stageconfig.m_objects) {
                ObjectInfo o;
                o.m_name   = obj.name;
                o.m_script = obj.script;

                objects.append(o);
            }

            for (QString &sfx : stageconfig.soundFX) {
                SoundInfo s;
                s.m_name = QFileInfo(sfx).baseName();
                s.m_path = sfx;

                m_soundFX.append(s);
            }
        } break;
        case ENGINE_v4: {
            RSDKv4::Stageconfig stageconfig(reader);

            loadGlobalScripts = stageconfig.loadGlobalScripts;
            m_stagePalette      = stageconfig.m_stagePalette;

            for (RSDKv4::Stageconfig::ObjectInfo &obj : stageconfig.m_objects) {
                ObjectInfo o;
                o.m_name   = obj.name;
                o.m_script = obj.script;

                objects.append(o);
            }

            for (RSDKv4::Stageconfig::SoundInfo &sfx : stageconfig.m_soundFX) {
                SoundInfo s;
                s.m_name = sfx.m_name;
                s.m_path = sfx.m_path;

                m_soundFX.append(s);
            }
        } break;
    }
}

void FormatHelpers::Stageconfig::write(byte ver, QString filename)
{
    if (filename == "")
        filename = m_filename;
    if (filename == "")
        return;
    Writer writer(filename);
    m_filename = filename;

    switch (ver) {
        default: break;
        case ENGINE_v1: {
            RSDKv1::Stageconfig stageconfig;

            stageconfig.palette = m_stagePalette;

            for (ObjectInfo &obj : objects) {
                RSDKv1::Stageconfig::ObjectInfo o;
                o.m_sheetID = obj.m_sheetID;
                o.script  = obj.m_script;

                stageconfig.objects.append(o);
            }

            for (QString &sht : m_objectSpritesheets) stageconfig.spriteSheets.append(sht);

            for (SoundInfo &sfx : m_soundFX) stageconfig.soundFX.append(sfx.m_path);

            for (QString &mus : m_music) stageconfig.music.append(mus);

            stageconfig.write(writer);
        } break;
        case ENGINE_v2: {
            RSDKv2::Stageconfig stageconfig;

            stageconfig.m_loadGlobalScripts = loadGlobalScripts;
            stageconfig.m_stagePalette      = m_stagePalette;

            for (ObjectInfo &obj : objects) stageconfig.m_scripts.append(obj.m_script);

            for (SoundInfo &sfx : m_soundFX) stageconfig.m_soundFX.append(sfx.m_path);

            stageconfig.write(writer);
        } break;
        case ENGINE_v3: {
            RSDKv3::Stageconfig stageconfig;

            stageconfig.loadGlobalScripts = loadGlobalScripts;
            stageconfig.m_stagePalette      = m_stagePalette;

            for (ObjectInfo &obj : objects) {
                RSDKv3::Stageconfig::ObjectInfo o;
                o.name   = obj.m_name;
                o.script = obj.m_script;

                stageconfig.m_objects.append(o);
            }

            for (SoundInfo &sfx : m_soundFX) {
                stageconfig.soundFX.append(sfx.m_path);
            }

            stageconfig.write(writer);
        } break;
        case ENGINE_v4: {
            RSDKv4::Stageconfig stageconfig;

            stageconfig.loadGlobalScripts = loadGlobalScripts;
            stageconfig.m_stagePalette      = m_stagePalette;

            for (ObjectInfo &obj : objects) {
                RSDKv4::Stageconfig::ObjectInfo o;
                o.name   = obj.m_name;
                o.script = obj.m_script;

                stageconfig.m_objects.append(o);
            }

            for (SoundInfo &sfx : m_soundFX) {
                RSDKv4::Stageconfig::SoundInfo s;
                s.m_name = sfx.m_name;
                s.m_path = sfx.m_path;

                stageconfig.m_soundFX.append(s);
            }

            stageconfig.write(writer);
        } break;
    }
}
