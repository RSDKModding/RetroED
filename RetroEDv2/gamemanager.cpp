#include "includes.hpp"
#include "ui_gamemanager.h"

#include "gamemanager.hpp"

GameManager::GameManager(QWidget *parent) : QDialog(parent), ui(new Ui::GameManager)
{
    ui->setupUi(this);

    // remove question mark from the title bar & disable resizing
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setFixedSize(QSize(width(), height()));

    QPushButton *setExeLocation[] = { ui->setExeLocV5, ui->setExeLocV4, ui->setExeLocV3,
                                      ui->setExeLocV2, ui->setExeLocV1 };
    QPushButton *setDataLocation[] = { ui->setDataLocV5, ui->setDataLocV4, ui->setDataLocV3,
                                       ui->setDataLocV2, ui->setDataLocV1 };
    QLineEdit *exeLocation[] = { ui->exeLocV5, ui->exeLocV4, ui->exeLocV3, ui->exeLocV2, ui->exeLocV1 };
    QLineEdit *dataLocation[] = { ui->dataLocV5, ui->dataLocV4, ui->dataLocV3, ui->dataLocV2, ui->dataLocV1 };

    ui->versionToolbox->setCurrentIndex(0);
    for (int v = 0; v <= ENGINE_v1; ++v) {
        exeLocation[v]->setText(appConfig.gameManager[v].exePath);
        dataLocation[v]->setText(appConfig.baseDataManager[v].dataPath);

        disconnect(setExeLocation[v], nullptr, nullptr, nullptr);
        disconnect(setDataLocation[v], nullptr, nullptr, nullptr);

        connect(setExeLocation[v], &QPushButton::clicked, [this, exeLocation, v] {
#if defined(Q_OS_WIN)
            QFileDialog filedialog(this, tr("Open Executable"), "",
                                   "Windows Executables (*.exe);;All Files (*)");
#elif defined(Q_OS_MACOS)
            QFileDialog filedialog(this, tr("Open Executable"), "",
                                   "Mac OS Executables (*.app);;All Files (*)");
#else
            QFileDialog filedialog(this, tr("Open Executable"), "",
                                   "All Files (*)");
#endif
            filedialog.setAcceptMode(QFileDialog::AcceptOpen);
            if (filedialog.exec() == QDialog::Accepted) {
                QString exePath = filedialog.selectedFiles()[0];
                exeLocation[v]->setText(exePath);

                appConfig.gameManager[v].exePath = exePath;
                appConfig.write();
            }
        });

        connect(setDataLocation[v], &QPushButton::clicked, [this, dataLocation, v] {
            QFileDialog filedialog(this, tr("Open Directory"), "", "");
            filedialog.setFileMode(QFileDialog::FileMode::Directory);
            filedialog.setAcceptMode(QFileDialog::AcceptOpen);
            if (filedialog.exec() == QDialog::Accepted) {
                QString dataPath = filedialog.selectedFiles()[0];
                dataLocation[v]->setText(dataPath);
                appConfig.baseDataManager[v].dataPath = dataPath;
                appConfig.write();
            }
        });

        connect(dataLocation[v], &QLineEdit::textEdited, [dataLocation, v](QString s){
            dataLocation[v]->setText(s);
            appConfig.baseDataManager[v].dataPath = s;
            appConfig.write();
        });
    }
}

GameManager::~GameManager() { delete ui; }
