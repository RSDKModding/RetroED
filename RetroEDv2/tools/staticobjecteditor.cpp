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
            writer.writeLine(QString("struct %1 {").arg(m_info.m_name));
            for (int a = 0; a < m_staticObject.m_arrays.count(); ++a) {
                QString type = "unknown";
                switch (m_staticObject.m_arrays[a].m_type) {
                    case RSDKv5::AttributeTypes::UINT8: type = "byte"; break;
                    case RSDKv5::AttributeTypes::UINT16: type = "ushort"; break;
                    case RSDKv5::AttributeTypes::UINT32: type = "uint"; break;
                    case RSDKv5::AttributeTypes::INT8: type = "byte"; break;
                    case RSDKv5::AttributeTypes::INT16: type = "short"; break;
                    case RSDKv5::AttributeTypes::INT32: type = "int"; break;
                    case RSDKv5::AttributeTypes::ENUM: type = "var"; break;
                    case RSDKv5::AttributeTypes::BOOL: type = "bool"; break;
                    case RSDKv5::AttributeTypes::STRING: type = "bool"; break;
                    case RSDKv5::AttributeTypes::VECTOR2: type = "Vector2"; break;
                    case RSDKv5::AttributeTypes::UNKNOWN: type = "unknownType"; break;
                    case RSDKv5::AttributeTypes::COLOR: type = "colour"; break;
                }

                QString arrName = m_info.m_arrays[a].m_name;
                arrName         = arrName.replace(" ", "");
                if (m_staticObject.m_arrays[a].m_values.count() > 1) {
                    writer.writeText(QString("\t%1 %2[%3] = {")
                                         .arg(type)
                                         .arg(arrName)
                                         .arg(m_staticObject.m_arrays[a].m_values.count()));
                    for (int v = 0; v < m_staticObject.m_arrays[a].m_values.count(); ++v) {
                        writer.writeText(QString::number(m_staticObject.m_arrays[a].m_values[v]));
                        if (v + 1 < m_staticObject.m_arrays[a].m_values.count())
                            writer.writeText(", ");
                    }
                    writer.writeLine("};");
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
            m_info.m_arrays[index.parent().row()].m_values[index.row()] = item->text();
            return;
        }
        m_info.m_arrays[index.row()].m_name = item->text();
    });

    ui->arrTree->setModel(m_arrayModel);

    connect(ui->arrTree->selectionModel(), &QItemSelectionModel::currentChanged,
            [this](const QModelIndex &c) {
                disconnect(ui->arrValName, nullptr, nullptr, nullptr);
                disconnect(ui->arrValue, nullptr, nullptr, nullptr);

                ui->arrValName->setDisabled(!c.parent().isValid());
                ui->arrValue->setDisabled(!c.parent().isValid());

                if (!c.isValid())
                    return;

                if (c.parent().isValid()) {
                    ui->arrValName->setDisabled(c.row()
                                                >= m_info.m_arrays[c.parent().row()].m_values.count());
                    ui->arrValue->setDisabled(
                        c.row() >= m_staticObject.m_arrays[c.parent().row()].m_values.count());

                    if (c.row() >= m_staticObject.m_arrays[c.parent().row()].m_values.count())
                        return;

                    ui->arrValName->setText(m_info.m_arrays[c.parent().row()].m_values[c.row()]);
                    ui->arrValue->setValue(m_staticObject.m_arrays[c.parent().row()].m_values[c.row()]);

                    connect(ui->arrValName, &QLineEdit::textEdited, [this, c](QString s) {
                        m_info.m_arrays[c.parent().row()].m_values[c.row()] = s;
                    });

                    connect(ui->arrValue, QOverload<int>::of(&QSpinBox::valueChanged),
                            [this, c](int v) {
                                m_staticObject.m_arrays[c.parent().row()].m_values[c.row()] = v;
                            });
                }

                ui->arrType->setCurrentIndex(m_staticObject.m_arrays[c.row()].m_type);
                ui->arrSize->setValue(m_staticObject.m_arrays[c.row()].m_size);
                ui->arrOffset->setValue(m_staticObject.getOffset(c.row()));
            });

    if (filePath != "") {
        QString selFile = filePath;
        m_staticObject.read(selFile);
        QString infoPath = QFileInfo(selFile).absolutePath() + "/info_" + QFileInfo(selFile).fileName();
        m_info           = StaticObjectInfo();
        if (QFile::exists(infoPath)) {
            m_info.read(infoPath);
        }
        else {
            m_info.m_arrays.clear();
            int aID = 0;
            for (auto &a : m_staticObject.m_arrays) {
                m_info.m_arrays.append(StaticObjectInfo::ArrayInfo());
                m_info.m_arrays.last().m_name = "Array " + QString::number(aID++);
                int vID                       = 0;
                for (auto &v : a.m_values)
                    m_info.m_arrays.last().m_values.append("Value[" + QString::number(vID++) + "]");
            }
            m_info.m_name = QFileInfo(selFile).baseName();
        }

        appConfig.addRecentFile(ENGINE_v5, TOOL_STATICOBJECTEDITOR, selFile, QList<QString>{});
        setupUI();
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
    for (int c = 0; c < m_staticObject.m_arrays.count(); ++c) {
        auto *arrItem = new QStandardItem(m_info.m_arrays[c].m_name);

        for (int v = 0; v < m_staticObject.m_arrays[c].m_values.count(); ++v) {
            auto *valItem =
                new QStandardItem(m_info.m_arrays[c].m_values[v] + ": "
                                  + QString::number(m_staticObject.m_arrays[c].m_values[v]));
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

    ui->objName->setText(m_info.m_name);

    connect(ui->objName, &QLineEdit::textEdited, [this](QString s) { m_info.m_name = s; });
}

bool StaticObjectEditor::event(QEvent *event)
{
    if (event->type() == (QEvent::Type)RE_EVENT_NEW) {
        m_staticObject = RSDKv5::StaticObject();
        m_info         = StaticObjectInfo();
        setupUI();
        return true;
    }
    if (event->type() == (QEvent::Type)RE_EVENT_OPEN) {
        QFileDialog filedialog(this, tr("Open RSDKv5 Static Object"), "",
                               tr("RSDKv5 Static Objects (*.bin)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            QString selFile = filedialog.selectedFiles()[0];
            m_staticObject.read(selFile);
            QString infoPath =
                QFileInfo(selFile).absolutePath() + "/info_" + QFileInfo(selFile).fileName();
            m_info = StaticObjectInfo();
            if (QFile::exists(infoPath)) {
                m_info.read(infoPath);
            }
            else {
                m_info.m_arrays.clear();
                int aID = 0;
                for (auto &a : m_staticObject.m_arrays) {
                    m_info.m_arrays.append(StaticObjectInfo::ArrayInfo());
                    m_info.m_arrays.last().m_name = "Array " + QString::number(aID++);
                    int vID                       = 0;
                    for (auto &v : a.m_values)
                        m_info.m_arrays.last().m_values.append("Value[" + QString::number(vID++) + "]");
                }
                m_info.m_name = QFileInfo(selFile).baseName();
            }

            appConfig.addRecentFile(ENGINE_v5, TOOL_STATICOBJECTEDITOR, selFile, QList<QString>{});
            setupUI();
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE) {
        if (m_staticObject.m_filename != "") {
            m_staticObject.write("");
            QString selFile = m_staticObject.m_filename;
            QString infoPath =
                QFileInfo(selFile).absolutePath() + "/info_" + QFileInfo(selFile).fileName();
            m_info.write(infoPath);
            return true;
        }

        QFileDialog filedialog(this, tr("Save RSDKv5 Static Object"), "",
                               tr("RSDKv5 Static Objects (*.bin)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            m_staticObject.write(filedialog.selectedFiles()[0]);
            QString selFile = filedialog.selectedFiles()[0];
            QString infoPath =
                QFileInfo(selFile).absolutePath() + "/info_" + QFileInfo(selFile).fileName();
            m_info.write(infoPath);

            appConfig.addRecentFile(ENGINE_v5, TOOL_STATICOBJECTEDITOR, selFile, QList<QString>{});
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE_AS) {
        QFileDialog filedialog(this, tr("Save RSDKv5 Static Object"), "",
                               tr("RSDKv5 Static Objects (*.bin)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            m_staticObject.write(filedialog.selectedFiles()[0]);
            QString selFile = filedialog.selectedFiles()[0];
            QString infoPath =
                QFileInfo(selFile).absolutePath() + "/info_" + QFileInfo(selFile).fileName();
            m_info.write(infoPath);

            appConfig.addRecentFile(ENGINE_v5, TOOL_STATICOBJECTEDITOR, selFile, QList<QString>{});
            return true;
        }
    }
    return QWidget::event(event);
}

#include "moc_staticobjecteditor.cpp"
