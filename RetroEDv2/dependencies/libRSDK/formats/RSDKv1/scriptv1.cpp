#include "libRSDK.hpp"

QList<QString> RSDKv1::Script::variableList = {
    "[NULL]",                   // 0x00
    "Object.Type",              // 0x01
    "Object.PropertyValue",     // 0x02
    "Object.XPos",              // 0x03
    "Object.YPos",              // 0x04
    "Object.ValueA",            // 0x05
    "Object.ValueB",            // 0x06
    "Object.ValueC",            // 0x07
    "Object.ValueD",            // 0x08
    "Object.ValueE",            // 0x09
    "Object.ValueF",            // 0x0A
    "TempObject.Type",          // 0x0B
    "TempObject.PropertyValue", // 0x0C
    "TempObject.XPos",          // 0x0D
    "TempObject.YPos",          // 0x0E
    "TempObject.ValueA",        // 0x0F
    "TempObject.ValueB",        // 0x10
    "TempObject.ValueC",        // 0x11
    "TempObject.ValueD",        // 0x12
    "TempObject.ValueE",        // 0x13
    "TempObject.ValueF",        // 0x14
    "Player.Direction",         // 0x15
    "Blank16",                  // 0x16
    "Player.State",             // 0x17
    "Blank18",                  // 0x18
    "Player.XPos",              // 0x19 //Sets "TempValC"
    "Player.YPos",              // 0x1A //Sets "TempValC"
    "Blank1B",                  // 0x1B
    "Blank1C",                  // 0x1C
    "Blank1D",                  // 0x1D
    "Player.Down",              // 0x1E
    "Blank1F",                  // 0x1F
    "Player.Animation",         // 0x20
    "Player.Frame",             // 0x21
    "Player.Rings",             // 0x22
    "Blank23",                  // 0x23
    "XBoundary1",               // 0x24 //Sets "newXBoundary1"
    "XBoundary2",               // 0x25 //Sets "newXBoundary2"
    "YBoundary1",               // 0x26 //Sets "newYBoundary1"
    "YBoundary2",               // 0x27 //Sets "newYBoundary2"
    "TempObjectPos",            // 0x28
    "MainGameMode",             // 0x29
    "TempValA",                 // 0x2A
    "TempValB",                 // 0x2B
    "CheckResult",              // 0x2C
    "AngleTimer",               // 0x2D
    "Object.Priority",          // 0x2E
    "DeformationPosB1",         // 0x2F
    "DeformationPosB2",         // 0x30
    "DeformationPosF1",         // 0x31
    "DeformationPosF2",         // 0x32
    "WaterLevel",               // 0x33
    "BGType",                   // 0x34
    "BGCustomXPos",             // 0x35
    "BGCustomYPos",             // 0x36
    "RingAni",                  // 0x37
    "Stage.State2",             // 0x38
    "MainGameMode2",            // 0x39
    "PlayerListSize",           // 0x3A
    "Player.CollisionLeft",     // 0x3B
    "Player.CollisionTop",      // 0x3C
    "Player.CollisionRight",    // 0x3D
    "Player.CollisionBottom",   // 0x3E
    "Player.Unknown3F",         // 0x3F
    "Player.XVelocity",         // 0x40
    "Player.YVelocity",         // 0x41
    "Player.MovesetType",       // 0x42
    "Object.EntityNo",          // 0x43
    "EarthquakeY",              // 0x44
};

