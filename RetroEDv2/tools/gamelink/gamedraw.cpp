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

    QString buffer = WorkingDirManager::GetPath(QString("Sprites/") + filename,
                                                v5Editor->dataPath + "/Sprites/" + filename);

    uint hash[4];
    Utils::getHashInt(filename, hash);

    for (int i = 0; i < v5_SPRFILE_COUNT; ++i) {
        SpriteAnimation *list = &v5Editor->viewer->spriteAnimationList[i];
        if (list->scope != SCOPE_NONE && memcmp(list->hash, hash, 0x10 * sizeof(byte)) == 0) {
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
        allocateStorage(v5Editor->dataStorage, frameCount * sizeof(SpriteFrame), (void **)&spr->frames,
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
        allocateStorage(v5Editor->dataStorage, spr->animCount * sizeof(SpriteAnimationEntry),
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

    allocateStorage(v5Editor->dataStorage,
                    sizeof(SpriteFrame) * (frameCount > 0x400 ? 0x400 : frameCount),
                    (void **)&spr->frames, DATASET_STG, true);
    allocateStorage(v5Editor->dataStorage,
                    sizeof(SpriteAnimationEntry) * (animCount > 0x40 ? 0x40 : animCount),
                    (void **)&spr->animations, DATASET_STG, true);

    return id;
}

ushort FunctionTable::getSpriteAnimation(ushort aniFrames, const char *name)
{
    if (!v5Editor)
        return -1;

    if (aniFrames >= v5_SPRFILE_COUNT)
        return NULL;
    SpriteAnimation *spr = &v5Editor->viewer->spriteAnimationList[aniFrames];

    uint hash[4];
    Utils::getHashInt(name, hash);

    for (int a = 0; a < spr->animCount; ++a) {
        if (memcmp(v5Editor->viewer->spriteAnimationList[a].hash, hash, 0x10 * sizeof(byte)) == 0) {
            return a;
        }
    }
    return -1;
}

SpriteFrame *FunctionTable::getFrame(ushort aniFrames, ushort anim, int frame)
{
    if (aniFrames >= v5_SPRFILE_COUNT)
        return NULL;
    SpriteAnimation *spr = &v5Editor->viewer->spriteAnimationList[aniFrames];
    if (anim >= spr->animCount)
        return NULL;
    return &spr->frames[frame + spr->animations[anim].frameListOffset];
}

Hitbox *FunctionTable::getHitbox(Animator *animator, byte hitboxID)
{
    if (!v5Editor)
        return NULL;

    if (animator && animator->framePtrs)
        return &animator->framePtrs[animator->frameID].hitboxes[hitboxID & (v5_FRAMEHITBOX_COUNT - 1)];
    else
        return NULL;
}

short FunctionTable::getFrameID(Animator *animator)
{
    if (animator && animator->framePtrs)
        return animator->framePtrs[animator->frameID].id;
    else
        return 0;
}

void FunctionTable::processAnimation(Animator *animator)
{
    if (!v5Editor)
        return;

    if (animator) {
        if (animator->framePtrs) {
            animator->animationTimer += animator->animationSpeed;
            if (animator->framePtrs == (SpriteFrame *)1) {
                int delay = animator->frameDelay;
                while (animator->animationTimer < delay) {
                    ++animator->frameID;
                    animator->animationTimer = animator->animationTimer - delay;
                    if (animator->frameID >= animator->frameCount)
                        animator->frameID = animator->loopIndex;
                }
            }
            else {
                while (animator->animationTimer > animator->frameDelay) {
                    ++animator->frameID;
                    animator->animationTimer = animator->animationTimer - animator->frameDelay;
                    if (animator->frameID >= animator->frameCount)
                        animator->frameID = animator->loopIndex;
                    animator->frameDelay = animator->framePtrs[animator->frameID].delay;
                }
            }
        }
    }
}

void FunctionTable::setSpriteAnimation(ushort aniFrames, ushort animationID, Animator *animator,
                                       bool32 forceApply, short frameID)
{
    if (!v5Editor)
        return;

    if (aniFrames >= v5_SPRFILE_COUNT) {
        if (animator)
            animator->framePtrs = NULL;
        return;
    }
    if (!animator)
        return;
    SpriteAnimation *spr = &v5Editor->viewer->spriteAnimationList[aniFrames];
    if (animationID >= spr->animCount)
        return;

    SpriteAnimationEntry *anim = &spr->animations[animationID];
    SpriteFrame *frames        = &spr->frames[anim->frameListOffset];
    if (animator->framePtrs == frames && !forceApply)
        return;

    animator->framePtrs       = frames;
    animator->animationTimer  = 0;
    animator->frameID         = frameID;
    animator->frameCount      = anim->frameCount;
    animator->frameDelay      = animator->framePtrs[frameID].delay;
    animator->animationSpeed  = anim->animationSpeed;
    animator->rotationFlag    = anim->rotationFlag;
    animator->loopIndex       = anim->loopIndex;
    animator->prevAnimationID = animator->animationID;
    animator->animationID     = animationID;
}

void FunctionTable::editSpriteAnimation(ushort aniFrames, ushort animID, const char *name,
                                        int frameOffset, ushort frameCount, short speed, byte loopIndex,
                                        byte rotationFlag)
{
    if (!v5Editor)
        return;

    if (aniFrames < v5_SPRFILE_COUNT) {
        SpriteAnimation *spr = &v5Editor->viewer->spriteAnimationList[aniFrames];
        if (animID < spr->animCount) {
            SpriteAnimationEntry *anim = &spr->animations[animID];
            Utils::getHashInt(name, anim->hash);
            anim->frameListOffset = frameOffset;
            anim->frameCount      = frameCount;
            anim->animationSpeed  = speed;
            anim->loopIndex       = loopIndex;
            anim->rotationFlag    = rotationFlag;
        }
    }
}

int FunctionTable::getStringWidth(ushort aniFrames, ushort animID, TextInfo *info, int startIndex,
                                  int length, int spacing)
{
    if (!v5Editor)
        return 0;

    if (aniFrames >= v5_SPRFILE_COUNT)
        return 0;
    if (!info)
        return 0;
    if (!info->text)
        return 0;

    SpriteAnimation *spr = &v5Editor->viewer->spriteAnimationList[aniFrames];
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

void FunctionTable::setSpriteString(ushort aniFrames, ushort animID, TextInfo *info)
{
    if (!v5Editor)
        return;

    if (aniFrames >= v5_SPRFILE_COUNT)
        return;
    if (!info)
        return;

    SpriteAnimation *spr = &v5Editor->viewer->spriteAnimationList[aniFrames];
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

    QString buffer = WorkingDirManager::GetPath(QString("Sprites/") + filename,
                                                v5Editor->dataPath + "/Sprites/" + filename);

    uint hash[4];
    Utils::getHashInt(buffer, hash);

    for (int i = 0; i < v5_SURFACE_MAX; ++i) {
        if (v5Editor->viewer->gfxSurface[i].scope != SCOPE_NONE
            && memcmp(v5Editor->viewer->gfxSurface[i].hash, hash, 0x10 * sizeof(byte)) == 0) {
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

void FunctionTable::setClipBounds(byte screenID, int x1, int y1, int x2, int y2)
{
    ScreenInfo *screen;

    if (screenID < 4) {
        screen = &v5Editor->viewer->screens[screenID];

        if (x1 <= screen->width)
            screen->clipBound_X1 = x1 >= 0 ? x1 : 0;
        else
            screen->clipBound_X1 = screen->width;

        if (y1 <= screen->height)
            screen->clipBound_Y1 = y1 >= 0 ? y1 : 0;
        else
            screen->clipBound_Y1 = screen->height;

        if (x2 >= 0)
            screen->clipBound_X2 = x2 < screen->width ? x2 : screen->width;
        else
            screen->clipBound_X2 = 0;

        if (y2 >= 0)
            screen->clipBound_Y2 = y2 < screen->height ? y2 : screen->height;
        else
            screen->clipBound_Y2 = 0;
    }
}

void FunctionTable::drawLine(int x1, int y1, int x2, int y2, uint color, int alpha,
                             InkEffects inkEffect, bool32 screenRelative)
{
    if (!v5Editor)
        return;

    switch (inkEffect) {
        case INK_NONE: alpha = 0xFF; break;
        case INK_BLEND: alpha = 0x80; break;
        case INK_ALPHA:
            if (alpha > 0xFF)
                inkEffect = INK_NONE;
            else if (alpha <= 0)
                return;
            break;
        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF)
                alpha = 0xFF;
            else if (alpha <= 0)
                return;
            break;
        case INK_TINT:
            // if (!lookupTable)
            //    return;
            break;
        case INK_MASKED: alpha = 0xFF; break;
        case INK_UNMASKED: alpha = 0xFF; break;
    }

    Vector4<float> rcolor = { ((color >> 16) & 0xFF) / 255.0f, ((color >> 8) & 0xFF) / 255.0f,
                              (color & 0xFF) / 255.0f, alpha / 255.0f };

    float x1f = x1, y1f = y1, x2f = x2, y2f = y2;
    if (!screenRelative) {
        x1f = (x1 >> 16) - v5Editor->viewer->cameraPos.x;
        y1f = (y1 >> 16) - v5Editor->viewer->cameraPos.y;
        x2f = (x2 >> 16) - v5Editor->viewer->cameraPos.x;
        y2f = (y2 >> 16) - v5Editor->viewer->cameraPos.y;
    }

    float entX      = v5Editor->viewer->activeDrawEntity->pos.x - v5Editor->viewer->cameraPos.x;
    float entY      = v5Editor->viewer->activeDrawEntity->pos.y - v5Editor->viewer->cameraPos.y;
    float boxLeft   = x1f < x2f ? x1f : x2f;
    float boxTop    = y1f < y2f ? y1f : y2f;
    float boxRight  = x1f > x2f ? x1f : x2f;
    float boxBottom = y1f > y2f ? y1f : y2f;

    if (!v5Editor->viewer->sceneInfo.debugMode) {
        if (boxLeft < entX + v5Editor->viewer->activeDrawEntity->box.x) {
            v5Editor->viewer->activeDrawEntity->box.x = boxLeft - entX;
        }
        if (boxTop < entY + v5Editor->viewer->activeDrawEntity->box.y) {
            v5Editor->viewer->activeDrawEntity->box.y = boxTop - entY;
        }
        if (boxRight > entX + v5Editor->viewer->activeDrawEntity->box.w) {
            v5Editor->viewer->activeDrawEntity->box.w = boxRight - entX;
        }
        if (boxBottom > entY + v5Editor->viewer->activeDrawEntity->box.h) {
            v5Editor->viewer->activeDrawEntity->box.h = boxBottom - entY;
        }
    }

    float x1p = x1, x2p = x2, y1p = y1, y2p = y2;
    if (!screenRelative) {
        x1p = x1 / (float)(1 << 16) - v5Editor->viewer->cameraPos.x;
        x2p = x2 / (float)(1 << 16) - v5Editor->viewer->cameraPos.x;
        y1p = y1 / (float)(1 << 16) - v5Editor->viewer->cameraPos.y;
        y2p = y2 / (float)(1 << 16) - v5Editor->viewer->cameraPos.y;
    }

    v5Editor->viewer->drawLine(x1p, y1p, x2p, y2p, rcolor, alpha, inkEffect);
}

void FunctionTable::drawRect(int x, int y, int width, int height, uint color, int alpha,
                             InkEffects inkEffect, bool32 screenRelative)
{
    if (!v5Editor)
        return;

    switch (inkEffect) {
        case INK_NONE: alpha = 0xFF; break;
        case INK_BLEND: alpha = 0x80; break;
        case INK_ALPHA:
            if (alpha > 0xFF)
                inkEffect = INK_NONE;
            else if (alpha <= 0)
                return;
            break;
        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF)
                alpha = 0xFF;
            else if (alpha <= 0)
                return;
            break;
        case INK_TINT:
            alpha = 0xFF;
            // if (!lookupTable)
            //    return;
            break;
        case INK_MASKED: alpha = 0xFF; break;
        case INK_UNMASKED: alpha = 0xFF; break;
    }

    Vector4<float> rcolor = { ((color >> 16) & 0xFF) / 255.0f, ((color >> 8) & 0xFF) / 255.0f,
                              (color & 0xFF) / 255.0f, alpha / 255.0f };

    float xf      = x;
    float yf      = y;
    float widthf  = width;
    float heightf = height;

    if (!screenRelative) {
        xf = (x / (float)(1 << 16)) - v5Editor->viewer->cameraPos.x;
        yf = (y / (float)(1 << 16)) - v5Editor->viewer->cameraPos.y;
        widthf /= (float)(1 << 16);
        heightf /= (float)(1 << 16);
    }

    float startX = xf;
    float startY = yf;

    if (widthf + xf > v5Editor->viewer->screens->clipBound_X2)
        widthf = v5Editor->viewer->screens->clipBound_X2 - xf;
    if (xf < v5Editor->viewer->screens->clipBound_X1) {
        widthf += xf - v5Editor->viewer->screens->clipBound_X1;
        xf = v5Editor->viewer->screens->clipBound_X1;
    }

    if (heightf + yf > v5Editor->viewer->screens->clipBound_Y2)
        heightf = v5Editor->viewer->screens->clipBound_Y2 - yf;
    if (yf < v5Editor->viewer->screens->clipBound_Y1) {
        heightf += yf - v5Editor->viewer->screens->clipBound_Y1;
        yf = v5Editor->viewer->screens->clipBound_Y1;
    }

    if (widthf <= 0 || heightf <= 0)
        return;

    float entX      = v5Editor->viewer->activeDrawEntity->pos.x - v5Editor->viewer->cameraPos.x;
    float entY      = v5Editor->viewer->activeDrawEntity->pos.y - v5Editor->viewer->cameraPos.y;
    float boxLeft   = startX;
    float boxTop    = startY;
    float boxRight  = startX + widthf;
    float boxBottom = startY + heightf;

    if (!v5Editor->viewer->sceneInfo.debugMode) {
        if (boxLeft < entX + v5Editor->viewer->activeDrawEntity->box.x) {
            v5Editor->viewer->activeDrawEntity->box.x = boxLeft - entX;
        }
        if (boxTop < entY + v5Editor->viewer->activeDrawEntity->box.y) {
            v5Editor->viewer->activeDrawEntity->box.y = boxTop - entY;
        }
        if (boxRight > entX + v5Editor->viewer->activeDrawEntity->box.w) {
            v5Editor->viewer->activeDrawEntity->box.w = boxRight - entX;
        }
        if (boxBottom > entY + v5Editor->viewer->activeDrawEntity->box.h) {
            v5Editor->viewer->activeDrawEntity->box.h = boxBottom - entY;
        }
    }

    v5Editor->viewer->drawRect(xf, yf, widthf, heightf, rcolor, false, alpha, inkEffect);
}

void FunctionTable::drawCircle(int x, int y, int radius, uint color, int alpha, InkEffects inkEffect,
                               bool32 screenRelative)
{
    if (!v5Editor)
        return;

    float xf = x;
    float yf = y;

    if (!screenRelative) {
        xf = (x / (float)(1 << 16)) - v5Editor->viewer->cameraPos.x;
        yf = (y / (float)(1 << 16)) - v5Editor->viewer->cameraPos.y;
    }
    Vector4<float> rcolor = { ((color >> 16) & 0xFF) / 255.0f, ((color >> 8) & 0xFF) / 255.0f,
                              (color & 0xFF) / 255.0f, alpha / 255.0f };
    // v5Editor->viewer->drawCircle(xf, yf, 0, radius, rcolor, alpha, inkEffect);
}
void FunctionTable::drawCircleOutline(int x, int y, int innerRadius, int outerRadius, uint color,
                                      int alpha, InkEffects inkEffect, bool32 screenRelative)
{
    if (!v5Editor)
        return;

    float xf = x;
    float yf = y;

    if (!screenRelative) {
        xf = (x / (float)(1 << 16)) - v5Editor->viewer->cameraPos.x;
        yf = (y / (float)(1 << 16)) - v5Editor->viewer->cameraPos.y;
    }
    Vector4<float> rcolor = { ((color >> 16) & 0xFF) / 255.0f, ((color >> 8) & 0xFF) / 255.0f,
                              (color & 0xFF) / 255.0f, alpha / 255.0f };
    // v5Editor->viewer->drawCircle(xf, yf, innerRadius, outerRadius, rcolor, alpha, inkEffect);
}

void FunctionTable::drawFace(Vector2<int> *vertices, int vertCount, int r, int g, int b, int alpha,
                             InkEffects inkEffect)
{
    if (!v5Editor)
        return;
    v5Editor->viewer->drawFace(vertices, vertCount, r, g, b, alpha, inkEffect);
}
void FunctionTable::drawBlendedFace(Vector2<int> *vertices, uint *colors, int vertCount, int alpha,
                                    InkEffects inkEffect)
{
    if (!v5Editor)
        return;
    v5Editor->viewer->drawBlendedFace(vertices, colors, vertCount, alpha, inkEffect);
}

void FunctionTable::drawSprite(Animator *animator, Vector2<int> *position, bool32 screenRelative)
{
    if (animator && animator->framePtrs && v5Editor) {
        if ((uint)animator->frameID >= (uint)animator->frameCount)
            return;

        SpriteFrame *frame = &animator->framePtrs[animator->frameID];
        Vector2<float> pos;
        if (!position) {
            pos.x = Utils::fixedToFloat(v5Editor->viewer->sceneInfo.entity->position.x);
            pos.y = Utils::fixedToFloat(v5Editor->viewer->sceneInfo.entity->position.y);
        }
        else {
            pos.x = Utils::fixedToFloat(position->x);
            pos.y = Utils::fixedToFloat(position->y);
        }

        if (!screenRelative) {
            pos.x -= v5Editor->viewer->cameraPos.x;
            pos.y -= v5Editor->viewer->cameraPos.y;
        }

        SceneInfo &sceneInfo = v5Editor->viewer->sceneInfo;

        int rotation = sceneInfo.entity->rotation;
        int drawFX   = sceneInfo.entity->drawFX;
        if (sceneInfo.entity->drawFX & FX_ROTATE) {
            switch (animator->rotationFlag) {
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
                            frame += animator->frameCount;
                            if (sceneInfo.entity->direction)
                                rotation = 0x00;
                            break;
                        case 2: // 90 deg
                            rotation = 0x80;
                            break;
                        case 3: // 135 deg
                            rotation = 0x100;
                            frame += animator->frameCount;
                            if (sceneInfo.entity->direction)
                                rotation = 0x80;
                            break;
                        case 4: // 180 deg
                            rotation = 0x100;
                            break;
                        case 5: // 225 deg
                            rotation = 0x180;
                            frame += animator->frameCount;
                            if (sceneInfo.entity->direction)
                                rotation = 0x100;
                            break;
                        case 6: // 270 deg
                            rotation = 0x180;
                            break;
                        case 7: // 315 deg
                            rotation = 0x180;
                            frame += animator->frameCount;
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
    if (!v5Editor)
        return;
}

void FunctionTable::drawText(Animator *animator, Vector2<int> *position, TextInfo *info, int startFrame,
                             int endFrame, byte align, int spacing, int a8, Vector2<int> *charOffsets,
                             bool32 screenRelative)
{
    if (animator && info && animator->framePtrs && v5Editor) {
        SceneInfo &sceneInfo = v5Editor->viewer->sceneInfo;

        if (!position)
            position = &sceneInfo.entity->position;
        GameEntity *entity = sceneInfo.entity;

        float x = 0;
        float y = 0;

        if (!position) {
            x = Utils::fixedToFloat(sceneInfo.entity->position.x);
            y = Utils::fixedToFloat(sceneInfo.entity->position.y);
        }
        else {
            x = Utils::fixedToFloat(position->x);
            y = Utils::fixedToFloat(position->y);
        }

        if (!screenRelative) {
            x -= v5Editor->viewer->cameraPos.x;
            y -= v5Editor->viewer->cameraPos.y;
        }

        if (startFrame >= 0) {
            if (startFrame >= info->textLength)
                startFrame = info->textLength - 1;
        }
        else {
            startFrame = 0;
        }

        if (endFrame > 0) {
            if (endFrame > info->textLength)
                endFrame = info->textLength;
        }
        else {
            endFrame = info->textLength;
        }

        switch (align) {
            case 0:
                if (charOffsets) {
                    for (; startFrame < endFrame; ++startFrame) {
                        ushort curChar = info->text[startFrame];
                        if (curChar < animator->frameCount) {
                            SpriteFrame *frame = &animator->framePtrs[curChar];
                            v5Editor->viewer->drawSpriteFlipped(
                                x + (charOffsets->x >> 0x10),
                                y + frame->pivotY + (charOffsets->y >> 0x10), frame->width,
                                frame->height, frame->sprX, frame->sprY, FLIP_NONE,
                                (InkEffects)entity->inkEffect, entity->alpha, frame->sheetID);
                            x += spacing + frame->width;
                            ++charOffsets;
                        }
                    }
                }
                else {
                    for (; startFrame < endFrame; ++startFrame) {
                        ushort curChar = info->text[startFrame];
                        if (curChar < animator->frameCount) {
                            SpriteFrame *frame = &animator->framePtrs[curChar];
                            v5Editor->viewer->drawSpriteFlipped(
                                x, y + frame->pivotY, frame->width, frame->height, frame->sprX,
                                frame->sprY, FLIP_NONE, (InkEffects)entity->inkEffect, entity->alpha,
                                frame->sheetID);
                            x += spacing + frame->width;
                        }
                    }
                }
                break;
            case 1:
            case 2:
                --endFrame;
                if (charOffsets) {
                    for (Vector2<int> *charOffset = &charOffsets[endFrame]; endFrame >= startFrame;
                         --endFrame) {
                        ushort curChar = info->text[endFrame];
                        if (curChar < animator->frameCount) {
                            SpriteFrame *frame = &animator->framePtrs[curChar];
                            v5Editor->viewer->drawSpriteFlipped(
                                x - frame->width + (charOffset->x >> 0x10),
                                y + frame->pivotY + (charOffset->y >> 0x10), frame->width,
                                frame->height, frame->sprX, frame->sprY, FLIP_NONE,
                                (InkEffects)entity->inkEffect, entity->alpha, frame->sheetID);
                            x = (x - frame->width) - spacing;
                            --charOffset;
                        }
                    }
                }
                else {
                    for (; endFrame >= startFrame; --endFrame) {
                        ushort curChar = info->text[endFrame];
                        if (curChar < animator->frameCount) {
                            SpriteFrame *frame = &animator->framePtrs[curChar];
                            v5Editor->viewer->drawSpriteFlipped(
                                x - frame->width, y + frame->pivotY, frame->width, frame->height,
                                frame->sprX, frame->sprY, FLIP_NONE, (InkEffects)entity->inkEffect,
                                entity->alpha, frame->sheetID);
                            x = (x - frame->width) - spacing;
                        }
                    }
                }
                break;
        }
    }
}

int FunctionTable::checkStageFolder(const char *folder)
{
    if (!v5Editor)
        return 0;
    return Utils::getFilenameAndFolder(v5Editor->scene.filepath).split("/")[0] == folder;
}
