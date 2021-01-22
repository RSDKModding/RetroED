#include "include.hpp"

void FormatHelpers::Animation::read(byte ver, QString filename)
{
    Reader reader(filename);
    m_filename = filename;

    switch (ver) {
        default: break;
        case ENGINE_v1: break;
        case ENGINE_v2: break;
        case ENGINE_v3: break;
        case ENGINE_v4: break;
    }
}

void FormatHelpers::Animation::write(byte ver, QString filename)
{
    if (filename == "")
        filename = m_filename;
    if (filename == "")
        return;
    Writer writer(filename);
    m_filename = filename;

    switch (ver) {
        default: break;
        case ENGINE_v1: break;
        case ENGINE_v2: break;
        case ENGINE_v3: break;
        case ENGINE_v4: break;
    }
}
