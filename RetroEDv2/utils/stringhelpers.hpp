#pragma once

#define MEM_ZERO(x)  memset(&(x), 0, sizeof((x)))
#define MEM_ZEROP(x) memset((x), 0, sizeof(*(x)))

inline void StrCopy(char *dest, const char *src)
{
    int i = 0;
    for (; src[i]; ++i) dest[i] = src[i];
    dest[i] = 0;
}

inline void StrAdd(char *dest, const char *src)
{
    int destStrPos = 0;
    int srcStrPos  = 0;
    while (dest[destStrPos]) ++destStrPos;
    while (true) {
        if (!src[srcStrPos]) {
            break;
        }
        dest[destStrPos++] = src[srcStrPos++];
    }
    dest[destStrPos] = 0;
}

inline bool StrComp(const char *stringA, const char *stringB)
{
    bool match    = true;
    bool finished = false;
    while (!finished) {
        if (*stringA == *stringB || *stringA == *stringB + ' ' || *stringA == *stringB - ' ') {
            if (*stringA) {
                ++stringA;
                ++stringB;
            }
            else {
                finished = true;
            }
        }
        else {
            match    = false;
            finished = true;
        }
    }
    return match;
}

inline int StrLength(const char *string)
{
    int len = 0;
    for (len = 0; string[len]; len++)
        ;
    return len;
}
int FindStringToken(const char *string, const char *token, char stopID);

inline void StringLowerCase(char *dest, const char *src)
{
    int destPos = 0;
    int curChar = *src;
    if (*src) {
        int srcPos = 0;
        do {
            while ((uint)(curChar - 'A') <= 0x19u) {
                destPos       = srcPos;
                dest[destPos] = curChar + ' ';
                curChar       = src[++srcPos];
                if (!curChar) {
                    dest[++destPos] = 0;
                    return;
                }
            }
            destPos       = srcPos;
            dest[destPos] = curChar;
            curChar       = src[++srcPos];
        } while (curChar);
    }
    dest[++destPos] = 0;
}

inline void StringUpperCase(char *dest, const char *src)
{
    int destPos = 0;
    int curChar = *src;
    if (*src) {
        int srcPos = 0;
        do {
            while ((uint)(curChar - 'a') <= 0x19u) {
                destPos       = srcPos;
                dest[destPos] = curChar - ' ';
                curChar       = src[++srcPos];
                if (!curChar) {
                    dest[++destPos] = 0;
                    return;
                }
            }
            destPos       = srcPos;
            dest[destPos] = curChar;
            curChar       = src[++srcPos];
        } while (curChar);
    }
    dest[++destPos] = 0;
}

void ConvertIntegerToString(char *text, int value);
bool ConvertStringToInteger(const char *text, int *value);
void AppendIntegerToString(char *text, int value);

void SetScriptTypeName(const char *name, char *buffer);


