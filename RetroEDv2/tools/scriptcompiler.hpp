#pragma once

#include <QWidget>

#include "tools/compiler/compilerv1.hpp"
#include "tools/compiler/compilerv2.hpp"
#include "tools/compiler/compilerv3.hpp"
#include "tools/compiler/compilerv4.hpp"

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


