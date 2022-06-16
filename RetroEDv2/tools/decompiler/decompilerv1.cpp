#include "includes.hpp"

void RSDKv1::Decompiler::decompile(RSDKv1::Script script, QString scriptPath)
{
    if (QFile::exists(scriptPath))
        QFile::remove(scriptPath);
    Writer writer(scriptPath);

    QString name = QFileInfo(scriptPath).baseName();
    writer.writeLine("//------------Retro-Sonic " + name + " Script-------------//");
    writer.writeLine("//--------Scripted by Christian Whitehead 'The Taxman'--------//");
    writer.writeLine("//-------Unpacked By Rubberduckycooly's Script Unpacker-------//");

    writer.writeLine();

    writer.writeLine();
    writer.writeLine();

    if (script.subs[0].scriptCode.count() > 1) {
        writer.writeLine("//---------------------------Main Sub---------------------------//");
        writer.writeLine("//-------Called once a frame, use this for most functions-------//");
        decompileScript(writer, script, 0);
    }

    if (script.subs[1].scriptCode.count() > 1) {
        writer.writeLine(
            "//-------------------------Player Interaction Sub---------------------------//");
        writer.writeLine(
            "//-------This sub is called when the object interacts with the player-------//");
        decompileScript(writer, script, 1);
    }

    if (script.subs[2].scriptCode.count() > 1) {
        writer.writeLine("//----------------------Drawing Sub-------------------//");
        writer.writeLine("//-------Called once a frame after the Main Sub-------//");
        decompileScript(writer, script, 2);
    }

    if (script.subs[3].scriptCode.count() > 1) {
        writer.writeLine("//-----------------------------Startup Sub---------------------------//");
        writer.writeLine("//-------Called once for each object type when the stage loads-------//");
        decompileScript(writer, script, 3);
    }

    if (script.subs[4].scriptCode.count() > 1) {
        writer.writeLine("//--------------------RSDK Sub---------------------//");
        writer.writeLine("//-----------Used for editor functionality---------//");
        decompileScript(writer, script, 4);
    }

    writer.writeLine();
    writer.flush();
}

