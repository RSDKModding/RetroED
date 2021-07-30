#include "includes.hpp"

bool32 validDraw = false;

enum RotationFlags {
    ROTFLAG_NONE,
    ROTFLAG_FULL,
    ROTFLAG_45DEG,
    ROTFLAG_90DEG,
    ROTFLAG_180DEG,
    ROTFLAG_STATICFRAMES
};

short FunctionTable::loadSpriteAnimation(const char *filename, Scopes scope)
{
    if (!v5Editor)
        return -1;

    char buffer[0x100];
    sprintf(buffer, "%s/Sprites/%s", v5Editor->dataPath.toStdString().c_str(), filename);

    uint hash[4];
    Utils::getHashInt(filename, hash);

    for (int i = 0; i < v5_SPRFILE_COUNT; ++i) {
        if (memcmp(v5Editor->viewer->spriteAnimationList[i].hash, hash, 0x10 * sizeof(byte)) == 0) {
            return i;
        }
    }

    ushort id = -1;
    for (id = 0; id < v5_SPRFILE_COUNT; ++id) {
        if (v5Editor->viewer->spriteAnimationList[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= v5_SPRFILE_COUNT)
        return -1;

    byte sheetIDs[0x18];
    sheetIDs[0] = 0;

    Reader reader = Reader(buffer);
    if (reader.initialised) {
        uint sig = reader.read<uint>();

        if (sig != 0x525053) {
            return -1;
        }

        SpriteAnimation *spr = &v5Editor->viewer->spriteAnimationList[id];
        spr->scope           = scope;
        memcpy(spr->hash, hash, 4 * sizeof(uint));

        uint frameCount = reader.read<uint>();
        AllocateStorage(v5Editor->dataStorage, frameCount * sizeof(SpriteFrame), (void **)&spr->frames,
                        DATASET_STG, false);

        byte sheetCount = reader.read<byte>();
        for (int s = 0; s < sheetCount; ++s) {
            QString sheet = reader.readString();
            sheetIDs[s]   = loadSpriteSheet(sheet.toStdString().c_str(), scope);
        }

        byte hitboxCount = reader.read<byte>();
        for (int h = 0; h < hitboxCount; ++h) {
            reader.readString();
        }

        spr->animCount = reader.read<ushort>();
        AllocateStorage(v5Editor->dataStorage, spr->animCount * sizeof(SpriteAnimationEntry),
                        (void **)&spr->animations, DATASET_STG, false);
        int frameID = 0;
        for (int a = 0; a < spr->animCount; ++a) {
            SpriteAnimationEntry *animation = &spr->animations[a];
            Utils::getHashInt(reader.readString(), animation->hash);

            animation->frameCount      = reader.read<ushort>();
            animation->frameListOffset = frameID;
            animation->animationSpeed  = reader.read<ushort>();
            animation->loopIndex       = reader.read<byte>();
            animation->rotationFlag    = reader.read<byte>();

            for (int f = 0; f < animation->frameCount; ++f) {
                SpriteFrame *frame = &spr->frames[frameID++];
                frame->sheetID     = sheetIDs[reader.read<byte>()];
                frame->delay       = reader.read<ushort>();
                frame->id          = reader.read<ushort>();
                frame->sprX        = reader.read<ushort>();
                frame->sprY        = reader.read<ushort>();
                frame->width       = reader.read<ushort>();
                frame->height      = reader.read<ushort>();
                frame->pivotX      = reader.read<short>();
                frame->pivotY      = reader.read<short>();

                frame->hitboxCnt = hitboxCount;
                for (int h = 0; h < hitboxCount; ++h) {
                    frame->hitboxes[h].left   = reader.read<short>();
                    frame->hitboxes[h].top    = reader.read<short>();
                    frame->hitboxes[h].right  = reader.read<short>();
                    frame->hitboxes[h].bottom = reader.read<short>();
                }
            }
        }

        return id;
    }
    return -1;
}

short FunctionTable::createSpriteAnimation(const char *filename, uint frameCount, uint animCount,
                                           Scopes scope)
{
    if (!v5Editor)
        return -1;

    char buffer[0x100];
    sprintf(buffer, "%s/Sprites/%s", v5Editor->dataPath.toStdString().c_str(), filename);

    uint hash[4];
    Utils::getHashInt(filename, hash);

    for (int i = 0; i < v5_SPRFILE_COUNT; ++i) {
        if (memcmp(v5Editor->viewer->spriteAnimationList[i].hash, hash, 0x10 * sizeof(byte)) == 0) {
            return i;
        }
    }

    ushort id = -1;
    for (id = 0; id < v5_SPRFILE_COUNT; ++id) {
        if (v5Editor->viewer->spriteAnimationList[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= v5_SPRFILE_COUNT)
        return -1;

    SpriteAnimation *spr = &v5Editor->viewer->spriteAnimationList[id];
    spr->scope           = scope;
    memcpy(spr->hash, hash, 4 * sizeof(uint));

    AllocateStorage(v5Editor->dataStorage,
                    sizeof(SpriteFrame) * (frameCount > 0x400 ? 0x400 : frameCount),
                    (void **)&spr->frames, DATASET_STG, true);
    AllocateStorage(v5Editor->dataStorage,
                    sizeof(SpriteAnimationEntry) * (animCount > 0x40 ? 0x40 : animCount),
                    (void **)&spr->animations, DATASET_STG, true);

    return id;
}

ushort FunctionTable::GetSpriteAnimation(ushort sprIndex, const char *name)
{
    if (!v5Editor)
        return -1;

    if (sprIndex >= v5_SPRFILE_COUNT)
        return NULL;
    SpriteAnimation *spr = &v5Editor->viewer->spriteAnimationList[sprIndex];

    uint hash[4];
    Utils::getHashInt(name, hash);

    for (int a = 0; a < spr->animCount; ++a) {
        if (memcmp(v5Editor->viewer->spriteAnimationList[a].hash, hash, 0x10 * sizeof(byte)) == 0) {
            return a;
        }
    }
    return -1;
}

SpriteFrame *FunctionTable::getFrame(ushort sprIndex, ushort anim, int frame)
{
    if (sprIndex >= v5_SPRFILE_COUNT)
        return NULL;
    SpriteAnimation *spr = &v5Editor->viewer->spriteAnimationList[sprIndex];
    if (anim >= spr->animCount)
        return NULL;
    return &spr->frames[frame + spr->animations[anim].frameListOffset];
}

Hitbox *FunctionTable::getHitbox(Animator *data, byte hitboxID)
{
    if (!v5Editor)
        return NULL;

    if (data && data->framePtrs)
        return &data->framePtrs[data->frameID].hitboxes[hitboxID & (v5_FRAMEHITBOX_COUNT - 1)];
    else
        return NULL;
}

short FunctionTable::getFrameID(Animator *data)
{
    if (data && data->framePtrs)
        return data->framePtrs[data->frameID].id;
    else
        return 0;
}

void FunctionTable::processAnimation(Animator *data)
{
    if (!v5Editor)
        return;

    if (data) {
        if (data->framePtrs) {
            data->animationTimer += data->animationSpeed;
            if (data->framePtrs == (SpriteFrame *)1) {
                int delay = data->frameDelay;
                while (data->animationTimer < delay) {
                    ++data->frameID;
                    data->animationTimer = data->animationTimer - delay;
                    if (data->frameID >= data->frameCount)
                        data->frameID = data->loopIndex;
                }
            }
            else {
                while (data->animationTimer > data->frameDelay) {
                    ++data->frameID;
                    data->animationTimer = data->animationTimer - data->frameDelay;
                    if (data->frameID >= data->frameCount)
                        data->frameID = data->loopIndex;
                    data->frameDelay = data->framePtrs[data->frameID].delay;
                }
            }
        }
    }
}

void FunctionTable::setSpriteAnimation(ushort spriteIndex, ushort animationID, Animator *data,
                                       bool32 forceApply, short frameID)
{
    if (!v5Editor)
        return;

    if (spriteIndex >= v5_SPRFILE_COUNT) {
        if (data)
            data->framePtrs = NULL;
        return;
    }
    if (!data)
        return;
    SpriteAnimation *spr = &v5Editor->viewer->spriteAnimationList[spriteIndex];
    if (animationID >= spr->animCount)
        return;

    SpriteAnimationEntry *anim = &spr->animations[animationID];
    SpriteFrame *frames        = &spr->frames[anim->frameListOffset];
    if (data->framePtrs == frames && !forceApply)
        return;

    data->framePtrs       = frames;
    data->animationTimer  = 0;
    data->frameID         = frameID;
    data->frameCount      = anim->frameCount;
    data->frameDelay      = data->framePtrs[frameID].delay;
    data->animationSpeed  = anim->animationSpeed;
    data->rotationFlag    = anim->rotationFlag;
    data->loopIndex       = anim->loopIndex;
    data->prevAnimationID = data->animationID;
    data->animationID     = animationID;
}

void FunctionTable::editSpriteAnimation(ushort spriteIndex, ushort animID, const char *name,
                                        int frameOffset, ushort frameCount, short animSpeed,
                                        byte loopIndex, byte rotationFlag)
{
    if (!v5Editor)
        return;

    if (spriteIndex < v5_SPRFILE_COUNT) {
        SpriteAnimation *spr = &v5Editor->viewer->spriteAnimationList[spriteIndex];
        if (animID < spr->animCount) {
            SpriteAnimationEntry *anim = &spr->animations[animID];
            Utils::getHashInt(name, anim->hash);
            anim->frameListOffset = frameOffset;
            anim->frameCount      = frameCount;
            anim->animationSpeed  = animSpeed;
            anim->loopIndex       = loopIndex;
            anim->rotationFlag    = rotationFlag;
        }
    }
}

int FunctionTable::getStringWidth(ushort sprIndex, ushort animID, TextInfo *info, int startIndex,
                                  int length, int spacing)
{
    if (!v5Editor)
        return 0;

    if (sprIndex >= v5_SPRFILE_COUNT)
        return 0;
    if (!info)
        return 0;
    if (!info->text)
        return 0;

    SpriteAnimation *spr = &v5Editor->viewer->spriteAnimationList[sprIndex];
    if (animID < spr->animCount) {
        SpriteAnimationEntry *anim = &spr->animations[animID];

        if (startIndex >= 0) {
            if (startIndex >= info->textLength)
                startIndex = info->textLength - 1;
        }
        else {
            startIndex = 0;
        }

        if (length > 0) {
            if (length > info->textLength)
                length = info->textLength;
        }
        else {
            length = info->textLength;
        }

        int w = 0;
        for (int c = startIndex; c < length; ++c) {
            int charFrame = info->text[c];
            if (charFrame < anim->frameCount) {
                w += spr->frames[charFrame + anim->frameListOffset].width;
                if (c + 1 >= length)
                    return w;
                w += spacing;
            }
        }
        return w;
    }
    return 0;
}

void FunctionTable::setSpriteString(ushort spriteIndex, ushort animID, TextInfo *info)
{
    if (!v5Editor)
        return;

    if (spriteIndex >= v5_SPRFILE_COUNT)
        return;
    if (!info)
        return;

    SpriteAnimation *spr = &v5Editor->viewer->spriteAnimationList[spriteIndex];
    if (animID < spr->animCount) {
        SpriteAnimationEntry *anim = &spr->animations[animID];

        for (int c = 0; c < info->textLength; ++c) {
            int charVal   = info->text[c];
            info->text[c] = -1;
            for (int f = 0; f < anim->frameCount; ++f) {
                if (spr->frames[f + anim->frameListOffset].id == charVal) {
                    info->text[c] = f;
                    break;
                }
            }
        }
    }
}

ushort FunctionTable::loadSpriteSheet(const char *filename, int scope)
{
    if (!v5Editor)
        return -1;

    char buffer[0x100];
    sprintf(buffer, "%s/Sprites/%s", v5Editor->dataPath.toStdString().c_str(), filename);

    uint hash[4];
    Utils::getHashInt(filename, hash);

    for (int i = 0; i < v5_SURFACE_MAX; ++i) {
        if (memcmp(v5Editor->viewer->gfxSurface[i].hash, hash, 0x10 * sizeof(byte)) == 0) {
            return i;
        }
    }

    ushort id = -1;
    for (id = 0; id < v5_SURFACE_MAX; ++id) {
        if (v5Editor->viewer->gfxSurface[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= v5_SURFACE_MAX)
        return -1;

    return v5Editor->viewer->addGraphicsFile(buffer, id, scope);
}

void FunctionTable::drawLine(int x1, int y1, int x2, int y2, uint color, int alpha,
                             InkEffects inkEffect, bool32 screenRelative)
{
    if (!v5Editor)
        return;
    float z               = v5Editor->viewer->currZ;
    Vector4<float> rcolor = { ((color >> 16) & 0xFF) / 255.0f, ((color >> 8) & 0xFF) / 255.0f,
                              (color & 0xFF) / 255.0f, alpha / 255.0f };

    if (!screenRelative) {
        x1 = (x1 >> 16) - v5Editor->viewer->cam.pos.x;
        y1 = (y1 >> 16) - v5Editor->viewer->cam.pos.y;
        x2 = (x2 >> 16) - v5Editor->viewer->cam.pos.x;
        y2 = (y2 >> 16) - v5Editor->viewer->cam.pos.y;
    }

    float zoom = v5Editor->viewer->zoom;
    v5Editor->viewer->drawLine(x1 * zoom, y1 * zoom, z, x2 * zoom, y2 * zoom, z, rcolor,
                               v5Editor->viewer->primitiveShader);
    v5Editor->viewer->incZ();
}

void FunctionTable::drawRect(int x, int y, int width, int height, uint color, int alpha,
                             InkEffects inkEffect, bool32 screenRelative)
{
    if (!v5Editor)
        return;
    Vector4<float> rcolor = { ((color >> 16) & 0xFF) / 255.0f, ((color >> 8) & 0xFF) / 255.0f,
                              (color & 0xFF) / 255.0f, alpha / 255.0f };

    if (!screenRelative) {
        x = (x >> 16) - v5Editor->viewer->cam.pos.x;
        y = (y >> 16) - v5Editor->viewer->cam.pos.y;
        width >>= 16;
        height >>= 16;
    }

    float zoom = v5Editor->viewer->zoom;
    v5Editor->viewer->drawRect(x * zoom, y * zoom, v5Editor->viewer->incZ(), width * zoom,
                               height * zoom, rcolor, v5Editor->viewer->primitiveShader);
}

void FunctionTable::drawCircle(int x, int y, int radius, uint color, int alpha, InkEffects inkEffect,
                               bool32 screenRelative)
{
    if (!v5Editor)
        return;
    Vector4<float> rcolor = { ((color >> 16) & 0xFF) / 255.0f, ((color >> 8) & 0xFF) / 255.0f,
                              (color & 0xFF) / 255.0f, alpha / 255.0f };
    float zoom            = v5Editor->viewer->zoom;
    v5Editor->viewer->drawCircle(x * zoom, y * zoom, v5Editor->viewer->incZ(), radius, rcolor,
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

void FunctionTable::drawSprite(Animator *data, Vector2<int> *position, bool32 screenRelative)
{
    if (data && data->framePtrs && v5Editor) {
        SpriteFrame *frame = &data->framePtrs[data->frameID];
        Vector2<int> pos;
        if (!position)
            pos = sceneInfo.entity->position;
        else
            pos = *position;

        pos.x >>= 0x10;
        pos.y >>= 0x10;
        if (!screenRelative) {
            pos.x -= v5Editor->viewer->cam.pos.x;
            pos.y -= v5Editor->viewer->cam.pos.y;
        }

        int rotation = sceneInfo.entity->rotation;
        int drawFX   = sceneInfo.entity->drawFX;
        if (sceneInfo.entity->drawFX & FX_ROTATE) {
            switch (data->rotationFlag) {
                case ROTFLAG_NONE:
                    rotation = 0;
                    if ((sceneInfo.entity->drawFX & FX_ROTATE) != FX_NONE)
                        drawFX ^= FX_ROTATE;
                    break;
                case ROTFLAG_FULL:
                    rotation = sceneInfo.entity->rotation & 0x1FF;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;
                case ROTFLAG_45DEG: // 0x00, 0x40, 0x80, 0xC0, 0x100, 0x140, 0x180, 0x1C0
                    rotation = (sceneInfo.entity->rotation + 0x20) & 0x1C0;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;
                case ROTFLAG_90DEG: // 0x00, 0x80, 0x100, 0x180
                    rotation = (sceneInfo.entity->rotation + 0x40) & 0x180;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;
                case ROTFLAG_180DEG: // 0x00, 0x100
                    rotation = (sceneInfo.entity->rotation + 0x80) & 0x100;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;
                case ROTFLAG_STATICFRAMES:
                    if (sceneInfo.entity->rotation >= 0x100) {
                        rotation = 0x08 - ((0x214 - sceneInfo.entity->rotation) >> 6);
                    }
                    else {
                        rotation = (sceneInfo.entity->rotation + 20) >> 6;
                    }

                    switch (rotation) {
                        case 0: // 0 deg
                        case 8: // 360 deg
                            rotation = 0x00;
                            if ((sceneInfo.entity->drawFX & FX_SCALE) != FX_NONE)
                                drawFX ^= FX_ROTATE;
                            break;
                        case 1: // 45 deg
                            rotation = 0x80;
                            frame += data->frameCount;
                            if (sceneInfo.entity->direction)
                                rotation = 0x00;
                            break;
                        case 2: // 90 deg
                            rotation = 0x80;
                            break;
                        case 3: // 135 deg
                            rotation = 0x100;
                            frame += data->frameCount;
                            if (sceneInfo.entity->direction)
                                rotation = 0x80;
                            break;
                        case 4: // 180 deg
                            rotation = 0x100;
                            break;
                        case 5: // 225 deg
                            rotation = 0x180;
                            frame += data->frameCount;
                            if (sceneInfo.entity->direction)
                                rotation = 0x100;
                            break;
                        case 6: // 270 deg
                            rotation = 0x180;
                            break;
                        case 7: // 315 deg
                            rotation = 0x180;
                            frame += data->frameCount;
                            if (!sceneInfo.entity->direction)
                                rotation = 0;
                            break;
                        default: break;
                    }
                    break;
                default: break;
            }
        }

        switch (drawFX) {
            case FX_NONE:
                v5Editor->viewer->drawSpriteFlipped(
                    pos.x + frame->pivotX, pos.y + frame->pivotY, frame->width, frame->height,
                    frame->sprX, frame->sprY, FLIP_NONE, (InkEffects)sceneInfo.entity->inkEffect,
                    sceneInfo.entity->alpha, frame->sheetID);
                break;
            case FX_FLIP:
                switch (sceneInfo.entity->direction) {
                    case FLIP_NONE:
                        v5Editor->viewer->drawSpriteFlipped(
                            pos.x + frame->pivotX, pos.y + frame->pivotY, frame->width, frame->height,
                            frame->sprX, frame->sprY, FLIP_NONE,
                            (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha,
                            frame->sheetID);
                        break;
                    case FLIP_X:
                        v5Editor->viewer->drawSpriteFlipped(
                            pos.x - frame->width - frame->pivotX, pos.y + frame->pivotY, frame->width,
                            frame->height, frame->sprX, frame->sprY, FLIP_X,
                            (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha,
                            frame->sheetID);
                        break;
                    case FLIP_Y:
                        v5Editor->viewer->drawSpriteFlipped(
                            pos.x + frame->pivotX, pos.y - frame->height - frame->pivotY, frame->width,
                            frame->height, frame->sprX, frame->sprY, FLIP_Y,
                            (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha,
                            frame->sheetID);
                        break;
                    case FLIP_XY:
                        v5Editor->viewer->drawSpriteFlipped(
                            pos.x - frame->width - frame->pivotX, pos.y - frame->height - frame->pivotY,
                            frame->width, frame->height, frame->sprX, frame->sprY, FLIP_XY,
                            (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha,
                            frame->sheetID);
                        break;
                    default: break;
                }
                break;
            case FX_ROTATE:
                v5Editor->viewer->drawSpriteRotozoom(
                    pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height,
                    frame->sprX, frame->sprY, 0x200, 0x200, FLIP_NONE, rotation,
                    (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                break;
            case FX_ROTATE | FX_FLIP:
                v5Editor->viewer->drawSpriteRotozoom(
                    pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height,
                    frame->sprX, frame->sprY, 0x200, 0x200,
                    FlipFlags(sceneInfo.entity->direction & FLIP_X), rotation,
                    (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                break;
            case FX_SCALE:
                v5Editor->viewer->drawSpriteRotozoom(
                    pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height,
                    frame->sprX, frame->sprY, sceneInfo.entity->scale.x, sceneInfo.entity->scale.y,
                    FLIP_NONE, 0, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha,
                    frame->sheetID);
                break;
            case FX_SCALE | FX_FLIP:
                v5Editor->viewer->drawSpriteRotozoom(
                    pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height,
                    frame->sprX, frame->sprY, sceneInfo.entity->scale.x, sceneInfo.entity->scale.y,
                    FlipFlags(sceneInfo.entity->direction & FLIP_X), 0,
                    (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                break;
            case FX_SCALE | FX_ROTATE:
                v5Editor->viewer->drawSpriteRotozoom(
                    pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height,
                    frame->sprX, frame->sprY, sceneInfo.entity->scale.x, sceneInfo.entity->scale.y,
                    FLIP_NONE, rotation, (InkEffects)sceneInfo.entity->inkEffect,
                    sceneInfo.entity->alpha, frame->sheetID);
                break;
            case FX_SCALE | FX_ROTATE | FX_FLIP:
                v5Editor->viewer->drawSpriteRotozoom(
                    pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height,
                    frame->sprX, frame->sprY, sceneInfo.entity->scale.x, sceneInfo.entity->scale.y,
                    FlipFlags(sceneInfo.entity->direction & FLIP_X), rotation,
                    (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                break;
            default: break;
        }
    }
}

void FunctionTable::drawTile(ushort *tileInfo, int countX, int countY, GameEntity *entityPtr,
                             Vector2<int> *position, bool32 screenRelative)
{
}

void FunctionTable::drawText(Animator *data, Vector2<int> *position, TextInfo *info, int endFrame,
                             int textLength, byte align, int spacing, int a8,
                             Vector2<int> *charPositions, bool32 screenRelative)
{
}