QList<RSDKv1::Script::FunctionInfo> RSDKv1::Script::opcodeList = {
    RSDKv1::Script::FunctionInfo("EndSub", 1),        // 0x00
    RSDKv1::Script::FunctionInfo("Equal", 2),         // 0x01 (LHS, RHS)
    RSDKv1::Script::FunctionInfo("Add", 2),           // 0x02 (LHS, RHS)
    RSDKv1::Script::FunctionInfo("Sub", 2),           // 0x03 (LHS, RHS)
    RSDKv1::Script::FunctionInfo("Inc", 1),           // 0x04 (LHS, RHS)
    RSDKv1::Script::FunctionInfo("Dec", 1),           // 0x05 (LHS, RHS)
    RSDKv1::Script::FunctionInfo("Mul", 2),           // 0x06 (LHS, RHS)
    RSDKv1::Script::FunctionInfo("Div", 2),           // 0x07 (LHS, RHS)
    RSDKv1::Script::FunctionInfo("ShR", 2),           // 0x08 (LHS, RHS)
    RSDKv1::Script::FunctionInfo("ShL", 2),           // 0x09 (LHS, RHS)
    RSDKv1::Script::FunctionInfo("[Null_0xA]", 3),    // 0x0A // possibly LABEL ?
    RSDKv1::Script::FunctionInfo("goto", 1),          // 0x0B (LabelID)
    RSDKv1::Script::FunctionInfo("Rand", 2),          // 0x0C (store, max)
    RSDKv1::Script::FunctionInfo("SetEditorIcon", 1), // 0x0D (Frame)
    RSDKv1::Script::FunctionInfo("SetEditorFrame",
                                 6), // 0x0E (Width, Height, PivotX, PivotY, SprX, SprY)
    RSDKv1::Script::FunctionInfo("ObjectFloorCollision", 3), // 0x0F (objID, XOffset, YOffset)
    RSDKv1::Script::FunctionInfo("ObjectFloorGrip", 3),      // 0x10 (objID, XOffset, YOffset)
    RSDKv1::Script::FunctionInfo("SpriteFrame", 6), // 0x11 (Width, Height, PivotX, PivotY, SprX, SprY)
    RSDKv1::Script::FunctionInfo("DrawSprite", 1),  // 0x12 (Frame)
    RSDKv1::Script::FunctionInfo("CreateTempObject", 4),    // 0x13 (Type, PropertyVal, XPos, YPos)
    RSDKv1::Script::FunctionInfo("TouchCollision", 4),      // 0x14 (left, top, right, bottom)
    RSDKv1::Script::FunctionInfo("BoxCollision", 4),        // 0x15 (left, top, right, bottom)
    RSDKv1::Script::FunctionInfo("PlatformCollision", 4),   // 0x16 (left, top, right, bottom)
    RSDKv1::Script::FunctionInfo("EnemyCollision", 4),      // 0x17 (left, top, right, bottom)
    RSDKv1::Script::FunctionInfo("HurtCollision", 4),       // 0x18 (left, top, right, bottom)
    RSDKv1::Script::FunctionInfo("CheckEqual", 2),          // 0x19 (A, B)
    RSDKv1::Script::FunctionInfo("CheckGreater", 2),        // 0x1A (A, B)
    RSDKv1::Script::FunctionInfo("CheckLower", 2),          // 0x1B (A, B)
    RSDKv1::Script::FunctionInfo("CheckNotEqual", 2),       // 0x1C (A, B)
    RSDKv1::Script::FunctionInfo("Sin", 2),                 // 0x1D (store, angle)
    RSDKv1::Script::FunctionInfo("Cos", 2),                 // 0x1E (store, angle)
    RSDKv1::Script::FunctionInfo("SinX", 4),                // 0x1F (store, origin, angle, shift)
    RSDKv1::Script::FunctionInfo("CosX", 4),                // 0x20 (store, origin, angle, shift)
    RSDKv1::Script::FunctionInfo("SinY", 4),                // 0x21 (store, origin, angle, shift)
    RSDKv1::Script::FunctionInfo("CosY", 4),                // 0x22 (store, origin, angle, shift)
    RSDKv1::Script::FunctionInfo("MovePlayerX", 1),         // 0x23 (Offset)
    RSDKv1::Script::FunctionInfo("MovePlayerY", 1),         // 0x24 (Offset)
    RSDKv1::Script::FunctionInfo("CheckPlayerDiagonal", 3), // 0x25 (X, Y, FlipX)
    RSDKv1::Script::FunctionInfo("RotatePalette", 3),       // 0x26 (start, end, rotateRight)
    RSDKv1::Script::FunctionInfo("PlaySfx", 2),             // 0x27 (SFX, Loop)
    RSDKv1::Script::FunctionInfo("StopSfx", 1),             // 0x28 (SFX)
    RSDKv1::Script::FunctionInfo(
        "SetBGDeformation", 6), // 0x29 (deformID, waveLength, waveWidth, waveType, waveStart, waveEnd)
    RSDKv1::Script::FunctionInfo("ClearBGDeformation", 1), // 0x2A (deformID)
    RSDKv1::Script::FunctionInfo("HandleCorkscrewStart",
                                 4), // 0x2B (corkscrewX, corkscrewY, corkscrewWidth, corkscrewHeight)
    RSDKv1::Script::FunctionInfo("HandleCorkscrewEnd", 1), // 0x2C (Unused)
    RSDKv1::Script::FunctionInfo("PlayMusic", 1),          // 0x2D (TrackID)
    RSDKv1::Script::FunctionInfo("SetWaterColour", 4),     // 0x2E (R, G, B, A)
    RSDKv1::Script::FunctionInfo(
        "SetBackground",
        5), // 0x2F (activeLayer0, activeLayer1, activeLayer2, activeLayer3, layerMidPoint)
    RSDKv1::Script::FunctionInfo("switch", 2),      // 0x30 (switchVal, jmp)
    RSDKv1::Script::FunctionInfo("break", 1),       // 0x31 (jmp)
    RSDKv1::Script::FunctionInfo("[Null_0x32]", 0), // 0x32 // possibly "case" ?
    RSDKv1::Script::FunctionInfo("[Null_0x33]", 0), // 0x33 // possibly "default" ?
    RSDKv1::Script::FunctionInfo("endswitch", 1),   // 0x34 ()
    RSDKv1::Script::FunctionInfo("Interpolate", 7), // 0x35 (X1, Y1, X2, Y2, outX, outY, percent)
    RSDKv1::Script::FunctionInfo("CopyGfx",
                                 8), // 0x36 (width, height, sprX1, sprY1, sprY2, sprX2, sheet1, sheet2)
    RSDKv1::Script::FunctionInfo("BossCollision", 4),       // 0x37 (left, top, right, bottom)
    RSDKv1::Script::FunctionInfo("BlockCollision", 4),      // 0x38 (left, top, right, bottom)
    RSDKv1::Script::FunctionInfo("HandleBonusMovement", 3), // 0x39 (Angle, outX, outY)
    RSDKv1::Script::FunctionInfo("GetObjectValue", 3),      // 0x3A (object, type, value)
    RSDKv1::Script::FunctionInfo("SetObjectValue", 3),      // 0x3B (object, type, store)
    RSDKv1::Script::FunctionInfo("BumperCollision", 4),     // 0x3C (left, top, right, bottom)
    RSDKv1::Script::FunctionInfo("SetFade", 4),             // 0x3D (R, G, B, A)
    RSDKv1::Script::FunctionInfo("HandleMode7Movement", 1), // 0x3E (Unused)
    RSDKv1::Script::FunctionInfo("DrawSpriteMode7", 1),     // 0x3F (Frame)
    RSDKv1::Script::FunctionInfo("Mode7Collision", 6),      // 0x40 (A, B, C, D, E, F)
    RSDKv1::Script::FunctionInfo("PlayStageSfx", 2),        // 0x41 (SFX, loop)
    RSDKv1::Script::FunctionInfo("StopStageSfx", 1),        // 0x42 (SFX)
};

