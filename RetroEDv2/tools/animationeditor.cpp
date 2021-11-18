#include "includes.hpp"
#include "ui_animationeditor.h"

#include "qgifimage.h"

enum SolFrameListRole { ROLE_PIXMAP = Qt::UserRole + 278 };

class FrameListDelegate : public QStyledItemDelegate
{
public:
    FrameListDelegate(QWidget *parent = nullptr) : QStyledItemDelegate(parent) {}

protected:
    void paint(QPainter *p, const QStyleOptionViewItem &opt, const QModelIndex &index) const override
    {
        p->save();
        const QPixmap &sheet = index.data(ROLE_PIXMAP).value<QPixmap>();
        if (opt.state.testFlag(QStyle::State_Selected)) {
            p->setPen(Qt::NoPen);
            p->setBrush(qApp->palette().highlight());
            p->drawRect(opt.rect);
        }
        QStyleOptionButton but;
        but.state |= QStyle::State_On;
        but.state |= QStyle::State_Enabled;
        but.state |= QStyle::State_HasFocus;
        but.state &= ~QStyle::State_Sunken;
        but.state |= QStyle::State_Raised;

        but.rect = opt.rect;
        qApp->style()->drawControl(QStyle::CE_PushButton, &but, p);
        QRect pixRect    = opt.rect.adjusted(8, 8, -8, -8);
        const QSize size = sheet.size().scaled(pixRect.size(), Qt::KeepAspectRatio);
        p->drawPixmap(
            (size.height() > size.width()) ? pixRect.center().x() - (size.width() / 2) : pixRect.x(),
            (size.height() > size.width()) ? pixRect.y() : pixRect.center().y() - (size.height() / 2),
            size.width(), size.height(), sheet);
        if (opt.state.testFlag(QStyle::State_Selected)) {
            p->setPen(Qt::NoPen);
            p->setBrush(qApp->palette().highlight());
            p->setOpacity(0.7);
            p->setCompositionMode(QPainter::CompositionMode_Overlay);
            p->drawRect(opt.rect);
        }
        p->restore();
    };
    QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const override
    {
        return QSize(128, 128);
    }
};

