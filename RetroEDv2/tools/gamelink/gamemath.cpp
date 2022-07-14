#include "includes.hpp"

int sin1024LookupTable[0x400];
int cos1024LookupTable[0x400];
int tan1024LookupTable[0x400];
int asin1024LookupTable[0x400];
int acos1024LookupTable[0x400];

int sin512LookupTable[0x200];
int cos512LookupTable[0x200];
int tan512LookupTable[0x200];
int asin512LookupTable[0x200];
int acos512LookupTable[0x200];

int sin256LookupTable[0x100];
int cos256LookupTable[0x100];
int tan256LookupTable[0x100];
int asin256LookupTable[0x100];
int acos256LookupTable[0x100];

byte arcTan256LookupTable[0x100 * 0x100];

uint randSeed = 0;

void FunctionTable::CalculateTrigAngles()
{
    srand((uint)time(NULL));
    randSeed = rand();

    for (int i = 0; i < 0x400; ++i) {
        sin1024LookupTable[i]  = (int)(sinf((i / 512.0) * RSDK_PI) * 1024.0);
        cos1024LookupTable[i]  = (int)(cosf((i / 512.0) * RSDK_PI) * 1024.0);
        tan1024LookupTable[i]  = (int)(tanf((i / 512.0) * RSDK_PI) * 1024.0);
        asin1024LookupTable[i] = (int)((asinf(i / 1023.0) * 512.0) / RSDK_PI);
        acos1024LookupTable[i] = (int)((acosf(i / 1023.0) * 512.0) / RSDK_PI);
    }

    cos1024LookupTable[0x000] = 0x400;
    cos1024LookupTable[0x100] = 0;
    cos1024LookupTable[0x200] = -0x400;
    cos1024LookupTable[0x300] = 0;

    sin1024LookupTable[0x000] = 0;
    sin1024LookupTable[0x100] = 0x400;
    sin1024LookupTable[0x200] = 0;
    sin1024LookupTable[0x300] = -0x400;

    for (int i = 0; i < 0x200; ++i) {
        sin512LookupTable[i]  = (int)(sinf((i / 256.0) * RSDK_PI) * 512.0);
        cos512LookupTable[i]  = (int)(cosf((i / 256.0) * RSDK_PI) * 512.0);
        tan512LookupTable[i]  = (int)(tanf((i / 256.0) * RSDK_PI) * 512.0);
        asin512LookupTable[i] = (int)((asinf(i / 511.0) * 256.0) / RSDK_PI);
        acos512LookupTable[i] = (int)((acosf(i / 511.0) * 256.0) / RSDK_PI);
    }

    cos512LookupTable[0x00]  = 0x200;
    cos512LookupTable[0x80]  = 0;
    cos512LookupTable[0x100] = -0x200;
    cos512LookupTable[0x180] = 0;

    sin512LookupTable[0x00]  = 0;
    sin512LookupTable[0x80]  = 0x200;
    sin512LookupTable[0x100] = 0;
    sin512LookupTable[0x180] = -0x200;

    for (int i = 0; i < 0x100; i++) {
        sin256LookupTable[i]  = (int)((sin512LookupTable[i * 2] >> 1));
        cos256LookupTable[i]  = (int)((cos512LookupTable[i * 2] >> 1));
        tan256LookupTable[i]  = (int)((tan512LookupTable[i * 2] >> 1));
        asin256LookupTable[i] = (int)((asinf(i / 255.0) * 128.0) / RSDK_PI);
        acos256LookupTable[i] = (int)((acosf(i / 255.0) * 128.0) / RSDK_PI);
    }

    for (int y = 0; y < 0x100; ++y) {
        byte *arcTan = (byte *)&arcTan256LookupTable[y];

        for (int x = 0; x < 0x100; ++x) {
            // 40.743664 = 0x100 / (2 * M_PI) (roughly)
            *arcTan = (int)(float)((float)atan2((float)y, x) * 40.743664);
            arcTan += 0x100;
        }
    }
}

byte FunctionTable::ArcTanLookup(int X, int Y)
{
    int x = abs(X);
    int y = abs(Y);

    if (x <= y) {
        while (y > 0xFF) {
            x >>= 4;
            y >>= 4;
        }
    }
    else {
        while (x > 0xFF) {
            x >>= 4;
            y >>= 4;
        }
    }
    if (X <= 0) {
        if (Y <= 0)
            return arcTan256LookupTable[(x << 8) + y] + 0x80;
        else
            return 0x80 - arcTan256LookupTable[(x << 8) + y];
    }
    else if (Y <= 0)
        return -arcTan256LookupTable[(x << 8) + y];
    else
        return arcTan256LookupTable[(x << 8) + y];
}
