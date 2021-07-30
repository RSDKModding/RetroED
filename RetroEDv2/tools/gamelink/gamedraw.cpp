#include "includes.hpp"

void FunctionTable::drawLine(int x1, int y1, int x2, int y2, uint color, int alpha, InkEffects inkEffect, bool32 screenRelative) {
    if (!v5Editor) return;
    float z = v5Editor->viewer->currZ;
    Vector4<float> rcolor = {
        ((color >> 16) & 0xFF) / 255.0f,
        ((color >> 8) & 0xFF) / 255.0f,
        (color & 0xFF) / 255.0f,
        alpha / 255.0f
    }; 
    v5Editor->viewer->drawLine(x1, y1, z, x2, y2, z, rcolor, v5Editor->viewer->primitiveShader);
    v5Editor->viewer->incZ();
}

void FunctionTable::drawRect(int x, int y, int width, int height, uint color, int alpha,
                             InkEffects inkEffect, bool32 screenRelative)
{
    if (!v5Editor)
        return;
    Vector4<float> rcolor = { ((color >> 16) & 0xFF) / 255.0f, ((color >> 8) & 0xFF) / 255.0f,
                             (color & 0xFF) / 255.0f, alpha / 255.0f };
    v5Editor->viewer->drawRect(x, y, v5Editor->viewer->incZ(), width, height, rcolor,
                               v5Editor->viewer->primitiveShader);
}

void FunctionTable::drawCircle(int x, int y, int radius, uint color, int alpha, InkEffects inkEffect,
                               bool32 screenRelative)
{
    if (!v5Editor)
        return;
    Vector4<float> rcolor = { ((color >> 16) & 0xFF) / 255.0f, ((color >> 8) & 0xFF) / 255.0f,
                             (color & 0xFF) / 255.0f, alpha / 255.0f };
    v5Editor->viewer->drawCircle(x, y, v5Editor->viewer->incZ(), radius, rcolor,
                               v5Editor->viewer->primitiveShader);
}
void FunctionTable::drawCircleOutline(int x, int y, int innerRadius, int outerRadius, uint color,
                                      int alpha, InkEffects inkEffect, bool32 screenRelative)
{
}

void FunctionTable::drawFace(Vector2<int> *vertices, int vertCount, int r, int g, int b, int alpha,
                             InkEffects inkEffect)
{
}
void FunctionTable::drawBlendedFace(Vector2<int> *vertices, uint *colors, int vertCount, int alpha,
                                    InkEffects inkEffect)
{
}

void FunctionTable::drawSprite(Animator *data, Vector2<int> *position, bool32 screenRelative) {}

void FunctionTable::drawTile(ushort *tileInfo, int countX, int countY, GameEntity *entityPtr,
                             Vector2<int> *position, bool32 screenRelative)
{
}

void FunctionTable::drawText(Animator *data, Vector2<int> *position, TextInfo *info, int endFrame,
                             int textLength, byte align, int spacing, int a8,
                             Vector2<int> *charPositions, bool32 screenRelative) {}