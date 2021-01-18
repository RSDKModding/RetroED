#ifndef SAVEFILE_V4_H
#define SAVEFILE_V4_H

namespace RSDKv4
{

class SaveFile
{
public:
    SaveFile() {}
    SaveFile(QString filename) { read(filename); }
    SaveFile(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    inline void read(Reader &reader)
    {
        m_filename = reader.m_filepath;
        for (int i = 0; i < 0x2000; ++i) m_saveRAM[i] = reader.read<int>();
    }

    inline void write(QString filename)
    {
        if (filename == "")
            filename = m_filename;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    inline void write(Writer &writer)
    {
        m_filename = writer.m_filename;
        for (int i = 0; i < 0x2000; ++i) writer.write(m_saveRAM[i]);
        writer.flush();
    }

    inline int getCharacterID(int saveFile) { return m_saveRAM[(saveFilePos(saveFile) + 0x00) / 4]; }
    inline int getLives(int saveFile) { return m_saveRAM[(saveFilePos(saveFile) + 0x04) / 4]; }
    inline int getScore(int saveFile)
    {
        Utils::intBytes b = Utils::intBytes(m_saveRAM[(saveFilePos(saveFile) + 0x08) / 4]);
        return b.m_bytes[0] + (b.m_bytes[1] << 8) + (b.m_bytes[2] << 16) + (b.m_bytes[3] << 24);
    }
    inline int getTargetScore(int saveFile) { return m_saveRAM[(saveFilePos(saveFile) + 0x0C) / 4]; }
    inline int getZoneID(int saveFile) { return m_saveRAM[(saveFilePos(saveFile) + 0x10) / 4]; }
    inline int getEmeralds(int saveFile) { return m_saveRAM[(saveFilePos(saveFile) + 0x14) / 4]; }
    inline int getEmerald(int saveFile, int timeStone)
    {
        return Utils::getBit(m_saveRAM[(saveFilePos(saveFile) + 0x14) / 4], timeStone);
    }
    inline int getSpecialZoneID(int saveFile) { return m_saveRAM[(saveFilePos(saveFile) + 0x18) / 4]; }

    inline void setCharacterID(int saveFile, int value)
    {
        m_saveRAM[(saveFilePos(saveFile) + 0x00) / 4] = value;
    }
    inline void setLives(int saveFile, int value)
    {
        m_saveRAM[(saveFilePos(saveFile) + 0x04) / 4] = value;
    }
    inline void setScore(int saveFile, int value)
    {
        Utils::intBytes b = Utils::intBytes(m_saveRAM[(saveFilePos(saveFile) + 0x08) / 4]);
        b.m_bytes[0]      = (byte)(value >> 0);
        b.m_bytes[1]      = (byte)(value >> 8);
        b.m_bytes[2]      = (byte)(value >> 16);
        b.m_bytes[3]      = (byte)(value >> 24);
        m_saveRAM[(saveFilePos(saveFile) + 0x08) / 4] = b.m_val;
    }
    inline void setTargetScore(int saveFile, int value)
    {
        m_saveRAM[(saveFilePos(saveFile) + 0x0C) / 4] = value;
    }
    inline void setZoneID(int saveFile, int value)
    {
        m_saveRAM[(saveFilePos(saveFile) + 0x10) / 4] = value;
    }
    inline void setTimeStones(int saveFile, int value)
    {
        m_saveRAM[(saveFilePos(saveFile) + 0x14) / 4] = value;
    }
    inline void setTimeStone(int saveFile, int timeStone, bool value)
    {
        Utils::setBit(m_saveRAM[(saveFilePos(saveFile) + 0x14) / 4], value, timeStone);
    }
    inline void setSpecialZoneID(int saveFile, int value)
    {
        m_saveRAM[(saveFilePos(saveFile) + 0x18) / 4] = value;
    }

    // Global Saves
    inline int getNewSave() { return m_saveRAM[0x80 / 4]; }
    inline int getMusVolume() { return m_saveRAM[0x84 / 4]; }
    inline int getSFXVolume() { return m_saveRAM[0x88 / 4]; }
    inline int getControlStyle() { return m_saveRAM[0x8C / 4]; }
    inline int getRegion() { return m_saveRAM[0x90 / 4]; }
    inline int getTochRectW() { return m_saveRAM[0x94 / 4]; }
    inline int getTochRectH() { return m_saveRAM[0x98 / 4]; }
    inline int getTouchRectJumpX() { return m_saveRAM[0x9C / 4]; }
    inline int getTouchRectJumpY() { return m_saveRAM[0xA0 / 4]; }
    inline int getTouchRectMoveX() { return m_saveRAM[0xA4 / 4]; }
    inline int getTouchRectMoveY() { return m_saveRAM[0xA8 / 4]; }
    inline int getFilter() { return m_saveRAM[0xAC / 4]; }
    inline int getDisplayValue() { return m_saveRAM[0xB0 / 4]; }
    inline int getUnknown() { return m_saveRAM[0xB4 / 4]; }

    inline void setNewSave(int value) { m_saveRAM[0x80 / 4] = value; }
    inline void setMusVolume(int value) { m_saveRAM[0x84 / 4] = value; }
    inline void setSFXVolume(int value) { m_saveRAM[0x88 / 4] = value; }
    inline void setControlStyle(int value) { m_saveRAM[0x8C / 4] = value; }
    inline void setRegion(int value) { m_saveRAM[0x90 / 4] = value; }
    inline void setTochRectW(int value) { m_saveRAM[0x94 / 4] = value; }
    inline void setTochRectH(int value) { m_saveRAM[0x98 / 4] = value; }
    inline void setTouchRectJumpX(int value) { m_saveRAM[0x9C / 4] = value; }
    inline void setTouchRectJumpY(int value) { m_saveRAM[0xA0 / 4] = value; }
    inline void setTouchRectMoveX(int value) { m_saveRAM[0xA4 / 4] = value; }
    inline void setTouchRectMoveY(int value) { m_saveRAM[0xA8 / 4] = value; }
    inline void setFilter(int value) { m_saveRAM[0xAC / 4] = value; }
    inline void setDisplayValue(int value) { m_saveRAM[0xB0 / 4] = value; }
    inline void setUnknown(int value) { m_saveRAM[0xB4 / 4] = value; }

    int m_saveRAM[0x2000];

    inline int saveFilePos(int saveFile) { return saveFile * 0x20; };

    QString m_filename = "";
};

} // namespace RSDKv4

#endif // SAVEFILE_V4_H
