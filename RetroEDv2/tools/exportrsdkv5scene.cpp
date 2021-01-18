#include "includes.hpp"
#include "ui_exportrsdkv5scene.h"

ExportRSDKv5Scene::ExportRSDKv5Scene(QString path, QWidget *parent)
    : QDialog(parent), ui(new Ui::ExportRSDKv5Scene)
{
    ui->setupUi(this);

    this->setWindowTitle("Export to RSDKv5");

    ui->nameLabel->setText(path);

    connect(ui->selFolderButton, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Open Folder"), "", tr("Folder"));
        filedialog.setFileMode(QFileDialog::Directory);
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            m_outputPath = filedialog.selectedFiles()[0];
            ui->outputFolder->setText(m_outputPath);
        }
    });
}

ExportRSDKv5Scene::~ExportRSDKv5Scene() { delete ui; }

#include "moc_exportrsdkv5scene.cpp"