void RSDKv1::Decompiler::decompileScript(Writer &writer, RSDKv1::Script &script, int scriptSub)
{
    QString strFuncName = "";
    switch (scriptSub) {
        case 0: strFuncName = "Main"; break;
        case 1: strFuncName = "PlayerInteraction"; break;
        case 2: strFuncName = "Draw"; break;
        case 3: strFuncName = "Startup"; break;
        case 4: strFuncName = "RSDK"; break;
        default: strFuncName = "Function"; break;
    }

    writer.writeLine("sub " + strFuncName);

    decompileSub(writer, script, scriptSub);
    writer.writeLine();
}
void RSDKv1::Decompiler::decompileSub(Writer writer, RSDKv1::Script &script, int scriptSub)
{
    QList<RSDKv1::Script::SwitchInfo> switchStack;
    int opcodeID      = 0;
    int scriptCodePos = 0;
    int scopeDepth    = 1;

    bool newline = true;
    int lineID   = 0;

    QList<QString> typeNames = {
        "TYPE_BLANKOBJ",         // 0
        "TYPE_RING",             // 1
        "TYPE_RINGLOSS",         // 2
        "TYPE_RINGSPARKLE",      // 3
        "TYPE_MONITOR",          // 4
        "TYPE_BROKENMONITOR",    // 5
        "TYPE_YELLOWSPRING",     // 6
        "TYPE_REDSPRING",        // 7
        "TYPE_SPIKES",           // 8
        "TYPE_LAMPPOST",         // 9
        "TYPE_PLANESWITCH_L",    // 10
        "TYPE_PLANESWITCH_R",    // 11
        "TYPE_PLANESWITCH_U",    // 12
        "TYPE_PLANESWITCH_D",    // 13
        "TYPE_FORCESPIN_R",      // 14
        "TYPE_FORCESPIN_L",      // 15
        "TYPE_UNKNOWN16",        // 16
        "TYPE_UNKNOWN17",        // 17
        "TYPE_SIGNPOST",         // 18
        "TYPE_EGGPRISON",        // 19
        "TYPE_SMALL_EXPLOSION",  // 20
        "TYPE_LARGE_EXPLOSION",  // 21
        "TYPE_EGGPRISONDEBRIS",  // 22
        "TYPE_ANIMAL",           // 23
        "TYPE_UNKNOWN24",        // 24
        "TYPE_UNKNOWN25",        // 25
        "TYPE_SPECIALRING",      // 26
        "TYPE_WATERSPLASH",      // 27
        "TYPE_BUBBLER",          // 28
        "TYPE_SMALL_AIRBUBBBLE", // 29
        "TYPE_SMOKEPUFF",        // 30
    };

    QList<QString> sfxNames = {
        "Sfx_Jump",    "Sfx_RingL",      "Sfx_RingR",      "Sfx_Spring",      "Sfx_LoseRings",
        "Sfx_Destroy", "Sfx_LampPost",   "Sfx_Hurt",       "Sfx_Spin",        "Sfx_SpinDash",
        "Sfx_Release", "Sfx_SignPost",   "Sfx_Button",     "Sfx_ScoreAdd",    "Sfx_Skidding",
        "Sfx_Explode", "Sfx_WarpRing",   "Sfx_EnterSS",    "Sfx_BossHit",     "Sfx_Breathe",
        "Sfx_Crumble", "Sfx_BlueShield", "Sfx_FireShield", "Sfx_WaterShield", "Sfx_ElecShield",
        "Sfx_Flame",   "Sfx_ElecJump",   "Sfx_Bounce",     "Sfx_Catch",       "Sfx_Splash",
        "Sfx_SSFail",  "Sfx_WaterChime", "Sfx_Drowning",   "Sfx_Bumper",      "Sfx_EnterBonus",
        "Sfx_ExitSS",  "Sfx_Landing",    "Sfx_GSlide",     "Sfx_Flying",      "Sfx_Tired",
    };

    QList<QString> priorities = {
        "PRIORITY_INACTIVE",
        "PRIORITY_BOUNDS",
        "PRIORITY_ACTIVE",
    };

    for (RSDKv1::Script::OpcodeInfo opInfo : script.subs[scriptSub].scriptCode) {
        for (int blankLineID : script.subs[scriptSub].blankLines) {
            if (lineID == blankLineID)
                writer.writeLine();
        }

        for (RSDKv1::Script::LabelInfo label : script.subs[scriptSub].labels) {
            if (label.scriptCodePos == scriptCodePos) {
                writer.writeLine(QString("LABEL %1:").arg(label.id));

                lineID++;
                for (int blankLineID : script.subs[scriptSub].blankLines) {
                    if (lineID == blankLineID)
                        writer.writeLine();
                }
            }
        }

        if (switchStack.count() > 0) {
            for (RSDKv1::Script::SwitchCaseInfo caseInfo : switchStack[switchStack.count() - 1].cases) {
                if (caseInfo.scriptCodePos != 0 && caseInfo.scriptCodePos == scriptCodePos) {
                    for (int i = 0; i < scopeDepth - 1; i++) writer.writeText("\t");
                    writer.writeLine(QString("case %1:").arg(caseInfo.caseNum));

                    lineID++;
                    for (int blankLineID : script.subs[scriptSub].blankLines) {
                        if (lineID == blankLineID)
                            writer.writeLine();
                    }
                }
            }

            int defaultPos = switchStack[switchStack.count() - 1].defaultScriptCodePos;
            if (defaultPos != 0 && defaultPos == scriptCodePos) {
                for (int i = 0; i < scopeDepth - 1; i++) writer.writeText("\t");
                writer.writeLine("default:");

                lineID++;
                for (int blankLineID : script.subs[scriptSub].blankLines) {
                    if (lineID == blankLineID)
                        writer.writeLine();
                }
            }
        }

        QString variableName[10];

        for (int i = 0; i < 10; i++) variableName[i] = "UNKNOWN VARIABLE";

        for (int i = 0; i < opInfo.paramCount(); i++)
            variableName[i] = getVariable(opInfo.parameters[i]);

        if (opInfo.opcode != 0) {
            if (opInfo.name() == "endswitch")
                scopeDepth--;

            for (int i = 0; i < scopeDepth; i++) writer.writeText("\t");
        }

        for (int i = 0; i < 10; i++) {
            if (variableName[i] == "")
                variableName[i] = "Object.ValueA";
        }

        // Aliases
        {
            if (variableName[0].contains("Object") && variableName[0].contains(".Type")) {
                bool ok = false;
                int id  = variableName[1].toInt(&ok);
                if (ok) {
                    if (id < typeNames.count())
                        variableName[1] = typeNames[id].replace(" ", "");
                }
            }

            if (variableName[1].contains("Object") && variableName[1].contains(".Type")) {
                bool ok = false;
                int id  = variableName[2].toInt(&ok);
                if (ok) {
                    if (id < typeNames.count())
                        variableName[2] = typeNames[id].replace(" ", "");
                }
            }

            if (variableName[0].contains("Object") && variableName[0].contains(".Priority")) {
                bool ok = false;
                int id  = variableName[1].toInt(&ok);
                if (ok) {
                    if (id < priorities.count())
                        variableName[1] = priorities[id].replace(" ", "");
                }
            }

            if (variableName[1].contains("Object") && variableName[1].contains(".Priority")) {
                bool ok = false;
                int id  = variableName[2].toInt(&ok);
                if (ok) {
                    if (id < priorities.count())
                        variableName[2] = priorities[id].replace(" ", "");
                }
            }

            if (opInfo.name() == "CreateTempObject") {
                bool ok = false;
                int id  = variableName[0].toInt(&ok);
                if (ok) {
                    if (id < typeNames.count())
                        variableName[0] = typeNames[id].replace(" ", "");
                }
            }

            if (opInfo.name() == "PlaySfx") {
                bool ok = false;
                int id  = variableName[0].toInt(&ok);
                if (ok) {
                    if (id < sfxNames.count())
                        variableName[0] = sfxNames[id].replace(" ", "");
                }
            }

            if (opInfo.name() == "PlaySfx" || opInfo.name() == "PlayStageSfx") {
                bool ok = false;
                int id  = variableName[1].toInt(&ok);
                if (ok)
                    variableName[1] = id != 0 ? "true" : "false";
            }
        }

        newline = true;
        switch (opInfo.opcode) {
            case 0x00: newline = false; break;
            case 0x01:
                writer.writeText(QString("%1 = %2;").arg(variableName[0]).arg(variableName[1]));
                break;
            case 0x02:
                writer.writeText(QString("%1 += %2;").arg(variableName[0]).arg(variableName[1]));
                break;
            case 0x03:
                writer.writeText(QString("%1 -= %2;").arg(variableName[0]).arg(variableName[1]));
                break;
            case 0x04: writer.writeText(QString("%1++;").arg(variableName[0])); break;
            case 0x05: writer.writeText(QString("%1--;").arg(variableName[0])); break;
            case 0x06:
                writer.writeText(QString("%1 *= %2;").arg(variableName[0]).arg(variableName[1]));
                break;
            case 0x07:
                writer.writeText(QString("%1 /= %2;").arg(variableName[0]).arg(variableName[1]));
                break;
            case 0x08:
                writer.writeText(QString("%1 >>= %2;").arg(variableName[0]).arg(variableName[1]));
                break;
            case 0x09:
                writer.writeText(QString("%1 <<= %2;").arg(variableName[0]).arg(variableName[1]));
                break;
            case 0x0B: {
                writer.writeText(QString("goto %1;").arg(variableName[0]));
                break;
            }
            case 0x30: {
                writer.writeText(QString("switch (%1)").arg(variableName[0]));
                scopeDepth++;

                bool ok           = false;
                int switchCodePos = variableName[1].toInt(&ok);
                if (ok) {
                    for (RSDKv1::Script::SwitchInfo swInfo : script.subs[scriptSub].jumpTable) {
                        if (switchCodePos == swInfo.scriptCodePos) {
                            switchStack.append(swInfo);
                            break;
                        }
                    }
                }
                break;
            }
            case 0x31: writer.writeText("break;"); break;
            case 0x34:
                writer.writeText("end switch");
                if (switchStack.count() > 0)
                    switchStack.removeAt(switchStack.count() - 1);
                break;
            default:
                writer.writeText(opInfo.name() + "(");
                for (int i = 0; i < opInfo.paramCount(); i++) {
                    writer.writeText(variableName[i]);
                    if (i + 1 < opInfo.paramCount())
                        writer.writeText(", ");
                }
                writer.writeText(");");
                break;
        }

        if (newline)
            writer.writeLine();

        opcodeID++;
        scriptCodePos += opInfo.size();
        lineID++;

        if (opcodeID >= script.subs[scriptSub].scriptCode.count()) {
            // Final Label Check
            for (RSDKv1::Script::LabelInfo label : script.subs[scriptSub].labels) {
                if (label.scriptCodePos == scriptCodePos) {
                    writer.writeLine(QString("LABEL %1:").arg(label.id));

                    lineID++;
                    for (int blankLineID : script.subs[scriptSub].blankLines) {
                        if (lineID == blankLineID)
                            writer.writeLine();
                    }
                }
            }

            writer.writeLine("end sub");
        }
    }
    writer.writeLine();
}

