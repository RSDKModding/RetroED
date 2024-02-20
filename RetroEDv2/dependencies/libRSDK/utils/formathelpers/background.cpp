#include "libRSDK.hpp"

#include "background.hpp"
#include "RSDKv1/backgroundv1.hpp"
#include "RSDKv2/backgroundv2.hpp"
#include "RSDKv3/backgroundv3.hpp"
#include "RSDKv4/backgroundv4.hpp"

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
                layer.lineScroll     = lyr.lineScroll;

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
                layer.lineScroll     = lyr.lineScroll;

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
                layer.lineScroll     = lyr.lineScroll;

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
                layer.lineScroll     = lyr.lineScroll;

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
                layer.lineScroll    = lyr.lineScroll;

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
                layer.lineScroll     = lyr.lineScroll;

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
                layer.lineScroll     = lyr.lineScroll;

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
                layer.lineScroll     = lyr.lineScroll;

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
    hScrollInfo.clear();
    vScrollInfo.clear();

    for (auto &scroll : hScroll) {
        ScrollIndexInfo info;

        info.parallaxFactor = scroll.parallaxFactor;
        info.scrollSpeed    = scroll.scrollSpeed;
        info.deform         = scroll.deform;
        info.instances.clear();

        hScrollInfo.append(info);
    }

    for (auto &scroll : vScroll) {
        ScrollIndexInfo info;

        info.parallaxFactor = scroll.parallaxFactor;
        info.scrollSpeed    = scroll.scrollSpeed;
        info.deform         = scroll.deform;
        info.instances.clear();

        vScrollInfo.append(info);
    }

    int layerID = 1;
    for (Layer &layer : layers) {
        if (layer.type == 1) {
            byte index     = layer.lineScroll.count() <= 0 ? 0 : layer.lineScroll[0];
            byte lastIndex = index;
            int startLine  = 0;
            for (int i = 0; i < layer.lineScroll.count(); ++i) {
                index = layer.lineScroll[i];

                if (index != lastIndex) {
                    ScrollInstance instance;
                    instance.startLine = startLine;
                    instance.length    = i - startLine;
                    instance.layerID   = layerID;

                    if (lastIndex < hScrollInfo.count()) {
                        hScrollInfo[lastIndex].instances.append(instance);
                    }
                    else {
                        // uhhh... error???
                    }

                    lastIndex = index;
                    startLine = i;
                }
            }

            if (layer.lineScroll.count() > 0) {
                ScrollInstance instance;
                instance.startLine = startLine;
                instance.length    = layer.lineScroll.count() - startLine;
                instance.layerID   = layerID;

                if (lastIndex < hScroll.count()) {
                    hScrollInfo[lastIndex].instances.append(instance);
                }
                else {
                    // uhhh... error???
                }
            }
        }

        if (layer.type == 2) {
            byte index     = layer.lineScroll.count() <= 0 ? 0 : layer.lineScroll[0];
            byte lastIndex = index;
            int startLine  = 0;
            for (int i = 0; i < layer.lineScroll.count(); ++i) {
                index = layer.lineScroll[i];

                if (index != lastIndex) {
                    ScrollInstance instance;
                    instance.startLine = startLine;
                    instance.length    = i - startLine;
                    instance.layerID   = layerID;

                    if (lastIndex < vScrollInfo.count()) {
                        vScrollInfo[lastIndex].instances.append(instance);
                    }
                    else {
                        // uhhh... error???
                    }

                    lastIndex = index;
                    startLine = i;
                }
            }

            if (layer.lineScroll.count() > 0) {
                ScrollInstance instance;
                instance.startLine = startLine;
                instance.length    = layer.lineScroll.count() - startLine;
                instance.layerID   = layerID;

                if (lastIndex < vScroll.count()) {
                    vScrollInfo[lastIndex].instances.append(instance);
                }
                else {
                    // uhhh... error???
                }
            }
        }

        layerID++;
    }
}

void FormatHelpers::Background::scrollIndicesFromInfo()
{
    hScroll.clear();
    vScroll.clear();

    for (Layer &layer : layers) {
        layer.lineScroll.clear();
        if (layer.type != 1 && layer.type != 2) {
            // other layers dont need any scrolling, TODO: check this works
            continue;
        }

        if (layer.width == 0 || layer.height == 0)
            continue; // basically invalid layers, dont write em

        if (layer.type == 1)
            layer.lineScroll.resize(layer.height * 0x80);
        else
            layer.lineScroll.resize(layer.width * 0x80);

        for (int i = 0; i < layer.lineScroll.count(); ++i) layer.lineScroll[i] = 0;
    }

    int scrID = 0;
    for (auto &info : hScrollInfo) {
        ScrollInfo scroll;

        scroll.parallaxFactor = info.parallaxFactor;
        scroll.scrollSpeed    = info.scrollSpeed;
        scroll.deform         = info.deform;

        for (auto &instance : info.instances) {
            Layer *layer = nullptr;
            if ((uint)(instance.layerID - 1) < (uint)layers.count())
                layer = &layers[instance.layerID - 1];

            if (!layer || layer->width == 0 || layer->height == 0 || layer->type != 1)
                continue;

            for (int i = instance.startLine; i < instance.startLine + instance.length; ++i)
                layer->lineScroll[i] = (byte)scrID;
        }

        hScroll.append(scroll);
        scrID++;
    }

    scrID = 0;
    for (auto &info : vScrollInfo) {
        ScrollInfo scroll;

        scroll.parallaxFactor = info.parallaxFactor;
        scroll.scrollSpeed    = info.scrollSpeed;
        scroll.deform         = info.deform;

        for (auto &instance : info.instances) {
            Layer *layer = nullptr;
            if (instance.layerID < layers.count())
                layer = &layers[instance.layerID];

            if (!layer || layer->width == 0 || layer->height == 0 || layer->type != 2)
                continue;

            for (int i = instance.startLine; i < instance.startLine + instance.length; ++i)
                layer->lineScroll[i] = (byte)scrID;
        }

        vScroll.append(scroll);
        scrID++;
    }
}
