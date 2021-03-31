#ifndef EXPORTRSDKV5SCENE_HPP
#define EXPORTRSDKV5SCENE_HPP

#include <QDialog>

namespace Ui
{
class ExportRSDKv5Scene;
}

class ExportRSDKv5Scene : public QDialog
{
    Q_OBJECT

public:
    explicit ExportRSDKv5Scene(QString path, QWidget *parent = nullptr);
    ~ExportRSDKv5Scene();

    QString m_outputPath     = "";
    bool m_exportLayers      = true;
    bool m_exportObjects     = true;
    bool m_exportChunks      = true;
    bool m_exportTileConfig  = true;
    bool m_exportStageConfig = true;

private:
    Ui::ExportRSDKv5Scene *ui;
};

#endif // EXPORTRSDKV5SCENE_HPP
