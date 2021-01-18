#include "include.hpp"

void RSDKv3::Bytecode::read(Reader &reader, int scriptCount, bool clear)
{
    m_filename = reader.m_filepath;
    if (clear) {
        m_scriptData.clear();
        m_jumpTableData.clear();
        m_scriptList.clear();
        m_functionList.clear();
    }
    else {
        m_globalScriptDataCount = m_scriptData.count();
        m_globalJumpTableCount  = m_jumpTableData.count();
    }

    for (int opcount = reader.read<int>(); opcount > 0;) {
        byte flags    = reader.read<byte>();
        int dataCount = (flags & 0x7F);

        if ((flags & 0x80) == 0) {
            for (int i = 0; i < dataCount; ++i) m_scriptData.append(reader.read<byte>());
            opcount -= dataCount;
        }
        else {
            for (int i = 0; i < dataCount; ++i) m_scriptData.append(reader.read<int>());
            opcount -= dataCount;
        }
    }

    for (int opcount = reader.read<int>(); opcount > 0;) {
        byte flags    = reader.read<byte>();
        int dataCount = (flags & 0x7F);
        if ((flags & 0x80) == 0) {
            for (int i = 0; i < dataCount; ++i) m_jumpTableData.append(reader.read<byte>());
            opcount -= dataCount;
        }
        else {
            for (int i = 0; i < dataCount; ++i) m_jumpTableData.append(reader.read<int>());
            opcount -= dataCount;
        }
    }

    if (!clear) {
        m_globalScriptCount = scriptCount;
    }

    int scriptCnt = reader.read<ushort>(); // File count

    // Fill in "blanks"
    for (int i = 0; i < scriptCount && clear; ++i) {
        m_scriptList.append(ObjectScript());
        m_scriptList[i].m_mainScript    = 0x3FFFF;
        m_scriptList[i].m_playerScript  = 0x3FFFF;
        m_scriptList[i].m_drawScript    = 0x3FFFF;
        m_scriptList[i].m_startupScript = 0x3FFFF;
    }

    for (int i = 0; i < scriptCnt; ++i) {
        m_scriptList.append(ObjectScript());
        m_scriptList[scriptCount + i].m_mainScript    = reader.read<int>();
        m_scriptList[scriptCount + i].m_playerScript  = reader.read<int>();
        m_scriptList[scriptCount + i].m_drawScript    = reader.read<int>();
        m_scriptList[scriptCount + i].m_startupScript = reader.read<int>();
    }
    for (int i = 0; i < scriptCnt; ++i) {
        m_scriptList[scriptCount + i].m_mainJumpTable    = reader.read<int>();
        m_scriptList[scriptCount + i].m_playerJumpTable  = reader.read<int>();
        m_scriptList[scriptCount + i].m_drawJumpTable    = reader.read<int>();
        m_scriptList[scriptCount + i].m_startupJumpTable = reader.read<int>();
    }

    int functionCount = reader.read<ushort>();
    m_functionList.clear();

    for (int i = 0; i < functionCount; ++i) {
        m_functionList.append(FunctionScript());
        m_functionList[i].m_mainScript = reader.read<int>();
    }

    for (int i = 0; i < functionCount; ++i) m_functionList[i].m_mainJumpTable = reader.read<int>();
}

struct DataInfo {
    QList<int> m_data;
    bool m_readInt = false;
};

void RSDKv3::Bytecode::write(Writer &writer)
{
    m_filename = writer.m_filename;

    QList<DataInfo> dataInfo;

    // Script Data
    dataInfo.clear();
    for (int dataPos = m_globalScriptDataCount; dataPos < m_scriptData.count();) {
        DataInfo info;
        info.m_data.clear();
        info.m_readInt = m_scriptData[dataPos] < 0 || m_scriptData[dataPos] >= 0x100;
        if (!info.m_readInt) {
            for (int i = 0; (m_scriptData[dataPos] >= 0 && m_scriptData[dataPos] < 0x100) && i < 0x7F;
                 ++i) {
                info.m_data.append(m_scriptData[dataPos++]);
                if (dataPos >= m_scriptData.count())
                    break;
            }
            dataInfo.append(info);
        }
        else {
            for (int i = 0; (m_scriptData[dataPos] < 0 || m_scriptData[dataPos] >= 0x100) && i < 0x7F;
                 ++i) {
                info.m_data.append(m_scriptData[dataPos++]);
                if (dataPos >= m_scriptData.count())
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
    for (int dataPos = m_globalJumpTableCount; dataPos < m_jumpTableData.count();) {
        DataInfo info;
        info.m_data.clear();
        info.m_readInt = m_jumpTableData[dataPos] < 0 || m_jumpTableData[dataPos] >= 0x100;
        if (!info.m_readInt) {
            for (int i = 0;
                 (m_jumpTableData[dataPos] >= 0 && m_jumpTableData[dataPos] < 0x100) && i < 0x7F; ++i) {
                info.m_data.append(m_jumpTableData[dataPos++]);
                if (dataPos >= m_jumpTableData.count())
                    break;
            }
            dataInfo.append(info);
        }
        else {
            for (int i = 0;
                 (m_jumpTableData[dataPos] < 0 || m_jumpTableData[dataPos] >= 0x100) && i < 0x7F; ++i) {
                info.m_data.append(m_jumpTableData[dataPos++]);
                if (dataPos >= m_jumpTableData.count())
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
    writer.write((ushort)(m_scriptList.count() - m_globalScriptCount));
    int cnt = (ushort)(m_scriptList.count() - m_globalScriptCount);

    for (int i = 0; i < cnt; ++i) {
        writer.write(m_scriptList[m_globalScriptCount + i].m_mainScript);
        writer.write(m_scriptList[m_globalScriptCount + i].m_playerScript);
        writer.write(m_scriptList[m_globalScriptCount + i].m_drawScript);
        writer.write(m_scriptList[m_globalScriptCount + i].m_startupScript);
    }
    for (int i = 0; i < cnt; ++i) {
        writer.write(m_scriptList[m_globalScriptCount + i].m_mainJumpTable);
        writer.write(m_scriptList[m_globalScriptCount + i].m_playerJumpTable);
        writer.write(m_scriptList[m_globalScriptCount + i].m_drawJumpTable);
        writer.write(m_scriptList[m_globalScriptCount + i].m_startupJumpTable);
    }

    // Function list
    writer.write((ushort)m_functionList.count());

    for (int i = 0; i < m_functionList.count(); ++i) writer.write(m_functionList[i].m_mainScript);
    for (int i = 0; i < m_functionList.count(); ++i) writer.write(m_functionList[i].m_mainJumpTable);

    writer.flush();
}
