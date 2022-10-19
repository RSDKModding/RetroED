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
    QLineEdit *exeLocation[] = { ui->exeLocV5, ui->exeLocV4, ui->exeLocV3, ui->exeLocV2, ui->exeLocV1 };

    QPushButton *runGame[] = { ui->runGameV5, ui->runGameV4, ui->runGameV3, ui->runGameV2,
                               ui->runGameV1 };

    ui->versionToolbox->setCurrentIndex(0);
    for (int v = 0; v <= ENGINE_v1; ++v) {
        exeLocation[v]->setText(appConfig.gameManager[v].exePath);

        disconnect(setExeLocation[v], nullptr, nullptr, nullptr);
        connect(setExeLocation[v], &QPushButton::clicked, [this, exeLocation, v] {
#if defined(Q_OS_WIN)
            QFileDialog filedialog(this, tr("Open Executable"), "",
                                   "Windows Executables (*.exe);;All Files (*)");
#elif defined(Q_OS_MAC)
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

#ifndef Q_NO_PROCESS
        disconnect(runGame[v], nullptr, nullptr, nullptr);
        connect(runGame[v], &QPushButton::clicked, [v] {
            QString gamePath = appConfig.gameManager[v].exePath;
            if (QFile::exists(gamePath)) {
                QStringList args;
                args << "console=true;";
                if (argInitStage.length())
                    args << QString("stage=%1;").arg(argInitStage);
                if (argInitScene.length())
                    args << QString("scene=%1;").arg(argInitScene);
                if (argInitFilter.length())
                    args << QString("filter=%1;").arg(argInitFilter);
                QProcess proc;
                proc.setProgram(gamePath);
                proc.setWorkingDirectory(QFileInfo(gamePath).absolutePath());
                proc.setArguments(args);
                proc.startDetached();
                proc.waitForStarted();
            }
        });
#else
        runGame[v]->setVisible(false);
#endif
    }
}

GameManager::~GameManager() { delete ui; }
