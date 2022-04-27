#ifndef GAMETEXT_HPP
#define GAMETEXT_HPP

namespace FunctionTable
{

void setText(TextInfo *textInfo, char *text, uint size);
void copyString(TextInfo *dst, TextInfo *src);
void getCString(char *dest, TextInfo *info);
void appendText(TextInfo *info, char *text);
void prependText(TextInfo *info, char *text);
void appendString(TextInfo *textA, TextInfo *textB);
bool32 stringCompare(TextInfo *textA, TextInfo *textB, bool32 exactMatch);
} // namespace FunctionTable

#endif // GAMETEXT_HPP
