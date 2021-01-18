#include "include.hpp"

void FormatHelpers::Chunks::read(byte ver, QString filename)
{
    Reader reader(filename);

    for (int c = 0; c < 0x200; ++c) {
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                m_chunks[c].m_tiles[y][x].m_direction   = 0;
                m_chunks[c].m_tiles[y][x].m_visualPlane = 0;
                m_chunks[c].m_tiles[y][x].m_tileIndex   = 0;
                m_chunks[c].m_tiles[y][x].m_solidityA   = 0;
                m_chunks[c].m_tiles[y][x].m_solidityB   = 0;
            }
        }
    }

    switch (ver) {
        default: break;
        case ENGINE_v1: {
            RSDKv1::Chunks chunks(reader);
            for (int c = 0; c < 0x100; ++c) {
                for (int y = 0; y < 8; ++y) {
                    for (int x = 0; x < 8; ++x) {
                        m_chunks[c].m_tiles[y][x].m_direction =
                            chunks.m_chunkList[c].m_tiles[y][x].m_direction;
                        m_chunks[c].m_tiles[y][x].m_visualPlane =
                            chunks.m_chunkList[c].m_tiles[y][x].m_visualPlane;
                        m_chunks[c].m_tiles[y][x].m_tileIndex =
                            chunks.m_chunkList[c].m_tiles[y][x].m_tileIndex;
                        m_chunks[c].m_tiles[y][x].m_solidityA =
                            chunks.m_chunkList[c].m_tiles[y][x].m_solidityA;
                        m_chunks[c].m_tiles[y][x].m_solidityB =
                            chunks.m_chunkList[c].m_tiles[y][x].m_solidityB;
                    }
                }
            }
        } break;
        case ENGINE_v2: {
            RSDKv2::Chunks chunks(reader);
            for (int c = 0; c < 0x200; ++c) {
                for (int y = 0; y < 8; ++y) {
                    for (int x = 0; x < 8; ++x) {
                        m_chunks[c].m_tiles[y][x].m_direction =
                            chunks.m_chunkList[c].m_tiles[y][x].m_direction;
                        m_chunks[c].m_tiles[y][x].m_visualPlane =
                            chunks.m_chunkList[c].m_tiles[y][x].m_visualPlane;
                        m_chunks[c].m_tiles[y][x].m_tileIndex =
                            chunks.m_chunkList[c].m_tiles[y][x].m_tileIndex;
                        m_chunks[c].m_tiles[y][x].m_solidityA =
                            chunks.m_chunkList[c].m_tiles[y][x].m_solidityA;
                        m_chunks[c].m_tiles[y][x].m_solidityB =
                            chunks.m_chunkList[c].m_tiles[y][x].m_solidityB;
                    }
                }
            }
        } break;
        case ENGINE_v3: {
            RSDKv3::Chunks chunks(reader);
            for (int c = 0; c < 0x200; ++c) {
                for (int y = 0; y < 8; ++y) {
                    for (int x = 0; x < 8; ++x) {
                        m_chunks[c].m_tiles[y][x].m_direction =
                            chunks.m_chunkList[c].m_tiles[y][x].m_direction;
                        m_chunks[c].m_tiles[y][x].m_visualPlane =
                            chunks.m_chunkList[c].m_tiles[y][x].m_visualPlane;
                        m_chunks[c].m_tiles[y][x].m_tileIndex =
                            chunks.m_chunkList[c].m_tiles[y][x].m_tileIndex;
                        m_chunks[c].m_tiles[y][x].m_solidityA =
                            chunks.m_chunkList[c].m_tiles[y][x].m_solidityA;
                        m_chunks[c].m_tiles[y][x].m_solidityB =
                            chunks.m_chunkList[c].m_tiles[y][x].m_solidityB;
                    }
                }
            }
        } break;
        case ENGINE_v4: {
            RSDKv4::Chunks chunks(reader);
            for (int c = 0; c < 0x200; ++c) {
                for (int y = 0; y < 8; ++y) {
                    for (int x = 0; x < 8; ++x) {
                        m_chunks[c].m_tiles[y][x].m_direction =
                            chunks.m_chunkList[c].m_tiles[y][x].m_direction;
                        m_chunks[c].m_tiles[y][x].m_visualPlane =
                            chunks.m_chunkList[c].m_tiles[y][x].m_visualPlane;
                        m_chunks[c].m_tiles[y][x].m_tileIndex =
                            chunks.m_chunkList[c].m_tiles[y][x].m_tileIndex;
                        m_chunks[c].m_tiles[y][x].m_solidityA =
                            chunks.m_chunkList[c].m_tiles[y][x].m_solidityA;
                        m_chunks[c].m_tiles[y][x].m_solidityB =
                            chunks.m_chunkList[c].m_tiles[y][x].m_solidityB;
                    }
                }
            }
        } break;
    }
}

