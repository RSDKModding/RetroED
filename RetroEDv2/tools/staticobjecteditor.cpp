#include "includes.hpp"
#include "ui_staticobjecteditor.h"

StaticObjectEditor::StaticObjectEditor(QString filePath, QWidget *parent)
    : QWidget(parent), ui(new Ui::StaticObjectEditor)
{
    ui->setupUi(this);

    icon_up   = QIcon(Utils::getColouredIcon(":/icons/ic_arrow_upward_48px.svg"));
    icon_down = QIcon(Utils::getColouredIcon(":/icons/ic_arrow_downward_48px.svg"));
    icon_add  = QIcon(Utils::getColouredIcon(":/icons/ic_add_circle_48px.svg"));
    icon_rm   = QIcon(Utils::getColouredIcon(":/icons/ic_cancel_48px.svg"));

    connect(ui->expCHdr, &QPushButton::pressed, [this] {
        QFileDialog filedialog(this, tr("Save Header"), "", tr("C Headers (*.h)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            QString selFile = filedialog.selectedFiles()[0];

            Writer writer(selFile);
            writer.writeLine(QString("struct %1 {").arg(info.m_name));
            for (int a = 0; a < staticObject.values.count(); ++a) {
                QString type = "unknown";
                switch (staticObject.values[a].type & 0x7F) {
                    default: type = "invalid"; break;
                    case RSDKv5::StaticObject::SVAR_UINT8: type = "byte"; break;
                    case RSDKv5::StaticObject::SVAR_UINT16: type = "ushort"; break;
                    case RSDKv5::StaticObject::SVAR_UINT32: type = "uint"; break;
                    case RSDKv5::StaticObject::SVAR_INT8: type = "sbyte"; break;
                    case RSDKv5::StaticObject::SVAR_INT16: type = "short"; break;
                    case RSDKv5::StaticObject::SVAR_INT32: type = "int"; break;
                    case RSDKv5::StaticObject::SVAR_BOOL: type = "bool"; break;
                    case RSDKv5::StaticObject::SVAR_PTR: type = "void*"; break;
                    case RSDKv5::StaticObject::SVAR_VEC2: type = "Vector2"; break;
                    case RSDKv5::StaticObject::SVAR_TEXT: type = "TextInfo"; break;
                    case RSDKv5::StaticObject::SVAR_ANIMATOR: type = "Animator"; break;
                    case RSDKv5::StaticObject::SVAR_HITBOX: type = "Hitbox"; break;
                    case RSDKv5::StaticObject::SVAR_UNKNOWN: type = "Unknown"; break;
                }

                QString arrName = info.values[a].m_name;
                arrName         = arrName.replace(" ", "");
                if (staticObject.values[a].entries.count() > 1) {
                    writer.writeText(QString("\t%1 %2[%3] = {")
                                         .arg(type)
                                         .arg(arrName)
                                         .arg(staticObject.values[a].entries.count()));
                    for (int v = 0; v < staticObject.values[a].entries.count(); ++v) {
                        writer.writeText(QString::number(staticObject.values[a].entries[v]));
                        if (v + 1 < staticObject.values[a].entries.count())
                            writer.writeText(", ");
                    }
                    writer.writeLine("};");
                }
                else if (staticObject.values[a].size > 1) {
                    writer.writeLine(QString("\t%1 %2[%3];")
                                         .arg(type)
                                         .arg(arrName)
                                         .arg(staticObject.values[a].size));
                }
                else if (staticObject.values[a].entries.count() == 1) {
                    writer.writeLine(QString("\t%1 %2 = %3;")
                                         .arg(type)
                                         .arg(arrName)
                                         .arg(staticObject.values[a].entries[0]));
                }
                else {
                    writer.writeLine(QString("\t%1 %2;").arg(type).arg(arrName));
                }
            }
            writer.writeLine("};");
            writer.flush();
        }
    });

    m_arrayModel = new QStandardItemModel(ui->arrTree);

    connect(m_arrayModel, &QStandardItemModel::itemChanged, [this](QStandardItem *item) {
        const QModelIndex &index = m_arrayModel->indexFromItem(item);
        if (index.parent().isValid()) { // Value
            info.values[index.parent().row()].entries[index.row()] = item->text();
            return;
        }
        info.values[index.row()].m_name = item->text();
    });

    ui->arrTree->setModel(m_arrayModel);

    connect(
        ui->arrTree->selectionModel(), &QItemSelectionModel::currentChanged,
        [this](const QModelIndex &c) {
            disconnect(ui->arrValName, nullptr, nullptr, nullptr);
            disconnect(ui->arrValue, nullptr, nullptr, nullptr);

            ui->arrValName->setDisabled(!c.parent().isValid());
            ui->arrValue->setDisabled(!c.parent().isValid());

            if (!c.isValid())
                return;

            if (c.parent().isValid()) {
                ui->arrValName->setDisabled(c.row() >= info.values[c.parent().row()].entries.count());
                ui->arrValue->setDisabled(c.row()
                                          >= staticObject.values[c.parent().row()].entries.count());

                if (c.row() >= staticObject.values[c.parent().row()].entries.count())
                    return;

                ui->arrValName->setText(info.values[c.parent().row()].entries[c.row()]);
                ui->arrValue->setValue(staticObject.values[c.parent().row()].entries[c.row()]);

                connect(ui->arrValName, &QLineEdit::textEdited,
                        [this, c](QString s) { info.values[c.parent().row()].entries[c.row()] = s; });

                connect(ui->arrValue, QOverload<int>::of(&QSpinBox::valueChanged), [this, c](int v) {
                    staticObject.values[c.parent().row()].entries[c.row()] = v;
                });
            }

            ui->arrType->setCurrentIndex(staticObject.values[c.row()].type);
            ui->arrSize->setValue(staticObject.values[c.row()].size);
            ui->arrOffset->setValue(staticObject.getOffset(c.row()));
        });

    if (filePath != "") {
        QString selFile = filePath;
        staticObject.read(selFile);
        setupInfo(selFile);

        appConfig.addRecentFile(ENGINE_v5, TOOL_STATICOBJECTEDITOR, selFile, QList<QString>{});
        setupUI();
    }
}


void StaticObjectEditor::setupInfo(QString selFile) {
    QString infoPath = QFileInfo(selFile).absolutePath() + "/info_" + QFileInfo(selFile).fileName();
    info             = StaticObjectInfo();
    if (QFile::exists(infoPath)) {
        info.read(infoPath);
    }
    else {
        info.values.clear();
        int aID = 0;
        for (auto &a : staticObject.values) {
            info.values.append(StaticObjectInfo::ArrayInfo());
            info.values.last().m_name = "Array " + QString::number(aID++);
            for (int vID = 0; vID < a.entries.size(); vID++)
                info.values.last().entries.append("Value[" + QString::number(vID) + "]");
        }
        info.m_name = QFileInfo(selFile).baseName();
    }
}

StaticObjectEditor::~StaticObjectEditor()
{
    appConfig.write(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                    + "/appConfig.bin");
    delete ui;
}

void StaticObjectEditor::setupUI()
{

    // Disconnect
    disconnect(ui->objName, nullptr, nullptr, nullptr);

    disconnect(ui->arrTree, nullptr, nullptr, nullptr);
    disconnect(ui->arrValName, nullptr, nullptr, nullptr);
    disconnect(ui->arrValue, nullptr, nullptr, nullptr);

    // Set Values
    m_arrayModel->clear();
    for (int c = 0; c < staticObject.values.count(); ++c) {
        auto *arrItem = new QStandardItem(info.values[c].m_name);

        for (int v = 0; v < staticObject.values[c].entries.count(); ++v) {
            auto *valItem = new QStandardItem(info.values[c].entries[v] + ": "
                                              + QString::number(staticObject.values[c].entries[v]));
            arrItem->appendRow(valItem);

            valItem->setFlags(valItem->flags() | Qt::ItemIsEditable);
        }

        arrItem->setFlags(arrItem->flags() | Qt::ItemIsEditable);
        m_arrayModel->appendRow(arrItem);
    }
    ui->arrValName->setText("");
    ui->arrValue->setValue(0x00);
    ui->arrType->setCurrentIndex(0x00);
    ui->arrSize->setValue(0x00);
    ui->arrOffset->setValue(0x00);

    ui->objName->setText(info.m_name);

    connect(ui->objName, &QLineEdit::textEdited, [this](QString s) { info.m_name = s; });
}

bool StaticObjectEditor::event(QEvent *event)
{
    if (event->type() == (QEvent::Type)RE_EVENT_NEW) {
        staticObject = RSDKv5::StaticObject();
        info         = StaticObjectInfo();
        setupUI();
        return true;
    }
    if (event->type() == (QEvent::Type)RE_EVENT_OPEN) {
        QFileDialog filedialog(this, tr("Open RSDKv5 Static Object"), "",
                               tr("RSDKv5 Static Objects (*.bin)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            QString selFile = filedialog.selectedFiles()[0];
            staticObject.read(selFile);
            setupInfo(selFile);

            appConfig.addRecentFile(ENGINE_v5, TOOL_STATICOBJECTEDITOR, selFile, QList<QString>{});
            setupUI();
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE) {
        if (staticObject.m_filename != "") {
            staticObject.write("");
            QString selFile = staticObject.m_filename;
            QString infoPath =
                QFileInfo(selFile).absolutePath() + "/info_" + QFileInfo(selFile).fileName();
            info.write(infoPath);
            return true;
        }

        QFileDialog filedialog(this, tr("Save RSDKv5 Static Object"), "",
                               tr("RSDKv5 Static Objects (*.bin)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            staticObject.write(filedialog.selectedFiles()[0]);
            QString selFile = filedialog.selectedFiles()[0];
            QString infoPath =
                QFileInfo(selFile).absolutePath() + "/info_" + QFileInfo(selFile).fileName();
            info.write(infoPath);

            appConfig.addRecentFile(ENGINE_v5, TOOL_STATICOBJECTEDITOR, selFile, QList<QString>{});
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE_AS) {
        QFileDialog filedialog(this, tr("Save RSDKv5 Static Object"), "",
                               tr("RSDKv5 Static Objects (*.bin)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            staticObject.write(filedialog.selectedFiles()[0]);
            QString selFile = filedialog.selectedFiles()[0];
            QString infoPath =
                QFileInfo(selFile).absolutePath() + "/info_" + QFileInfo(selFile).fileName();
            info.write(infoPath);

            appConfig.addRecentFile(ENGINE_v5, TOOL_STATICOBJECTEDITOR, selFile, QList<QString>{});
            return true;
        }
    }
    return QWidget::event(event);
}

#include "moc_staticobjecteditor.cpp"
