#include "include.hpp"

void RSDKv2::Bytecode::read(Reader &reader, int scriptCount, bool clear)
{
    filePath = reader.filepath;
    if (clear) {
        scriptData.clear();
        jumpTableData.clear();
        scriptList.clear();
        playerScript.playerStates.clear();
    }
    else {
        globalScriptDataCount = scriptData.count();
        globalJumpTableCount  = jumpTableData.count();
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
        scriptList[i].startupScript = 0x3FFFF;
    }

    for (int i = 0; i < scriptCnt; ++i) {
        scriptList.append(ObjectScript());
        scriptList[scriptCount + i].mainScript    = reader.read<int>();
        scriptList[scriptCount + i].playerScript  = reader.read<int>();
        scriptList[scriptCount + i].drawScript    = reader.read<int>();
        scriptList[scriptCount + i].startupScript = reader.read<int>();
    }
    for (int i = 0; i < scriptCnt; ++i) {
        scriptList[scriptCount + i].mainJumpTable    = reader.read<int>();
        scriptList[scriptCount + i].playerJumpTable  = reader.read<int>();
        scriptList[scriptCount + i].drawJumpTable    = reader.read<int>();
        scriptList[scriptCount + i].startupJumpTable = reader.read<int>();
    }

    playerScript.scriptCodePtr_PlayerMain = reader.read<int>();
    playerScript.jumpTablePtr_PlayerMain  = reader.read<int>();
    int stateCount                        = reader.read<byte>();
    playerScript.playerStates.clear();

    for (int i = 0; i < stateCount; ++i) {
        playerScript.playerStates.append(FunctionScript());
        playerScript.playerStates[i].mainScript = reader.read<int>();
    }

    for (int i = 0; i < stateCount; ++i)
        playerScript.playerStates[i].mainJumpTable = reader.read<int>();
}

struct DataInfov2 {
    QList<int> data;
    bool readInt = false;
};

void RSDKv2::Bytecode::write(Writer &writer)
{
    filePath = writer.filePath;

    QList<DataInfov2> dataInfo;

    // Script Data
    dataInfo.clear();
    for (int dataPos = globalScriptDataCount; dataPos < scriptData.count();) {
        DataInfov2 info;
        info.data.clear();
        info.readInt = scriptData[dataPos] < 0 || scriptData[dataPos] >= 0x100;
        if (!info.readInt) {
            for (int i = 0; (scriptData[dataPos] >= 0 && scriptData[dataPos] < 0x100) && i < 0x7F;
                 ++i) {
                info.data.append(scriptData[dataPos++]);
                if (dataPos >= scriptData.count())
                    break;
            }
            dataInfo.append(info);
        }
        else {
            for (int i = 0; (scriptData[dataPos] < 0 || scriptData[dataPos] >= 0x100) && i < 0x7F;
                 ++i) {
                info.data.append(scriptData[dataPos++]);
                if (dataPos >= scriptData.count())
                    break;
            }
            dataInfo.append(info);
        }
    }

    int count = 0;
    for (int i = 0; i < dataInfo.count(); ++i) count += dataInfo[i].data.size();
    writer.write(count);

    for (int i = 0; i < dataInfo.count(); ++i) {
        DataInfov2 &info = dataInfo[i];
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
    for (int dataPos = globalJumpTableCount; dataPos < jumpTableData.count();) {
        DataInfov2 info;
        info.data.clear();
        info.readInt = jumpTableData[dataPos] < 0 || jumpTableData[dataPos] >= 0x100;
        if (!info.readInt) {
            for (int i = 0; (jumpTableData[dataPos] >= 0 && jumpTableData[dataPos] < 0x100) && i < 0x7F;
                 ++i) {
                info.data.append(jumpTableData[dataPos++]);
                if (dataPos >= jumpTableData.count())
                    break;
            }
            dataInfo.append(info);
        }
        else {
            for (int i = 0; (jumpTableData[dataPos] < 0 || jumpTableData[dataPos] >= 0x100) && i < 0x7F;
                 ++i) {
                info.data.append(jumpTableData[dataPos++]);
                if (dataPos >= jumpTableData.count())
                    break;
            }
            dataInfo.append(info);
        }
    }

    count = 0;
    for (int i = 0; i < dataInfo.count(); ++i) count += dataInfo[i].data.size();
    writer.write(count);

    for (int i = 0; i < dataInfo.count(); ++i) {
        DataInfov2 &info = dataInfo[i];
        writer.write<byte>(info.data.size() | ((byte)info.readInt << 7));
        for (int d = 0; d < info.data.count(); ++d) {
            if (info.readInt)
                writer.write<int>(info.data[d]);
            else
                writer.write<byte>(info.data[d]);
        }
    }

    // Script list
    writer.write((ushort)(scriptList.count() - globalScriptCount));
    int cnt = (ushort)(scriptList.count() - globalScriptCount);

    for (int i = 0; i < cnt; ++i) {
        writer.write(scriptList[globalScriptCount + i].mainScript);
        writer.write(scriptList[globalScriptCount + i].playerScript);
        writer.write(scriptList[globalScriptCount + i].drawScript);
        writer.write(scriptList[globalScriptCount + i].startupScript);
    }
    for (int i = 0; i < cnt; ++i) {
        writer.write(scriptList[globalScriptCount + i].mainJumpTable);
        writer.write(scriptList[globalScriptCount + i].playerJumpTable);
        writer.write(scriptList[globalScriptCount + i].drawJumpTable);
        writer.write(scriptList[globalScriptCount + i].startupJumpTable);
    }

    // Player script
    writer.write(playerScript.scriptCodePtr_PlayerMain);
    writer.write(playerScript.jumpTablePtr_PlayerMain);
    writer.write((byte)playerScript.playerStates.count());

    for (int i = 0; i < playerScript.playerStates.count(); ++i)
        writer.write(playerScript.playerStates[i].mainScript);
    for (int i = 0; i < playerScript.playerStates.count(); ++i)
        writer.write(playerScript.playerStates[i].mainJumpTable);

    writer.flush();
}
