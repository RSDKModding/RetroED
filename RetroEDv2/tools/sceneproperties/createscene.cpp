#include "includes.hpp"
#include "createscene.hpp"
#include "ui_createscene.h"

CreateScene::CreateScene(QWidget *parent) : QDialog(parent), ui(new Ui::CreateScene)
{
    ui->setupUi(this);
    this->setWindowTitle("Create Scene");

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    connect(ui->confirmButton, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->confirmButton, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(ui->loadGC, &QCheckBox::toggled, [this](bool v){
        loadGC = v;
    });
    connect(ui->sceneVer, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int v){ sceneVer = v;});
    connect(ui->setSceneDir, &QPushButton::clicked, [this]{
        QFileDialog filedialog(this, tr("Open Scene Folder"), "", tr("Folder"));
        filedialog.setFileMode(QFileDialog::Directory);
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            scenePath = filedialog.selectedFiles()[0];
            ui->sceneDir->setText(scenePath);
        }
    });

    connect(ui->setGCDir, &QPushButton::clicked, [this]{
        QFileDialog filedialog(this, tr("Open GameConfig"), "",
                    tr("RSDKv5 GameConfig (GameConfig*.bin);;RSDK Game.xml Files (Game*.xml)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            gcPath = filedialog.selectedFiles()[0];
            ui->gcDir->setText(gcPath);
        }
    });
}

void CreateScene::LoadSceneVer(QList<QString> list){
    ui->sceneVer->clear();
    for (int c = 0; c < list.count(); ++c)
        ui->sceneVer->addItem(list[c]);
}

CreateScene::~CreateScene()
{
    delete ui;
}
