#ifndef SCRIPTCOMPILER_HPP
#define SCRIPTCOMPILER_HPP

#include <QWidget>

namespace Ui
{
class ScriptCompiler;
}

class ScriptCompiler : public QWidget
{
    Q_OBJECT

public:
    explicit ScriptCompiler(QWidget *parent = nullptr);
    ~ScriptCompiler();

private:
    Ui::ScriptCompiler *ui;

    Compilerv1 compilerv1;
    Compilerv2 compilerv2;
    Compilerv3 compilerv3;
    Compilerv4 compilerv4;

    byte engineType = ENGINE_v3;
};

#endif // SCRIPTCOMPILER_HPP
