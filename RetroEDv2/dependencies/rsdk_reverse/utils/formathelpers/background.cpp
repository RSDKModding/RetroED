#include "include.hpp"

void FormatHelpers::Background::read(byte ver, QString filename)
{
    Reader reader(filename);
    m_filename = filename;

    m_hScroll.clear();
    m_vScroll.clear();
    layers.clear();

    switch (ver) {
        default: break;
        case ENGINE_v1: {
            RSDKv1::Background bg(reader);

            for (RSDKv1::Background::ScrollInfo &scr : bg.hScroll) {
                ScrollInfo scroll;
                scroll.m_behaviour     = scr.behaviour;
                scroll.m_relativeSpeed = scr.relativeSpeed / 64.0f;
                scroll.m_constantSpeed = (scr.constantSpeed << 10) / 65536.0f;

                m_hScroll.append(scroll);
            }

            for (RSDKv1::Background::ScrollInfo &scr : bg.vScroll) {
                ScrollInfo scroll;
                scroll.m_behaviour     = scr.behaviour;
                scroll.m_relativeSpeed = scr.relativeSpeed / 64.0f;
                scroll.m_constantSpeed = (scr.constantSpeed << 10) / 65536.0f;

                m_vScroll.append(scroll);
            }

            for (RSDKv1::Background::Layer &lyr : bg.layers) {
                Layer layer;
                layer.width         = lyr.width;
                layer.height        = lyr.height;
                layer.type     = lyr.behaviour;
                layer.parallaxFactor = lyr.relativeSpeed / 64.0f;
                layer.scrollSpeed = (lyr.constantSpeed << 10) / 65536.0f;
                layer.m_lineIndexes   = lyr.lineIndexes;

                layer.layout.clear();
                for (int y = 0; y < layer.height; ++y) {
                    layer.layout.append(QList<ushort>());
                    for (int x = 0; x < layer.width; ++x) {
                        layer.layout[y].append(lyr.layout[y][x]);
                    }
                }

                layers.append(layer);
            }
        } break;
        case ENGINE_v2: {
            RSDKv2::Background bg(reader);

            for (RSDKv2::Background::ScrollInfo &scr : bg.m_hScroll) {
                ScrollInfo scroll;
                scroll.m_behaviour     = scr.m_behaviour;
                scroll.m_relativeSpeed = scr.m_relativeSpeed / 64.0f;
                scroll.m_constantSpeed = (scr.m_constantSpeed << 10) / 65536.0f;

                m_hScroll.append(scroll);
            }

            for (RSDKv2::Background::ScrollInfo &scr : bg.m_vScroll) {
                ScrollInfo scroll;
                scroll.m_behaviour     = scr.m_behaviour;
                scroll.m_relativeSpeed = scr.m_relativeSpeed / 64.0f;
                scroll.m_constantSpeed = (scr.m_constantSpeed << 10) / 65536.0f;

                m_vScroll.append(scroll);
            }

            for (RSDKv2::Background::Layer &lyr : bg.m_layers) {
                Layer layer;
                layer.width         = lyr.m_width;
                layer.height        = lyr.m_height;
                layer.type     = lyr.m_behaviour;
                layer.parallaxFactor = lyr.m_relativeSpeed / 64.0f;
                layer.scrollSpeed = (lyr.m_constantSpeed << 10) / 65536.0f;
                layer.m_lineIndexes   = lyr.m_lineIndexes;

                layer.layout.clear();
                for (int y = 0; y < layer.height; ++y) {
                    layer.layout.append(QList<ushort>());
                    for (int x = 0; x < layer.width; ++x) {
                        layer.layout[y].append(lyr.m_layout[y][x]);
                    }
                }

                layers.append(layer);
            }
        } break;
        case ENGINE_v3: {
            RSDKv3::Background bg(reader);

            for (RSDKv3::Background::ScrollInfo &scr : bg.m_hScroll) {
                ScrollInfo scroll;
                scroll.m_behaviour     = scr.m_behaviour;
                scroll.m_relativeSpeed = scr.m_relativeSpeed / 256.0f;
                scroll.m_constantSpeed = (scr.m_constantSpeed << 10) / 65536.0f;

                m_hScroll.append(scroll);
            }

            for (RSDKv3::Background::ScrollInfo &scr : bg.m_vScroll) {
                ScrollInfo scroll;
                scroll.m_behaviour     = scr.m_behaviour;
                scroll.m_relativeSpeed = scr.m_relativeSpeed / 256.0f;
                scroll.m_constantSpeed = (scr.m_constantSpeed << 10) / 65536.0f;

                m_vScroll.append(scroll);
            }

            for (RSDKv3::Background::Layer &lyr : bg.m_layers) {
                Layer layer;
                layer.width         = lyr.m_width;
                layer.height        = lyr.m_height;
                layer.type     = lyr.m_behaviour;
                layer.parallaxFactor = lyr.m_relativeSpeed / 256.0f;
                layer.scrollSpeed = (lyr.m_constantSpeed << 10) / 65536.0f;
                layer.m_lineIndexes   = lyr.m_lineIndexes;

                layer.layout.clear();
                for (int y = 0; y < layer.height; ++y) {
                    layer.layout.append(QList<ushort>());
                    for (int x = 0; x < layer.width; ++x) {
                        layer.layout[y].append(lyr.m_layout[y][x]);
                    }
                }

                layers.append(layer);
            }
        } break;
        case ENGINE_v4: {
            RSDKv4::Background bg(reader);

            for (RSDKv4::Background::ScrollInfo &scr : bg.m_hScroll) {
                ScrollInfo scroll;
                scroll.m_behaviour     = scr.m_behaviour;
                scroll.m_relativeSpeed = scr.m_relativeSpeed / 256.0f;
                scroll.m_constantSpeed = (scr.m_constantSpeed << 10) / 65536.0f;

                m_hScroll.append(scroll);
            }

            for (RSDKv4::Background::ScrollInfo &scr : bg.m_vScroll) {
                ScrollInfo scroll;
                scroll.m_behaviour     = scr.m_behaviour;
                scroll.m_relativeSpeed = scr.m_relativeSpeed / 256.0f;
                scroll.m_constantSpeed = (scr.m_constantSpeed << 10) / 65536.0f;

                m_vScroll.append(scroll);
            }

            for (RSDKv4::Background::Layer &lyr : bg.m_layers) {
                Layer layer;
                layer.width         = lyr.m_width;
                layer.height        = lyr.m_height;
                layer.type     = lyr.m_behaviour;
                layer.parallaxFactor = lyr.m_relativeSpeed / 256.0f;
                layer.scrollSpeed = (lyr.m_constantSpeed << 10) / 65536.0f;
                layer.m_lineIndexes   = lyr.m_lineIndexes;

                layer.layout.clear();
                for (int y = 0; y < layer.height; ++y) {
                    layer.layout.append(QList<ushort>());
                    for (int x = 0; x < layer.width; ++x) {
                        layer.layout[y].append(lyr.m_layout[y][x]);
                    }
                }

                layers.append(layer);
            }
        } break;
    }

    scrollInfoFromIndices();
}

