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
        QString m_path     = "";
        bool m_loadGlobals = false;

        QList<QString> m_sourceNames; // Script names
        QList<QString> m_typeNames;   // Object names
        QList<QString> m_sfxNames;    // SoundFX names
    };

    Ui::ScriptUnpacker *ui;

    RSDKv3::Decompiler m_decompilerv3;
    RSDKv4::Decompiler m_decompilerv4;
    RSDKv1::Decompiler m_decompilerv1;

    QString m_dataPath = "";

    QList<BytecodeInfo> m_bytecodeList; // Bytecode Paths
    QList<QString> m_variableNames;     // Global Variable Names

    int m_globalScriptCount   = 0;
    int m_globalFunctionCount = 0;
    int m_globalConstantCount = 0;
    int m_globalArrayCount    = 0;
    int m_globalSFXCount      = 0;
    int m_globalOffset        = 0;
    bool m_mobileVer          = false;
};

#endif // SCRIPTUNPACKER_H
