#include "include.hpp"

void RSDKv3::Bytecode::read(Reader &reader, int scriptCount, bool clear)
{
    m_filename = reader.filepath;
    if (clear) {
        scriptData.clear();
        jumpTableData.clear();
        scriptList.clear();
        functionList.clear();
    }
    else {
        m_globalScriptDataCount = scriptData.count();
        m_globalJumpTableCount  = jumpTableData.count();
    }

    for (int opcount = reader.read<int>(); opcount > 0;) {
        byte flags    = reader.read<byte>();
        int dataCount = (flags & 0x7F);

        if ((flags & 0x80) == 0) {
            for (int i = 0; i < dataCount; ++i) scriptData.append(reader.read<byte>());
            opcount -= dataCount;
        }
        else {
            for (int i = 0; i < dataCount; ++i) scriptData.append(reader.read<int>());
            opcount -= dataCount;
        }
    }

    for (int opcount = reader.read<int>(); opcount > 0;) {
        byte flags    = reader.read<byte>();
        int dataCount = (flags & 0x7F);
        if ((flags & 0x80) == 0) {
            for (int i = 0; i < dataCount; ++i) jumpTableData.append(reader.read<byte>());
            opcount -= dataCount;
        }
        else {
            for (int i = 0; i < dataCount; ++i) jumpTableData.append(reader.read<int>());
            opcount -= dataCount;
        }
    }

    if (!clear) {
        globalScriptCount = scriptCount;
    }

    int scriptCnt = reader.read<ushort>(); // File count

    // Fill in "blanks"
    for (int i = 0; i < scriptCount && clear; ++i) {
        scriptList.append(ObjectScript());
        scriptList[i].mainScript    = 0x3FFFF;
        scriptList[i].playerScript  = 0x3FFFF;
        scriptList[i].drawScript    = 0x3FFFF;
        scriptList[i].m_startupScript = 0x3FFFF;
    }

    for (int i = 0; i < scriptCnt; ++i) {
        scriptList.append(ObjectScript());
        scriptList[scriptCount + i].mainScript    = reader.read<int>();
        scriptList[scriptCount + i].playerScript  = reader.read<int>();
        scriptList[scriptCount + i].drawScript    = reader.read<int>();
        scriptList[scriptCount + i].m_startupScript = reader.read<int>();
    }
    for (int i = 0; i < scriptCnt; ++i) {
        scriptList[scriptCount + i].mainJumpTable    = reader.read<int>();
        scriptList[scriptCount + i].playerJumpTable  = reader.read<int>();
        scriptList[scriptCount + i].drawJumpTable    = reader.read<int>();
        scriptList[scriptCount + i].m_startupJumpTable = reader.read<int>();
    }

    int functionCount = reader.read<ushort>();
    functionList.clear();

    for (int i = 0; i < functionCount; ++i) {
        functionList.append(FunctionScript());
        functionList[i].mainScript = reader.read<int>();
    }

    for (int i = 0; i < functionCount; ++i) functionList[i].mainJumpTable = reader.read<int>();
}

struct DataInfo {
    QList<int> m_data;
    bool m_readInt = false;
};

void RSDKv3::Bytecode::write(Writer &writer)
{
    m_filename = writer.filePath;

    QList<DataInfo> dataInfo;

    // Script Data
    dataInfo.clear();
    for (int dataPos = m_globalScriptDataCount; dataPos < scriptData.count();) {
        DataInfo info;
        info.m_data.clear();
        info.m_readInt = scriptData[dataPos] < 0 || scriptData[dataPos] >= 0x100;
        if (!info.m_readInt) {
            for (int i = 0; (scriptData[dataPos] >= 0 && scriptData[dataPos] < 0x100) && i < 0x7F;
                 ++i) {
                info.m_data.append(scriptData[dataPos++]);
                if (dataPos >= scriptData.count())
                    break;
            }
            dataInfo.append(info);
        }
        else {
            for (int i = 0; (scriptData[dataPos] < 0 || scriptData[dataPos] >= 0x100) && i < 0x7F;
                 ++i) {
                info.m_data.append(scriptData[dataPos++]);
                if (dataPos >= scriptData.count())
                    break;
            }
            dataInfo.append(info);
        }
    }

    int count = 0;
    for (int i = 0; i < dataInfo.count(); ++i) count += dataInfo[i].m_data.size();
    writer.write(count);

    for (int i = 0; i < dataInfo.count(); ++i) {
        DataInfo &info = dataInfo[i];
        writer.write<byte>(info.m_data.size() | ((byte)info.m_readInt << 7));
        for (int d = 0; d < info.m_data.count(); ++d) {
            if (info.m_readInt)
                writer.write<int>(info.m_data[d]);
            else
                writer.write<byte>(info.m_data[d]);
        }
    }

    // Jump Table
    dataInfo.clear();
    for (int dataPos = m_globalJumpTableCount; dataPos < jumpTableData.count();) {
        DataInfo info;
        info.m_data.clear();
        info.m_readInt = jumpTableData[dataPos] < 0 || jumpTableData[dataPos] >= 0x100;
        if (!info.m_readInt) {
            for (int i = 0;
                 (jumpTableData[dataPos] >= 0 && jumpTableData[dataPos] < 0x100) && i < 0x7F; ++i) {
                info.m_data.append(jumpTableData[dataPos++]);
                if (dataPos >= jumpTableData.count())
                    break;
            }
            dataInfo.append(info);
        }
        else {
            for (int i = 0;
                 (jumpTableData[dataPos] < 0 || jumpTableData[dataPos] >= 0x100) && i < 0x7F; ++i) {
                info.m_data.append(jumpTableData[dataPos++]);
                if (dataPos >= jumpTableData.count())
                    break;
            }
            dataInfo.append(info);
        }
    }

    count = 0;
    for (int i = 0; i < dataInfo.count(); ++i) count += dataInfo[i].m_data.size();
    writer.write(count);

    for (int i = 0; i < dataInfo.count(); ++i) {
        DataInfo &info = dataInfo[i];
        writer.write<byte>(info.m_data.size() | ((byte)info.m_readInt << 7));
        for (int d = 0; d < info.m_data.count(); ++d) {
            if (info.m_readInt)
                writer.write<int>(info.m_data[d]);
            else
                writer.write<byte>(info.m_data[d]);
        }
    }

    // Script list
    writer.write((ushort)(scriptList.count() - globalScriptCount));
    int cnt = (ushort)(scriptList.count() - globalScriptCount);

    for (int i = 0; i < cnt; ++i) {
        writer.write(scriptList[globalScriptCount + i].mainScript);
        writer.write(scriptList[globalScriptCount + i].playerScript);
        writer.write(scriptList[globalScriptCount + i].drawScript);
        writer.write(scriptList[globalScriptCount + i].m_startupScript);
    }
    for (int i = 0; i < cnt; ++i) {
        writer.write(scriptList[globalScriptCount + i].mainJumpTable);
        writer.write(scriptList[globalScriptCount + i].playerJumpTable);
        writer.write(scriptList[globalScriptCount + i].drawJumpTable);
        writer.write(scriptList[globalScriptCount + i].m_startupJumpTable);
    }

    // Function list
    writer.write((ushort)functionList.count());

    for (int i = 0; i < functionList.count(); ++i) writer.write(functionList[i].mainScript);
    for (int i = 0; i < functionList.count(); ++i) writer.write(functionList[i].mainJumpTable);

    writer.flush();
}
