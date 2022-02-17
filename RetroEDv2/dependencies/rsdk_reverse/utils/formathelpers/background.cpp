#include "rsdkreverse.hpp"

void FormatHelpers::Background::read(byte ver, QString filename)
{
    Reader reader(filename);
    filePath = filename;

    hScroll.clear();
    vScroll.clear();
    layers.clear();

    switch (ver) {
        default: break;
        case ENGINE_v1: {
            RSDKv1::Background bg(reader);

            for (RSDKv1::Background::ScrollInfo &scr : bg.hScroll) {
                ScrollInfo scroll;
                scroll.deform         = scr.deform;
                scroll.parallaxFactor = scr.parallaxFactor / 128.0f;
                scroll.scrollSpeed    = (scr.scrollSpeed << 10) / 65536.0f;

                hScroll.append(scroll);
            }

            for (RSDKv1::Background::ScrollInfo &scr : bg.vScroll) {
                ScrollInfo scroll;
                scroll.deform         = scr.deform;
                scroll.parallaxFactor = scr.parallaxFactor / 128.0f;
                scroll.scrollSpeed    = (scr.scrollSpeed << 10) / 65536.0f;

                vScroll.append(scroll);
            }

            for (RSDKv1::Background::Layer &lyr : bg.layers) {
                Layer layer;
                layer.width          = lyr.width;
                layer.height         = lyr.height;
                layer.type           = lyr.type;
                layer.parallaxFactor = lyr.relativeSpeed / 128.0f;
                layer.scrollSpeed    = (lyr.constantSpeed << 10) / 65536.0f;
                layer.lineIndexes    = lyr.lineIndexes;

                layer.layout.clear();
                for (int y = 0; y < layer.height; ++y) {
                    layer.layout.append(QList<ushort>());
                    for (int x = 0; x < layer.width; ++x) {
                        layer.layout[y].append(lyr.layout[y][x]);
                    }
                }

                layers.append(layer);
            }
            break;
        }

        case ENGINE_v2: {
            RSDKv2::Background bg(reader);

            for (RSDKv2::Background::ScrollInfo &scr : bg.hScroll) {
                ScrollInfo scroll;
                scroll.deform         = scr.deform;
                scroll.parallaxFactor = scr.parallaxFactor / 128.0f;
                scroll.scrollSpeed    = (scr.scrollSpeed << 10) / 65536.0f;

                hScroll.append(scroll);
            }

            for (RSDKv2::Background::ScrollInfo &scr : bg.vScroll) {
                ScrollInfo scroll;
                scroll.deform         = scr.deform;
                scroll.parallaxFactor = scr.parallaxFactor / 128.0f;
                scroll.scrollSpeed    = (scr.scrollSpeed << 10) / 65536.0f;

                vScroll.append(scroll);
            }

            for (RSDKv2::Background::Layer &lyr : bg.layers) {
                Layer layer;
                layer.width          = lyr.width;
                layer.height         = lyr.height;
                layer.type           = lyr.type;
                layer.parallaxFactor = lyr.parallaxFactor / 128.0f;
                layer.scrollSpeed    = (lyr.scrollSpeed << 10) / 65536.0f;
                layer.lineIndexes    = lyr.lineIndexes;

                layer.layout.clear();
                for (int y = 0; y < layer.height; ++y) {
                    layer.layout.append(QList<ushort>());
                    for (int x = 0; x < layer.width; ++x) {
                        layer.layout[y].append(lyr.layout[y][x]);
                    }
                }

                layers.append(layer);
            }
            break;
        }

        case ENGINE_v3: {
            RSDKv3::Background bg(reader);

            for (RSDKv3::Background::ScrollInfo &scr : bg.hScroll) {
                ScrollInfo scroll;
                scroll.deform         = scr.deform;
                scroll.parallaxFactor = scr.parallaxFactor / 256.0f;
                scroll.scrollSpeed    = (scr.scrollSpeed << 10) / 65536.0f;

                hScroll.append(scroll);
            }

            for (RSDKv3::Background::ScrollInfo &scr : bg.vScroll) {
                ScrollInfo scroll;
                scroll.deform         = scr.deform;
                scroll.parallaxFactor = scr.parallaxFactor / 256.0f;
                scroll.scrollSpeed    = (scr.scrollSpeed << 10) / 65536.0f;

                vScroll.append(scroll);
            }

            for (RSDKv3::Background::Layer &lyr : bg.layers) {
                Layer layer;
                layer.width          = lyr.width;
                layer.height         = lyr.height;
                layer.type           = lyr.type;
                layer.parallaxFactor = lyr.parallaxFactor / 256.0f;
                layer.scrollSpeed    = (lyr.scrollSpeed << 10) / 65536.0f;
                layer.lineIndexes    = lyr.lineIndexes;

                layer.layout.clear();
                for (int y = 0; y < layer.height; ++y) {
                    layer.layout.append(QList<ushort>());
                    for (int x = 0; x < layer.width; ++x) {
                        layer.layout[y].append(lyr.layout[y][x]);
                    }
                }

                layers.append(layer);
            }
            break;
        }

        case ENGINE_v4: {
            RSDKv4::Background bg(reader);

            for (RSDKv4::Background::ScrollInfo &scr : bg.hScroll) {
                ScrollInfo scroll;
                scroll.deform         = scr.deform;
                scroll.parallaxFactor = scr.parallaxFactor / 256.0f;
                scroll.scrollSpeed    = (scr.scrollSpeed << 10) / 65536.0f;

                hScroll.append(scroll);
            }

            for (RSDKv4::Background::ScrollInfo &scr : bg.vScroll) {
                ScrollInfo scroll;
                scroll.deform         = scr.deform;
                scroll.parallaxFactor = scr.parallaxFactor / 256.0f;
                scroll.scrollSpeed    = (scr.scrollSpeed << 10) / 65536.0f;

                vScroll.append(scroll);
            }

            for (RSDKv4::Background::Layer &lyr : bg.layers) {
                Layer layer;
                layer.width          = lyr.width;
                layer.height         = lyr.height;
                layer.type           = lyr.type;
                layer.parallaxFactor = lyr.parallaxFactor / 256.0f;
                layer.scrollSpeed    = (lyr.scrollSpeed << 10) / 65536.0f;
                layer.lineIndexes    = lyr.lineIndexes;

                layer.layout.clear();
                for (int y = 0; y < layer.height; ++y) {
                    layer.layout.append(QList<ushort>());
                    for (int x = 0; x < layer.width; ++x) {
                        layer.layout[y].append(lyr.layout[y][x]);
                    }
                }

                layers.append(layer);
            }
            break;
        }
    }

    scrollInfoFromIndices();
}

