#include "libRSDK.hpp"

void RSDKv4::Bytecode::read(Reader &reader)
{
    filePath = reader.filePath;
    scriptCode.clear();
    jumpTable.clear();
    scriptList.clear();
    functionList.clear();

    for (int scriptCodeSize = reader.read<int>(); scriptCodeSize > 0;) {
        byte buffer   = reader.read<byte>();
        int blockSize = (buffer & 0x7F);

        if ((buffer & 0x80) == 0) {
            for (int i = 0; i < blockSize; ++i) scriptCode.append(reader.read<byte>());
            scriptCodeSize -= blockSize;
        }
        else {
            for (int i = 0; i < blockSize; ++i) scriptCode.append(reader.read<int>());
            scriptCodeSize -= blockSize;
        }
    }

    for (int jumpTableSize = reader.read<int>(); jumpTableSize > 0;) {
        byte buffer   = reader.read<byte>();
        int blockSize = (buffer & 0x7F);

        if (!(buffer & 0x80)) {
            for (int i = 0; i < blockSize; ++i) jumpTable.append(reader.read<byte>());
            jumpTableSize -= blockSize;
        }
        else {
            for (int i = 0; i < blockSize; ++i) jumpTable.append(reader.read<int>());
            jumpTableSize -= blockSize;
        }
    }

    ushort scriptCount = reader.read<ushort>();

    // Fill in "blanks"
    for (int i = 0; i < scriptCount; ++i) {
        scriptList.append(ScriptInfo());
        scriptList[i].update.scriptCodePos  = 0x3FFFF;
        scriptList[i].draw.scriptCodePos    = 0x3FFFF;
        scriptList[i].startup.scriptCodePos = 0x3FFFF;
    }

    for (ScriptInfo &script : scriptList) {
        script.update.scriptCodePos  = reader.read<int>();
        script.draw.scriptCodePos    = reader.read<int>();
        script.startup.scriptCodePos = reader.read<int>();
    }

    for (ScriptInfo &script : scriptList) {
        script.update.jumpTablePos  = reader.read<int>();
        script.draw.jumpTablePos    = reader.read<int>();
        script.startup.jumpTablePos = reader.read<int>();
    }

    ushort functionCount = reader.read<ushort>();
    functionList.clear();
    for (int i = 0; i < functionCount; ++i) {
        functionList.append(FunctionInfo());
        functionList[i].scriptCodePos = reader.read<int>();
    }

    for (FunctionInfo &func : functionList) func.jumpTablePos = reader.read<int>();
}

struct DataInfo {
    QList<int> data;
    bool readInt = false;
};

void RSDKv4::Bytecode::write(Writer &writer)
{
    // ScriptCode = Appends
    // JumpTable = Appends
    // ScriptPtrs = Appends
    // FunctionList = Copied to every one

    filePath = writer.filePath;

    QList<DataInfo> dataInfo;

    // Script Data
    dataInfo.clear();
    for (int dataPos = 0; dataPos < scriptCode.count();) {
        DataInfo info;
        info.data.clear();
        info.readInt = scriptCode[dataPos] < 0 || scriptCode[dataPos] >= 0x100;
        if (!info.readInt) {
            for (int i = 0; (scriptCode[dataPos] >= 0 && scriptCode[dataPos] < 0x100) && i < 0x7F;
                 ++i) {
                info.data.append(scriptCode[dataPos++]);
                if (dataPos >= scriptCode.count())
                    break;
            }
            dataInfo.append(info);
        }
        else {
            for (int i = 0; (scriptCode[dataPos] < 0 || scriptCode[dataPos] >= 0x100) && i < 0x7F;
                 ++i) {
                info.data.append(scriptCode[dataPos++]);
                if (dataPos >= scriptCode.count())
                    break;
            }
            dataInfo.append(info);
        }
    }

    int count = 0;
    for (int i = 0; i < dataInfo.count(); ++i) count += dataInfo[i].data.size();
    writer.write(count);

    for (int i = 0; i < dataInfo.count(); ++i) {
        DataInfo &info = dataInfo[i];
        writer.write<byte>(info.data.size() | ((byte)info.readInt << 7));
        for (int d = 0; d < info.data.count(); ++d) {
            if (info.readInt)
                writer.write<int>(info.data[d]);
            else
                writer.write<byte>(info.data[d]);
        }
    }

    // Jump Table
    dataInfo.clear();
    for (int dataPos = 0; dataPos < jumpTable.count();) {
        DataInfo info;
        info.data.clear();
        info.readInt = jumpTable[dataPos] < 0 || jumpTable[dataPos] >= 0x100;
        if (!info.readInt) {
            for (int i = 0; (jumpTable[dataPos] >= 0 && jumpTable[dataPos] < 0x100) && i < 0x7F; ++i) {
                info.data.append(jumpTable[dataPos++]);
                if (dataPos >= jumpTable.count())
                    break;
            }
            dataInfo.append(info);
        }
        else {
            for (int i = 0; (jumpTable[dataPos] < 0 || jumpTable[dataPos] >= 0x100) && i < 0x7F; ++i) {
                info.data.append(jumpTable[dataPos++]);
                if (dataPos >= jumpTable.count())
                    break;
            }
            dataInfo.append(info);
        }
    }

    count = 0;
    for (int i = 0; i < dataInfo.count(); ++i) count += dataInfo[i].data.size();
    writer.write(count);

    for (int i = 0; i < dataInfo.count(); ++i) {
        DataInfo &info = dataInfo[i];
        writer.write<byte>(info.data.size() | ((byte)info.readInt << 7));
        for (int d = 0; d < info.data.count(); ++d) {
            if (info.readInt)
                writer.write<int>(info.data[d]);
            else
                writer.write<byte>(info.data[d]);
        }
    }

    // Script list
    writer.write((ushort)scriptList.count());

    for (ScriptInfo &script : scriptList) {
        writer.write(script.update.scriptCodePos);
        writer.write(script.draw.scriptCodePos);
        writer.write(script.startup.scriptCodePos);
    }

    for (ScriptInfo &script : scriptList) {
        writer.write(script.update.jumpTablePos);
        writer.write(script.draw.jumpTablePos);
        writer.write(script.startup.jumpTablePos);
    }

    // Function list
    writer.write((ushort)functionList.count());

    for (FunctionInfo &func : functionList) writer.write(func.scriptCodePos);
    for (FunctionInfo &func : functionList) writer.write(func.jumpTablePos);

    writer.flush();
}
