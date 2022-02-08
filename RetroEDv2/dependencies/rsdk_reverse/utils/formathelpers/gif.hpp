#ifndef RSDK_GIF_H
#define RSDK_GIF_H

namespace FormatHelpers
{

class Gif
{
public:
    Gif()
    {
        for (int c = 0; c < 0x100; ++c) palette[c] = QColor(0xFF, 0x00, 0xFF);
    }
    Gif(ushort w, ushort h) : Gif()
    {
        width  = w;
        height = h;
        pixels.resize(w * h);
    }

    inline void read(QString filename, bool skipHeader = false, int clrCnt = 0x80)
    {
        Reader reader(filename);
        read(reader);
    }
    void read(Reader &reader, bool skipHeader = false, int clrCnt = 0x80);

    inline void write(QString filename, bool skipHeader = false, bool useLocal = false)
    {
        if (filename == "")
            filename = filePath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer, bool skipHeader = false, bool useLocal = false);

    void fromImage(QImage img);
    QImage toImage();

    ushort width  = 0;
    ushort height = 0;
    QColor palette[0x100];
    QByteArray pixels;

    QString filePath = "";

private:
    // DECLARATIONS
    const static int PARSING_IMAGE  = 0;
    const static int PARSE_COMPLETE  = 1;
    const static int LZ_MAX_CODE     = 4095;
    const static int LZ_BITS         = 12;
    const static int FIRST_CODE      = 4097;
    const static int NO_SUCH_CODE    = 4098;

    const static int HT_SIZE = 8192;
    const static int HT_KEY_MASK = 0x1FFF;

    const static int codeMasks[16];
    const static int initialRows[4];
    const static int rowInc[4];

    class Decoder
    {
    public:
        int depth;
        int clearCode;
        int eofCode;
        int runningCode;
        int runningBits;
        int prevCode;
        int currentCode;
        int maxCodePlusOne;
        int stackPtr;
        int shiftState;
        int fileState;
        int position;
        int bufferSize;
        uint shiftData;
        uint pixelCount;
        byte buffer[0x100];
        byte stack[0x1000];
        byte suffix[0x1000];
        uint prefix[0x1000];
    };

    class Encoder
    {
    public:
        int depth;
        int clearCode;
        int eofCode;
        int runningCode;
        int runningBits;
        int currentCode;
        int maxCodePlusOne;
        int shiftState;
        int bufferSize;
        uint shiftData;
        int fileState;
        byte buffer[0x100];
        uint hashTable[0x2000];
    };

    Decoder decoder;
    Encoder encoder;

    // GIF READING
    void initDecoder(Reader &reader);
    void readLine(Reader &reader, int length, int offset);
    int readCode(Reader &reader);
    byte readByte(Reader &reader);
    byte tracePrefix(int code, int clearCode);
    void readPictureData(int width, int height, bool interlaced, Reader &reader);

    // GIF WRITING
    void insertHashTable(uint key, int code);
    int existsHashTable(uint key);

    void initEncoder(Writer &writer, byte bitsPerPixel);
    void writeByte(Writer &writer, int b, bool flush = false);
    void writeCode(Writer &writer, int code, bool flush = false);
    void writeLine(Writer &writer, byte *line, int length);
    void writePictureData(int width, int height, bool interlaced, byte bitsPerPixel, Writer &writer);
};

} // namespace FormatHelpers

#endif // RSDK_GIF_H