void RSDKv1::Script::read(Reader &reader)
{
    filePath = reader.filePath;
    for (ScriptSub &sub : subs) {
        sub.scriptCode.clear();
        sub.blankLines.clear();
        sub.labels.clear();
        sub.jumpTable.clear();

        bool finishedScriptCode = false;

        int scriptCodeLength = reader.read<byte>() << 8;
        scriptCodeLength |= reader.read<byte>();

        sub.scriptCode.clear();
        while (!finishedScriptCode) {
            byte scriptOpcode = reader.read<byte>();
            if (scriptOpcode == 0xFF) {
                scriptOpcode = reader.read<byte>();
                if (scriptOpcode == 0xFF) {
                    scriptOpcode = reader.read<byte>();
                    if (scriptOpcode == 0xFF) {
                        scriptOpcode = reader.read<byte>();
                        if (scriptOpcode == 0xFF)
                            finishedScriptCode = true;
                    }
                }
            }

            if (!finishedScriptCode) {
                OpcodeInfo opInfo;

                opInfo.opcode = scriptOpcode;

                uint paramCount = (uint)opcodeList[scriptOpcode].paramCount;
                opInfo.parameters.clear();

                for (uint p = 0; p < paramCount; p++) {
                    ParamInfo param;

                    int paramType = reader.read<byte>(); // if 0 then int constant, else variable

                    if (paramType != 0) {
                        param.isVariable = true;

                        param.value = reader.read<byte>();

                        int arrayIndex = reader.read<byte>();
                        if (arrayIndex > 0x80)
                            arrayIndex = 0x80 - arrayIndex;

                        param.arrayIndex = (sbyte)arrayIndex;
                    }
                    else {
                        param.isVariable = false;
                        param.arrayIndex = -1;

                        int byte1    = reader.read<byte>();
                        int constVal = 0;
                        if (byte1 < 0x80) // unsigned uint16
                        {
                            constVal = byte1 << 8;
                            constVal |= reader.read<byte>();
                        }
                        else // signed uint16
                        {
                            constVal = (byte1 - 0x80) << 8;
                            constVal |= reader.read<byte>();
                            constVal = -constVal;
                        }

                        param.value = constVal;
                    }
                    opInfo.parameters.append(param);
                }

                sub.scriptCode.append(opInfo);
            }
        }

        int blankLineUnused = reader.read<byte>();
        int blankLineCount  = reader.read<byte>();
        Q_UNUSED(blankLineUnused);

        for (int u = 0; u < blankLineCount; u++) {
            int blankLineID = reader.read<byte>() << 8;
            blankLineID |= reader.read<byte>();
            sub.blankLines.append(blankLineID);
        }

        int labelUnused = reader.read<byte>();
        int labelCount  = reader.read<byte>();
        Q_UNUSED(labelUnused);

        for (int l = 0; l < labelCount; l++) {
            LabelInfo label;

            label.scriptCodePos = reader.read<byte>() << 8;
            label.scriptCodePos |= reader.read<byte>();

            label.id = reader.read<byte>() << 8;
            label.id |= reader.read<byte>();

            label.lineID = reader.read<byte>() << 8;
            label.lineID |= reader.read<byte>();

            sub.labels.append(label);
        }

        int jumpTableUnused = reader.read<byte>();
        int jumpTableCount  = reader.read<byte>();
        Q_UNUSED(jumpTableUnused);

        for (int s = 0; s < jumpTableCount; ++s) {
            SwitchInfo info;

            info.scriptCodePos = reader.read<byte>() << 8;
            info.scriptCodePos |= reader.read<byte>();

            int caseCount = reader.read<byte>() << 8;
            caseCount |= reader.read<byte>();

            info.defaultScriptCodePos = reader.read<byte>() << 8;
            info.defaultScriptCodePos |= reader.read<byte>();

            info.defaultCaseLineID = reader.read<byte>() << 8;
            info.defaultCaseLineID |= reader.read<byte>();

            info.endScriptCodePos = reader.read<byte>() << 8;
            info.endScriptCodePos |= reader.read<byte>();

            int lowestCase  = 0x8000;
            int highestCase = 0;
            info.cases.clear();
            for (int c = 0; c < caseCount; c++) {
                SwitchCaseInfo caseInfo;

                caseInfo.scriptCodePos = reader.read<byte>() << 8;
                caseInfo.scriptCodePos |= reader.read<byte>();

                caseInfo.caseNum = reader.read<byte>() << 8;
                caseInfo.caseNum |= reader.read<byte>();

                if (caseInfo.caseNum < lowestCase)
                    lowestCase = caseInfo.caseNum;

                if (caseInfo.caseNum > highestCase)
                    highestCase = caseInfo.caseNum;

                caseInfo.lineID = reader.read<byte>() << 8;
                caseInfo.lineID |= reader.read<byte>();

                info.cases.append(caseInfo);
            }

            info.lowestCase  = lowestCase;
            info.highestCase = highestCase;

            // wow this is weird
            // it manually goes and sets up the offsets instead of doing it at compile-time like later
            // RSDK versions
            int pos = 0;
            for (OpcodeInfo &opcodeInfo : sub.scriptCode) {
                if (pos == info.scriptCodePos) {
                    opcodeInfo.parameters[1].value = pos;
                    break;
                }
                pos += opcodeInfo.size();
            }

            sub.jumpTable.append(info);
        }
    }
}

