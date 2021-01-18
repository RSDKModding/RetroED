#ifndef RSDK_UTILS_H
#define RSDK_UTILS_H

namespace Utils
{

union intBytes {
    intBytes() : m_val(0) {}
    intBytes(int v) : m_val(v) {}
    int m_val;
    byte m_bytes[4];
};

template <typename T> inline void setBit(T &value, char set, int pos)
{
    value ^= (-set ^ value) & (1 << pos);
}
inline bool getBit(int b, int pos) { return (b >> pos & 1); }

inline QString getFilenameAndFolder(QString filepath)
{
    QFileInfo info(filepath);
    QDir dir(info.dir());
    dir.cdUp();
    return dir.relativeFilePath(filepath);
}

inline QByteArray splitNybbles(byte b)
{
    QByteArray nybbles;
    nybbles.append((byte)((b & 0xF0) >> 4));
    nybbles.append((byte)(b & 0x0F));
    return nybbles;
}

inline byte addNybbles(byte a, byte b) { return (byte)((a & 0xF) << 4 | (b & 0xF)); }

inline QByteArray getMd5HashByteArray(QString input)
{
    return QCryptographicHash::hash(input.toLatin1(), QCryptographicHash::Md5);
}

inline QByteArray getMd5HashByteArray(QByteArray input)
{
    return QCryptographicHash::hash(input, QCryptographicHash::Md5);
}

inline QString getMd5HashString(QString input)
{
    return QCryptographicHash::hash(input.toLatin1(), QCryptographicHash::Md5).toHex();
}

inline QString getMd5HashString(QByteArray input) { return input.toHex(); }

} // namespace Utils

#endif // RSDK_UTILS_H
