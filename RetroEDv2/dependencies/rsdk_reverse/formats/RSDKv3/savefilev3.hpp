#ifndef SAVEFILE_V3_H
#define SAVEFILE_V3_H

namespace RSDKv3
{

class SaveFile
{
public:
    SaveFile() { memset(m_saveRAM, 0, 0x2000 * sizeof(int)); }
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
    inline int getZoneID(int saveFile) { return m_saveRAM[(saveFilePos(saveFile) + 0x0C) / 4]; }
    inline int getTimeStones(int saveFile) { return m_saveRAM[(saveFilePos(saveFile) + 0x10) / 4]; }
    inline int getTimeStone(int saveFile, int timeStone)
    {
        return Utils::getBit(m_saveRAM[(saveFilePos(saveFile) + 0x10) / 4], timeStone);
    }
    inline int getSpecialZoneID(int saveFile) { return m_saveRAM[(saveFilePos(saveFile) + 0x14) / 4]; }
    inline int getGoodFutures(int saveFile) { return m_saveRAM[(saveFilePos(saveFile) + 0x18) / 4]; }
    inline int getFuturesSaved(int saveFile)
    {
        Utils::intBytes b = Utils::intBytes(m_saveRAM[(saveFilePos(saveFile) + 0x1C) / 4]);
        return b.m_bytes[2] + (b.m_bytes[3] << 8);
    }
    inline int getMSDestroyed(int saveFile)
    {
        Utils::intBytes b = Utils::intBytes(m_saveRAM[(saveFilePos(saveFile) + 0x1C) / 4]);
        return b.m_bytes[0] + (b.m_bytes[1] << 8);
    }

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
    inline void setZoneID(int saveFile, int value)
    {
        m_saveRAM[(saveFilePos(saveFile) + 0x0C) / 4] = value;
    }
    inline void setTimeStones(int saveFile, int value)
    {
        m_saveRAM[(saveFilePos(saveFile) + 0x10) / 4] = value;
    }
    inline void setTimeStone(int saveFile, int timeStone, bool value)
    {
        Utils::setBit(m_saveRAM[(saveFilePos(saveFile) + 0x10) / 4], value, timeStone);
    }
    inline void setSpecialZoneID(int saveFile, int value)
    {
        m_saveRAM[(saveFilePos(saveFile) + 0x14) / 4] = value;
    }
    inline void setGoodFutures(int saveFile, int value)
    {
        m_saveRAM[(saveFilePos(saveFile) + 0x18) / 4] = value;
    }
    inline void setFuturesSaved(int saveFile, short value)
    {
        Utils::intBytes b = Utils::intBytes(m_saveRAM[(saveFilePos(saveFile) + 0x1C) / 4]);
        b.m_bytes[2]      = (byte)(value >> 0);
        b.m_bytes[3]      = (byte)(value >> 8);
        m_saveRAM[(saveFilePos(saveFile) + 0x08) / 4] = b.m_val;
    }
    inline void setMSDestroyed(int saveFile, short value)
    {
        Utils::intBytes b = Utils::intBytes(m_saveRAM[(saveFilePos(saveFile) + 0x1C) / 4]);
        b.m_bytes[0]      = (byte)(value >> 0);
        b.m_bytes[1]      = (byte)(value >> 8);
        m_saveRAM[(saveFilePos(saveFile) + 0x08) / 4] = b.m_val;
    }

    // Global Saves
    inline int getNewSave() { return m_saveRAM[0x80 / 4]; }
    inline int getMusVolume() { return m_saveRAM[0x84 / 4]; }
    inline int getSFXVolume() { return m_saveRAM[0x88 / 4]; }
    inline int getSpindashStyle() { return m_saveRAM[0x8C / 4]; }
    inline int getUnknown() { return m_saveRAM[0x90 / 4]; }
    inline int getFilter() { return m_saveRAM[0x94 / 4]; }
    inline int getOSTStyle() { return m_saveRAM[0x98 / 4]; }
    inline bool getTailsUnlocked()
    {
        bool bits[3];
        bits[0] = Utils::getBit(m_saveRAM[0x9C / 4], 0);
        bits[1] = Utils::getBit(m_saveRAM[0x9C / 4], 1);
        bits[2] = Utils::getBit(m_saveRAM[0x9C / 4], 2);
        return bits[0] && bits[1] && bits[2];
    }
    inline int getTimeAttackUnlocks() { return m_saveRAM[0x9C / 4]; }

    inline void setNewSave(int value) { m_saveRAM[0x80 / 4] = value; }
    inline void setMusVolume(int value) { m_saveRAM[0x84 / 4] = value; }
    inline void setSFXVolume(int value) { m_saveRAM[0x88 / 4] = value; }
    inline void setSpindashStyle(int value) { m_saveRAM[0x8C / 4] = value; }
    inline void setUnknown(int value) { m_saveRAM[0x90 / 4] = value; }
    inline void setFilter(int value) { m_saveRAM[0x94 / 4] = value; }
    inline void setOSTStyle(int value) { m_saveRAM[0x98 / 4] = value; }
    inline void setTailsUnlocked(bool unlocked)
    {
        Utils::setBit(m_saveRAM[0x9C / 4], unlocked, 0);
        Utils::setBit(m_saveRAM[0x9C / 4], unlocked, 1);
        Utils::setBit(m_saveRAM[0x9C / 4], unlocked, 2);
    }
    inline void setTimeAttackUnlocks(int value) { m_saveRAM[0x9C / 4] = value; }

    int m_saveRAM[0x2000];

    inline int saveFilePos(int saveFile) { return saveFile * 0x20; };

    QString m_filename = "";
};

} // namespace RSDKv3

#endif // SAVEFILE_V3_H