QString RSDKv1::Decompiler::setArrayValue(QString strIn, QString index)
{
    QString strOut = strIn;
    int point      = -1;

    for (int i = 0; i < strIn.length(); ++i) {
        if (strIn[i] == '.') {
            point = i;
            break;
        }
    }
    if (point >= 0) {
        QStringList split = strIn.split('.');
        strOut            = split[0] + "[" + index + "]." + split[1];
    }
    else {
        strOut = strOut + "[" + index + "]";
    }
    return strOut;
}

QString RSDKv1::Decompiler::getVariable(RSDKv1::Script::ParamInfo param)
{
    if (!param.isVariable) {
        return QString::number(param.value);
    }
    else {
        int arrVal = param.arrayIndex;

        QString var = RSDKv1::Script::variableList[param.value];
        if (arrVal != 0 || (param.value >= 0x34 && param.value <= 0x36)) {
            switch (param.value) {
                case 1:
                    var = setArrayValue(var, QString::number(arrVal));
                    // [Array + objectLoop]
                    break;
                case 2:
                    var = setArrayValue(var, QString::number(arrVal));
                    // [Array + objectLoop]
                    break;
                case 3:
                    var = setArrayValue(var, QString::number(arrVal));
                    // [Array + objectLoop]
                    break;
                case 4:
                    var = setArrayValue(var, QString::number(arrVal));
                    // [Array + objectLoop]
                    break;
                case 5:
                    var = setArrayValue(var, QString::number(arrVal));
                    // [Array + objectLoop]
                    break;
                case 6:
                    var = setArrayValue(var, QString::number(arrVal));
                    // [Array + objectLoop]
                    break;
                case 7:
                    var = setArrayValue(var, QString::number(arrVal));
                    // [Array + objectLoop]
                    break;
                case 8:
                    var = setArrayValue(var, QString::number(arrVal));
                    // [Array + objectLoop]
                    break;
                case 9:
                    var = setArrayValue(var, QString::number(arrVal));
                    // [Array + objectLoop]
                    break;
                case 0xA:
                    var = setArrayValue(var, QString::number(arrVal));
                    // [Array + objectLoop]
                    break;
                case 0x2E:
                    var = setArrayValue(var, QString::number(arrVal));
                    // [Array + objectLoop]
                    break;
                case 0x34:
                    var = setArrayValue(var, QString::number(arrVal));
                    // [Array]
                    break;
                case 0x35:
                    var = setArrayValue(var, QString::number(arrVal));
                    // [Array]
                    break;
                case 0x36:
                    var = setArrayValue(var, QString::number(arrVal));
                    // [Array]
                    break;
                case 0x43:
                    var = setArrayValue(var, QString::number(arrVal));
                    // [Array + objectLoop]
                    break;
                default: PrintLog(QString("Unexpected array index (%1)").arg(arrVal)); break;
            }
        }

        return var;
    }
}
