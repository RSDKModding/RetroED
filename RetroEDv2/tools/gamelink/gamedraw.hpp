#ifndef GAMEDRAW_HPP
#define GAMEDRAW_HPP

namespace FunctionTable
{
    void drawLine(int x1, int y1, int x2, int y2, uint color, int alpha, InkEffects inkEffect, bool32 screenRelative);
    void drawRect(int x, int y, int width, int height, uint color, int alpha, InkEffects inkEffect,
                  bool32 screenRelative);
    void drawCircle(int x, int y, int radius, uint color, int alpha, InkEffects inkEffect,
                    bool32 screenRelative);
    void drawCircleOutline(int x, int y, int innerRadius, int outerRadius, uint color, int alpha,
                           InkEffects inkEffect, bool32 screenRelative);

    void drawFace(Vector2<int> *vertices, int vertCount, int r, int g, int b, int alpha,
                  InkEffects inkEffect);
    void drawBlendedFace(Vector2<int> *vertices, uint *colors, int vertCount, int alpha,
                         InkEffects inkEffect);

    void drawSprite(Animator *data, Vector2<int> *position, bool32 screenRelative);

    void drawTile(ushort *tileInfo, int countX, int countY, GameEntity *entityPtr, Vector2<int> *position,
                  bool32 screenRelative);

    void drawText(Animator *data, Vector2<int> *position, TextInfo *info, int endFrame, int textLength,
                  byte align, int spacing, int a8, Vector2<int> *charPositions, bool32 screenRelative);
}


#endif