void FormatHelpers::Background::write(byte ver, QString filename)
{
    if (filename == "")
        filename = m_filename;
    if (filename == "")
        return;
    Writer writer(filename);
    m_filename = filename;

    scrollIndicesFromInfo();

    switch (ver) {
        default: break;
        case ENGINE_v1: {
            RSDKv1::Background bg;

            for (ScrollInfo &scr : m_hScroll) {
                RSDKv1::Background::ScrollInfo scroll;
                scroll.behaviour     = scr.m_behaviour;
                scroll.relativeSpeed = scr.m_relativeSpeed * 64;
                scroll.constantSpeed = (int)(scr.m_constantSpeed * 65536) >> 10;

                bg.hScroll.append(scroll);
            }

            for (ScrollInfo &scr : m_vScroll) {
                RSDKv1::Background::ScrollInfo scroll;
                scroll.behaviour     = scr.m_behaviour;
                scroll.relativeSpeed = scr.m_relativeSpeed * 64;
                scroll.constantSpeed = (int)(scr.m_constantSpeed * 65536) >> 10;

                bg.vScroll.append(scroll);
            }

            for (Layer &lyr : layers) {
                RSDKv1::Background::Layer layer;
                layer.width         = lyr.width;
                layer.height        = lyr.height;
                layer.behaviour     = lyr.type;
                layer.relativeSpeed = lyr.parallaxFactor * 64;
                layer.constantSpeed = (int)(lyr.scrollSpeed * 65536) >> 10;
                layer.lineIndexes   = lyr.m_lineIndexes;

                layer.layout.clear();
                for (int y = 0; y < layer.height; ++y) {
                    layer.layout.append(QList<byte>());
                    for (int x = 0; x < layer.width; ++x) {
                        int index = lyr.layout[y][x];
                        if (index >= 0x100)
                            index = 0;
                        layer.layout[y].append((byte)index);
                    }
                }

                bg.layers.append(layer);
            }

            bg.write(writer);
        } break;
        case ENGINE_v2: {
            RSDKv2::Background bg;

            for (ScrollInfo &scr : m_hScroll) {
                RSDKv2::Background::ScrollInfo scroll;
                scroll.m_behaviour     = scr.m_behaviour;
                scroll.m_relativeSpeed = scr.m_relativeSpeed * 64;
                scroll.m_constantSpeed = (int)(scr.m_constantSpeed * 65536) >> 10;

                bg.m_hScroll.append(scroll);
            }

            for (ScrollInfo &scr : m_vScroll) {
                RSDKv2::Background::ScrollInfo scroll;
                scroll.m_behaviour     = scr.m_behaviour;
                scroll.m_relativeSpeed = scr.m_relativeSpeed * 64;
                scroll.m_constantSpeed = (int)(scr.m_constantSpeed * 65536) >> 10;

                bg.m_vScroll.append(scroll);
            }

            for (Layer &lyr : layers) {
                RSDKv2::Background::Layer layer;
                layer.m_width         = lyr.width;
                layer.m_height        = lyr.height;
                layer.m_behaviour     = lyr.type;
                layer.m_relativeSpeed = lyr.parallaxFactor * 64;
                layer.m_constantSpeed = (int)(lyr.scrollSpeed * 65536) >> 10;
                layer.m_lineIndexes   = lyr.m_lineIndexes;

                layer.m_layout.clear();
                for (int y = 0; y < layer.m_height; ++y) {
                    layer.m_layout.append(QList<byte>());
                    for (int x = 0; x < layer.m_width; ++x) {
                        int index = lyr.layout[y][x];
                        if (index >= 0x100)
                            index = 0;
                        layer.m_layout[y].append((byte)index);
                    }
                }

                bg.m_layers.append(layer);
            }

            bg.write(writer);
        } break;
        case ENGINE_v3: {
            RSDKv3::Background bg;

            for (ScrollInfo &scr : m_hScroll) {
                RSDKv3::Background::ScrollInfo scroll;
                scroll.m_behaviour     = scr.m_behaviour;
                scroll.m_relativeSpeed = scr.m_relativeSpeed * 256;
                scroll.m_constantSpeed = (int)(scr.m_constantSpeed * 65536) >> 10;

                bg.m_hScroll.append(scroll);
            }

            for (ScrollInfo &scr : m_vScroll) {
                RSDKv3::Background::ScrollInfo scroll;
                scroll.m_behaviour     = scr.m_behaviour;
                scroll.m_relativeSpeed = scr.m_relativeSpeed * 256;
                scroll.m_constantSpeed = (int)(scr.m_constantSpeed * 65536) >> 10;

                bg.m_vScroll.append(scroll);
            }

            for (Layer &lyr : layers) {
                RSDKv3::Background::Layer layer;
                layer.m_width         = lyr.width;
                layer.m_height        = lyr.height;
                layer.m_behaviour     = lyr.type;
                layer.m_relativeSpeed = lyr.parallaxFactor * 256;
                layer.m_constantSpeed = (int)(lyr.scrollSpeed * 65536) >> 10;
                layer.m_lineIndexes   = lyr.m_lineIndexes;

                layer.m_layout.clear();
                for (int y = 0; y < layer.m_height; ++y) {
                    layer.m_layout.append(QList<ushort>());
                    for (int x = 0; x < layer.m_width; ++x) {
                        layer.m_layout[y].append(lyr.layout[y][x]);
                    }
                }

                bg.m_layers.append(layer);
            }

            bg.write(writer);
        } break;
        case ENGINE_v4: {
            RSDKv4::Background bg;

            for (ScrollInfo &scr : m_hScroll) {
                RSDKv4::Background::ScrollInfo scroll;
                scroll.m_behaviour     = scr.m_behaviour;
                scroll.m_relativeSpeed = scr.m_relativeSpeed * 256;
                scroll.m_constantSpeed = (int)(scr.m_constantSpeed * 65536) >> 10;

                bg.m_hScroll.append(scroll);
            }

            for (ScrollInfo &scr : m_vScroll) {
                RSDKv4::Background::ScrollInfo scroll;
                scroll.m_behaviour     = scr.m_behaviour;
                scroll.m_relativeSpeed = scr.m_relativeSpeed * 256;
                scroll.m_constantSpeed = (int)(scr.m_constantSpeed * 65536) >> 10;

                bg.m_vScroll.append(scroll);
            }

            for (Layer &lyr : layers) {
                RSDKv4::Background::Layer layer;
                layer.m_width         = lyr.width;
                layer.m_height        = lyr.height;
                layer.m_behaviour     = lyr.type;
                layer.m_relativeSpeed = lyr.parallaxFactor * 256;
                layer.m_constantSpeed = (int)(lyr.scrollSpeed * 65536) >> 10;
                layer.m_lineIndexes   = lyr.m_lineIndexes;

                layer.m_layout.clear();
                for (int y = 0; y < layer.m_height; ++y) {
                    layer.m_layout.append(QList<ushort>());
                    for (int x = 0; x < layer.m_width; ++x) {
                        layer.m_layout[y].append(lyr.layout[y][x]);
                    }
                }

                bg.m_layers.append(layer);
            }

            bg.write(writer);
        } break;
    }
}