void RSDKv1::Script::write(Writer &writer)
{
    filePath = writer.filePath;

    for (ScriptSub sub : subs) {
        // Script Code
        writer.write((byte)(sub.scriptCodeLength() >> 8));
        writer.write((byte)(sub.scriptCodeLength() & 0xFF));

        for (OpcodeInfo opInfo : sub.scriptCode) {
            writer.write(opInfo.opcode);

            int paramID = 0;
            for (ParamInfo param : opInfo.parameters) {
                writer.write((byte)(param.isVariable ? 1 : 0));
                if (param.isVariable) {
                    writer.write((byte)param.value);

                    if (param.arrayIndex < 0) {
                        byte value = (byte)-param.value;
                        writer.write((byte)(value + 0x80));
                    }
                    else {
                        writer.write((byte)param.arrayIndex);
                    }
                }
                else {
                    // Gross, but its set using jumpTable, not here
                    if (opInfo.name() == "switch" && paramID == 1) {
                        writer.write((byte)(0 >> 8));
                        writer.write((byte)(0 & 0xFF));
                    }
                    else {
                        if (param.value < 0) {
                            int value   = -param.value;
                            sbyte byte1 = (sbyte)(value >> 8);
                            writer.write((byte)(byte1 + 0x80));
                            writer.write((byte)(value & 0xFF));
                        }
                        else {
                            writer.write((byte)(param.value >> 8));
                            writer.write((byte)(param.value & 0xFF));
                        }
                    }
                }

                ++paramID;
            }
        }
        writer.write(0xFFFFFFFF);

        // the lines to put blank spaces on
        writer.write((byte)(0 >> 8));
        writer.write((byte)(sub.blankLines.count() & 0xFF));
        for (int lineID : sub.blankLines) {
            // a line to put a blank space on
            writer.write((byte)(lineID >> 8));
            writer.write((byte)(lineID & 0xFF));
        }

        // Labels
        writer.write((byte)(0 >> 8));
        writer.write((byte)(sub.labels.count() & 0xFF));
        for (LabelInfo label : sub.labels) {
            // Where the label should jump to
            writer.write((byte)(label.scriptCodePos >> 8));
            writer.write((byte)(label.scriptCodePos & 0xFF));

            // the id of the label
            writer.write((byte)(label.id >> 8));
            writer.write((byte)(label.id & 0xFF));

            // the line ID of the label
            writer.write((byte)(label.lineID >> 8));
            writer.write((byte)(label.lineID & 0xFF));
        }

        // JumpTable (switch cases)
        writer.write((byte)(0 >> 8));
        writer.write((byte)(sub.jumpTable.count() & 0xFF));
        for (SwitchInfo switchStatement : sub.jumpTable) {
            // the script code pos of the corresponding switch function call
            writer.write((byte)(switchStatement.scriptCodePos >> 8));
            writer.write((byte)(switchStatement.scriptCodePos & 0xFF));

            // How many cases the switch statement has
            writer.write((byte)(switchStatement.cases.count() >> 8));
            writer.write((byte)(switchStatement.cases.count() & 0xFF));

            // the scriptCodePos to jump to on default cases
            writer.write((byte)(switchStatement.defaultScriptCodePos >> 8));
            writer.write((byte)(switchStatement.defaultScriptCodePos & 0xFF));

            // the line ID of the default case
            writer.write((byte)(switchStatement.defaultCaseLineID >> 8));
            writer.write((byte)(switchStatement.defaultCaseLineID & 0xFF));

            // the scriptCodePos to jump to on end switch
            writer.write((byte)(switchStatement.endScriptCodePos >> 8));
            writer.write((byte)(switchStatement.endScriptCodePos & 0xFF));

            for (SwitchCaseInfo caseInfo : switchStatement.cases) {
                // Where to set the scriptCodePos to
                writer.write((byte)(caseInfo.scriptCodePos >> 8));
                writer.write((byte)(caseInfo.scriptCodePos & 0xFF));

                // Case Number
                writer.write((byte)(caseInfo.caseNum >> 8));
                writer.write((byte)(caseInfo.caseNum & 0xFF));

                // the line ID of the switch case
                writer.write((byte)(caseInfo.lineID >> 8));
                writer.write((byte)(caseInfo.lineID & 0xFF));
            }
        }
    }

    writer.flush();
}
