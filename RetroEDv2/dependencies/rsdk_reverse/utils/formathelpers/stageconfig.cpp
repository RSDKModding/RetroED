#include "include.hpp"

void FormatHelpers::Stageconfig::read(byte ver, QString filename)
{
    Reader reader(filename);
    filePath = filename;

    objects.clear();
    soundFX.clear();
    music.clear();
    spriteSheets.clear();
    loadGlobalScripts = false;
    palette           = Palette();

    switch (ver) {
        default: break;
        case ENGINE_v1: {
            RSDKv1::StageConfig stageconfig(reader);

            palette = stageconfig.palette;

            for (RSDKv1::StageConfig::ObjectInfo &obj : stageconfig.objects) {
                ObjectInfo o;
                o.m_name  = QFileInfo(obj.script).baseName();
                o.script  = obj.script;
                o.sheetID = obj.sheetID;

                objects.append(o);
            }

            for (QString &sht : stageconfig.spriteSheets) spriteSheets.append(sht);

            for (QString &sfx : stageconfig.soundFX) {
                SoundInfo s;
                s.m_name = QFileInfo(sfx).baseName();
                s.path   = sfx;

                soundFX.append(s);
            }

            for (QString &mus : stageconfig.music) music.append(mus);
            break;
        }
        case ENGINE_v2: {
            RSDKv2::StageConfig stageconfig(reader);

            loadGlobalScripts = stageconfig.loadGlobalScripts;
            palette           = stageconfig.palette;

            for (QString &obj : stageconfig.scripts) {
                ObjectInfo o;
                o.m_name = QFileInfo(obj).baseName();
                o.script = obj;

                objects.append(o);
            }

            for (QString &sfx : stageconfig.soundFX) {
                SoundInfo s;
                s.m_name = QFileInfo(sfx).baseName();
                s.path   = sfx;

                soundFX.append(s);
            }
            break;
        }
        case ENGINE_v3: {
            RSDKv3::StageConfig stageconfig(reader);

            loadGlobalScripts = stageconfig.loadGlobalScripts;
            palette           = stageconfig.palette;

            for (RSDKv3::StageConfig::ObjectInfo &obj : stageconfig.objects) {
                ObjectInfo o;
                o.m_name = obj.name;
                o.script = obj.script;

                objects.append(o);
            }

            for (QString &sfx : stageconfig.soundFX) {
                SoundInfo s;
                s.m_name = QFileInfo(sfx).baseName();
                s.path   = sfx;

                soundFX.append(s);
            }
            break;
        }
        case ENGINE_v4: {
            RSDKv4::StageConfig stageconfig(reader);

            loadGlobalScripts = stageconfig.loadGlobalScripts;
            palette           = stageconfig.palette;

            for (RSDKv4::StageConfig::ObjectInfo &obj : stageconfig.objects) {
                ObjectInfo o;
                o.m_name = obj.name;
                o.script = obj.script;

                objects.append(o);
            }

            for (RSDKv4::StageConfig::SoundInfo &sfx : stageconfig.soundFX) {
                SoundInfo s;
                s.m_name = sfx.name;
                s.path   = sfx.path;

                soundFX.append(s);
            }
            break;
        }
    }
}

void FormatHelpers::Stageconfig::write(byte ver, QString filename)
{
    if (filename == "")
        filename = filePath;
    if (filename == "")
        return;
    Writer writer(filename);
    filePath = filename;

    switch (ver) {
        default: break;
        case ENGINE_v1: {
            RSDKv1::StageConfig stageconfig;

            stageconfig.palette = palette;

            for (ObjectInfo &obj : objects) {
                RSDKv1::StageConfig::ObjectInfo o;
                o.sheetID = obj.sheetID;
                o.script  = obj.script;

                stageconfig.objects.append(o);
            }

            for (QString &sheet : spriteSheets) stageconfig.spriteSheets.append(sheet);

            for (SoundInfo &sfx : soundFX) stageconfig.soundFX.append(sfx.path);

            for (QString &mus : music) stageconfig.music.append(mus);

            stageconfig.write(writer);
        } break;
        case ENGINE_v2: {
            RSDKv2::StageConfig stageconfig;

            stageconfig.loadGlobalScripts = loadGlobalScripts;
            stageconfig.palette           = palette;

            for (ObjectInfo &obj : objects) stageconfig.scripts.append(obj.script);

            for (SoundInfo &sfx : soundFX) stageconfig.soundFX.append(sfx.path);

            stageconfig.write(writer);
        } break;
        case ENGINE_v3: {
            RSDKv3::StageConfig stageconfig;

            stageconfig.loadGlobalScripts = loadGlobalScripts;
            stageconfig.palette           = palette;

            for (ObjectInfo &obj : objects) {
                RSDKv3::StageConfig::ObjectInfo o;
                o.name   = obj.m_name;
                o.script = obj.script;

                stageconfig.objects.append(o);
            }

            for (SoundInfo &sfx : soundFX) {
                stageconfig.soundFX.append(sfx.path);
            }

            stageconfig.write(writer);
        } break;
        case ENGINE_v4: {
            RSDKv4::StageConfig stageconfig;

            stageconfig.loadGlobalScripts = loadGlobalScripts;
            stageconfig.palette           = palette;

            for (ObjectInfo &obj : objects) {
                RSDKv4::StageConfig::ObjectInfo o;
                o.name   = obj.m_name;
                o.script = obj.script;

                stageconfig.objects.append(o);
            }

            for (SoundInfo &sfx : soundFX) {
                RSDKv4::StageConfig::SoundInfo s;
                s.name = sfx.m_name;
                s.path = sfx.path;

                stageconfig.soundFX.append(s);
            }

            stageconfig.write(writer);
        } break;
    }
}