void FormatHelpers::Background::write(byte ver, QString filename)
{
    if (filename == "")
        filename = filePath;
    if (filename == "")
        return;
    Writer writer(filename);
    filePath = filename;

    scrollIndicesFromInfo();

    switch (ver) {
        default: break;
        case ENGINE_v1: {
            RSDKv1::Background bg;

            for (ScrollInfo &scr : hScroll) {
                RSDKv1::Background::ScrollInfo scroll;
                scroll.deform         = scr.deform;
                scroll.parallaxFactor = scr.parallaxFactor * 128;
                scroll.scrollSpeed    = (int)(scr.scrollSpeed * 65536) >> 10;

                bg.hScroll.append(scroll);
            }

            for (ScrollInfo &scr : vScroll) {
                RSDKv1::Background::ScrollInfo scroll;
                scroll.deform         = scr.deform;
                scroll.parallaxFactor = scr.parallaxFactor * 128;
                scroll.scrollSpeed    = (int)(scr.scrollSpeed * 65536) >> 10;

                bg.vScroll.append(scroll);
            }

            for (Layer &lyr : layers) {
                RSDKv1::Background::Layer layer;
                layer.width         = lyr.width;
                layer.height        = lyr.height;
                layer.type          = lyr.type;
                layer.relativeSpeed = lyr.parallaxFactor * 128;
                layer.constantSpeed = (int)(lyr.scrollSpeed * 65536) >> 10;
                layer.lineIndexes   = lyr.lineIndexes;

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
            break;
        }

        case ENGINE_v2: {
            RSDKv2::Background bg;

            for (ScrollInfo &scr : hScroll) {
                RSDKv2::Background::ScrollInfo scroll;
                scroll.deform         = scr.deform;
                scroll.parallaxFactor = scr.parallaxFactor * 128;
                scroll.scrollSpeed    = (int)(scr.scrollSpeed * 65536) >> 10;

                bg.hScroll.append(scroll);
            }

            for (ScrollInfo &scr : vScroll) {
                RSDKv2::Background::ScrollInfo scroll;
                scroll.deform         = scr.deform;
                scroll.parallaxFactor = scr.parallaxFactor * 128;
                scroll.scrollSpeed    = (int)(scr.scrollSpeed * 65536) >> 10;

                bg.vScroll.append(scroll);
            }

            for (Layer &lyr : layers) {
                RSDKv2::Background::Layer layer;
                layer.width          = lyr.width;
                layer.height         = lyr.height;
                layer.type           = lyr.type;
                layer.parallaxFactor = lyr.parallaxFactor * 128;
                layer.scrollSpeed    = (int)(lyr.scrollSpeed * 65536) >> 10;
                layer.lineIndexes    = lyr.lineIndexes;

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
            break;
        }

        case ENGINE_v3: {
            RSDKv3::Background bg;

            for (ScrollInfo &scr : hScroll) {
                RSDKv3::Background::ScrollInfo scroll;
                scroll.deform         = scr.deform;
                scroll.parallaxFactor = scr.parallaxFactor * 256;
                scroll.scrollSpeed    = (int)(scr.scrollSpeed * 65536) >> 10;

                bg.hScroll.append(scroll);
            }

            for (ScrollInfo &scr : vScroll) {
                RSDKv3::Background::ScrollInfo scroll;
                scroll.deform         = scr.deform;
                scroll.parallaxFactor = scr.parallaxFactor * 256;
                scroll.scrollSpeed    = (int)(scr.scrollSpeed * 65536) >> 10;

                bg.vScroll.append(scroll);
            }

            for (Layer &lyr : layers) {
                RSDKv3::Background::Layer layer;
                layer.width          = lyr.width;
                layer.height         = lyr.height;
                layer.type           = lyr.type;
                layer.parallaxFactor = lyr.parallaxFactor * 256;
                layer.scrollSpeed    = (int)(lyr.scrollSpeed * 65536) >> 10;
                layer.lineIndexes    = lyr.lineIndexes;

                layer.layout.clear();
                for (int y = 0; y < layer.height; ++y) {
                    layer.layout.append(QList<ushort>());
                    for (int x = 0; x < layer.width; ++x) {
                        layer.layout[y].append(lyr.layout[y][x]);
                    }
                }

                bg.layers.append(layer);
            }

            bg.write(writer);
            break;
        }

        case ENGINE_v4: {
            RSDKv4::Background bg;

            for (ScrollInfo &scr : hScroll) {
                RSDKv4::Background::ScrollInfo scroll;
                scroll.deform         = scr.deform;
                scroll.parallaxFactor = scr.parallaxFactor * 256;
                scroll.scrollSpeed    = (int)(scr.scrollSpeed * 65536) >> 10;

                bg.hScroll.append(scroll);
            }

            for (ScrollInfo &scr : vScroll) {
                RSDKv4::Background::ScrollInfo scroll;
                scroll.deform         = scr.deform;
                scroll.parallaxFactor = scr.parallaxFactor * 256;
                scroll.scrollSpeed    = (int)(scr.scrollSpeed * 65536) >> 10;

                bg.vScroll.append(scroll);
            }

            for (Layer &lyr : layers) {
                RSDKv4::Background::Layer layer;
                layer.width          = lyr.width;
                layer.height         = lyr.height;
                layer.type           = lyr.type;
                layer.parallaxFactor = lyr.parallaxFactor * 256;
                layer.scrollSpeed    = (int)(lyr.scrollSpeed * 65536) >> 10;
                layer.lineIndexes    = lyr.lineIndexes;

                layer.layout.clear();
                for (int y = 0; y < layer.height; ++y) {
                    layer.layout.append(QList<ushort>());
                    for (int x = 0; x < layer.width; ++x) {
                        layer.layout[y].append(lyr.layout[y][x]);
                    }
                }

                bg.layers.append(layer);
            }

            bg.write(writer);
            break;
        }
    }
}

void FormatHelpers::Background::scrollInfoFromIndices()
{
    for (Layer &layer : layers) {
        layer.scrollInfos.clear();
        QList<ScrollInfo> infos;
        if (layer.type == 1)
            infos = hScroll;
        else if (layer.type == 2)
            infos = vScroll;
        else
            continue;

        int prev  = layer.lineIndexes.count() > 0 ? layer.lineIndexes[0] : -1;
        int start = 0;
        int l     = 0;

        for (; l < layer.lineIndexes.count(); ++l) {
            if ((byte)layer.lineIndexes[l] != prev) {
                ScrollIndexInfo info;

                info.startLine      = start;
                info.length         = l - start;
                info.parallaxFactor = infos[prev].parallaxFactor;
                info.scrollSpeed    = infos[prev].scrollSpeed;
                info.scrollPos      = 0.0f;
                info.deform         = infos[prev].deform;

                layer.scrollInfos.append(info);
                start = l;
            }

            prev = (byte)layer.lineIndexes[l];
        }

        {
            ScrollIndexInfo info;

            info.startLine      = start;
            info.length         = l - start;
            info.parallaxFactor = infos[0].parallaxFactor;
            info.scrollSpeed    = infos[0].scrollSpeed;
            info.scrollPos      = 0.0f;
            info.deform         = infos[0].deform;

            layer.scrollInfos.append(info);
        }
    }
}

void FormatHelpers::Background::scrollIndicesFromInfo()
{
    hScroll.clear();
    vScroll.clear();

    int hID = 0;
    int vID = 0;

    for (Layer &layer : layers) {
        layer.lineIndexes.clear();

        if (layer.type != 1 && layer.type != 2) {
            // other layers dont need any scrolling, TODO: check this works
            continue;
        }
        if (layer.width == 0 || layer.height == 0)
            continue; // basically invalid layers, dont write em

        if (layer.type == 1) {
            layer.lineIndexes.resize(layer.height * 0x80);
        }
        else {
            layer.lineIndexes.resize(layer.width * 0x80);
        }

        int id = 0;
        for (ScrollIndexInfo &info : layer.scrollInfos) {
            int infoID = id;
            ScrollInfo sInfo;
            sInfo.deform         = info.deform;
            sInfo.parallaxFactor = info.parallaxFactor;
            sInfo.scrollSpeed    = info.scrollSpeed;

            int scrollID = 0;
            if (layer.type == 1) {
                for (ScrollInfo &info : hScroll) {
                    if (info.parallaxFactor == sInfo.parallaxFactor
                        && info.scrollSpeed == sInfo.scrollSpeed && info.deform == sInfo.deform) {
                        // infoID = scrollID;
                        break;
                    }
                    ++scrollID;
                }
            }
            else {
                for (ScrollInfo &info : vScroll) {
                    if (info.parallaxFactor == sInfo.parallaxFactor
                        && info.scrollSpeed == sInfo.scrollSpeed && info.deform == sInfo.deform) {
                        // infoID = scrollID;
                        break;
                    }
                    ++scrollID;
                }
            }

            for (int i = info.startLine; i < info.startLine + info.length; ++i) {
                layer.lineIndexes[i] = (byte)infoID;
            }

            // New Info needs to be added
            if (infoID == id) {
                if (layer.type == 1) {
                    hScroll.append(sInfo);
                    ++hID;
                }
                else {
                    vScroll.append(sInfo);
                    ++vID;
                }
                ++id;
            }
        }
    }
}
