#include "includes.hpp"

byte stringFlags[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                       2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                       3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6 };

void FunctionTable::setText(TextInfo *textInfo, char *text, uint size)
{
    if (!v5Editor)
        return;

    if (text) {
        bool match = true;
        // No point setting smth we already have
        if (textInfo->length != strlen(text))
            match = false;

        for (int i = 0; i < textInfo->length && match; ++i) {
            if (textInfo->text[i] != text[i])
                match = false;
        }

        if (match)
            return;

        textInfo->text = NULL;

        if (true) // TODO: stop it from crashing when chaing scenes & text is loaded
            return;

        if (*text) {
            int cnt = 0;
            do textInfo->textLength = ++cnt;
            while (text[cnt]);
        }

        if (size && size >= textInfo->textLength)
            textInfo->length = size;
        else
            textInfo->length = textInfo->textLength;

        if (!textInfo->length)
            textInfo->length = 1;

        allocateStorage(v5Editor->dataStorage, sizeof(ushort) * textInfo->length,
                        (void **)&textInfo->text, DATASET_STR, false);

        char *txt = text;
        if (*text) {
            int cnt = 0;
            do {
                textInfo->text[cnt++] = *txt;
                ++txt;
            } while (*txt);
        }
    }
}

void FunctionTable::copyString(TextInfo *dst, TextInfo *src)
{
    if (!v5Editor)
        return;

    if (dst == src)
        return;

    int textLen = src->textLength;
    dst->text   = NULL;
    if (dst->length >= textLen) {
        if (!dst->text) {
            allocateStorage(v5Editor->dataStorage, sizeof(ushort) * dst->length, (void **)&dst->text,
                            DATASET_STR, false);
        }
    }
    else {
        dst->length = textLen;
        allocateStorage(v5Editor->dataStorage, sizeof(ushort) * dst->length, (void **)&dst->text,
                        DATASET_STR, false);
    }

    dst->textLength = src->textLength;
    for (int c = 0; c < dst->textLength; ++c) {
        dst->text[c] = src->text[c];
    }
}
void FunctionTable::getCString(char *dest, TextInfo *info)
{
    if (!v5Editor)
        return;

    if (!info->text)
        return;

    char *text = (char *)"";
    if (dest)
        text = dest;
    int textLen = 0x400;
    if (dest)
        textLen = info->textLength;

    int c = 0;
    for (; c < textLen; ++c) {
        text[c] = info->text[c];
    }
    text[c] = 0;
}

void FunctionTable::prependText(TextInfo *info, char *text)
{
    if (!v5Editor)
        return;

    if (!*text)
        return;

    int len = 0;
    for (int pos = 0; text[pos]; ++len) {
        pos += stringFlags[*text & 0xFF];
    }
    if (!len)
        return;

    if (info->length < len || !info->text) {
        info->length = len;
        allocateStorage(v5Editor->dataStorage, sizeof(ushort) * info->length, (void **)&info->text,
                        DATASET_STR, false);
    }

    info->textLength = len;
    for (int pos = 0; pos < info->textLength; ++pos) {
        ushort c = 0;
        switch (stringFlags[*text & 0xFF]) {
            case 1:
                c = text[0];
                ++text;
                break;
            case 2:
                c = (text[1] & 0x3F) | ((text[0] & 0x1F) << 6);
                text += 2;
                break;
            case 3:
                c = (text[2] & 0x3F) | ((text[1] & 0x3F) << 6) | (text[0] << 12);
                text += 3;
                break;
            case 4:
                c = (text[3] & 0x3F) | ((text[2] & 0x3F) << 6) | (text[1] << 12);
                text += 4;
                break;
            case 5: text += 5; break;
            case 6: text += 6; break;
            default: break;
        }
        info->text[pos] = c;
    }
}

void FunctionTable::appendText(TextInfo *info, char *text)
{
    if (!v5Editor)
        return;

    if (!*text)
        return;

    int len = 0;
    for (int pos = 0; text[pos]; ++len) {
        pos += stringFlags[*text & 0xFF];
    }
    if (!len)
        return;

    int newLen = (len + info->length);
    if (info->length < newLen || !info->text) {
        info->length = newLen;
        allocateStorage(v5Editor->dataStorage, sizeof(ushort) * info->length, (void **)&info->text,
                        DATASET_STR, false);
    }

    for (int pos = info->textLength; pos < len; ++pos) {
        ushort c = 0;
        switch (stringFlags[*text & 0xFF]) {
            case 1:
                c = text[0];
                ++text;
                break;
            case 2:
                c = (text[1] & 0x3F) | ((text[0] & 0x1F) << 6);
                text += 2;
                break;
            case 3:
                c = (text[2] & 0x3F) | ((text[1] & 0x3F) << 6) | (text[0] << 12);
                text += 3;
                break;
            case 4:
                c = (text[3] & 0x3F) | ((text[2] & 0x3F) << 6) | (text[1] << 12);
                text += 4;
                break;
            case 5: text += 5; break;
            case 6: text += 6; break;
            default: break;
        }
        info->text[pos] = c;
    }
    info->textLength = newLen;
}

void FunctionTable::appendString(TextInfo *info, TextInfo *string)
{
    if (!v5Editor)
        return;

    uint totalLen = string->textLength + info->textLength;
    if (info->length < totalLen || !info->text) {
        if (info->text) {
            ushort *buffer = info->text;
            allocateStorage(v5Editor->dataStorage, sizeof(ushort) * totalLen, (void **)&info->text,
                            DATASET_STR, false);
            for (int charID = 0; charID < info->textLength; ++charID) {
                info->text[charID] = buffer[charID];
            }
            buffer = NULL;
        }
        else {
            allocateStorage(v5Editor->dataStorage, sizeof(ushort) * totalLen, (void **)&info->text,
                            DATASET_STR, false);
        }
        info->length = string->textLength + info->textLength;
    }

    int textLen = info->textLength;
    info->textLength += string->textLength;
    int id = 0;
    for (; textLen < info->textLength; ++textLen) {
        info->text[textLen] = string->text[id++];
    }
}

bool32 FunctionTable::stringCompare(TextInfo *textA, TextInfo *textB, bool32 exactMatch)
{
    if (!v5Editor)
        return false;

    if (textA->length != textB->length)
        return false;

    if (exactMatch) { // each character has to match
        for (int i = 0; i < textA->length; ++i) {
            if (textA->text[i] != textB->text[i])
                return false;
        }
        return true;
    }
    else { // attempt to ignore case sensitivity when matching
        if (textA->length <= 0)
            return true;

        for (int i = 0; i < textA->length; ++i) {
            if (textA->text[i] != textB->text[i]) {
                if (textA->text[i] != textB->text[i] + 0x20 && textA->text[i] != textB->text[i] - 0x20)
                    return false;
            }
        }
        return true;
    }
}
