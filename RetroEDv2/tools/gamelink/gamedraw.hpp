#ifndef GAMEDRAW_HPP
#define GAMEDRAW_HPP

#define v5_SPRFILE_COUNT     (0x400)
#define v5_FRAMEHITBOX_COUNT (0x8)

#define v5_SURFACE_MAX (0x40)

struct SpriteAnimationEntry {
    uint hash[4];
    int frameListOffset;
    ushort frameCount;
    short animationSpeed;
    byte loopIndex;
    byte rotationFlag;
};

struct SpriteAnimation {
    uint hash[4];
    SpriteFrame *frames;
    SpriteAnimationEntry *animations;
    ushort animCount;
    byte scope;
};

struct GFXSurface {
    uint hash[4];
    int height;
    int width;
    byte scope;
    QString name;
    QOpenGLTexture *texturePtr;
};

extern bool32 validDraw;

namespace FunctionTable
{
short loadSpriteAnimation(const char *filename, Scopes scope);
short createSpriteAnimation(const char *filename, uint frameCount, uint animCount, Scopes scope);
ushort getSpriteAnimation(ushort sprIndex, const char *name);
SpriteFrame *getFrame(ushort sprIndex, ushort anim, int frame);
Hitbox *getHitbox(Animator *data, byte hitboxID);
short getFrameID(Animator *data);
void processAnimation(Animator *data);
void setSpriteAnimation(ushort spriteIndex, ushort animationID, Animator *data, bool32 forceApply,
                        short frameID);
void editSpriteAnimation(ushort spriteIndex, ushort animID, const char *name, int frameOffset,
                         ushort frameCount, short animSpeed, byte loopIndex, byte rotationFlag);
int getStringWidth(ushort sprIndex, ushort animID, TextInfo *info, int startIndex, int length,
                   int spacing);
void setSpriteString(ushort spriteIndex, ushort animID, TextInfo *info);

ushort loadSpriteSheet(const char *filename, int scope);

void drawLine(int x1, int y1, int x2, int y2, uint color, int alpha, InkEffects inkEffect,
              bool32 screenRelative);
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

int checkStageFolder(const char *folder);
} // namespace FunctionTable

#endif
