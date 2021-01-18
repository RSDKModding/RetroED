#include "include.hpp"

void RSDKv1::Bytecode::read(Reader &reader) { m_filename = reader.m_filepath; }

void RSDKv1::Bytecode::write(Writer &writer)
{
    m_filename = writer.m_filename;

    // TODO: Write
}
