#ifndef GAMEDRAW_H
#define GAMEDRAW_H

#define v5_SPRFILE_COUNT     (0x400)
#define v5_FRAMEHITBOX_COUNT (0x8)

#define v5_SURFACE_MAX     (0x40)
#define v5_DRAWGROUP_COUNT (16)

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
    QColor transClr;
};

struct DrawList {
    QList<ushort> entries;
    QList<ushort> layerDrawList;
    void (*callback)(void);
    bool32 sorted;
};

extern bool32 validDraw;

namespace FunctionTable
{
short LoadSpriteAnimation(const char *filename, Scopes scope);
short CreateSpriteAnimation(const char *filename, uint frameCount, uint animCount, Scopes scope);
ushort FindSpriteAnimation(ushort sprIndex, const char *name);
SpriteFrame *GetFrame(ushort sprIndex, ushort anim, int frame);
Hitbox *GetHitbox(Animator *data, byte hitboxID);
short GetFrameID(Animator *data);
void ProcessAnimation(Animator *data);
void SetSpriteAnimation(ushort spriteIndex, ushort animationID, Animator *data, bool32 forceApply,
                        short frameID);
void EditSpriteAnimation(ushort spriteIndex, ushort animID, const char *name, int frameOffset,
                         ushort frameCount, short animSpeed, byte loopIndex, byte rotationFlag);
int GetStringWidth(ushort sprIndex, ushort animID, TextInfo *info, int startIndex, int length,
                   int spacing);
void SetSpriteString(ushort spriteIndex, ushort animID, TextInfo *info);

ushort LoadSpriteSheet(const char *filename, int scope);

void DrawLine(int x1, int y1, int x2, int y2, uint color, int alpha, InkEffects inkEffect,
              bool32 screenRelative);
void DrawRect(int x, int y, int width, int height, uint color, int alpha, InkEffects inkEffect,
              bool32 screenRelative);

void DrawSprite(Animator *data, Vector2<int> *position, bool32 screenRelative);

int CheckSceneFolder(const char *folder);
} // namespace FunctionTable

#endif