void FormatHelpers::Background::scrollInfoFromIndices()
{
    for (Layer &layer : layers) {
        layer.scrollInfos.clear();
        QList<ScrollInfo> infos;
        if (layer.type == 1)
            infos = m_hScroll;
        else if (layer.type == 2)
            infos = m_vScroll;
        else
            continue;

        int prev  = layer.m_lineIndexes.count() > 0 ? layer.m_lineIndexes[0] : -1;
        int start = 0;
        int h     = 0;

        for (; h < layer.m_lineIndexes.count(); ++h) {
            if ((byte)layer.m_lineIndexes[h] != prev) {
                ScrollIndexInfo info;

                info.startLine     = start;
                info.length        = (h - start);
                info.parallaxFactor = infos[prev].m_relativeSpeed;
                info.scrollSpeed = infos[prev].m_constantSpeed;
                info.m_scrollPos     = 0.0f;
                info.deform     = infos[prev].m_behaviour;

                layer.scrollInfos.append(info);
                start = h;
            }

            prev = (byte)layer.m_lineIndexes[h];
        }

        {
            ScrollIndexInfo info;

            info.startLine     = start;
            info.length        = (h - start);
            info.parallaxFactor = infos[0].m_relativeSpeed;
            info.scrollSpeed = infos[0].m_constantSpeed;
            info.m_scrollPos     = 0.0f;
            info.deform     = infos[0].m_behaviour;

            layer.scrollInfos.append(info);
        }
    }
}