AnimationEditor::AnimationEditor(QString filepath, byte type, QWidget *parent)
    : QWidget(parent), ui(new Ui::AnimationEditor)
{
    ui->setupUi(this);

    QStandardItemModel *model = new QStandardItemModel;

    if (updateTimer)
        delete updateTimer;

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, QOverload<>::of(&AnimationEditor::processAnimation));

    ui->frameList->setModel(model);
    ui->frameList->setFlow(QListView::LeftToRight);
    ui->frameList->setWrapping(false);
    ui->frameList->setResizeMode(QListWidget::Adjust);
    ui->frameList->setGridSize(QSize(128, 128));
    ui->frameList->setDragEnabled(false);
    ui->frameList->setItemDelegate(new FrameListDelegate);

    auto setupSheetBox = [this] {
        ui->sheetID->clear();
        ui->sheetID->blockSignals(true);
        for (QString &s : animFile.sheets) {
            ui->sheetID->addItem(s);
        }
        ui->sheetID->blockSignals(false);

        if (currentAnim < animCount()) {
            if (currentFrame < frameCount()) {
                ui->sheetID->setCurrentIndex(
                    animFile.animations[currentAnim].frames[currentFrame].sheet);
            }
        }
        else {
            ui->sheetID->setCurrentIndex(0);
        }
    };

    auto setupHiboxTypeBox = [this] {
        ui->hitboxType->clear();
        for (int i = 0; i < animFile.hitboxTypes.count(); ++i) {
            ui->hitboxType->addItem(animFile.hitboxTypes[i]);
        }
    };

    setupSheetBox();
    setupHiboxTypeBox();
    currentHitbox = 0; // temp

    auto setFramePreview = [this, model] {
        if (currentFrame < frameCount()) {
            FormatHelpers::Animation::Frame &f = animFile.animations[currentAnim].frames[currentFrame];
            QRect boundingRect;
            boundingRect.setX(f.sprX);
            boundingRect.setY(f.sprY);
            boundingRect.setWidth(f.width);
            boundingRect.setHeight(f.height);

            model->itemFromIndex(model->index(currentFrame, 0))
                ->setData(QPixmap::fromImage((f.width == 0 || f.height == 0)
                                                 ? QImage(":/icons/missing.png")
                                                 : sheets[f.sheet].copy(boundingRect)),
                          ROLE_PIXMAP);
        }
    };

    std::function<void(int)> animFunc([this, setFramePreview, model](int c) {
        std::function<void(int)> frameFunc([this, setFramePreview](int c) {
            disconnect(ui->sheetID, nullptr, nullptr, nullptr);
            disconnect(ui->boundingBoxX, nullptr, nullptr, nullptr);
            disconnect(ui->boundingBoxY, nullptr, nullptr, nullptr);
            disconnect(ui->boundingBoxW, nullptr, nullptr, nullptr);
            disconnect(ui->boundingBoxH, nullptr, nullptr, nullptr);
            disconnect(ui->selBoundBox, nullptr, nullptr, nullptr);
            disconnect(ui->offsetX, nullptr, nullptr, nullptr);
            disconnect(ui->offsetY, nullptr, nullptr, nullptr);
            disconnect(ui->hitboxL, nullptr, nullptr, nullptr);
            disconnect(ui->hitboxR, nullptr, nullptr, nullptr);
            disconnect(ui->hitboxB, nullptr, nullptr, nullptr);
            disconnect(ui->hitboxT, nullptr, nullptr, nullptr);
            disconnect(ui->duration, nullptr, nullptr, nullptr);
            disconnect(ui->id, nullptr, nullptr, nullptr);

            ui->sheetID->setCurrentIndex(-1);
            ui->boundingBoxX->setValue(0);
            ui->boundingBoxY->setValue(0);
            ui->boundingBoxW->setValue(0);
            ui->boundingBoxH->setValue(0);
            ui->offsetX->setValue(0);
            ui->offsetY->setValue(0);
            ui->hitboxL->setValue(0);
            ui->hitboxR->setValue(0);
            ui->hitboxB->setValue(0);
            ui->hitboxT->setValue(0);
            ui->duration->setValue(0);
            ui->id->setValue(0);

            bool invalid = c == -1 || c >= frameCount();

            ui->sheetID->setDisabled(invalid);
            ui->boundingBoxX->setDisabled(invalid);
            ui->boundingBoxY->setDisabled(invalid);
            ui->boundingBoxW->setDisabled(invalid);
            ui->boundingBoxH->setDisabled(invalid);
            ui->selBoundBox->setDisabled(invalid);
            ui->offsetX->setDisabled(invalid);
            ui->offsetY->setDisabled(invalid);
            ui->hitboxType->setDisabled(invalid);
            ui->hitboxL->setDisabled(invalid);
            ui->hitboxR->setDisabled(invalid);
            ui->hitboxB->setDisabled(invalid);
            ui->hitboxT->setDisabled(invalid);
            ui->duration->setDisabled(invalid || aniType != ENGINE_v5);
            ui->id->setDisabled(invalid || aniType != ENGINE_v5);

            ui->copyFrame->setDisabled(invalid);

            if (!invalid) {
                ui->properties->setItemText(1,
                                            QString("Frame (%1 of %2)")
                                                .arg(c)
                                                .arg(animFile.animations[currentAnim].frames.count()));
            }
            else {
                ui->properties->setItemText(1, "Frame");
            }

            if (invalid) {
                updateView();
                return;
            }

            currentFrame = c;

            FormatHelpers::Animation::Frame &f = animFile.animations[currentAnim].frames[c];

            ui->sheetID->setCurrentIndex(f.sheet);
            ui->boundingBoxX->setValue(f.sprX);
            ui->boundingBoxY->setValue(f.sprY);
            ui->boundingBoxW->setValue(f.width);
            ui->boundingBoxH->setValue(f.height);
            ui->offsetX->setValue(f.pivotX);
            ui->offsetY->setValue(f.pivotY);
            ui->hitboxType->setCurrentIndex(0);
            if (ui->hitboxType->currentIndex() >= 0
                && ui->hitboxType->currentIndex() < f.hitboxes.count()) {
                ui->hitboxL->setValue(f.hitboxes[ui->hitboxType->currentIndex()].left);
                ui->hitboxT->setValue(f.hitboxes[ui->hitboxType->currentIndex()].top);
                ui->hitboxR->setValue(f.hitboxes[ui->hitboxType->currentIndex()].right);
                ui->hitboxB->setValue(f.hitboxes[ui->hitboxType->currentIndex()].bottom);
            }
            else {
                ui->hitboxL->setDisabled(true);
                ui->hitboxT->setDisabled(true);
                ui->hitboxR->setDisabled(true);
                ui->hitboxB->setDisabled(true);
            }
            ui->duration->setValue(f.duration);
            ui->id->setValue(f.id);

            connect(ui->sheetID, QOverload<int>::of(&QComboBox::currentIndexChanged),
                    [&f, this, setFramePreview](int v) {
                        if (v < 0)
                            return;
                        f.sheet = (byte)v;
                        setFramePreview();
                        updateView();
                        updateTitle(true);
                    });

            connect(ui->boundingBoxX, QOverload<int>::of(&QSpinBox::valueChanged),
                    [&f, this, setFramePreview](int v) {
                        f.sprX = (ushort)v;
                        setFramePreview();
                        updateView();
                        updateTitle(true);
                    });
            connect(ui->boundingBoxY, QOverload<int>::of(&QSpinBox::valueChanged),
                    [&f, this, setFramePreview](int v) {
                        f.sprY = (ushort)v;
                        setFramePreview();
                        updateView();
                        updateTitle(true);
                    });
            connect(ui->boundingBoxW, QOverload<int>::of(&QSpinBox::valueChanged),
                    [&f, this, setFramePreview](int v) {
                        f.width = (ushort)v;
                        setFramePreview();
                        updateView();
                        updateTitle(true);
                    });
            connect(ui->boundingBoxH, QOverload<int>::of(&QSpinBox::valueChanged),
                    [&f, this, setFramePreview](int v) {
                        f.height = (ushort)v;
                        setFramePreview();
                        updateView();
                        updateTitle(true);
                    });
            connect(ui->selBoundBox, &QToolButton::clicked, [&f, setFramePreview, this] {
                auto *sel = new AnimSheetSelector(animFile.sheets[f.sheet], &sheets[f.sheet]);
                sel->exec();
                if (sel->returnRect.x < 0 || sel->returnRect.y < 0 || sel->returnRect.w < 0
                    || sel->returnRect.h < 0) {
                    // error!
                }
                else {
                    f.sprX   = sel->returnRect.x;
                    f.sprY   = sel->returnRect.y;
                    f.width  = sel->returnRect.w;
                    f.height = sel->returnRect.h;
                    f.pivotX = -(f.width / 2);
                    f.pivotY = -(f.height / 2);
                    setFramePreview();
                    updateView();
                    updateTitle(true);
                }
            });

            connect(ui->offsetX, QOverload<int>::of(&QSpinBox::valueChanged), [&f, this](int v) {
                f.pivotX = (short)v;
                updateView();
                updateTitle(true);
            });
            connect(ui->offsetY, QOverload<int>::of(&QSpinBox::valueChanged), [&f, this](int v) {
                f.pivotY = (short)v;
                updateView();
                updateTitle(true);
            });

            connect(ui->duration, QOverload<int>::of(&QSpinBox::valueChanged), [&f, this](int v) {
                f.duration = (ushort)v;
                updateTitle(true);
            });

            connect(ui->hitboxType, QOverload<int>::of(&QComboBox::currentIndexChanged),
                    [&f, this](int v) {
                        if (v < 0)
                            return;
                        currentHitbox = (ushort)v;

                        if (v >= 0 && v < f.hitboxes.count()) {
                            ui->hitboxL->blockSignals(true);
                            ui->hitboxT->blockSignals(true);
                            ui->hitboxR->blockSignals(true);
                            ui->hitboxB->blockSignals(true);

                            ui->hitboxL->setValue(f.hitboxes[v].left);
                            ui->hitboxT->setValue(f.hitboxes[v].top);
                            ui->hitboxR->setValue(f.hitboxes[v].right);
                            ui->hitboxB->setValue(f.hitboxes[v].bottom);

                            ui->hitboxL->blockSignals(false);
                            ui->hitboxT->blockSignals(false);
                            ui->hitboxR->blockSignals(false);
                            ui->hitboxB->blockSignals(false);
                        }

                        updateView();
                        updateTitle(true);
                    });

            connect(ui->hitboxL, QOverload<int>::of(&QSpinBox::valueChanged), [&f, this](int v) {
                if (ui->hitboxType->currentIndex() >= 0
                    && ui->hitboxType->currentIndex() < f.hitboxes.count()) {
                    f.hitboxes[ui->hitboxType->currentIndex()].left = (short)v;
                    updateView();
                    updateTitle(true);
                }
            });
            connect(ui->hitboxT, QOverload<int>::of(&QSpinBox::valueChanged), [&f, this](int v) {
                if (ui->hitboxType->currentIndex() >= 0
                    && ui->hitboxType->currentIndex() < f.hitboxes.count()) {
                    f.hitboxes[ui->hitboxType->currentIndex()].top = (short)v;
                    updateView();
                    updateTitle(true);
                }
            });
            connect(ui->hitboxR, QOverload<int>::of(&QSpinBox::valueChanged), [&f, this](int v) {
                if (ui->hitboxType->currentIndex() >= 0
                    && ui->hitboxType->currentIndex() < f.hitboxes.count()) {
                    f.hitboxes[ui->hitboxType->currentIndex()].right = (short)v;
                    updateView();
                    updateTitle(true);
                }
            });
            connect(ui->hitboxB, QOverload<int>::of(&QSpinBox::valueChanged), [&f, this](int v) {
                if (ui->hitboxType->currentIndex() >= 0
                    && ui->hitboxType->currentIndex() < f.hitboxes.count()) {
                    f.hitboxes[ui->hitboxType->currentIndex()].bottom = (short)v;
                    updateView();
                    updateTitle(true);
                }
            });

            connect(ui->id, QOverload<int>::of(&QSpinBox::valueChanged), [&f, this](int v) {
                f.id = v;
                updateTitle(true);
            });

            updateView();
        });

        connect(ui->frameList->selectionModel(), &QItemSelectionModel::currentRowChanged,
                [this, model, frameFunc](const QModelIndex &aindex) {
                    ui->upFrame->setDisabled(!aindex.isValid());
                    ui->downFrame->setDisabled(!aindex.isValid());
                    ui->rmFrame->setDisabled(!aindex.isValid());

                    frameFunc(aindex.row());

                    if (!aindex.isValid())
                        return;

                    if (aindex.row() >= 0 && frameCount() > 0) {
                        ui->properties->setCurrentIndex(1);
                    }
                    else {
                        ui->properties->setCurrentIndex(0);
                    }

                    ui->downFrame->setDisabled(aindex.row() == model->rowCount() - 1);
                    ui->upFrame->setDisabled(!aindex.row());
                });

        currentAnim = c;
        disconnect(ui->loopIndex, nullptr, nullptr, nullptr);
        disconnect(ui->rotationStyle, nullptr, nullptr, nullptr);
        disconnect(ui->speedMult, nullptr, nullptr, nullptr);
        disconnect(ui->frameList, nullptr, nullptr, nullptr);

        ui->loopIndex->setDisabled(c == -1);
        ui->rotationStyle->setDisabled(c == -1 || aniType >= ENGINE_v2);
        ui->speedMult->setDisabled(c == -1);
        ui->playerID->setDisabled(c == -1 || aniType != ENGINE_v1);
        ui->copyAnim->setDisabled(c == -1);

        ui->impAnim->setDisabled(c == -1);
        ui->expAnim->setDisabled(c == -1);

        if (c > -1) {
            ui->properties->setItemText(
                0, QString("Animation (%1 of %2)").arg(c).arg(animFile.animations.count()));
        }
        else {
            ui->properties->setItemText(0, "Animation");
        }

        std::function<void(int, QListWidgetItem *)> blankModify;
        ui->frameList->blockSignals(true);
        if (c > -1) {
            disconnect(model, &QStandardItemModel::itemChanged, nullptr, nullptr);

            model->clear();
            uint fID = 0;
            for (FormatHelpers::Animation::Frame &f : animFile.animations[currentAnim].frames) {
                QRect boundingRect;
                boundingRect.setX(f.sprX);
                boundingRect.setY(f.sprY);
                boundingRect.setWidth(f.width);
                boundingRect.setHeight(f.height);

                QStandardItem *item = new QStandardItem;
                item->setEditable(false);
                item->setData(QPixmap::fromImage((f.width == 0 || f.height == 0)
                                                     ? QImage(":/icons/missing.png")
                                                     : sheets[f.sheet].copy(boundingRect)),

                              ROLE_PIXMAP);
                model->appendRow(item);
                ++fID;
                updateView();
            }

            ui->frameList->blockSignals(false);

            connect(model, &QStandardItemModel::itemChanged, [frameFunc, model](QStandardItem *item) {
                frameFunc(model->indexFromItem(item).row());
            });

            bool invalid         = c == -1 || c >= animCount();
            bool durationInvalid = (aniType != ENGINE_v5);

            ui->loopIndex->setDisabled(invalid);
            ui->rotationStyle->setDisabled(invalid || aniType >= ENGINE_v2);
            ui->speedMult->setDisabled(invalid);
            ui->playerID->setDisabled(invalid || aniType != ENGINE_v1);

            ui->sheetID->setDisabled(invalid);
            ui->boundingBoxX->setDisabled(invalid);
            ui->boundingBoxY->setDisabled(invalid);
            ui->boundingBoxW->setDisabled(invalid);
            ui->boundingBoxH->setDisabled(invalid);
            ui->offsetX->setDisabled(invalid);
            ui->offsetY->setDisabled(invalid);
            ui->hitboxL->setDisabled(invalid);
            ui->hitboxR->setDisabled(invalid);
            ui->hitboxB->setDisabled(invalid);
            ui->hitboxT->setDisabled(invalid);
            ui->duration->setDisabled(invalid || durationInvalid);
            ui->id->setDisabled(invalid);

            ui->copyFrame->setDisabled(invalid);
            ui->expFrame->setDisabled(invalid);
            ui->impFrame->setDisabled(invalid);

            if (invalid)
                return;

            ui->loopIndex->setValue(animFile.animations[c].loopIndex);
            ui->rotationStyle->setCurrentIndex(animFile.animations[c].rotationStyle);
            ui->speedMult->setValue(animFile.animations[c].speed);
            ui->playerID->setCurrentIndex(animFile.playerType);

            connect(ui->loopIndex, QOverload<int>::of(&QSpinBox::valueChanged), [this, c](int v) {
                int val = v;
                if (val >= frameCount())
                    v = frameCount() - 1;
                if (val < 0)
                    v = 0;

                if (val != v) {
                    ui->loopIndex->setValue(val);
                }

                animFile.animations[c].loopIndex = (ushort)v;
                updateTitle(true);
            });

            connect(ui->rotationStyle, QOverload<int>::of(&QComboBox::currentIndexChanged),
                    [this, c](int v) {
                        animFile.animations[c].rotationStyle = (byte)v;
                        updateTitle(true);
                    });

            connect(ui->speedMult, QOverload<int>::of(&QSpinBox::valueChanged),
                    [this, c](int v) { animFile.animations[c].speed = (byte)v; });

            currentFrame = 0;
            if (c >= 0 && frameCount() > 0) {
                ui->frameList->setCurrentIndex(model->item(0)->index());
                frameFunc(0);
                // ui->properties->setCurrentIndex(1);
            }
            else {
                frameFunc(-1);
                // ui->properties->setCurrentIndex(0);
            }
            ui->properties->setCurrentIndex(0);
        };
    });

    connect(ui->animationList, &QListWidget::currentRowChanged, animFunc);

    connect(ui->addFrame, &QToolButton::clicked, [this, model] {
        QList<FormatHelpers::Animation::Frame> &f = animFile.animations[currentAnim].frames;
        uint c                                    = ui->frameList->currentIndex().row() + 1;
        FormatHelpers::Animation::Frame frame     = FormatHelpers::Animation::Frame();
        frame.width = frame.height = 0x10;
        frame.hitboxes.clear();
        for (int h = 0; h < animFile.hitboxTypes.count(); ++h) {
            FormatHelpers::Animation::Hitbox::HitboxInfo hitbox;
            frame.hitboxes.append(hitbox);
        }
        f.insert(c, frame);
        QRect boundingRect;
        boundingRect.setX(frame.sprX);
        boundingRect.setY(frame.sprY);
        boundingRect.setWidth(frame.width);
        boundingRect.setHeight(frame.height);

        auto *item = new QStandardItem();
        item->setData(QPixmap::fromImage((frame.width == 0 || frame.height == 0)
                                             ? QImage(":/icons/missing.png")
                                             : sheets[frame.sheet].copy(boundingRect)),
                      ROLE_PIXMAP);
        model->insertRow(c, item);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->frameList->setCurrentIndex(item->index());

        updateView();
        updateTitle(true);
    });

    connect(ui->rmFrame, &QToolButton::clicked, [this, model] {
        int c = ui->frameList->currentIndex().row();
        int n = c == model->rowCount() - 1 ? c - 1 : c;
        model->takeRow(c);
        // delete model->itemFromIndex(ui->frameList->currentIndex());
        animFile.animations[currentAnim].frames.removeAt(c);
        ui->frameList->blockSignals(true);
        ui->frameList->setCurrentIndex(model->index(n, 0));
        ui->frameList->blockSignals(false);
    });

    connect(ui->upFrame, &QToolButton::clicked, [this, model] {
        uint c                      = ui->frameList->currentIndex().row();
        QList<QStandardItem *> item = model->takeRow(c);
        animFile.animations[currentAnim].frames.move(c, c - 1);
        model->insertRow(c - 1, item);
        ui->frameList->setCurrentIndex(model->index(c - 1, 0));
    });

    connect(ui->downFrame, &QToolButton::clicked, [this, model] {
        uint c                      = ui->frameList->currentIndex().row();
        QList<QStandardItem *> item = model->takeRow(c);
        animFile.animations[currentAnim].frames.move(c, c + 1);
        model->insertRow(c + 1, item);
        ui->frameList->setCurrentIndex(model->index(c + 1, 0));
    });

    std::function<void(int)> sourceFunc([this](int c) {
        if (c > -1) {
            ui->properties->setItemText(
                2, QString("Sheets (%1 of %2)").arg(c).arg(animFile.sheets.count()));
        }
        else {
            ui->properties->setItemText(2, "Sheets");
        }

        bool invalid = c == -1 || c >= animFile.sheets.count();

        if (invalid)
            return;
    });

    connect(ui->sourceList, &QListWidget::currentRowChanged, sourceFunc);

    std::function<void(int)> hitboxFunc([this](int c) {
        if (c > -1) {
            ui->properties->setItemText(
                3, QString("Hitbox (%1 of %2)").arg(c).arg(animFile.hitboxTypes.count()));
        }
        else {
            ui->properties->setItemText(3, "Hitbox");
        }

        bool invalid = c == -1 || c >= animFile.hitboxTypes.count();

        ui->hitboxType->setDisabled(invalid);
        ui->hitboxL->setDisabled(invalid);
        ui->hitboxT->setDisabled(invalid);
        ui->hitboxR->setDisabled(invalid);
        ui->hitboxB->setDisabled(invalid);

        ui->addHB->setDisabled(aniType == ENGINE_v1 || animFile.hitboxTypes.count() >= 8);
        ui->rmHB->setDisabled(aniType == ENGINE_v1 || animFile.hitboxTypes.count() < 0);
        ui->upHB->setDisabled(aniType == ENGINE_v1 || c >= animFile.hitboxTypes.count() - 1);
        ui->downHB->setDisabled(aniType == ENGINE_v1 || c <= 0);

        if (invalid)
            return;

        ui->hitboxType->setItemText(c, animFile.hitboxTypes[c]);
    });

    connect(ui->hitboxList, &QListWidget::currentRowChanged, hitboxFunc);

    connect(ui->impAnim, &QToolButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Open Anim"), QFileInfo(animFile.filePath).absolutePath(),
                               tr("json Files (*.json)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() != QDialog::Accepted)
            return;
        QString filename = filedialog.selectedFiles()[0];

        ui->animationList->blockSignals(true);
        FormatHelpers::Animation::AnimationEntry anim;

        // TODO: import json

        uint c     = ui->animationList->currentRow() + 1;
        auto *item = new QListWidgetItem(anim.name);
        ui->animationList->insertItem(c, item);
        updateTitle(true);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        animFile.animations.insert(c, anim);
        ui->animationList->blockSignals(false);
    });

    connect(ui->expAnim, &QToolButton::clicked, [this] {
        if (ui->animationList->currentRow() == -1)
            return;
        QFileDialog filedialog(this, tr("Save Anim"), QFileInfo(animFile.filePath).absolutePath(),
                               tr("json Files (*.json);;RetroScript SpriteFrames (*.txt)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() != QDialog::Accepted)
            return;

        QString selectedFilter = filedialog.selectedNameFilter();
        QString filename       = filedialog.selectedFiles()[0];

        if (selectedFilter == "json Files (*.json)") {
            // export json
        }
        else {
            Writer writer(filename);

            writer.writeLine(QString("// Animation: %1").arg(animFile.animations[currentAnim].name));

            int id = 0;
            for (auto &frame : animFile.animations[currentAnim].frames) {
                writer.writeLine(QString("SpriteFrame(%1, %2, %3, %4, %5, %6) // Frame %7")
                                     .arg(frame.pivotX)
                                     .arg(frame.pivotY)
                                     .arg(frame.width)
                                     .arg(frame.height)
                                     .arg(frame.sprX)
                                     .arg(frame.sprY)
                                     .arg(id++));
            }

            writer.flush();
        }
    });

    connect(ui->impFrame, &QToolButton::clicked, [this, model] {
        QFileDialog filedialog(this, tr("Open Anim"), QFileInfo(animFile.filePath).absolutePath(),
                               tr("json Files (*.json)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() != QDialog::Accepted)
            return;
        QString filename = filedialog.selectedFiles()[0];

        ui->frameList->blockSignals(true);
        FormatHelpers::Animation::Frame frame;

        // TODO: json

        QRect boundingRect;
        boundingRect.setX(frame.sprX);
        boundingRect.setY(frame.sprY);
        boundingRect.setWidth(frame.width);
        boundingRect.setHeight(frame.height);

        uint c     = ui->frameList->currentIndex().row() + 1;
        auto *item = new QStandardItem();
        if (frame.width == 0 || frame.height == 0) {
            item->setIcon(QPixmap::fromImage(QImage(":/icons/missing.png")));
        }
        else {
            item->setIcon(QPixmap::fromImage(sheets[frame.sheet].copy(boundingRect)));
        }
        model->insertRow(c, item);
        updateTitle(true);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        animFile.animations[currentAnim].frames.insert(c, frame);
        ui->frameList->blockSignals(false);
    });

    connect(ui->expFrame, &QToolButton::clicked, [this] {
        if (ui->animationList->currentRow() == -1)
            return;
        QFileDialog filedialog(this, tr("Save Anim"), QFileInfo(animFile.filePath).absolutePath(),
                               tr("json Files (*.json);;RetroScript SpriteFrames (*.txt)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() != QDialog::Accepted)
            return;

        QString selectedFilter = filedialog.selectedNameFilter();
        QString filename       = filedialog.selectedFiles()[0];

        if (selectedFilter == "json Files (*.json)") {
            // export json
        }
        else {
            Writer writer(filename);

            writer.writeLine(QString("// Animation: %1").arg(animFile.animations[currentAnim].name));
            writer.writeLine(QString("SpriteFrame(%1, %2, %3, %4, %5, %6) // Frame %7")
                                 .arg(animFile.animations[currentAnim].frames[currentFrame].pivotX)
                                 .arg(animFile.animations[currentAnim].frames[currentFrame].pivotY)
                                 .arg(animFile.animations[currentAnim].frames[currentFrame].width)
                                 .arg(animFile.animations[currentAnim].frames[currentFrame].height)
                                 .arg(animFile.animations[currentAnim].frames[currentFrame].sprX)
                                 .arg(animFile.animations[currentAnim].frames[currentFrame].sprY)
                                 .arg(currentFrame));

            writer.flush();
        }
    });

    QObject::connect(ui->importSource, &QPushButton::clicked, [this, setupSheetBox] {
        int c = ui->sourceList->currentRow();
        if (c == -1)
            return;

        QFileDialog filedialog(this, tr("Open Image"), "", tr("GIF Images (*.gif)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() != QDialog::Accepted)
            return;
        QString filename = filedialog.selectedFiles()[0];
        QString name     = Utils::getFilenameAndFolder(filename);
        ui->sourceList->item(ui->sourceList->currentRow())->setText(name);

        removeSheet(c);
        loadSheet(filename, c);
        updateTitle(true);

        setupSheetBox();
        updateView();
    });

    connect(ui->addSheet, &QToolButton::clicked, [this, setupSheetBox] {
        QFileDialog filedialog(this, tr("Open Image"), "", tr("GIF Images (*.gif)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() != QDialog::Accepted)
            return;
        QString filename = filedialog.selectedFiles()[0];
        QString name     = Utils::getFilenameAndFolder(filename);

        ui->sourceList->blockSignals(true);
        uint c = ui->sourceList->currentRow() + 1;
        loadSheet(filename, c);
        updateTitle(true);
        auto *item = new QListWidgetItem(name);
        ui->sourceList->insertItem(c, item);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);

        for (int a = 0; a < animCount(); ++a) {
            for (int f = 0; f < animFile.animations[a].frames.count(); ++f) {
                int sheet                              = animFile.animations[a].frames[f].sheet;
                int p                                  = ui->sourceList->currentRow();
                animFile.animations[a].frames[f].sheet = sheet < p    ? sheet
                                                         : sheet >= p ? (sheet + 1)
                                                                      : 0;
            }
        }

        if (currentFrame < frameCount()) {
            ui->sheetID->setCurrentIndex(animFile.animations[currentAnim].frames[currentFrame].sheet);
        }
        ui->sourceList->blockSignals(false);

        setupSheetBox();
        updateView();
    });

    connect(ui->rmSheet, &QToolButton::clicked, [this, setupSheetBox] {
        int c = ui->sourceList->currentRow();
        int n = ui->sourceList->currentRow() == ui->sourceList->count() - 1 ? c - 1 : c;
        delete ui->sourceList->item(c);
        removeSheet(c);
        ui->sourceList->blockSignals(true);
        ui->sourceList->setCurrentRow(n);
        ui->sourceList->blockSignals(false);

        for (int a = 0; a < animCount(); ++a) {
            for (int f = 0; f < animFile.animations[a].frames.count(); ++f) {
                int sheet                              = animFile.animations[a].frames[f].sheet;
                animFile.animations[a].frames[f].sheet = sheet < c   ? sheet
                                                         : sheet > c ? (sheet - 1)
                                                                     : 0;
            }
        }

        setupSheetBox();
        updateView();
        updateTitle(true);
    });

    auto moveSheetFunc = [this, setupSheetBox](sbyte translation) {
        QList<byte> idList;

        for (int s = 0; s < animFile.sheets.count(); ++s) idList.append(s);

        uint c     = ui->sourceList->currentRow();
        auto *item = ui->sourceList->takeItem(c);
        moveSheet(c, c + translation);
        idList.move(c, c + translation);
        ui->sourceList->insertItem(c + translation, item);
        ui->sourceList->setCurrentRow(c + translation);

        for (int a = 0; a < animCount(); ++a) {
            for (int f = 0; f < frameCount(); ++f) {
                animFile.animations[a].frames[f].sheet = idList[animFile.animations[a].frames[f].sheet];
            }
        }

        setupSheetBox();
        updateView();
        updateTitle(true);
    };

    connect(ui->addHB, &QToolButton::clicked, [this, setupHiboxTypeBox] {
        ui->hitboxList->blockSignals(true);
        uint c = ui->hitboxList->currentRow() + 1;
        animFile.hitboxTypes.insert(c, "Hitbox " + QString::number(c));
        hitboxVisible.insert(c, false);
        auto *item = new QListWidgetItem(animFile.hitboxTypes[c]);
        ui->hitboxList->insertItem(c, item);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->hitboxList->setCurrentItem(item);

        ui->hitboxType->blockSignals(true);
        ui->hitboxType->setCurrentIndex(c);
        currentHitbox = c;
        ui->hitboxType->blockSignals(false);

        for (int a = 0; a < animCount(); ++a) {
            for (int f = 0; f < animFile.animations[a].frames.count(); ++f) {
                FormatHelpers::Animation::Hitbox::HitboxInfo hitbox;
                animFile.animations[a].frames[f].hitboxes.insert(c, hitbox);
            }
        }
        ui->hitboxList->blockSignals(false);

        ui->addHB->setDisabled(animFile.hitboxes.count() >= 8);

        setupHiboxTypeBox();
        updateView();
        updateTitle(true);
    });

    connect(ui->rmHB, &QToolButton::clicked, [this, setupHiboxTypeBox] {
        int c = ui->hitboxList->currentRow();
        int n = ui->hitboxList->currentRow() == ui->sourceList->count() - 1 ? c - 1 : c;
        delete ui->hitboxList->item(c);
        animFile.hitboxes.removeAt(c);
        animFile.hitboxTypes.removeAt(c);
        ui->hitboxList->blockSignals(true);
        ui->hitboxList->setCurrentRow(n);
        ui->hitboxList->blockSignals(false);

        // ui->hitboxType->blockSignals(true);
        // ui->hitboxType->setCurrentIndex(c);
        // mainView->currentHitbox = c;
        // ui->hitboxType->blockSignals(false);

        for (int a = 0; a < animCount(); ++a) {
            for (int f = 0; f < animFile.animations[a].frames.count(); ++f) {
                animFile.animations[a].frames[f].hitboxes.removeAt(c);
            }
        }

        ui->addHB->setDisabled(animFile.hitboxes.count() <= 0);

        setupHiboxTypeBox();
        updateView();
        updateTitle(true);
    });

    auto moveHB = [this, setupHiboxTypeBox](char translation) {
        uint c     = ui->hitboxList->currentRow();
        auto *item = ui->hitboxList->takeItem(c);
        animFile.hitboxes.move(c, c + translation);
        animFile.hitboxTypes.move(c, c + translation);
        ui->hitboxList->insertItem(c + translation, item);
        ui->hitboxList->setCurrentRow(c + translation);

        for (int a = 0; a < animCount(); ++a) {
            for (int f = 0; f < animFile.animations[a].frames.count(); ++f) {
                animFile.animations[a].frames[f].hitboxes.move(c, c + translation);
            }
        }

        setupHiboxTypeBox();
        updateView();
        updateTitle(true);
    };

    connect(ui->upSheet, &QToolButton::clicked, [moveSheetFunc] { moveSheetFunc(-1); });

    connect(ui->downSheet, &QToolButton::clicked, [moveSheetFunc] { moveSheetFunc(1); });

    connect(ui->upHB, &QToolButton::clicked, [moveHB] { moveHB(-1); });

    connect(ui->downHB, &QToolButton::clicked, [moveHB] { moveHB(1); });

    ui->showTransClr->setDown(showTransparentClr);
    connect(ui->showTransClr, &QToolButton::clicked, [this] {
        showTransparentClr = !showTransparentClr;
        ui->showTransClr->setDown(showTransparentClr);
        updateView();
    });

    connect(ui->hitboxList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        int c = ui->hitboxList->row(item);
        if ((uint)c < (uint)animFile.hitboxTypes.count()) {
            hitboxVisible[c] = item->checkState() == Qt::Checked;
            updateView();
        }
    });

    connect(ui->play, &QToolButton::clicked, [this] {
        if (currentAnim < animCount()) {
            if (!playingAnim) {
                startAnim();
            }
            else {
                stopAnim();
                setupUI();
            }

            if (currentFrame >= frameCount()) {
                currentFrame = 0;
            }
        }
        // ui->play->setIcon(playPauseIco[playingAnim]);
    });

    connect(ui->prevFrame, &QToolButton::clicked, [this, model] {
        if (currentAnim < animCount()) {
            if (currentFrame > 0) {
                --currentFrame;
            }
            else {
                currentFrame = (ushort)(frameCount() - 1);
            }
            ui->frameList->setCurrentIndex(model->index(currentFrame, 0));
        }
    });

    connect(ui->nextFrame, &QToolButton::clicked, [this, model] {
        if (currentAnim < animCount()) {
            if (currentFrame < frameCount() - 1) {
                ++currentFrame;
            }
            else {
                currentFrame = 0;
            }
            ui->frameList->setCurrentIndex(model->index(currentFrame, 0));
        }
    });

    connect(ui->skipStart, &QToolButton::clicked, [this, model] {
        if (currentAnim < animCount()) {
            currentFrame = 0;
            ui->frameList->setCurrentIndex(model->index(currentFrame, 0));
        }
    });

    connect(ui->skipEnd, &QToolButton::clicked, [this, model] {
        if (currentAnim < animCount()) {
            currentFrame = (ushort)(frameCount() - 1);
            ui->frameList->setCurrentIndex(model->index(currentFrame, 0));
        }
    });

    connect(ui->copyAnim, &QToolButton::clicked, [this] {
        int c = ui->animationList->currentRow() + 1;
        if (currentAnim < animCount()) {
            ui->animationList->blockSignals(true);
            FormatHelpers::Animation::AnimationEntry anim = animFile.animations[currentAnim];
            animFile.animations.insert(c, anim);
            auto *item = new QListWidgetItem();
            item->setText(anim.name);
            ui->animationList->insertItem(c, item);
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            ui->animationList->blockSignals(false);
            ui->animationList->setCurrentRow(c);
        }
    });

    connect(ui->copyFrame, &QToolButton::clicked, [this] {
        ui->frameList->blockSignals(true);
        int c = ui->frameList->currentIndex().row() + 1;
        if (currentAnim < animCount()) {
            if (currentFrame < frameCount()) {
                FormatHelpers::Animation::Frame frame =
                    animFile.animations[currentAnim].frames[currentFrame];
                animFile.animations[currentAnim].frames.insert(c, frame);
            }
        }
        ui->frameList->blockSignals(false);
    });

    connect(ui->bgColour, &QToolButton::clicked, [this] {
        ColourDialog dlg(bgColour);
        if (dlg.exec() == QDialog::Accepted) {
            bgColour = dlg.colour().toQColor();
            updateView();
        }
    });

    if (QFile::exists(filepath)) {
        loadAnim(filepath, type);
    }

    stopAnim();
    setupUI();
}

AnimationEditor::~AnimationEditor()
{
    if (updateTimer) {
        updateTimer->stop();
        delete updateTimer;
        updateTimer = nullptr;
    }

    delete ui;
}

void AnimationEditor::setupUI()
{
    ui->animationList->blockSignals(true);
    ui->animationList->clear();
    for (auto &anim : animFile.animations) {
        ui->animationList->addItem(anim.name);
    }
    ui->animationList->blockSignals(false);
    ui->animationList->setCurrentRow(-1);
    if (ui->animationList->count() >= 0)
        ui->animationList->setCurrentRow(currentAnim);

    ui->sourceList->blockSignals(true);
    ui->sheetID->blockSignals(true);
    ui->sourceList->clear();
    ui->sheetID->clear();
    for (auto &sheet : animFile.sheets) {
        ui->sourceList->addItem(sheet);
        ui->sheetID->addItem(sheet);
    }
    ui->sheetID->blockSignals(true);
    ui->sourceList->blockSignals(false);
    ui->sourceList->setCurrentRow(-1);
    if (ui->sourceList->count() >= 0)
        ui->sourceList->setCurrentRow(0);

    ui->hitboxList->blockSignals(true);
    ui->hitboxType->blockSignals(true);

    ui->hitboxList->clear();
    ui->hitboxType->clear();
    int hID = 0;
    for (auto &hitbox : animFile.hitboxTypes) {
        ui->hitboxType->addItem(hitbox);

        QListWidgetItem *item = new QListWidgetItem(hitbox, ui->hitboxList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(hitboxVisible[hID++] ? Qt::Checked : Qt::Unchecked);
    }
    ui->hitboxType->blockSignals(false);
    ui->hitboxList->blockSignals(false);
    ui->hitboxList->setCurrentRow(-1);
    if (ui->hitboxList->count() >= 0)
        ui->hitboxList->setCurrentRow(0);

    ui->boundingBoxX->setMaximum(aniType == ENGINE_v5 ? 0xFFFF : 0xFF);
    ui->boundingBoxY->setMaximum(aniType == ENGINE_v5 ? 0xFFFF : 0xFF);
    ui->boundingBoxW->setMaximum(aniType == ENGINE_v5 ? 0xFFFF : 0xFF);
    ui->boundingBoxH->setMaximum(aniType == ENGINE_v5 ? 0xFFFF : 0xFF);
    ui->offsetX->setRange(aniType == ENGINE_v5 ? -0x8000 : -0x80, aniType == ENGINE_v5 ? 0x7FFF : 0x7F);
    ui->offsetY->setRange(aniType == ENGINE_v5 ? -0x8000 : -0x80, aniType == ENGINE_v5 ? 0x7FFF : 0x7F);

    ui->addHB->setDisabled(aniType == ENGINE_v1);
    ui->rmHB->setDisabled(aniType == ENGINE_v1);
    ui->upHB->setDisabled(aniType == ENGINE_v1);
    ui->downHB->setDisabled(aniType == ENGINE_v1);

    updateView();
}

void AnimationEditor::updateView()
{
    QPixmap view(ui->viewerFrame->width(), ui->viewerFrame->height());
    QPainter painter(&view);

    float w = ui->viewerFrame->width(), h = ui->viewerFrame->height();
    float cx = w / 2, cy = h / 2;

    QBrush bgBrush(bgColour);
    painter.fillRect(0, 0, ui->viewerFrame->width(), ui->viewerFrame->height(), bgBrush);

    painter.setPen(QColor(0x80E0E0E0));
    painter.drawLine(0, cy, w, cy);
    painter.drawLine(cx, 0, cx, h);

    if (currentAnim < animFile.animations.count()
        && currentFrame < animFile.animations[currentAnim].frames.count()) {
        FormatHelpers::Animation::Frame &frame = animFile.animations[currentAnim].frames[currentFrame];
        if (frame.width && frame.height) {
            QRect boundingRect;
            boundingRect.setX(frame.sprX);
            boundingRect.setY(frame.sprY);
            boundingRect.setWidth(frame.width);
            boundingRect.setHeight(frame.height);

            QImage frameImg = sheets[frame.sheet].copy(boundingRect);
            if (frameImg.format() == QImage::Format_Indexed8 && !showTransparentClr)
                frameImg.setColor(0, 0x00000000);
            QImage zoomedFrame = frameImg.scaled(frameImg.width() * zoom, frameImg.height() * zoom);

            painter.drawImage(cx + frame.pivotX * zoom, cy + frame.pivotY * zoom, zoomedFrame);

            for (int h = 0; h < animFile.hitboxTypes.count(); ++h) {
                if (hitboxVisible[h]) {
                    QRect hitbox;
                    hitbox.setX(cx + (frame.hitboxes[h].left * zoom));
                    hitbox.setY(cy + (frame.hitboxes[h].top * zoom));
                    hitbox.setWidth(cx + (frame.hitboxes[h].right * zoom));
                    hitbox.setHeight(cy + (frame.hitboxes[h].bottom * zoom));
                    QBrush hitboxBrush(QColor(255, 0, 0, 0x80));

                    painter.setPen(QColor(255, 0, 0, 0x80));
                    painter.fillRect(hitbox.x(), hitbox.y(), abs(hitbox.x() - hitbox.width()),
                                     abs(hitbox.y() - hitbox.height()), hitboxBrush);
                }
            }
        }
    }

    ui->viewerFrame->setPixmap(view);
}

void AnimationEditor::processAnimation()
{
    bool changed = false;
    if (currentAnim < animCount() && currentFrame < frameCount()) {
        animTimer += animFile.animations[currentAnim].speed;
        int duration = animFile.animations[currentAnim].frames[currentFrame].duration;
        while (animTimer > duration) {
            animTimer -= duration;
            ++currentFrame;
            if (currentFrame >= animFile.animations[currentAnim].frames.count())
                currentFrame = animFile.animations[currentAnim].loopIndex;
            duration = animFile.animations[currentAnim].frames[currentFrame].duration;
            changed  = true;
        }
    }

    if (changed)
        updateView();
}

void AnimationEditor::loadSheet(QString filepath, int index, bool addSource)
{
    if (QFile::exists(filepath)) {
        if (QFileInfo(filepath).suffix().contains("gif")) {
            QGifImage gif(filepath);

            QImage sheet = gif.frame(0);

            sheets.insert(index, sheet);
        }
        else if (QFileInfo(filepath).suffix().contains("bmp")) {
            QImage sheet(filepath);
            sheets.insert(index, sheet);
        }
        else {
            // uh oh floshed
        }
    }

    if (addSource) {
        animFile.sheets.insert(index, Utils::getFilenameAndFolder(filepath));
    }
}

void AnimationEditor::removeSheet(int index, bool removeSource)
{
    if (removeSource) {
        animFile.sheets.removeAt(index);
    }

    sheets.removeAt(index);
}

void AnimationEditor::moveSheet(int from, int to)
{
    animFile.sheets.move(from, to);
    sheets.move(from, to);
}

void AnimationEditor::loadAnim(QString filepath, int aniType)
{
    appConfig.addRecentFile(aniType, TOOL_ANIMATIONEDITOR, filepath, QList<QString>{});

    this->aniType = aniType;
    animFile      = FormatHelpers::Animation(aniType, filepath);

    QStringList folderList;
    folderList << filepath.replace("\\", "/").split("/");

    QString baseDir = "";

    int pos = 0;
    while (true) {
        if (folderList[pos].toLower() == "animations"
            && (aniType != ENGINE_v5 && aniType != ENGINE_v1)) {
            folderList[pos] = "Sprites";
        }

        baseDir += folderList[pos++] + "/";
        if (pos >= folderList.count())
            break;

        if (folderList[pos - 1].toLower() == "sprites" && aniType != ENGINE_v1)
            break;

        if (folderList[pos - 1].toLower() == "characters" && aniType == ENGINE_v1)
            break;
    }

    int id = 0;
    for (auto &sheet : animFile.sheets) {
        ui->sheetID->addItem(sheet);

        loadSheet(baseDir + sheet, id++, false);
    }

    hitboxVisible.clear();
    for (int h = 0; h < animFile.hitboxTypes.count(); ++h) {
        hitboxVisible.append(false);
    }

    updateTitle(false);
    setupUI();
}

bool AnimationEditor::event(QEvent *event)
{
    QStringList types = { "RSDKv5 Animation Files (*.bin)", "RSDKv4/RSDKv3 Animation Files (*.ani)",
                          "RSDKv2 Animation Files (*.ani)", "RSDKv1 Animation Files (*.ani)" };
    QList<QString> typesList = { "RSDKv5 Animation Files (*.bin)",
                                 "RSDKv4/RSDKv3 Animation Files (*.ani)",
                                 "RSDKv4/RSDKv3 Animation Files (*.ani)",
                                 "RSDKv2 Animation Files (*.ani)", "RSDKv1 Animation Files (*.ani)" };

    if (event->type() == (QEvent::Type)RE_EVENT_NEW) {
        animFile = FormatHelpers::Animation();
        hitboxVisible.clear();
        setupUI();
        return true;
    }

    if (event->type() == (QEvent::Type)RE_EVENT_OPEN) {
        QFileDialog filedialog(this, tr("Open Animation"), "",
                               tr(types.join(";;").toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            setStatus("Opening Animation \""
                      + Utils::getFilenameAndFolder(filedialog.selectedFiles()[0]) + "\"");

            QString filepath = filedialog.selectedFiles()[0];
            aniType          = typesList.indexOf(filedialog.selectedNameFilter());
            loadAnim(filepath, aniType);
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE) {
        if (!QFile::exists(animFile.filePath)) {
            QFileDialog filedialog(this, tr("Save Animation"), "",
                                   tr(typesList[aniType].toStdString().c_str()));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {
                setStatus("Saving animation...");

                QString filepath = filedialog.selectedFiles()[0];
                animFile.write(aniType, filepath);
                appConfig.addRecentFile(aniType, TOOL_ANIMATIONEDITOR, filepath, QList<QString>{});
                return true;
            }
        }
        else {
            setStatus("Saving animation...");

            QString filepath = animFile.filePath;
            animFile.write(aniType, filepath);
            appConfig.addRecentFile(aniType, TOOL_ANIMATIONEDITOR, filepath, QList<QString>{});
            return true;
        }
    }
    if (event->type() == (QEvent::Type)RE_EVENT_SAVE_AS) {
        QFileDialog filedialog(
            this, tr("Save Animation"),
            QFile::exists(animFile.filePath) ? QFileInfo(animFile.filePath).absolutePath() : "",
            tr(typesList[aniType].toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            setStatus("Saving animation...");

            QString filepath = filedialog.selectedFiles()[0];
            animFile.write(aniType, filepath);
            appConfig.addRecentFile(aniType, TOOL_ANIMATIONEDITOR, filepath, QList<QString>{});
            return true;
        }
    }

    if (event->type() == QEvent::Wheel) {
        QWheelEvent *wEvent = static_cast<QWheelEvent *>(event);

        if (wEvent->modifiers() & Qt::ControlModifier) {
            if (wEvent->angleDelta().y() > 0 && zoom < 20)
                zoom *= 1.5;
            else if (wEvent->angleDelta().y() < 0 && zoom > 0.5)
                zoom /= 1.5;
            updateView();
            return true;
        }
        updateView();
    }

    if (event->type() == QEvent::Resize) {
        updateView();
    }

    return QWidget::event(event);
}

#include "moc_animationeditor.cpp"