void FormatHelpers::Chunks::write(byte ver, QString filename)
{
    if (filename == "")
        filename = m_filename;
    if (filename == "")
        return;
    Writer writer(filename);

    switch (ver) {
        default: break;
        case ENGINE_v1: {
            RSDKv1::Chunks chunks;
            for (int c = 0; c < 0x100; ++c) {
                for (int y = 0; y < 8; ++y) {
                    for (int x = 0; x < 8; ++x) {
                        chunks.m_chunkList[c].m_tiles[y][x].m_direction =
                            m_chunks[c].m_tiles[y][x].m_direction;
                        chunks.m_chunkList[c].m_tiles[y][x].m_visualPlane =
                            m_chunks[c].m_tiles[y][x].m_visualPlane;
                        chunks.m_chunkList[c].m_tiles[y][x].m_tileIndex =
                            m_chunks[c].m_tiles[y][x].m_tileIndex;
                        chunks.m_chunkList[c].m_tiles[y][x].m_solidityA =
                            m_chunks[c].m_tiles[y][x].m_solidityA;
                        chunks.m_chunkList[c].m_tiles[y][x].m_solidityB =
                            m_chunks[c].m_tiles[y][x].m_solidityB;
                    }
                }
            }
            chunks.write(writer);
        } break;
        case ENGINE_v2: {
            RSDKv2::Chunks chunks;
            for (int c = 0; c < 0x200; ++c) {
                for (int y = 0; y < 8; ++y) {
                    for (int x = 0; x < 8; ++x) {
                        chunks.m_chunkList[c].m_tiles[y][x].m_direction =
                            m_chunks[c].m_tiles[y][x].m_direction;
                        chunks.m_chunkList[c].m_tiles[y][x].m_visualPlane =
                            m_chunks[c].m_tiles[y][x].m_visualPlane;
                        chunks.m_chunkList[c].m_tiles[y][x].m_tileIndex =
                            m_chunks[c].m_tiles[y][x].m_tileIndex;
                        chunks.m_chunkList[c].m_tiles[y][x].m_solidityA =
                            m_chunks[c].m_tiles[y][x].m_solidityA;
                        chunks.m_chunkList[c].m_tiles[y][x].m_solidityB =
                            m_chunks[c].m_tiles[y][x].m_solidityB;
                    }
                }
            }
            chunks.write(writer);
        } break;
        case ENGINE_v3: {
            RSDKv3::Chunks chunks;
            for (int c = 0; c < 0x200; ++c) {
                for (int y = 0; y < 8; ++y) {
                    for (int x = 0; x < 8; ++x) {
                        chunks.m_chunkList[c].m_tiles[y][x].m_direction =
                            m_chunks[c].m_tiles[y][x].m_direction;
                        chunks.m_chunkList[c].m_tiles[y][x].m_visualPlane =
                            m_chunks[c].m_tiles[y][x].m_visualPlane;
                        chunks.m_chunkList[c].m_tiles[y][x].m_tileIndex =
                            m_chunks[c].m_tiles[y][x].m_tileIndex;
                        chunks.m_chunkList[c].m_tiles[y][x].m_solidityA =
                            m_chunks[c].m_tiles[y][x].m_solidityA;
                        chunks.m_chunkList[c].m_tiles[y][x].m_solidityB =
                            m_chunks[c].m_tiles[y][x].m_solidityB;
                    }
                }
            }
            chunks.write(writer);
        } break;
        case ENGINE_v4: {
            RSDKv4::Chunks chunks;
            for (int c = 0; c < 0x200; ++c) {
                for (int y = 0; y < 8; ++y) {
                    for (int x = 0; x < 8; ++x) {
                        chunks.m_chunkList[c].m_tiles[y][x].m_direction =
                            m_chunks[c].m_tiles[y][x].m_direction;
                        chunks.m_chunkList[c].m_tiles[y][x].m_visualPlane =
                            m_chunks[c].m_tiles[y][x].m_visualPlane;
                        chunks.m_chunkList[c].m_tiles[y][x].m_tileIndex =
                            m_chunks[c].m_tiles[y][x].m_tileIndex;
                        chunks.m_chunkList[c].m_tiles[y][x].m_solidityA =
                            m_chunks[c].m_tiles[y][x].m_solidityA;
                        chunks.m_chunkList[c].m_tiles[y][x].m_solidityB =
                            m_chunks[c].m_tiles[y][x].m_solidityB;
                    }
                }
            }
            chunks.write(writer);
        } break;
    }
}

QImage FormatHelpers::Chunks::Chunk::getImage(QList<QImage> tiles)
{
    QImage img(0x80, 0x80, QImage::Format::Format_ARGB32);

    for (int h = 0; h < 0x8; ++h) {
        for (int w = 0; w < 0x8; ++w) {
            QImage &tile = tiles[m_tiles[h][w].m_tileIndex];
            Vector2<bool> flip;
            flip.x = (m_tiles[h][w].m_direction & 1) == 1;
            flip.y = (m_tiles[h][w].m_direction & 2) == 2;

            for (int y = 0; y < 0x10; ++y) {
                for (int x = 0; x < 0x10; ++x) {
                    int xpos = x;
                    int ypos = y;
                    if (flip.y)
                        ypos = 0x0F - y;
                    if (flip.x)
                        xpos = 0x0F - x;

                    img.setPixelColor((w * 0x10) + x, (h * 0x10) + y,
                                      tile.pixelColor(xpos, ypos).rgb());
                }
            }
        }
    }

    return img;
}
