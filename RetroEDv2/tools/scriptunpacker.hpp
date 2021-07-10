#ifndef SCRIPTUNPACKER_H
#define SCRIPTUNPACKER_H

#include <QWidget>

namespace Ui
{
class ScriptUnpacker;
}

class ScriptUnpacker : public QWidget
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
    RSDKv1::Decompiler m_decompilerv1;

    QString dataPath = "";

    QList<BytecodeInfo> bytecodeList; // Bytecode Paths
    QList<QString> variableNames;     // Global Variable Names

    int globalScriptCount     = 0;
    int globalFunctionCount   = 0;
    int m_globalConstantCount = 0;
    int m_globalArrayCount    = 0;
    int globalSFXCount        = 0;
    int m_globalOffset        = 0;
    bool mobileVer            = false;
};

#endif // SCRIPTUNPACKER_H
