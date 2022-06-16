#ifndef SCRIPTUNPACKER_H
#define SCRIPTUNPACKER_H

#include <QDialog>

namespace Ui
{
class ScriptUnpacker;
}

class ScriptUnpacker : public QDialog
{
    Q_OBJECT

public:
    explicit ScriptUnpacker(QWidget *parent = nullptr);
    ~ScriptUnpacker();

private:
    struct BytecodeInfo {
        QString path     = "";
        bool loadGlobals = false;

        QList<QString> sourceNames; // Script names
        QList<QString> typeNames;   // Object names
        QList<QString> sfxNames;    // SoundFX names
    };

    Ui::ScriptUnpacker *ui;

    RSDKv3::Decompiler decompilerv3;
    RSDKv4::Decompiler decompilerv4;
    RSDKv1::Decompiler decompilerv1;

    QString dataPath = "";

    QList<BytecodeInfo> bytecodeList; // Bytecode Paths
    QList<QString> globalVariableNames;     // Global Variable Names

    int globalScriptCount   = 0;
    int globalFunctionCount = 0;
    int globalConstantCount = 0;
    int globalArrayCount    = 0;
    int globalSFXCount      = 0;
    int globalOffset        = 0;
    bool mobileVer          = false;
};

#endif // SCRIPTUNPACKER_H
