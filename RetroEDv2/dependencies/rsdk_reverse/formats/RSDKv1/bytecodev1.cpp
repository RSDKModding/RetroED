#include "include.hpp"

void RSDKv1::Bytecode::read(Reader &reader) { filepath = reader.filepath; }

void RSDKv1::Bytecode::write(Writer &writer)
{
    filepath = writer.filePath;

    // TODO: Write
}