void FormatHelpers::Background::scrollIndicesFromInfo()
{
    m_hScroll.clear();
    m_vScroll.clear();

    int hID = 0;
    int vID = 0;

    for (Layer &layer : layers) {
        bool hScroll = layer.type == 1;
        layer.m_lineIndexes.clear();

        if (layer.type != 1 && layer.type != 2) {
            // other layers dont need any scrolling, TODO: check this works
            continue;
        }
        if (layer.width == 0 || layer.height == 0)
            continue; // basically invalid layers, dont write em

        if (hScroll) {
            layer.m_lineIndexes.resize(layer.height * 0x80);
        }
        else {
            layer.m_lineIndexes.resize(layer.width * 0x80);
        }

        int id = 0;
        for (ScrollIndexInfo &info : layer.scrollInfos) {
            int infoID = id;
            ScrollInfo sInfo;
            sInfo.m_behaviour     = info.deform;
            sInfo.m_relativeSpeed = info.parallaxFactor;
            sInfo.m_constantSpeed = info.scrollSpeed;

            int scrollID = 0;
            if (hScroll) {
                for (ScrollInfo &info : m_hScroll) {
                    if (info.m_relativeSpeed == sInfo.m_relativeSpeed
                        && info.m_constantSpeed == sInfo.m_constantSpeed
                        && info.m_behaviour == sInfo.m_behaviour) {
                        infoID = scrollID;
                        break;
                    }
                    ++scrollID;
                }
            }
            else {
                for (ScrollInfo &info : m_vScroll) {
                    if (info.m_relativeSpeed == sInfo.m_relativeSpeed
                        && info.m_constantSpeed == sInfo.m_constantSpeed
                        && info.m_behaviour == sInfo.m_behaviour) {
                        infoID = scrollID;
                        break;
                    }
                    ++scrollID;
                }
            }

            for (int i = info.startLine; i < info.startLine + info.length; ++i) {
                layer.m_lineIndexes[i] = (byte)infoID;
            }

            // New Info needs to be added
            if (infoID == id) {
                if (hScroll) {
                    m_hScroll.append(sInfo);
                    ++hID;
                }
                else {
                    m_vScroll.append(sInfo);
                    ++vID;
                }
                ++id;
            }
        }
    }
}
