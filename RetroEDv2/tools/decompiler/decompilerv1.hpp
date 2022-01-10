#ifndef DECOMPILER_V1_H
#define DECOMPILER_V1_H

namespace RSDKv1
{

class Decompiler
{
public:
    Decompiler() {}

    void decompile(RSDKv1::Script script, QString destPath = "");

private:
    void decompileScript(Writer &writer, RSDKv1::Script &script, int scriptSub);
    void decompileSub(Writer writer, RSDKv1::Script &script, int scriptSub);

    QString setArrayValue(QString strIn, QString index);
    QString getVariable(RSDKv1::Script::ParamInfo param);
};

}; // namespace RSDKv1

#endif // DECOMPILER_V1_H
