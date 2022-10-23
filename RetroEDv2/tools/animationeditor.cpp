#include "includes.hpp"
#include "ui_animationeditor.h"
#include "animationeditor.hpp"

#include "qgifimage.h"

#include "animationeditor/animsheetselector.hpp"

#include "paletteeditor/colourdialog.hpp"

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
            PrintLog(QString("1Index %1").arg(index.row()));
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
            PrintLog(QString("2Index %1").arg(index.row()));
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

    frameModel = new QStandardItemModel;
    missingImg = QPixmap(":/icons/missing.png");

    ui->play->setIcon(playPauseIco[0]);

    if (updateTimer)
        delete updateTimer;

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, QOverload<>::of(&AnimationEditor::ProcessAnimation));

    ui->frameList->setModel(frameModel);
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

        if (currentAnim < animFile.animations.count()) {
            if (currentFrame < FrameCount()) {
                ui->sheetID->setCurrentIndex(
                    animFile.animations[currentAnim].frames[currentFrame].sheet);
            }
        }
        else {
            ui->sheetID->setCurrentIndex(0);
        }
    };

    auto setupHiboxTypeBox = [this] {
        ui->hitboxType->blockSignals(true);
        ui->hitboxType->clear();
        for (int i = 0; i < animFile.hitboxTypes.count(); ++i) {
            if (aniType == ENGINE_v5 || aniType == ENGINE_v1)
                ui->hitboxType->addItem(animFile.hitboxTypes[i]);
            else
                ui->hitboxType->addItem(QString("(%1, %2, %3, %4)")
                                            .arg(animFile.hitboxes[i].hitboxes[0].left)
                                            .arg(animFile.hitboxes[i].hitboxes[0].top)
                                            .arg(animFile.hitboxes[i].hitboxes[0].right)
                                            .arg(animFile.hitboxes[i].hitboxes[0].bottom));
        }
        ui->hitboxType->blockSignals(false);
    };

    setupSheetBox();
    setupHiboxTypeBox();
    currentHitbox = 0; // temp

    std::function<void(int)> animFunc([this](int c) {
        std::function<void(int)> frameFunc([this](int c) {
            disconnect(ui->sheetID, nullptr, nullptr, nullptr);
            disconnect(ui->boundingBoxX, nullptr, nullptr, nullptr);
            disconnect(ui->boundingBoxY, nullptr, nullptr, nullptr);
            disconnect(ui->boundingBoxW, nullptr, nullptr, nullptr);
            disconnect(ui->boundingBoxH, nullptr, nullptr, nullptr);
            disconnect(ui->selBoundBox, nullptr, nullptr, nullptr);
            disconnect(ui->offsetX, nullptr, nullptr, nullptr);
            disconnect(ui->offsetY, nullptr, nullptr, nullptr);
            disconnect(ui->hitboxType, nullptr, nullptr, nullptr);
            disconnect(ui->hitboxID, nullptr, nullptr, nullptr);
            disconnect(ui->hitboxL, nullptr, nullptr, nullptr);
            disconnect(ui->hitboxR, nullptr, nullptr, nullptr);
            disconnect(ui->hitboxB, nullptr, nullptr, nullptr);
            disconnect(ui->hitboxT, nullptr, nullptr, nullptr);
            disconnect(ui->duration, nullptr, nullptr, nullptr);
            disconnect(ui->id, nullptr, nullptr, nullptr);

            bool invalid = c == -1 || c >= FrameCount();

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

            ui->addFrame->setDisabled(invalid);
            ui->upFrame->setDisabled(invalid);
            ui->downFrame->setDisabled(invalid);
            ui->rmFrame->setDisabled(invalid);
            ui->copyFrame->setDisabled(invalid);
            ui->impFrame->setDisabled(invalid);
            ui->expFrame->setDisabled(invalid);

            if (!invalid) {
                ui->properties->setItemText(
                    1, QString("Frames (%1 of %2, ID: %3)").arg(c + 1).arg(FrameCount()).arg(c));
            }
            else {
                ui->properties->setItemText(1, "Frames");
            }

            if (invalid) {
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
                ui->id->setText("");

                currentFrame = -1;

                UpdateView();
                return;
            }

            currentFrame = c;

            FormatHelpers::Animation::Frame &f = animFile.animations[currentAnim].frames[c];

            ui->upFrame->setDisabled(c <= 0);
            ui->downFrame->setDisabled(c >= animFile.animations[currentAnim].frames.count() - 1
                                       || c < 0);
            ui->rmFrame->setDisabled(!animFile.animations[currentAnim].frames.count() || c < 0);

            ui->sheetID->setCurrentIndex(f.sheet);
            ui->boundingBoxX->setValue(f.sprX);
            ui->boundingBoxY->setValue(f.sprY);
            ui->boundingBoxW->setValue(f.width);
            ui->boundingBoxH->setValue(f.height);
            ui->offsetX->setValue(f.pivotX);
            ui->offsetY->setValue(f.pivotY);
            if (aniType == ENGINE_v5 || aniType == ENGINE_v1) {
                ui->hitboxType->setCurrentIndex(-1);
                ui->hitboxType->setCurrentIndex(0);
            }
            else {
                ui->hitboxType->blockSignals(true);
                ui->hitboxType->setCurrentIndex(-1);
                ui->hitboxType->setCurrentIndex(f.collisionBox);
                ui->hitboxType->blockSignals(false);
                ui->hitboxID->setCurrentIndex(-1);
                ui->hitboxID->setCurrentIndex(0);
            }

            ui->hitboxL->setDisabled(true);
            ui->hitboxT->setDisabled(true);
            ui->hitboxR->setDisabled(true);
            ui->hitboxB->setDisabled(true);
            if (aniType == ENGINE_v5 || aniType == ENGINE_v1) {
                uint c = ui->hitboxType->currentIndex();
                if (c < (uint)animFile.hitboxes.count()) {
                    ui->hitboxL->setValue(f.hitboxes[ui->hitboxType->currentIndex()].left);
                    ui->hitboxT->setValue(f.hitboxes[ui->hitboxType->currentIndex()].top);
                    ui->hitboxR->setValue(f.hitboxes[ui->hitboxType->currentIndex()].right);
                    ui->hitboxB->setValue(f.hitboxes[ui->hitboxType->currentIndex()].bottom);

                    ui->hitboxL->setDisabled(invalid);
                    ui->hitboxT->setDisabled(invalid);
                    ui->hitboxR->setDisabled(invalid);
                    ui->hitboxB->setDisabled(invalid);
                }
            }
            else {
                uint c = ui->hitboxType->currentIndex();
                if (f.collisionBox < animFile.hitboxes.count()) {
                    uint id      = ui->hitboxID->currentIndex();
                    auto &hitbox = animFile.hitboxes[f.collisionBox];
                    if (c < (uint)animFile.hitboxes.count() && id < 2) {
                        ui->hitboxL->setValue(hitbox.hitboxes[id].left);
                        ui->hitboxT->setValue(hitbox.hitboxes[id].top);
                        ui->hitboxR->setValue(hitbox.hitboxes[id].right);
                        ui->hitboxB->setValue(hitbox.hitboxes[id].bottom);

                        ui->hitboxL->setDisabled(invalid);
                        ui->hitboxT->setDisabled(invalid);
                        ui->hitboxR->setDisabled(invalid);
                        ui->hitboxB->setDisabled(invalid);
                    }
                }
            }
            ui->duration->setValue(f.duration);
            ui->id->setText(QChar::fromLatin1(f.id));

            connect(ui->sheetID, QOverload<int>::of(&QComboBox::currentIndexChanged),
                    [&f, this](int v) {
                        if (v < 0)
                            return;
                        f.sheet = (byte)v;
                        SetFramePreview();
                        UpdateView();
                        DoAction("Changed SheetID", true);
                    });

            connect(ui->boundingBoxX, QOverload<int>::of(&QSpinBox::valueChanged), [&f, this](int v) {
                f.sprX = (ushort)v;
                SetFramePreview();
                UpdateView();
                DoAction("Changed sprX", true);
            });
            connect(ui->boundingBoxY, QOverload<int>::of(&QSpinBox::valueChanged), [&f, this](int v) {
                f.sprY = (ushort)v;
                SetFramePreview();
                UpdateView();
                DoAction("Changed sprY", true);
            });
            connect(ui->boundingBoxW, QOverload<int>::of(&QSpinBox::valueChanged), [&f, this](int v) {
                f.width = (ushort)v;
                SetFramePreview();
                UpdateView();
                DoAction("Changed frame width", true);
            });
            connect(ui->boundingBoxH, QOverload<int>::of(&QSpinBox::valueChanged), [&f, this](int v) {
                f.height = (ushort)v;
                SetFramePreview();
                UpdateView();
                DoAction("Changed frame height", true);
            });
            connect(ui->selBoundBox, &QToolButton::clicked, [&f, this] {
                auto *sel = new AnimSheetSelector(animFile.sheets[f.sheet], &sheets[f.sheet]);
                if (sel->exec() == QDialog::Accepted) {
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
                        SetFramePreview();
                        UpdateView();
                        DoAction("Set bounding box", true);
                    }
                }
            });

            connect(ui->offsetX, QOverload<int>::of(&QSpinBox::valueChanged), [&f, this](int v) {
                f.pivotX = (short)v;
                UpdateView();
                DoAction("Changed pivotX", true);
            });
            connect(ui->offsetY, QOverload<int>::of(&QSpinBox::valueChanged), [&f, this](int v) {
                f.pivotY = (short)v;
                UpdateView();
                DoAction("Changed pivotY", true);
            });

            connect(ui->duration, QOverload<int>::of(&QSpinBox::valueChanged), [&f, this](int v) {
                f.duration = (ushort)v;
                DoAction("Changed duration", true);
            });

            connect(ui->hitboxType, QOverload<int>::of(&QComboBox::currentIndexChanged),
                    [&f, this](int v) {
                        if (v < 0)
                            return;
                        currentHitbox = (ushort)v;

                        if (aniType == ENGINE_v5 || aniType == ENGINE_v1) {
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
                        }
                        else {
                            ui->hitboxID->setCurrentIndex(-1);
                            ui->hitboxID->setCurrentIndex(0);
                            f.collisionBox = v;
                        }
                    });

            connect(ui->hitboxID, QOverload<int>::of(&QComboBox::currentIndexChanged),
                    [this, invalid](int v) {
                        if (v < 0)
                            return;
                        currentSubHitbox = (ushort)v;

                        ui->hitboxL->setDisabled(true);
                        ui->hitboxT->setDisabled(true);
                        ui->hitboxR->setDisabled(true);
                        ui->hitboxB->setDisabled(true);
                        if (v >= 0 && v < animFile.hitboxes.count()) {
                            ui->hitboxL->blockSignals(true);
                            ui->hitboxT->blockSignals(true);
                            ui->hitboxR->blockSignals(true);
                            ui->hitboxB->blockSignals(true);

                            auto &hitbox = animFile.hitboxes[currentHitbox];

                            ui->hitboxL->setValue(hitbox.hitboxes[v].left);
                            ui->hitboxT->setValue(hitbox.hitboxes[v].top);
                            ui->hitboxR->setValue(hitbox.hitboxes[v].right);
                            ui->hitboxB->setValue(hitbox.hitboxes[v].bottom);

                            ui->hitboxL->blockSignals(false);
                            ui->hitboxT->blockSignals(false);
                            ui->hitboxR->blockSignals(false);
                            ui->hitboxB->blockSignals(false);

                            ui->hitboxL->setDisabled(invalid);
                            ui->hitboxT->setDisabled(invalid);
                            ui->hitboxR->setDisabled(invalid);
                            ui->hitboxB->setDisabled(invalid);
                        }
                    });

            connect(ui->hitboxL, QOverload<int>::of(&QSpinBox::valueChanged), [&f, this](int v) {
                uint c = ui->hitboxType->currentIndex();
                if (aniType == ENGINE_v5 || aniType == ENGINE_v1) {
                    if (c < (uint)f.hitboxes.count()) {
                        f.hitboxes[c].left = (short)v;
                        UpdateView();
                        DoAction("Changed hitbox left", true);
                    }
                }
                else {
                    uint id                      = ui->hitboxID->currentIndex();
                    auto &hitbox                 = animFile.hitboxes[c];
                    hitbox.hitboxes[id + 0].left = (short)v;

                    if (id == 0) {
                        QString name = QString("(%1, %2, %3, %4)")
                                           .arg(hitbox.hitboxes[id].left)
                                           .arg(hitbox.hitboxes[id].top)
                                           .arg(hitbox.hitboxes[id].right)
                                           .arg(hitbox.hitboxes[id].bottom);
                        ui->hitboxList->blockSignals(true);
                        ui->hitboxList->item(c)->setText(name);
                        ui->hitboxList->blockSignals(false);
                        ui->hitboxType->setItemText(c, name);
                    }

                    UpdateView();
                    DoAction("Changed hitbox left", true);
                }
            });
            connect(ui->hitboxT, QOverload<int>::of(&QSpinBox::valueChanged), [&f, this](int v) {
                uint c = ui->hitboxType->currentIndex();
                if (aniType == ENGINE_v5 || aniType == ENGINE_v1) {
                    if (c < (uint)f.hitboxes.count()) {
                        f.hitboxes[c].top = (short)v;
                        UpdateView();
                        DoAction("Changed hitbox top", true);
                    }
                }
                else {
                    uint id                     = ui->hitboxID->currentIndex();
                    auto &hitbox                = animFile.hitboxes[c];
                    hitbox.hitboxes[id + 0].top = (short)v;

                    if (id == 0) {
                        QString name = QString("(%1, %2, %3, %4)")
                                           .arg(hitbox.hitboxes[id].left)
                                           .arg(hitbox.hitboxes[id].top)
                                           .arg(hitbox.hitboxes[id].right)
                                           .arg(hitbox.hitboxes[id].bottom);
                        ui->hitboxList->blockSignals(true);
                        ui->hitboxList->item(c)->setText(name);
                        ui->hitboxList->blockSignals(false);
                        ui->hitboxType->setItemText(c, name);
                    }

                    UpdateView();
                    DoAction("Changed hitbox top", true);
                }
            });
            connect(ui->hitboxR, QOverload<int>::of(&QSpinBox::valueChanged), [&f, this](int v) {
                uint c = ui->hitboxType->currentIndex();
                if (aniType == ENGINE_v5 || aniType == ENGINE_v1) {
                    if (c < (uint)f.hitboxes.count()) {
                        f.hitboxes[c].right = (short)v;
                        UpdateView();
                        DoAction("Changed hitbox right", true);
                    }
                }
                else {
                    uint id                       = ui->hitboxID->currentIndex();
                    auto &hitbox                  = animFile.hitboxes[c];
                    hitbox.hitboxes[id + 0].right = (short)v;

                    if (id == 0) {
                        QString name = QString("(%1, %2, %3, %4)")
                                           .arg(hitbox.hitboxes[id].left)
                                           .arg(hitbox.hitboxes[id].top)
                                           .arg(hitbox.hitboxes[id].right)
                                           .arg(hitbox.hitboxes[id].bottom);
                        ui->hitboxList->blockSignals(true);
                        ui->hitboxList->item(c)->setText(name);
                        ui->hitboxList->blockSignals(false);
                        ui->hitboxType->setItemText(c, name);
                    }

                    UpdateView();
                    DoAction("Changed hitbox right", true);
                }
            });
            connect(ui->hitboxB, QOverload<int>::of(&QSpinBox::valueChanged), [&f, this](int v) {
                uint c = ui->hitboxType->currentIndex();
                if (aniType == ENGINE_v5 || aniType == ENGINE_v1) {
                    if (c < (uint)f.hitboxes.count()) {
                        f.hitboxes[c].bottom = (short)v;
                        UpdateView();
                        DoAction("Changed hitbox bottom", true);
                    }
                }
                else {
                    uint id                        = ui->hitboxID->currentIndex();
                    auto &hitbox                   = animFile.hitboxes[c];
                    hitbox.hitboxes[id + 0].bottom = (short)v;

                    if (id == 0) {
                        QString name = QString("(%1, %2, %3, %4)")
                                           .arg(hitbox.hitboxes[id].left)
                                           .arg(hitbox.hitboxes[id].top)
                                           .arg(hitbox.hitboxes[id].right)
                                           .arg(hitbox.hitboxes[id].bottom);
                        ui->hitboxList->blockSignals(true);
                        ui->hitboxList->item(c)->setText(name);
                        ui->hitboxList->blockSignals(false);
                        ui->hitboxType->setItemText(c, name);
                    }

                    UpdateView();
                    DoAction("Changed hitbox bottom", true);
                }
            });

            connect(ui->id, &QLineEdit::textChanged, [&f, this](QString s) {
                f.id = (s.length() > 0 ? s[0].toLatin1() : 0);
                DoAction("Changed frame ID", true);
            });

            UpdateView();
        });

        disconnect(ui->frameList->selectionModel(), nullptr, nullptr, nullptr);
        connect(ui->frameList->selectionModel(), &QItemSelectionModel::currentRowChanged,
                [this, frameFunc](const QModelIndex &aindex) {
                    ui->upFrame->setDisabled(!aindex.isValid());
                    ui->downFrame->setDisabled(!aindex.isValid());
                    ui->rmFrame->setDisabled(!aindex.isValid());

                    frameFunc(aindex.row());

                    ui->frameList->doItemsLayout();

                    if (!aindex.isValid())
                        return;

                    if (ui->properties->currentIndex() == 0) {
                        if (aindex.row() >= 0 && FrameCount() > 0)
                            ui->properties->setCurrentIndex(1); // frame selected, use that
                        else
                            ui->properties->setCurrentIndex(0); // no frame selected, show anim props
                    }

                    ui->downFrame->setDisabled(aindex.row() == frameModel->rowCount() - 1);
                    ui->upFrame->setDisabled(!aindex.row());
                });

        offset.x = 0;
        offset.y = 0;
        ui->frameOffLabel->setText(QString("Frame Offset: (%1, %2)").arg(offset.x).arg(offset.y));

        if (playingAnim)
            ui->play->clicked();

        currentAnim = c;
        disconnect(ui->animName, nullptr, nullptr, nullptr);
        disconnect(ui->loopIndex, nullptr, nullptr, nullptr);
        disconnect(ui->rotationStyle, nullptr, nullptr, nullptr);
        disconnect(ui->speedMult, nullptr, nullptr, nullptr);
        disconnect(ui->frameList, nullptr, nullptr, nullptr);

        ui->animName->setDisabled(c == -1);
        ui->loopIndex->setDisabled(c == -1);
        ui->rotationStyle->setDisabled(c == -1 || aniType >= ENGINE_v2);
        ui->speedMult->setDisabled(c == -1);
        ui->playerID->setDisabled(c == -1 || aniType != ENGINE_v1);
        ui->copyAnim->setDisabled((aniType == ENGINE_v1 || aniType == ENGINE_v2) || c == -1);

        ui->impAnim->setDisabled(c == -1);
        ui->expAnim->setDisabled(c == -1);

        ui->upAnim->setDisabled((aniType == ENGINE_v1 || aniType == ENGINE_v2) || c == -1 || c - 1 < 0
                                || animFile.animations.count() <= 1);
        ui->downAnim->setDisabled((aniType == ENGINE_v1 || aniType == ENGINE_v2) || c == -1
                                  || c + 1 >= animFile.animations.count()
                                  || animFile.animations.count() <= 1);
        ui->rmAnim->setDisabled((aniType == ENGINE_v1 || aniType == ENGINE_v2) || c == -1
                                || !animFile.animations.count());

        if (c > -1) {
            ui->properties->setItemText(0, QString("Animations (%1 of %2, ID: %3)")
                                               .arg(c + 1)
                                               .arg(animFile.animations.count())
                                               .arg(c));
        }
        else {
            ui->properties->setItemText(0, "Animations");
        }

        std::function<void(int, QListWidgetItem *)> blankModify;
        ui->frameList->blockSignals(true);
        if (c > -1) {
            disconnect(frameModel, &QStandardItemModel::itemChanged, nullptr, nullptr);

            SetupFrameList(animFile.animations[currentAnim].frames);
            UpdateView();

            ui->frameList->blockSignals(false);

            connect(frameModel, &QStandardItemModel::itemChanged,
                    [this, frameFunc](QStandardItem *item) {
                        frameFunc(frameModel->indexFromItem(item).row());
                    });

            bool invalid         = c == -1 || c >= animFile.animations.count();
            bool durationInvalid = (aniType != ENGINE_v5);

            ui->animName->setDisabled(invalid);
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

            if (invalid) {
                ui->animName->setText("");
                ui->loopIndex->setValue(0);
                ui->rotationStyle->setCurrentIndex(-1);
                ui->speedMult->setValue(0);
                ui->playerID->setCurrentIndex(-1);

                return;
            }

            if (aniType != ENGINE_v5)
                animFile.animations[c].name.truncate(0x10 - 1);

            ui->animName->setText(animFile.animations[c].name);
            ui->loopIndex->setValue(animFile.animations[c].loopIndex);
            ui->rotationStyle->setCurrentIndex(animFile.animations[c].rotationStyle);
            ui->speedMult->setValue(animFile.animations[c].speed);
            ui->playerID->setCurrentIndex(animFile.playerType);

            connect(ui->animName, &QLineEdit::textChanged, [this, c](QString s) {
                animFile.animations[c].name = s;

                ui->animationList->blockSignals(true);
                ui->animationList->item(currentAnim)->setText(s);
                ui->animationList->blockSignals(false);

                DoAction("Changed animation name", true);
            });

            connect(ui->loopIndex, QOverload<int>::of(&QSpinBox::valueChanged), [this, c](int v) {
                int val = v;
                if (val >= FrameCount())
                    v = FrameCount() - 1;
                if (val < 0)
                    v = 0;

                if (val != v)
                    ui->loopIndex->setValue(val);

                animFile.animations[c].loopIndex = (ushort)v;
                DoAction("Changed loop index", true);
            });

            connect(ui->rotationStyle, QOverload<int>::of(&QComboBox::currentIndexChanged),
                    [this, c](int v) {
                        animFile.animations[c].rotationStyle = (byte)v;
                        DoAction("Changed rotation style", true);
                    });

            connect(ui->speedMult, QOverload<int>::of(&QSpinBox::valueChanged), [this, c](int v) {
                animFile.animations[c].speed = (byte)v;
                DoAction("Changed speed", true);
            });

            currentFrame = 0;
            if (c >= 0 && FrameCount() > 0) {
                ui->frameList->setCurrentIndex(frameModel->item(0)->index());
                frameFunc(0);
            }
            else {
                frameFunc(-1);
            }
            ui->properties->setCurrentIndex(0);
        };
    });

    connect(ui->animationList, &QListWidget::currentRowChanged, animFunc);

    connect(ui->addFrame, &QToolButton::clicked, [this] {
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
        item->setEditable(false);
        item->setData((frame.width == 0 || frame.height == 0 || frame.sheet >= sheets.count())
                          ? missingImg
                          : QPixmap::fromImage(sheets[frame.sheet].copy(boundingRect)),
                      ROLE_PIXMAP);
        frameModel->insertRow(c, item);
        ui->frameList->setCurrentIndex(item->index());

        UpdateView();
        DoAction("Added frame", true);
    });

    connect(ui->rmFrame, &QToolButton::clicked, [this] {
        int c = ui->frameList->currentIndex().row();
        int n = c == frameModel->rowCount() - 1 ? c - 1 : c;
        if (n >= 0)
            frameModel->takeRow(c);
        else
            frameModel->clear();
        // delete model->itemFromIndex(ui->frameList->currentIndex());
        animFile.animations[currentAnim].frames.removeAt(c);
        DoAction("Removed frame", true);

        ui->frameList->blockSignals(true);
        ui->frameList->setCurrentIndex(frameModel->index(n, 0));
        ui->frameList->blockSignals(false);

        ui->rmFrame->setDisabled(currentAnim < 0 || !animFile.animations[currentAnim].frames.count());
    });

    connect(ui->upFrame, &QToolButton::clicked, [this] {
        uint c                      = ui->frameList->currentIndex().row();
        QList<QStandardItem *> item = frameModel->takeRow(c);
        animFile.animations[currentAnim].frames.move(c, c - 1);
        frameModel->insertRow(c - 1, item);
        DoAction("Moved frame up", true);
        ui->frameList->setCurrentIndex(frameModel->index(c - 1, 0));

        ui->upFrame->setDisabled(c - 1 <= 0);
    });

    connect(ui->downFrame, &QToolButton::clicked, [this] {
        uint c                      = ui->frameList->currentIndex().row();
        QList<QStandardItem *> item = frameModel->takeRow(c);
        animFile.animations[currentAnim].frames.move(c, c + 1);
        frameModel->insertRow(c + 1, item);
        DoAction("Moved frame down", true);
        ui->frameList->setCurrentIndex(frameModel->index(c + 1, 0));

        ui->downFrame->setDisabled(c + 1 >= animFile.animations[currentAnim].frames.count() - 1
                                   || c < 0);
    });

    std::function<void(int)> sheetFunc([this](int c) {
        if (c > -1) {
            ui->properties->setItemText(
                2, QString("Sheets (%1 of %2, ID: %3)").arg(c + 1).arg(animFile.sheets.count()).arg(c));
        }
        else {
            ui->properties->setItemText(2, "Sheets");
        }

        bool invalid = c == -1 || c >= animFile.sheets.count();

        ui->addSheet->setDisabled(animFile.sheets.count() >= 24);
        ui->rmSheet->setDisabled(invalid || animFile.sheets.count() < 0);
        ui->upSheet->setDisabled(invalid || c >= animFile.sheets.count() - 1);
        ui->downSheet->setDisabled(invalid || c <= 0);
    });

    connect(ui->sheetList, &QListWidget::currentRowChanged, sheetFunc);

    std::function<void(int)> hitboxFunc([this](int c) {
        if (c > -1) {
            ui->properties->setItemText(3, QString("Hitboxes (%1 of %2, ID: %3)")
                                               .arg(c + 1)
                                               .arg(animFile.hitboxTypes.count())
                                               .arg(c));
        }
        else {
            ui->properties->setItemText(3, "Hitboxes");
        }

        bool invalid = c == -1 || c >= animFile.hitboxTypes.count();

        ui->hitboxType->setDisabled(invalid);
        ui->hitboxID->setDisabled(invalid);
        ui->hitboxL->setDisabled(invalid);
        ui->hitboxT->setDisabled(invalid);
        ui->hitboxR->setDisabled(invalid);
        ui->hitboxB->setDisabled(invalid);

        ui->addHB->setDisabled(aniType == ENGINE_v1 || animFile.hitboxTypes.count() >= 8);
        ui->rmHB->setDisabled(aniType == ENGINE_v1 || animFile.hitboxTypes.count() < 0);
        ui->upHB->setDisabled(aniType == ENGINE_v1 || c >= animFile.hitboxTypes.count() - 1);
        ui->downHB->setDisabled(aniType == ENGINE_v1 || c <= 0);
    });

    connect(ui->hitboxList, &QListWidget::currentRowChanged, hitboxFunc);

    connect(ui->impFile, &QToolButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Import Anim File"),
                               QFileInfo(animFile.filePath).absolutePath(), tr("JSON Files (*.json)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() != QDialog::Accepted)
            return;
        QString filename = filedialog.selectedFiles()[0];

        ui->animationList->blockSignals(true);

        Reader reader(filename);

        QByteArray bytes = reader.readByteArray(reader.filesize);
        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(bytes, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            PrintLog("fromJson failed: " + jsonError.errorString()
                     + ", offset: " + QString::number(jsonError.offset));
            return;
        }

        if (document.isObject()) {
            animFile = FormatHelpers::Animation();
            hitboxVisible.clear();

            QJsonObject jsonObj = document.object();

            if (jsonObj.contains("Sheets")) {
                QJsonArray sheetsArr = jsonObj.value("Sheets").toArray();
                for (int s = 0; s < sheetsArr.count(); ++s) {
                    LoadSheet(sheetsArr.at(s).toString(), s, true);
                }
            }
            if (jsonObj.contains("Hitbox Types")) {
                QJsonArray hitboxArr = jsonObj.value("Hitbox Types").toArray();
                for (int h = 0; h < hitboxArr.count(); ++h) {
                    animFile.hitboxTypes.append(hitboxArr.at(h).toString());
                    animFile.hitboxes.append(FormatHelpers::Animation::Hitbox());
                    hitboxVisible.append(false);
                }
            }

            if (jsonObj.contains("Animations")) {
                QJsonArray animsArr = jsonObj.value("Animations").toArray();

                for (int a = 0; a < animsArr.count(); ++a) {
                    QJsonObject animObj = animsArr.at(a).toObject();
                    FormatHelpers::Animation::AnimationEntry anim;

                    if (animObj.contains("Name")) {
                        auto name = animObj.take("Name");
                        anim.name = name.toString();

                        if (aniType != ENGINE_v5)
                            anim.name.truncate(0x10 - 1);
                    }

                    if (animObj.contains("Speed")) {
                        auto speed = animObj.take("Speed");
                        anim.speed = speed.toInt();
                    }

                    if (animObj.contains("Loop Index")) {
                        auto loopIndex = animObj.take("Loop Index");
                        anim.loopIndex = loopIndex.toInt();
                    }

                    if (animObj.contains("Rotation Style")) {
                        auto rotationStyle = animObj.take("Rotation Style");
                        anim.rotationStyle = rotationStyle.toInt();
                    }

                    if (animObj.contains("Frames")) {
                        QJsonArray framesArr = animObj.value("Frames").toArray();

                        for (int i = 0; i < framesArr.count(); ++i) {
                            QJsonObject frameObj = framesArr.at(i).toObject();
                            FormatHelpers::Animation::Frame frame;

                            {
                                if (frameObj.contains("Sheet")) {
                                    auto sheet  = frameObj.take("Sheet");
                                    int sheetID = animFile.sheets.indexOf(sheet.toString());
                                    if (sheetID == -1) {
                                        sheetID = animFile.sheets.count();

                                        LoadSheet(sheet.toString(), sheetID);
                                    }
                                    frame.sheet = sheetID;
                                }

                                if (frameObj.contains("Hitbox")) {
                                    auto hitbox = frameObj.take("Hitbox");
                                    if (hitbox.toString() == "None") {
                                        frame.collisionBox = 0xFF;
                                    }
                                    else {
                                        int hitboxID = animFile.hitboxTypes.indexOf(hitbox.toString());
                                        if (hitboxID == -1) {
                                            hitboxID = animFile.hitboxTypes.count();
                                            animFile.hitboxTypes.append(hitbox.toString());
                                            animFile.hitboxes.append(
                                                FormatHelpers::Animation::Hitbox());
                                            hitboxVisible.append(false);
                                        }
                                        frame.collisionBox = hitboxID;
                                    }
                                }

                                if (frameObj.contains("Duration")) {
                                    auto duration  = frameObj.take("Duration");
                                    frame.duration = duration.toInt();
                                }

                                if (frameObj.contains("ID")) {
                                    auto id  = frameObj.take("ID");
                                    frame.id = id.toInt();
                                }

                                if (frameObj.contains("Src")) {
                                    QJsonObject srcObj = frameObj.value("Src").toObject();

                                    if (srcObj.contains("x")) {
                                        auto srcX  = srcObj.take("x");
                                        frame.sprX = srcX.toInt();
                                    }

                                    if (srcObj.contains("y")) {
                                        auto srcY  = srcObj.take("y");
                                        frame.sprY = srcY.toInt();
                                    }
                                }

                                if (frameObj.contains("Size")) {
                                    QJsonObject sizeObj = frameObj.value("Size").toObject();

                                    if (sizeObj.contains("x")) {
                                        auto sizeX  = sizeObj.take("x");
                                        frame.width = sizeX.toInt();
                                    }

                                    if (sizeObj.contains("y")) {
                                        auto sizeY   = sizeObj.take("y");
                                        frame.height = sizeY.toInt();
                                    }
                                }

                                if (frameObj.contains("Pivot")) {
                                    QJsonObject pivotObj = frameObj.value("Pivot").toObject();

                                    if (pivotObj.contains("x")) {
                                        auto pivotX  = pivotObj.take("x");
                                        frame.pivotX = pivotX.toInt();
                                    }

                                    if (pivotObj.contains("y")) {
                                        auto pivotY  = pivotObj.take("y");
                                        frame.pivotY = pivotY.toInt();
                                    }
                                }

                                QJsonObject hitboxesObj = frameObj.value("Hitboxes").toObject();
                                frame.hitboxes.clear();
                                for (auto &h : animFile.hitboxTypes) {
                                    FormatHelpers::Animation::Hitbox::HitboxInfo hitbox;

                                    if (hitboxesObj.contains(h)) {
                                        QJsonObject hitboxObj = hitboxesObj.value(h).toObject();

                                        if (hitboxObj.contains("Left")) {
                                            auto left   = hitboxObj.take("Left");
                                            hitbox.left = left.toInt();
                                        }

                                        if (hitboxObj.contains("Top")) {
                                            auto top   = hitboxObj.take("Top");
                                            hitbox.top = top.toInt();
                                        }

                                        if (hitboxObj.contains("Right")) {
                                            auto right   = hitboxObj.take("Right");
                                            hitbox.right = right.toInt();
                                        }

                                        if (hitboxObj.contains("Bottom")) {
                                            auto bottom   = hitboxObj.take("Bottom");
                                            hitbox.bottom = bottom.toInt();
                                        }
                                    }

                                    frame.hitboxes.append(hitbox);
                                }
                            }

                            anim.frames.append(frame);
                        }
                    }

                    animFile.animations.append(anim);
                }
            }
            else {
                return;
            }
        }

        SetupUI();
        DoAction("Imported animation file", true);
    });

    connect(ui->expFile, &QToolButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Export Anim File"),
                               QFileInfo(animFile.filePath).absolutePath(), tr("JSON Files (*.json)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() != QDialog::Accepted)
            return;

        QString filename = filedialog.selectedFiles()[0];

        // export json
        QJsonObject aniFileObj;

        int aniVer = 0;
        switch (aniType) {
            case ENGINE_v1: aniVer = 1; break;
            case ENGINE_v2: aniVer = 2; break;
            case ENGINE_v3: aniVer = 3; break;
            case ENGINE_v4: aniVer = 4; break;
            case ENGINE_v5: aniVer = 5; break;
        }

        aniFileObj.insert("Anim Version", aniVer);

        QJsonArray sheetsObj;
        for (auto &s : animFile.sheets) {
            sheetsObj.append(s);
        }
        aniFileObj.insert("Sheets", sheetsObj);

        QJsonArray hitboxTypesObj;
        for (auto &h : animFile.hitboxTypes) {
            hitboxTypesObj.append(h);
        }
        aniFileObj.insert("Hitbox Types", hitboxTypesObj);

        QJsonArray animsObj;
        for (auto &a : animFile.animations) {
            QJsonObject animObj;
            animObj.insert("Name", a.name);
            animObj.insert("Speed", a.speed);
            animObj.insert("Loop Index", a.loopIndex);
            animObj.insert("Rotation Style", a.rotationStyle);

            QJsonArray framesObj;
            for (auto &f : a.frames) {
                QJsonObject frameObj;

                frameObj.insert("Sheet", animFile.sheets[f.sheet]);
                if (f.collisionBox == 0xFF)
                    frameObj.insert("Hitbox", "None");
                else
                    frameObj.insert("Hitbox", animFile.hitboxTypes[f.collisionBox]);
                frameObj.insert("ID", f.id);
                frameObj.insert("Duration", f.duration);

                QJsonObject srcObj;
                srcObj.insert("x", f.sprX);
                srcObj.insert("y", f.sprY);
                frameObj.insert("Src", srcObj);

                QJsonObject sizeObj;
                sizeObj.insert("x", f.width);
                sizeObj.insert("y", f.height);
                frameObj.insert("Size", sizeObj);

                QJsonObject pivotObj;
                pivotObj.insert("x", f.pivotX);
                pivotObj.insert("y", f.pivotY);
                frameObj.insert("Pivot", pivotObj);

                QJsonObject hitboxesObj;
                int hitboxID = 0;
                for (auto &h : f.hitboxes) {
                    QJsonObject hitboxObj;

                    hitboxObj.insert("Left", h.left);
                    hitboxObj.insert("Top", h.top);
                    hitboxObj.insert("Right", h.right);
                    hitboxObj.insert("Bottom", h.bottom);

                    hitboxesObj.insert(animFile.hitboxTypes[hitboxID++], hitboxObj);
                }
                frameObj.insert("Hitboxes", hitboxesObj);

                framesObj.append(frameObj);
            }

            animObj.insert("Frames", framesObj);
            animsObj.append(animObj);
        }
        aniFileObj.insert("Animations", animsObj);

        Writer writer(filename);
        writer.write(QJsonDocument(aniFileObj).toJson());
        writer.flush();
    });

    connect(ui->impAnim, &QToolButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Import Anim"), QFileInfo(animFile.filePath).absolutePath(),
                               tr("JSON Files (*.json)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() != QDialog::Accepted)
            return;
        QString filename = filedialog.selectedFiles()[0];

        ui->animationList->blockSignals(true);
        FormatHelpers::Animation::AnimationEntry anim;

        Reader reader(filename);

        QByteArray bytes = reader.readByteArray(reader.filesize);
        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(bytes, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            PrintLog("fromJson failed: " + jsonError.errorString()
                     + ", offset: " + QString::number(jsonError.offset));
            return;
        }

        if (document.isObject()) {
            QJsonObject jsonObj = document.object();

            if (jsonObj.contains("Animation")) {
                QJsonObject animObj = jsonObj.value("Animation").toObject();

                if (animObj.contains("Name")) {
                    auto name = animObj.take("Name");
                    anim.name = name.toString();

                    if (aniType != ENGINE_v5)
                        anim.name.truncate(0x10 - 1);
                }

                if (animObj.contains("Speed")) {
                    auto speed = animObj.take("Speed");
                    anim.speed = speed.toInt();
                }

                if (animObj.contains("Loop Index")) {
                    auto loopIndex = animObj.take("Loop Index");
                    anim.loopIndex = loopIndex.toInt();
                }

                if (animObj.contains("Rotation Style")) {
                    auto rotationStyle = animObj.take("Rotation Style");
                    anim.rotationStyle = rotationStyle.toInt();
                }

                if (animObj.contains("Frames")) {
                    QJsonArray framesArr = animObj.value("Frames").toArray();

                    for (int i = 0; i < framesArr.count(); ++i) {
                        QJsonObject frameObj = framesArr.at(i).toObject();
                        FormatHelpers::Animation::Frame frame;

                        {
                            if (frameObj.contains("Sheet")) {
                                auto sheet  = frameObj.take("Sheet");
                                int sheetID = animFile.sheets.indexOf(sheet.toString());
                                if (sheetID == -1) {
                                    sheetID = animFile.sheets.count();

                                    LoadSheet(sheet.toString(), sheetID);
                                }
                                frame.sheet = sheetID;
                            }

                            if (frameObj.contains("Hitbox")) {
                                auto hitbox = frameObj.take("Hitbox");
                                if (hitbox.toString() == "None") {
                                    frame.collisionBox = 0xFF;
                                }
                                else {
                                    int hitboxID = animFile.hitboxTypes.indexOf(hitbox.toString());
                                    if (hitboxID == -1) {
                                        hitboxID = animFile.hitboxTypes.count();
                                        animFile.hitboxTypes.append(hitbox.toString());
                                        animFile.hitboxes.append(FormatHelpers::Animation::Hitbox());
                                        hitboxVisible.append(false);
                                    }
                                    frame.collisionBox = hitboxID;
                                }
                            }

                            if (frameObj.contains("Duration")) {
                                auto duration  = frameObj.take("Duration");
                                frame.duration = duration.toInt();
                            }

                            if (frameObj.contains("ID")) {
                                auto id  = frameObj.take("ID");
                                frame.id = id.toInt();
                            }

                            if (frameObj.contains("Src")) {
                                QJsonObject srcObj = frameObj.value("Src").toObject();

                                if (srcObj.contains("x")) {
                                    auto srcX  = srcObj.take("x");
                                    frame.sprX = srcX.toInt();
                                }

                                if (srcObj.contains("y")) {
                                    auto srcY  = srcObj.take("y");
                                    frame.sprY = srcY.toInt();
                                }
                            }

                            if (frameObj.contains("Size")) {
                                QJsonObject sizeObj = frameObj.value("Size").toObject();

                                if (sizeObj.contains("x")) {
                                    auto sizeX  = sizeObj.take("x");
                                    frame.width = sizeX.toInt();
                                }

                                if (sizeObj.contains("y")) {
                                    auto sizeY   = sizeObj.take("y");
                                    frame.height = sizeY.toInt();
                                }
                            }

                            if (frameObj.contains("Pivot")) {
                                QJsonObject pivotObj = frameObj.value("Pivot").toObject();

                                if (pivotObj.contains("x")) {
                                    auto pivotX  = pivotObj.take("x");
                                    frame.pivotX = pivotX.toInt();
                                }

                                if (pivotObj.contains("y")) {
                                    auto pivotY  = pivotObj.take("y");
                                    frame.pivotY = pivotY.toInt();
                                }
                            }

                            QJsonObject hitboxesObj = frameObj.value("Hitboxes").toObject();
                            frame.hitboxes.clear();
                            for (auto &h : animFile.hitboxTypes) {
                                FormatHelpers::Animation::Hitbox::HitboxInfo hitbox;

                                if (hitboxesObj.contains(h)) {
                                    QJsonObject hitboxObj = hitboxesObj.value(h).toObject();

                                    if (hitboxObj.contains("Left")) {
                                        auto left   = hitboxObj.take("Left");
                                        hitbox.left = left.toInt();
                                    }

                                    if (hitboxObj.contains("Top")) {
                                        auto top   = hitboxObj.take("Top");
                                        hitbox.top = top.toInt();
                                    }

                                    if (hitboxObj.contains("Right")) {
                                        auto right   = hitboxObj.take("Right");
                                        hitbox.right = right.toInt();
                                    }

                                    if (hitboxObj.contains("Bottom")) {
                                        auto bottom   = hitboxObj.take("Bottom");
                                        hitbox.bottom = bottom.toInt();
                                    }
                                }

                                frame.hitboxes.append(hitbox);
                            }
                        }

                        anim.frames.append(frame);
                    }
                }
            }
            else {
                return;
            }
        }

        uint c     = ui->animationList->currentRow() + 1;
        auto *item = new QListWidgetItem(anim.name);
        ui->animationList->insertItem(c, item);
        animFile.animations.insert(c, anim);
        ui->animationList->blockSignals(false);

        SetupUI();
        DoAction("Imported animation", true);
    });

    connect(ui->expAnim, &QToolButton::clicked, [this] {
        if (ui->animationList->currentRow() == -1)
            return;
        QFileDialog filedialog(this, tr("Export Anim"), QFileInfo(animFile.filePath).absolutePath(),
                               tr("JSON Files (*.json);;RetroScript SpriteFrames (*.txt)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() != QDialog::Accepted)
            return;

        QString selectedFilter = filedialog.selectedNameFilter();
        QString filename       = filedialog.selectedFiles()[0];

        if (selectedFilter == "JSON Files (*.json)") {
            // export json
            QJsonObject aniFileObj;

            int aniVer = 0;
            switch (aniType) {
                case ENGINE_v1: aniVer = 1; break;
                case ENGINE_v2: aniVer = 2; break;
                case ENGINE_v3: aniVer = 3; break;
                case ENGINE_v4: aniVer = 4; break;
                case ENGINE_v5: aniVer = 5; break;
            }

            aniFileObj.insert("Anim Version", aniVer);

            FormatHelpers::Animation::AnimationEntry &a = animFile.animations[currentAnim];

            QJsonObject animObj;
            animObj.insert("Name", a.name);
            animObj.insert("Speed", a.speed);
            animObj.insert("Loop Index", a.loopIndex);
            animObj.insert("Rotation Style", a.rotationStyle);

            QJsonArray framesObj;
            for (auto &f : a.frames) {
                QJsonObject frameObj;

                frameObj.insert("Sheet", animFile.sheets[f.sheet]);
                if (f.collisionBox == 0xFF)
                    frameObj.insert("Hitbox", "None");
                else
                    frameObj.insert("Hitbox", animFile.hitboxTypes[f.collisionBox]);
                frameObj.insert("ID", f.id);
                frameObj.insert("Duration", f.duration);

                QJsonObject srcObj;
                srcObj.insert("x", f.sprX);
                srcObj.insert("y", f.sprY);
                frameObj.insert("Src", srcObj);

                QJsonObject sizeObj;
                sizeObj.insert("x", f.width);
                sizeObj.insert("y", f.height);
                frameObj.insert("Size", sizeObj);

                QJsonObject pivotObj;
                pivotObj.insert("x", f.pivotX);
                pivotObj.insert("y", f.pivotY);
                frameObj.insert("Pivot", pivotObj);

                QJsonObject hitboxesObj;
                int hitboxID = 0;
                for (auto &h : f.hitboxes) {
                    QJsonObject hitboxObj;

                    hitboxObj.insert("Left", h.left);
                    hitboxObj.insert("Top", h.top);
                    hitboxObj.insert("Right", h.right);
                    hitboxObj.insert("Bottom", h.bottom);

                    hitboxesObj.insert(animFile.hitboxTypes[hitboxID++], hitboxObj);
                }
                frameObj.insert("Hitboxes", hitboxesObj);

                framesObj.append(frameObj);
            }

            animObj.insert("Frames", framesObj);

            aniFileObj.insert("Animation", animObj);

            Writer writer(filename);
            writer.write(QJsonDocument(aniFileObj).toJson());
            writer.flush();
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

    connect(ui->impFrame, &QToolButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Import Frame"), QFileInfo(animFile.filePath).absolutePath(),
                               tr("JSON Files (*.json)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() != QDialog::Accepted)
            return;
        QString filename = filedialog.selectedFiles()[0];

        ui->frameList->blockSignals(true);
        FormatHelpers::Animation::Frame frame;

        Reader reader(filename);

        QByteArray bytes = reader.readByteArray(reader.filesize);
        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(bytes, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            PrintLog("fromJson failed: " + jsonError.errorString()
                     + ", offset: " + QString::number(jsonError.offset));
            return;
        }

        bool setUI = false;
        if (document.isObject()) {
            QJsonObject jsonObj = document.object();

            if (jsonObj.contains("Frame")) {
                QJsonObject frameObj = jsonObj.value("Frame").toObject();

                if (frameObj.contains("Sheet")) {
                    auto sheet  = frameObj.take("Sheet");
                    int sheetID = animFile.sheets.indexOf(sheet.toString());
                    if (sheetID == -1) {
                        sheetID = animFile.sheets.count();

                        LoadSheet(sheet.toString(), sheetID);
                        setUI = true;
                    }
                    frame.sheet = sheetID;
                }

                if (frameObj.contains("Hitbox")) {
                    auto hitbox = frameObj.take("Hitbox");
                    if (hitbox.toString() == "None") {
                        frame.collisionBox = 0xFF;
                    }
                    else {
                        int hitboxID = animFile.hitboxTypes.indexOf(hitbox.toString());
                        if (hitboxID == -1) {
                            hitboxID = animFile.hitboxTypes.count();
                            animFile.hitboxTypes.append(hitbox.toString());
                            animFile.hitboxes.append(FormatHelpers::Animation::Hitbox());
                            hitboxVisible.append(false);
                            setUI = true;
                        }
                        frame.collisionBox = hitboxID;
                    }
                }

                if (frameObj.contains("Duration")) {
                    auto duration  = frameObj.take("Duration");
                    frame.duration = duration.toInt();
                }

                if (frameObj.contains("ID")) {
                    auto id  = frameObj.take("ID");
                    frame.id = id.toInt();
                }

                if (frameObj.contains("Src")) {
                    QJsonObject srcObj = frameObj.value("Src").toObject();

                    if (srcObj.contains("x")) {
                        auto srcX  = srcObj.take("x");
                        frame.sprX = srcX.toInt();
                    }

                    if (srcObj.contains("y")) {
                        auto srcY  = srcObj.take("y");
                        frame.sprY = srcY.toInt();
                    }
                }

                if (frameObj.contains("Size")) {
                    QJsonObject sizeObj = frameObj.value("Size").toObject();

                    if (sizeObj.contains("x")) {
                        auto sizeX  = sizeObj.take("x");
                        frame.width = sizeX.toInt();
                    }

                    if (sizeObj.contains("y")) {
                        auto sizeY   = sizeObj.take("y");
                        frame.height = sizeY.toInt();
                    }
                }

                if (frameObj.contains("Pivot")) {
                    QJsonObject pivotObj = frameObj.value("Pivot").toObject();

                    if (pivotObj.contains("x")) {
                        auto pivotX  = pivotObj.take("x");
                        frame.pivotX = pivotX.toInt();
                    }

                    if (pivotObj.contains("y")) {
                        auto pivotY  = pivotObj.take("y");
                        frame.pivotY = pivotY.toInt();
                    }
                }

                QJsonObject hitboxesObj = frameObj.value("Hitboxes").toObject();
                frame.hitboxes.clear();
                for (auto &h : animFile.hitboxTypes) {
                    FormatHelpers::Animation::Hitbox::HitboxInfo hitbox;

                    if (hitboxesObj.contains(h)) {
                        QJsonObject hitboxObj = hitboxesObj.value(h).toObject();

                        if (hitboxObj.contains("Left")) {
                            auto left   = hitboxObj.take("Left");
                            hitbox.left = left.toInt();
                        }

                        if (hitboxObj.contains("Top")) {
                            auto top   = hitboxObj.take("Top");
                            hitbox.top = top.toInt();
                        }

                        if (hitboxObj.contains("Right")) {
                            auto right   = hitboxObj.take("Right");
                            hitbox.right = right.toInt();
                        }

                        if (hitboxObj.contains("Bottom")) {
                            auto bottom   = hitboxObj.take("Bottom");
                            hitbox.bottom = bottom.toInt();
                        }
                    }

                    frame.hitboxes.append(hitbox);
                }
            }
            else {
                return;
            }
        }

        QRect boundingRect;
        boundingRect.setX(frame.sprX);
        boundingRect.setY(frame.sprY);
        boundingRect.setWidth(frame.width);
        boundingRect.setHeight(frame.height);

        uint c     = ui->frameList->currentIndex().row() + 1;
        auto *item = new QStandardItem();
        if (frame.width == 0 || frame.height == 0 || frame.sheet >= sheets.count()) {
            item->setIcon(missingImg);
        }
        else {
            item->setIcon(QPixmap::fromImage(sheets[frame.sheet].copy(boundingRect)));
        }
        frameModel->insertRow(c, item);
        item->setEditable(false);
        animFile.animations[currentAnim].frames.insert(c, frame);
        ui->frameList->blockSignals(false);

        int cur      = currentFrame;
        currentFrame = ui->frameList->currentIndex().row() + 1;
        SetFramePreview();
        currentFrame = cur;

        if (setUI)
            SetupUI();

        DoAction("Imported frame", true);
    });

    connect(ui->expFrame, &QToolButton::clicked, [this] {
        if (ui->animationList->currentRow() == -1)
            return;
        QFileDialog filedialog(this, tr("Export Frame"), QFileInfo(animFile.filePath).absolutePath(),
                               tr("JSON Files (*.json);;RetroScript SpriteFrames (*.txt)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() != QDialog::Accepted)
            return;

        QString selectedFilter = filedialog.selectedNameFilter();
        QString filename       = filedialog.selectedFiles()[0];

        if (selectedFilter == "JSON Files (*.json)") {
            // export json
            QJsonObject aniFileObj;

            int aniVer = 0;
            switch (aniType) {
                case ENGINE_v1: aniVer = 1; break;
                case ENGINE_v2: aniVer = 2; break;
                case ENGINE_v3: aniVer = 3; break;
                case ENGINE_v4: aniVer = 4; break;
                case ENGINE_v5: aniVer = 5; break;
            }

            aniFileObj.insert("Anim Version", aniVer);

            QJsonObject frameObj;

            FormatHelpers::Animation::Frame &f = animFile.animations[currentAnim].frames[currentFrame];

            frameObj.insert("Sheet", animFile.sheets[f.sheet]);
            if (f.collisionBox == 0xFF)
                frameObj.insert("Hitbox", "None");
            else
                frameObj.insert("Hitbox", animFile.hitboxTypes[f.collisionBox]);
            frameObj.insert("ID", f.id);
            frameObj.insert("Duration", f.duration);

            QJsonObject srcObj;
            srcObj.insert("x", f.sprX);
            srcObj.insert("y", f.sprY);
            frameObj.insert("Src", srcObj);

            QJsonObject sizeObj;
            sizeObj.insert("x", f.width);
            sizeObj.insert("y", f.height);
            frameObj.insert("Size", sizeObj);

            QJsonObject pivotObj;
            pivotObj.insert("x", f.pivotX);
            pivotObj.insert("y", f.pivotY);
            frameObj.insert("Pivot", pivotObj);

            QJsonObject hitboxesObj;
            int hitboxID = 0;
            for (auto &h : f.hitboxes) {
                QJsonObject hitboxObj;

                hitboxObj.insert("Left", h.left);
                hitboxObj.insert("Top", h.top);
                hitboxObj.insert("Right", h.right);
                hitboxObj.insert("Bottom", h.bottom);

                hitboxesObj.insert(animFile.hitboxTypes[hitboxID++], hitboxObj);
            }
            frameObj.insert("Hitboxes", hitboxesObj);

            aniFileObj.insert("Frame", frameObj);

            Writer writer(filename);
            writer.write(QJsonDocument(aniFileObj).toJson());
            writer.flush();
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

    QObject::connect(ui->importSheet, &QPushButton::clicked, [this, setupSheetBox] {
        int c = ui->sheetList->currentRow();
        if (c == -1)
            return;

        QFileDialog filedialog(this, tr("Open Image"), "", tr("GIF Images (*.gif)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() != QDialog::Accepted)
            return;
        QString filename = filedialog.selectedFiles()[0];
        QString name     = Utils::getFilenameAndFolder(filename);
        name.truncate(31); // size limit

        ui->sheetList->item(ui->sheetList->currentRow())->setText(name);

        RemoveSheet(c);

        QString wDir = WorkingDirManager::workingDir;
        WorkingDirManager::workingDir += "Sprites/";
        LoadSheet(name, c);
        WorkingDirManager::workingDir = wDir;
        animFile.sheets[c]            = name;
        DoAction("Imported sheet", true);

        setupSheetBox();
        UpdateView();
    });

    connect(ui->addAnim, &QToolButton::clicked, [this] {
        ui->animationList->blockSignals(true);
        uint c = ui->animationList->currentRow() + 1;

        FormatHelpers::Animation::AnimationEntry anim;

        if (aniType != ENGINE_v5) {
            anim.name = "New Anim " + QString::number(animFile.animations.count());
            anim.name.truncate(0x10 - 1);
        }
        else {
            anim.name = "New Animation " + QString::number(animFile.animations.count());
        }

        animFile.animations.insert(c, anim);
        auto *item = new QListWidgetItem(animFile.animations[c].name);
        ui->animationList->insertItem(c, item);

        ui->animationList->blockSignals(false);

        currentAnim = c;
        SetupUI();

        ui->animationList->blockSignals(true);
        ui->animationList->setCurrentRow(c);
        ui->animationList->blockSignals(false);

        ui->addAnim->setDisabled((aniType == ENGINE_v1 || aniType == ENGINE_v2)
                                 || animFile.animations.count() >= 0x100);

        UpdateView();
        DoAction("Added animation", true);
    });

    connect(ui->rmAnim, &QToolButton::clicked, [this] {
        int c = ui->animationList->currentRow();
        int n = ui->animationList->currentRow() == ui->animationList->count() - 1 ? c - 1 : c;
        delete ui->animationList->item(c);

        animFile.animations.removeAt(c);
        SetupUI();

        ui->rmAnim->setDisabled((aniType == ENGINE_v1 || aniType == ENGINE_v2)
                                || animFile.animations.count() <= 0);

        UpdateView();
        DoAction("Removed animation", true);

        ui->animationList->blockSignals(true);
        ui->animationList->setCurrentRow(n);
        ui->animationList->blockSignals(false);
    });

    auto moveAnim = [this](char translation) {
        uint c = ui->animationList->currentRow();
        uint n = ui->animationList->currentRow() + translation;
        if (n >= (uint)animFile.animations.count())
            return;

        auto *item = ui->animationList->takeItem(c);
        animFile.animations.move(c, n);
        ui->animationList->insertItem(n, item);
        SetupUI();

        UpdateView();
        DoAction("Moved animation", true);

        ui->animationList->setCurrentRow(n);
    };

    connect(ui->upAnim, &QToolButton::clicked, [moveAnim] { moveAnim(-1); });

    connect(ui->downAnim, &QToolButton::clicked, [moveAnim] { moveAnim(1); });

    connect(ui->addSheet, &QToolButton::clicked, [this, setupSheetBox] {
        QFileDialog filedialog(this, tr("Open Image"), "", tr("GIF Images (*.gif)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() != QDialog::Accepted)
            return;
        QString filename = filedialog.selectedFiles()[0];
        QString name     = Utils::getFilenameAndFolder(filename);
        name.truncate(31); // size limit

        ui->sheetList->blockSignals(true);
        uint c       = ui->sheetList->currentRow() + 1;
        QString wDir = WorkingDirManager::workingDir;
        WorkingDirManager::workingDir += "Sprites/";
        LoadSheet(name, c);
        WorkingDirManager::workingDir = wDir;
        animFile.sheets[c]            = name;
        DoAction("Added sheet", true);
        auto *item = new QListWidgetItem(name);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->sheetList->insertItem(c, item);

        for (int a = 0; a < animFile.animations.count(); ++a) {
            for (int f = 0; f < animFile.animations[a].frames.count(); ++f) {
                int sheet = animFile.animations[a].frames[f].sheet;
                int p     = ui->sheetList->currentRow();
                if (sheet > p)
                    animFile.animations[a].frames[f].sheet++;
            }
        }

        if (currentFrame < FrameCount()) {
            ui->sheetID->setCurrentIndex(animFile.animations[currentAnim].frames[currentFrame].sheet);
        }
        ui->sheetList->blockSignals(false);

        setupSheetBox();
        UpdateView();
    });

    connect(ui->rmSheet, &QToolButton::clicked, [this, setupSheetBox] {
        int c = ui->sheetList->currentRow();
        int n = ui->sheetList->currentRow() == ui->sheetList->count() - 1 ? c - 1 : c;

        delete ui->sheetList->item(c);
        RemoveSheet(c);

        for (int a = 0; a < animFile.animations.count(); ++a) {
            for (int f = 0; f < animFile.animations[a].frames.count(); ++f) {
                int sheet                              = animFile.animations[a].frames[f].sheet;
                animFile.animations[a].frames[f].sheet = sheet < c   ? sheet
                                                         : sheet > c ? (sheet - 1)
                                                                     : 0;
            }
        }

        setupSheetBox();
        UpdateView();
        DoAction("Removed sheet", true);

        ui->sheetList->setCurrentRow(n);
    });

    auto moveSheetFunc = [this, setupSheetBox](sbyte translation) {
        uint c = ui->sheetList->currentRow();
        uint n = c + translation;
        if (n >= (uint)animFile.sheets.count())
            return;

        QList<byte> idList;

        for (int s = 0; s < animFile.sheets.count(); ++s) idList.append(s);

        auto *item = ui->sheetList->takeItem(c);
        MoveSheet(c, n);
        idList.move(c, n);
        ui->sheetList->insertItem(n, item);

        for (int a = 0; a < animFile.animations.count(); ++a) {
            for (int f = 0; f < animFile.animations[a].frames.count(); ++f) {
                animFile.animations[a].frames[f].sheet = idList[animFile.animations[a].frames[f].sheet];
            }
        }

        setupSheetBox();
        UpdateView();
        DoAction("Moved sheet", true);

        ui->sheetList->setCurrentRow(n);
    };

    connect(ui->addHB, &QToolButton::clicked, [this, setupHiboxTypeBox] {
        uint c = ui->hitboxList->currentRow() + 1;

        animFile.hitboxTypes.insert(c, "Hitbox " + QString::number(c));
        FormatHelpers::Animation::Hitbox hitbox;
        animFile.hitboxes.insert(c, hitbox);

        QString name = animFile.hitboxTypes[c];
        if (aniType != ENGINE_v5)
            QString name = "(0, 0, 0, 0)";

        hitboxVisible.insert(c, false);
        auto *item = new QListWidgetItem(name);
        item->setFlags(item->flags() | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        ui->hitboxList->insertItem(c, item);
        ui->hitboxList->setCurrentItem(item);

        if (aniType == ENGINE_v5) {
            ui->hitboxType->blockSignals(true);
            ui->hitboxType->setCurrentIndex(c);
            ui->hitboxType->blockSignals(false);
            currentHitbox = c;
        }

        if (aniType == ENGINE_v5) {
            for (int a = 0; a < animFile.animations.count(); ++a) {
                for (int f = 0; f < animFile.animations[a].frames.count(); ++f) {
                    FormatHelpers::Animation::Hitbox::HitboxInfo hitbox;
                    animFile.animations[a].frames[f].hitboxes.insert(c, hitbox);
                }
            }
        }
        else {
            for (int a = 0; a < animFile.animations.count(); ++a) {
                for (int f = 0; f < animFile.animations[a].frames.count(); ++f) {
                    if (animFile.animations[a].frames[f].collisionBox >= c)
                        animFile.animations[a].frames[f].collisionBox++;
                }
            }
        }

        ui->addHB->setDisabled(animFile.hitboxes.count() >= 8);

        setupHiboxTypeBox();
        UpdateView();
        DoAction("Added hitbox", true);
    });

    connect(ui->rmHB, &QToolButton::clicked, [this, setupHiboxTypeBox] {
        int c = ui->hitboxList->currentRow();
        int n = ui->hitboxList->currentRow() == ui->hitboxList->count() - 1 ? c - 1 : c;

        delete ui->hitboxList->item(c);
        animFile.hitboxes.removeAt(c);
        animFile.hitboxTypes.removeAt(c);

        if (aniType == ENGINE_v5) {
            for (int a = 0; a < animFile.animations.count(); ++a) {
                for (int f = 0; f < animFile.animations[a].frames.count(); ++f) {
                    animFile.animations[a].frames[f].hitboxes.removeAt(c);
                }
            }
        }
        else {
            for (int a = 0; a < animFile.animations.count(); ++a) {
                for (int f = 0; f < animFile.animations[a].frames.count(); ++f) {
                    if (animFile.animations[a].frames[f].collisionBox >= c)
                        animFile.animations[a].frames[f].collisionBox--;
                }
            }
        }

        ui->rmHB->setDisabled(animFile.hitboxTypes.count() <= 0 || animFile.hitboxes.count() <= 0);

        setupHiboxTypeBox();
        UpdateView();

        DoAction("Removed hitbox", true);

        ui->hitboxList->setCurrentRow(n);
        currentHitbox = c;
    });

    auto moveHB = [this, setupHiboxTypeBox](char translation) {
        uint c = ui->hitboxList->currentRow();
        uint n = c + translation;
        if (n >= (uint)animFile.hitboxTypes.count())
            return;

        auto *item = ui->hitboxList->takeItem(c);
        animFile.hitboxes.move(c, n);
        animFile.hitboxTypes.move(c, n);
        ui->hitboxList->insertItem(n, item);

        for (int a = 0; a < animFile.animations.count(); ++a) {
            for (int f = 0; f < animFile.animations[a].frames.count(); ++f) {
                animFile.animations[a].frames[f].hitboxes.move(c, n);
            }
        }

        setupHiboxTypeBox();
        UpdateView();
        DoAction("Moved hitbox", true);

        ui->hitboxList->setCurrentRow(n);
    };

    connect(ui->upSheet, &QToolButton::clicked, [moveSheetFunc] { moveSheetFunc(-1); });

    connect(ui->downSheet, &QToolButton::clicked, [moveSheetFunc] { moveSheetFunc(1); });

    connect(ui->upHB, &QToolButton::clicked, [moveHB] { moveHB(-1); });

    connect(ui->downHB, &QToolButton::clicked, [moveHB] { moveHB(1); });

    ui->showTransClr->setDown(showTransparentClr);
    connect(ui->showTransClr, &QToolButton::clicked, [this] {
        showTransparentClr = !showTransparentClr;
        ui->showTransClr->setDown(showTransparentClr);
        UpdateView();
    });

    connect(ui->hitboxList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        int c = ui->hitboxList->row(item);
        if ((uint)c < (uint)animFile.hitboxTypes.count()) {
            hitboxVisible[c] = item->checkState() == Qt::Checked;
            UpdateView();
        }
    });

    connect(ui->play, &QToolButton::clicked, [this] {
        if (currentAnim < animFile.animations.count()) {
            if (!playingAnim) {
                StartAnim();
                ui->play->setIcon(playPauseIco[1]);
            }
            else {
                stopAnim();
                ui->frameList->setCurrentIndex(frameModel->index(currentFrame, 0));
                ui->play->setIcon(playPauseIco[0]);
            }

            if (currentFrame >= FrameCount()) {
                currentFrame = 0;
            }
        }
    });

    connect(ui->prevFrame, &QToolButton::clicked, [this] {
        if (currentAnim < animFile.animations.count()) {
            if (currentFrame > 0) {
                --currentFrame;
            }
            else {
                currentFrame = (ushort)(FrameCount() - 1);
            }
            ui->frameList->setCurrentIndex(frameModel->index(currentFrame, 0));
        }
    });

    connect(ui->nextFrame, &QToolButton::clicked, [this] {
        if (currentAnim < animFile.animations.count()) {
            if (currentFrame < FrameCount() - 1) {
                ++currentFrame;
            }
            else {
                currentFrame = 0;
            }
            ui->frameList->setCurrentIndex(frameModel->index(currentFrame, 0));
        }
    });

    connect(ui->skipStart, &QToolButton::clicked, [this] {
        if (currentAnim < animFile.animations.count()) {
            currentFrame = 0;
            ui->frameList->setCurrentIndex(frameModel->index(currentFrame, 0));
        }
    });

    connect(ui->skipEnd, &QToolButton::clicked, [this] {
        if (currentAnim < animFile.animations.count()) {
            currentFrame = (ushort)(FrameCount() - 1);
            ui->frameList->setCurrentIndex(frameModel->index(currentFrame, 0));
        }
    });

    connect(ui->copyAnim, &QToolButton::clicked, [this] {
        int c = ui->animationList->currentRow() + 1;
        if (currentAnim < animFile.animations.count()) {
            ui->animationList->blockSignals(true);

            FormatHelpers::Animation::AnimationEntry anim = animFile.animations[currentAnim];
            animFile.animations.insert(c, anim);

            auto *item = new QListWidgetItem();
            item->setText(anim.name);

            ui->animationList->insertItem(c, item);
            ui->animationList->blockSignals(false);

            currentAnim = c;
            ui->animationList->setCurrentRow(c);

            DoAction("Copied animation", true);
        }
    });

    connect(ui->copyFrame, &QToolButton::clicked, [this] {
        ui->frameList->blockSignals(true);
        int c = ui->frameList->currentIndex().row() + 1;
        if (currentAnim < animFile.animations.count()) {
            if (currentFrame < FrameCount()) {
                FormatHelpers::Animation::Frame frame =
                    animFile.animations[currentAnim].frames[currentFrame];
                animFile.animations[currentAnim].frames.insert(c, frame);

                QRect boundingRect;
                boundingRect.setX(frame.sprX);
                boundingRect.setY(frame.sprY);
                boundingRect.setWidth(frame.width);
                boundingRect.setHeight(frame.height);

                auto *item = new QStandardItem();
                item->setEditable(false);
                item->setData((frame.width == 0 || frame.height == 0)
                                  ? missingImg
                                  : QPixmap::fromImage(sheets[frame.sheet].copy(boundingRect)),
                              ROLE_PIXMAP);
                frameModel->insertRow(c, item);

                DoAction("Copied frame", true);
            }
        }
        ui->frameList->blockSignals(false);
    });

    connect(ui->bgColor, &QToolButton::clicked, [this] {
        RSDKColorDialog dlg(bgColor);
        if (dlg.exec() == QDialog::Accepted) {
            bgColor = dlg.color().toQColor();
            UpdateView();
        }
    });

    if (QFile::exists(filepath)) {
        LoadAnim(filepath, type);
    }

    stopAnim();
    ui->play->setIcon(playPauseIco[0]);

    SetupUI();
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

void AnimationEditor::SetupUI(bool setFrame, bool setRow)
{
    ui->animationList->blockSignals(true);
    ui->animationList->clear();
    for (auto &anim : animFile.animations) {
        ui->animationList->addItem(anim.name);
    }
    ui->animationList->blockSignals(false);

    if (setFrame) {
        ui->frameList->blockSignals(true);
        ui->frameList->setCurrentIndex(frameModel->index(-1, 0));
        ui->frameList->blockSignals(false);
        ui->frameList->setCurrentIndex(frameModel->index(currentFrame, 0));
        SetFramePreview();
    }
    else if (setRow) {
        if (ui->animationList->count() >= 0)
            ui->animationList->setCurrentRow(currentAnim);
        else
            ui->animationList->setCurrentRow(-1);
    }

    ui->sheetList->blockSignals(true);
    ui->sheetID->blockSignals(true);
    ui->sheetList->clear();
    ui->sheetID->clear();
    for (auto &sheet : animFile.sheets) {
        ui->sheetList->addItem(sheet);
        ui->sheetID->addItem(sheet);
    }
    ui->sheetID->blockSignals(false);
    ui->sheetList->blockSignals(false);
    if (setRow) {
        if (ui->sheetList->count() >= 0)
            ui->sheetList->setCurrentRow(0);
        else
            ui->sheetList->setCurrentRow(-1);
    }

    ui->hitboxList->blockSignals(true);
    ui->hitboxType->blockSignals(true);
    ui->hitboxID->blockSignals(true);

    ui->hitboxList->clear();
    ui->hitboxType->clear();
    ui->hitboxID->clear();
    int hID = 0;
    for (auto &hitbox : animFile.hitboxTypes) {
        QString name = hitbox;

        if (aniType != ENGINE_v5 && aniType != ENGINE_v1)
            name = QString("(%1, %2, %3, %4)")
                       .arg(animFile.hitboxes[hID].hitboxes[0].left)
                       .arg(animFile.hitboxes[hID].hitboxes[0].top)
                       .arg(animFile.hitboxes[hID].hitboxes[0].right)
                       .arg(animFile.hitboxes[hID].hitboxes[0].bottom);

        ui->hitboxType->addItem(name);
        QListWidgetItem *item = new QListWidgetItem(name, ui->hitboxList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(hitboxVisible[hID++] ? Qt::Checked : Qt::Unchecked);
    }

    if (aniType == ENGINE_v5 || aniType == ENGINE_v1) {
        ui->hitboxID->addItem("Default");
        ui->hitboxID->setDisabled(true);
    }
    else {
        ui->hitboxID->addItem("Outer Box");
        ui->hitboxID->addItem("Inner Box");
        ui->hitboxID->setDisabled(false);
    }

    ui->hitboxID->blockSignals(false);
    ui->hitboxType->blockSignals(false);
    ui->hitboxList->blockSignals(false);
    if (setRow) {
        if (ui->hitboxList->count() >= 0)
            ui->hitboxList->setCurrentRow(0);
        else
            ui->hitboxList->setCurrentRow(-1);
    }

    ui->animName->setReadOnly(aniType == ENGINE_v1 || aniType == ENGINE_v2);

    ui->animName->setMaxLength((aniType != ENGINE_v4 && aniType != ENGINE_v3) ? (0x400 - 1)
                                                                              : (0x10 - 1));
    ui->speedMult->setRange(aniType == ENGINE_v5 ? -0x8000 : 0x00,
                            aniType == ENGINE_v5 ? 0x7FFF : 0xFF);

    ui->boundingBoxX->setMaximum(aniType == ENGINE_v5 ? 0xFFFF : 0xFF);
    ui->boundingBoxY->setMaximum(aniType == ENGINE_v5 ? 0xFFFF : 0xFF);
    ui->boundingBoxW->setMaximum(aniType == ENGINE_v5 ? 0xFFFF : 0xFF);
    ui->boundingBoxH->setMaximum(aniType == ENGINE_v5 ? 0xFFFF : 0xFF);
    ui->offsetX->setRange(aniType == ENGINE_v5 ? -0x8000 : -0x80, aniType == ENGINE_v5 ? 0x7FFF : 0x7F);
    ui->offsetY->setRange(aniType == ENGINE_v5 ? -0x8000 : -0x80, aniType == ENGINE_v5 ? 0x7FFF : 0x7F);

    ui->hitboxL->setRange(aniType == ENGINE_v5 ? -0x8000 : -0x80, aniType == ENGINE_v5 ? 0x7FFF : 0x7F);
    ui->hitboxT->setRange(aniType == ENGINE_v5 ? -0x8000 : -0x80, aniType == ENGINE_v5 ? 0x7FFF : 0x7F);
    ui->hitboxR->setRange(aniType == ENGINE_v5 ? -0x8000 : -0x80, aniType == ENGINE_v5 ? 0x7FFF : 0x7F);
    ui->hitboxB->setRange(aniType == ENGINE_v5 ? -0x8000 : -0x80, aniType == ENGINE_v5 ? 0x7FFF : 0x7F);

    ui->addAnim->setDisabled(aniType == ENGINE_v1 || aniType == ENGINE_v2);
    ui->rmAnim->setDisabled(aniType == ENGINE_v1 || aniType == ENGINE_v2);
    ui->upAnim->setDisabled(aniType == ENGINE_v1 || aniType == ENGINE_v2);
    ui->downAnim->setDisabled(aniType == ENGINE_v1 || aniType == ENGINE_v2);
    ui->copyAnim->setDisabled(aniType == ENGINE_v1 || aniType == ENGINE_v2);

    ui->addHB->setDisabled(aniType == ENGINE_v1);
    ui->rmHB->setDisabled(aniType == ENGINE_v1);
    ui->upHB->setDisabled(aniType == ENGINE_v1);
    ui->downHB->setDisabled(aniType == ENGINE_v1);

    if (currentAnim < animFile.animations.count()) {
        ui->animName->blockSignals(true);
        ui->speedMult->blockSignals(true);
        ui->loopIndex->blockSignals(true);
        ui->rotationStyle->blockSignals(true);

        ui->animName->setText(animFile.animations[currentAnim].name);
        ui->speedMult->setValue(animFile.animations[currentAnim].speed);
        ui->loopIndex->setValue(animFile.animations[currentAnim].loopIndex);
        ui->rotationStyle->setCurrentIndex(animFile.animations[currentAnim].rotationStyle);

        ui->animName->blockSignals(false);
        ui->speedMult->blockSignals(false);
        ui->loopIndex->blockSignals(false);
        ui->rotationStyle->blockSignals(false);
    }

    // set labels
    if (currentAnim < animFile.animations.count()) {
        ui->properties->setItemText(0, QString("Animations (%1 of %2, ID: %3)")
                                           .arg(currentAnim + 1)
                                           .arg(animFile.animations.count())
                                           .arg(currentAnim));
    }
    else {
        ui->properties->setItemText(0, "Animations");
    }

    if (currentAnim < animFile.animations.count() && currentFrame < FrameCount()) {
        ui->properties->setItemText(1, QString("Frames (%1 of %2, ID: %3)")
                                           .arg(currentFrame + 1)
                                           .arg(FrameCount())
                                           .arg(currentFrame));
    }
    else {
        ui->properties->setItemText(1, "Frames");
    }

    if (ui->sheetList->currentRow() > -1) {
        int c = ui->sheetList->currentRow();

        ui->properties->setItemText(
            2, QString("Sheets (%1 of %2, ID: %3)").arg(c + 1).arg(animFile.sheets.count()).arg(c));
    }
    else {
        ui->properties->setItemText(2, "Sheets");
    }

    if (ui->hitboxList->currentRow() > -1) {
        int c = ui->hitboxList->currentRow();

        ui->properties->setItemText(
            3,
            QString("Hitboxes (%1 of %2, ID: %3)").arg(c + 1).arg(animFile.hitboxTypes.count()).arg(c));
    }
    else {
        ui->properties->setItemText(3, "Hitboxes");
    }

    UpdateView();
}

void AnimationEditor::UpdateView()
{
    QPixmap *view     = new QPixmap(ui->viewerFrame->width(), ui->viewerFrame->height());
    QPainter *painter = new QPainter(view);

    float w = ui->viewerFrame->width(), h = ui->viewerFrame->height();
    float cx = w / 2, cy = h / 2;
    cx -= offset.x;
    cy -= offset.y;

    QBrush bgBrush(bgColor);
    painter->fillRect(0, 0, ui->viewerFrame->width(), ui->viewerFrame->height(), bgBrush);

    painter->setPen(QColor(0x80E0E0E0));
    painter->drawLine(0, cy, w, cy);
    painter->drawLine(cx, 0, cx, h);

    if (currentAnim < animFile.animations.count() && currentFrame < FrameCount()) {
        FormatHelpers::Animation::Frame &frame = animFile.animations[currentAnim].frames[currentFrame];
        if (frame.width && frame.height && frame.sheet < sheets.count()) {
            QRect boundingRect;
            boundingRect.setX(frame.sprX);
            boundingRect.setY(frame.sprY);
            boundingRect.setWidth(frame.width);
            boundingRect.setHeight(frame.height);

            QImage frameImg = sheets[frame.sheet].copy(boundingRect);
            if (frameImg.format() == QImage::Format_Indexed8 && !showTransparentClr)
                frameImg.setColor(0, 0x00000000);
            QImage zoomedFrame = frameImg.scaled(frameImg.width() * zoom, frameImg.height() * zoom);

            QPointF point;
            point.setX(cx + frame.pivotX * zoom);
            point.setY(cy + frame.pivotY * zoom);
            painter->drawImage(point, zoomedFrame);

            for (int h = 0; h < animFile.hitboxTypes.count(); ++h) {
                if (hitboxVisible[h]) {
                    if (aniType == ENGINE_v5 || aniType == ENGINE_v1) {
                        QRect hitbox;
                        hitbox.setX(cx + (frame.hitboxes[h].left * zoom));
                        hitbox.setY(cy + (frame.hitboxes[h].top * zoom));
                        hitbox.setWidth(cx + (frame.hitboxes[h].right * zoom));
                        hitbox.setHeight(cy + (frame.hitboxes[h].bottom * zoom));
                        QBrush hitboxBrush(QColor(255, 0, 0, 0x80));

                        painter->setPen(QColor(255, 0, 0, 0x80));
                        painter->fillRect(hitbox.x(), hitbox.y(), abs(hitbox.x() - hitbox.width()),
                                          abs(hitbox.y() - hitbox.height()), hitboxBrush);
                    }
                    else {
                        auto &frameHitbox = animFile.hitboxes[h];
                        for (int s = 0; s < 2; ++s) {
                            QRect hitbox;
                            hitbox.setX(cx + (frameHitbox.hitboxes[s].left * zoom));
                            hitbox.setY(cy + (frameHitbox.hitboxes[s].top * zoom));
                            hitbox.setWidth(cx + (frameHitbox.hitboxes[s].right * zoom));
                            hitbox.setHeight(cy + (frameHitbox.hitboxes[s].bottom * zoom));
                            QBrush hitboxBrush(QColor(255, 0, 0, 0x80));

                            painter->setPen(QColor(255, 0, 0, 0x80));
                            painter->fillRect(hitbox.x(), hitbox.y(), abs(hitbox.x() - hitbox.width()),
                                              abs(hitbox.y() - hitbox.height()), hitboxBrush);
                        }
                    }
                }
            }
        }
    }

    ui->viewerFrame->setPixmap(*view);

    delete painter;
    delete view;
}

void AnimationEditor::ProcessAnimation()
{
    bool changed = false;

    if (currentAnim < animFile.animations.count() && currentFrame < FrameCount()) {
        int duration = 0;
        switch (aniType) {
            case ENGINE_v5:
                animTimer += animFile.animations[currentAnim].speed;
                duration = animFile.animations[currentAnim].frames[currentFrame].duration;
                while (animTimer > duration) {
                    animTimer -= duration;
                    ++currentFrame;

                    int frameCount = FrameCount();

                    if (currentFrame >= frameCount)
                        currentFrame = animFile.animations[currentAnim].loopIndex;
                    if (animFile.animations[currentAnim].loopIndex >= frameCount)
                        currentFrame = 0;

                    duration = animFile.animations[currentAnim].frames[currentFrame].duration;
                    changed  = true;
                }
                break;

            case ENGINE_v4:
            case ENGINE_v3:
            case ENGINE_v2:
                animTimer += animFile.animations[currentAnim].speed;
                while (animTimer > 240) {
                    animTimer -= 240;
                    ++currentFrame;

                    int frameCount = FrameCount();
                    if (animFile.animations[currentAnim].rotationStyle == 3)
                        frameCount >>= 1;

                    if (currentFrame >= frameCount)
                        currentFrame = animFile.animations[currentAnim].loopIndex;
                    if (animFile.animations[currentAnim].loopIndex >= frameCount)
                        currentFrame = 0;
                    changed = true;
                }
                break;

            case ENGINE_v1:
                animTimer += animFile.animations[currentAnim].speed;
                while (animTimer > 60) {
                    animTimer -= 60;
                    ++currentFrame;

                    int frameCount = FrameCount();

                    if (currentFrame >= frameCount)
                        currentFrame = animFile.animations[currentAnim].loopIndex;
                    if (animFile.animations[currentAnim].loopIndex >= frameCount)
                        currentFrame = 0;
                    changed = true;
                }
                break;
        }
    }

    if (changed)
        UpdateView();
}

void AnimationEditor::SetFramePreview()
{
    if (currentFrame < FrameCount()) {
        FormatHelpers::Animation::Frame &f = animFile.animations[currentAnim].frames[currentFrame];
        QRect boundingRect;
        boundingRect.setX(f.sprX);
        boundingRect.setY(f.sprY);
        boundingRect.setWidth(f.width);
        boundingRect.setHeight(f.height);

        frameModel->itemFromIndex(frameModel->index(currentFrame, 0))
            ->setData((f.width == 0 || f.height == 0 || f.sheet >= sheets.count())
                          ? missingImg
                          : QPixmap::fromImage(sheets[f.sheet].copy(boundingRect)),
                      ROLE_PIXMAP);
    }
}

void AnimationEditor::SetupFrameList(QList<FormatHelpers::Animation::Frame> &frames)
{
    frameModel->clear();
    int fID = 0;
    for (FormatHelpers::Animation::Frame &f : frames) {
        QRect boundingRect;
        boundingRect.setX(f.sprX);
        boundingRect.setY(f.sprY);
        boundingRect.setWidth(f.width);
        boundingRect.setHeight(f.height);

        QStandardItem *item = new QStandardItem;
        item->setEditable(false);
        item->setData((f.width == 0 || f.height == 0 || f.sheet >= sheets.count())
                          ? missingImg
                          : QPixmap::fromImage(sheets[f.sheet].copy(boundingRect)),
                      ROLE_PIXMAP);
        frameModel->appendRow(item);
        ++fID;
    }
}

void AnimationEditor::LoadSheet(QString filepath, int index, bool addSource)
{
    QString fullPath = GetBaseDir() + filepath;

    if (WorkingDirManager::FileExists(filepath, fullPath)) {
        fullPath = WorkingDirManager::GetPath(filepath, fullPath);

        if (QFileInfo(fullPath).suffix().contains("gif")) {
            QGifImage gif(fullPath);

            QImage sheet = gif.frame(0);

            sheets.insert(index, sheet);
        }
        else if (QFileInfo(fullPath).suffix().contains("bmp")) {
            QImage sheet(fullPath);
            sheets.insert(index, sheet);
        }
        else {
            // uh oh floshed
        }
    }
    else {
        sheets.insert(index, missingImg.toImage());
    }

    if (addSource) {
        animFile.sheets.insert(index, Utils::getFilenameAndFolder(filepath));
    }
}

void AnimationEditor::RemoveSheet(int index, bool removeSource)
{
    if (removeSource) {
        animFile.sheets.removeAt(index);
    }

    sheets.removeAt(index);
}

void AnimationEditor::MoveSheet(int from, int to)
{
    animFile.sheets.move(from, to);
    sheets.move(from, to);
}

QString AnimationEditor::GetBaseDir()
{
    QStringList folderList;
    folderList << animFile.filePath.replace("\\", "/").split("/");

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

    return baseDir;
}

void AnimationEditor::LoadAnim(QString filepath, int aniType)
{
    // prolly dont need to do this since its so quick but if you see this loading then hello to all slow
    // PC users out there how do you live with that speed
    SetStatus("Loading animation file...", true);

    stopAnim(); // stop that
    ui->play->setIcon(playPauseIco[0]);

    appConfig.addRecentFile(aniType, TOOL_ANIMATIONEDITOR, filepath, QList<QString>{});

    this->aniType = aniType;
    animFile      = FormatHelpers::Animation(aniType, filepath);
    AddStatusProgress(0.33); // finished loading animation

    int id = 0;
    for (auto &sheet : animFile.sheets) {
        ui->sheetID->addItem(sheet);

        LoadSheet(sheet, id++, false);
    }
    AddStatusProgress(0.33); // finished loading sheets

    hitboxVisible.clear();
    for (int h = 0; h < animFile.hitboxTypes.count(); ++h) {
        hitboxVisible.append(false);
    }

    if (aniType == ENGINE_v5 || aniType == ENGINE_v1)
        currentSubHitbox = -1;
    else
        currentSubHitbox = 0;

    currentAnim   = -1;
    currentFrame  = -1;
    currentHitbox = -1;

    ui->animationList->blockSignals(true);
    ui->animationList->setCurrentRow(-1);
    ui->animationList->blockSignals(false);

    frameModel->clear();

    ui->sheetList->blockSignals(true);
    ui->sheetList->setCurrentRow(-1);
    ui->sheetList->blockSignals(false);

    ui->hitboxList->blockSignals(true);
    ui->hitboxList->setCurrentRow(-1);
    ui->hitboxList->blockSignals(false);

    tabTitle = Utils::getFilenameAndFolder(animFile.filePath);
    ClearActions();
    SetupUI();

    SetStatus("Loaded animation file " + tabTitle); // done!
}

void AnimationEditor::RotateHitboxes()
{
    if (aniType == ENGINE_v1 || ENGINE_v5)
        return;

    auto hitboxes = animFile.hitboxes;

    for (int h = 0; h < animFile.hitboxes.count(); ++h) {
        // Outer box
        if (true) {
            // LWall
            if (true) {
                int store                                 = -animFile.hitboxes[h].hitboxes[2 + 0].left;
                animFile.hitboxes[h].hitboxes[2 + 0].left = -animFile.hitboxes[h].hitboxes[2 + 0].right;
                animFile.hitboxes[h].hitboxes[2 + 0].right = store;
            }

            // Roof
            if (true) {
                int store                                = -animFile.hitboxes[h].hitboxes[4 + 0].top;
                animFile.hitboxes[h].hitboxes[4 + 0].top = -animFile.hitboxes[h].hitboxes[4 + 0].bottom;
                animFile.hitboxes[h].hitboxes[4 + 0].bottom = store;
            }

            // RWall
            if (true) {
                int store                                 = -animFile.hitboxes[h].hitboxes[6 + 0].left;
                animFile.hitboxes[h].hitboxes[6 + 0].left = -animFile.hitboxes[h].hitboxes[6 + 0].right;
                animFile.hitboxes[h].hitboxes[6 + 0].right = store;

                store                                    = -animFile.hitboxes[h].hitboxes[6 + 0].top;
                animFile.hitboxes[h].hitboxes[6 + 0].top = -animFile.hitboxes[h].hitboxes[6 + 0].bottom;
                animFile.hitboxes[h].hitboxes[6 + 0].bottom = store;
            }
        }

        // Inner box
        if (true) {
            // LWall
            if (true) {
                int store                                 = -animFile.hitboxes[h].hitboxes[2 + 1].left;
                animFile.hitboxes[h].hitboxes[2 + 1].left = -animFile.hitboxes[h].hitboxes[2 + 1].right;
                animFile.hitboxes[h].hitboxes[2 + 1].right = store;
            }

            // Roof
            if (true) {
                int store                                = -animFile.hitboxes[h].hitboxes[4 + 1].top;
                animFile.hitboxes[h].hitboxes[4 + 1].top = -animFile.hitboxes[h].hitboxes[4 + 1].bottom;
                animFile.hitboxes[h].hitboxes[4 + 1].bottom = store;
            }

            // RWall
            if (true) {
                int store                                 = -animFile.hitboxes[h].hitboxes[6 + 1].left;
                animFile.hitboxes[h].hitboxes[6 + 1].left = -animFile.hitboxes[h].hitboxes[6 + 1].right;
                animFile.hitboxes[h].hitboxes[6 + 1].right = store;

                store                                    = -animFile.hitboxes[h].hitboxes[6 + 1].top;
                animFile.hitboxes[h].hitboxes[6 + 1].top = -animFile.hitboxes[h].hitboxes[6 + 1].bottom;
                animFile.hitboxes[h].hitboxes[6 + 1].bottom = store;
            }
        }
    }
}

bool AnimationEditor::event(QEvent *event)
{
    QStringList types = { "RSDKv5 Animation Files (*.bin)", "RSDKv4/RSDKv3 Animation Files (*.ani)",
                          "RSDKv2 Animation Files (*.ani)", "RSDKv1 Animation Files (*.ani)" };
    QList<QString> typesList = { "RSDKv5 Animation Files (*.bin)",
                                 "RSDKv4/RSDKv3 Animation Files (*.ani)",
                                 "RSDKv4/RSDKv3 Animation Files (*.ani)",
                                 "RSDKv2 Animation Files (*.ani)", "RSDKv1 Animation Files (*.ani)" };

    switch ((int)event->type()) {
        default: break;

        case RE_EVENT_NEW:
            animFile = FormatHelpers::Animation();
            tabTitle = "Animation Editor";
            ClearActions();
            hitboxVisible.clear();
            SetupUI();
            return true;

        case RE_EVENT_OPEN: {
            QFileDialog filedialog(this, tr("Open Animation"), "",
                                   tr(types.join(";;").toStdString().c_str()));
            filedialog.setAcceptMode(QFileDialog::AcceptOpen);
            if (filedialog.exec() == QDialog::Accepted) {
                SetStatus("Opening animation \""
                          + Utils::getFilenameAndFolder(filedialog.selectedFiles()[0]) + "\"");

                QString filepath = filedialog.selectedFiles()[0];
                aniType          = typesList.indexOf(filedialog.selectedNameFilter());
                LoadAnim(filepath, aniType);
                return true;
            }
            break;
        }

        case RE_EVENT_SAVE:
            if (!QFile::exists(animFile.filePath)) {
                QFileDialog filedialog(this, tr("Save Animation"), "",
                                       tr(types.join(";;").toStdString().c_str()));

                switch (aniType) {
                    default:
                    case ENGINE_v5: filedialog.selectNameFilter(types[0]); break;
                    case ENGINE_v4:
                    case ENGINE_v3: filedialog.selectNameFilter(types[1]); break;
                    case ENGINE_v2: filedialog.selectNameFilter(types[2]); break;
                    case ENGINE_v1: filedialog.selectNameFilter(types[3]); break;
                }

                filedialog.setAcceptMode(QFileDialog::AcceptSave);
                if (filedialog.exec() == QDialog::Accepted) {
                    SetStatus("Saving animation...");

                    QString filepath = filedialog.selectedFiles()[0];
                    int type         = typesList.indexOf(filedialog.selectedNameFilter());
                    switch (type) {
                        default:
                        case 0: aniType = ENGINE_v5; break;
                        case 1: aniType = ENGINE_v4; break;
                        case 2: aniType = ENGINE_v3; break;
                        case 3: aniType = ENGINE_v2; break;
                        case 4: aniType = ENGINE_v1; break;
                    }

                    RotateHitboxes();
                    animFile.write(aniType, filepath);
                    appConfig.addRecentFile(aniType, TOOL_ANIMATIONEDITOR, filepath, QList<QString>{});
                    SetStatus("Saved animation to " + QFile(animFile.filePath).fileName());
                    ClearActions();
                    return true;
                }
            }
            else {
                SetStatus("Saving animation...");

                QString filepath = animFile.filePath;

                RotateHitboxes();
                animFile.write(aniType, filepath);
                appConfig.addRecentFile(aniType, TOOL_ANIMATIONEDITOR, filepath, QList<QString>{});
                SetStatus("Saved animation to " + QFile(animFile.filePath).fileName());
                ClearActions();
                return true;
            }
            break;

        case RE_EVENT_SAVE_AS: {
            QFileDialog filedialog(
                this, tr("Save Animation"),
                QFile::exists(animFile.filePath) ? QFileInfo(animFile.filePath).absolutePath() : "",
                tr(types.join(";;").toStdString().c_str()));

            switch (aniType) {
                default:
                case ENGINE_v5: filedialog.selectNameFilter(types[0]); break;
                case ENGINE_v4:
                case ENGINE_v3: filedialog.selectNameFilter(types[1]); break;
                case ENGINE_v2: filedialog.selectNameFilter(types[2]); break;
                case ENGINE_v1: filedialog.selectNameFilter(types[3]); break;
            }

            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {
                SetStatus("Saving animation...");

                QString filepath = filedialog.selectedFiles()[0];
                int type         = typesList.indexOf(filedialog.selectedNameFilter());
                switch (type) {
                    default:
                    case 0: aniType = ENGINE_v5; break;
                    case 1: aniType = ENGINE_v4; break;
                    case 2: aniType = ENGINE_v3; break;
                    case 3: aniType = ENGINE_v2; break;
                    case 4: aniType = ENGINE_v1; break;
                }

                RotateHitboxes();
                animFile.write(aniType, filepath);
                appConfig.addRecentFile(aniType, TOOL_ANIMATIONEDITOR, filepath, QList<QString>{});
                SetStatus("Saved animation to " + QFile(animFile.filePath).fileName());
                ClearActions();
                return true;
            }
            break;
        }

        case RE_EVENT_UNDO: UndoAction(); return true;

        case RE_EVENT_REDO: RedoAction(); return true;

        case QEvent::MouseButtonPress: {
            QMouseEvent *mEvent = static_cast<QMouseEvent *>(event);
            reference           = mEvent->pos();

            mousePos.x = mEvent->pos().x();
            mousePos.y = mEvent->pos().y();

            if ((mEvent->button() & Qt::LeftButton) == Qt::LeftButton)
                mouseDownL = true;
            if ((mEvent->button() & Qt::MiddleButton) == Qt::MiddleButton)
                mouseDownM = true;
            if ((mEvent->button() & Qt::RightButton) == Qt::RightButton)
                mouseDownR = true;

            if (mouseDownM)
                setCursor(Qt::ClosedHandCursor);

            break;
        }

        case QEvent::MouseMove: {
            // bool status         = false;
            QMouseEvent *mEvent = static_cast<QMouseEvent *>(event);

            mousePos.x = mEvent->pos().x();
            mousePos.y = mEvent->pos().y();

            if (mouseDownL || mouseDownM) {
                offset.x -= mousePos.x - reference.x();
                offset.y -= mousePos.y - reference.y();
                ui->frameOffLabel->setText(
                    QString("Frame Offset: (%1, %2)").arg(offset.x).arg(offset.y));
                reference = mEvent->pos();
                // status    = true;
                UpdateView();
            }
            break;
        }

        case QEvent::MouseButtonRelease: {
            QMouseEvent *mEvent = static_cast<QMouseEvent *>(event);

            mousePos.x = mEvent->pos().x();
            mousePos.y = mEvent->pos().y();

            if ((mEvent->button() & Qt::LeftButton) == Qt::LeftButton)
                mouseDownL = false;
            if ((mEvent->button() & Qt::MiddleButton) == Qt::MiddleButton)
                mouseDownM = false;
            if ((mEvent->button() & Qt::RightButton) == Qt::RightButton)
                mouseDownR = false;

            unsetCursor();
            break;
        }

        case QEvent::Wheel: {
            QWheelEvent *wEvent = static_cast<QWheelEvent *>(event);

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
            QPoint mousePos = QPoint(wEvent->x(), wEvent->y());
#else
            QPoint mousePos = QPoint(wEvent->position().x(), wEvent->position().y());
#endif

            if (ui->viewerFrame->rect().contains(mousePos.x(), mousePos.y())) {
                if (wEvent->modifiers() & Qt::ControlModifier) {
                    if (wEvent->angleDelta().y() > 0 && zoom < 20)
                        zoom += 1;
                    else if (wEvent->angleDelta().y() < 0 && zoom > 1.5)
                        zoom -= 1;

                    // round to nearest whole number
                    zoom = (int)zoom;
                    if (zoom < 1)
                        zoom = 1;
                }
                else {
                    if (wEvent->angleDelta().y() > 0 && zoom < 20)
                        zoom *= 1.1f;
                    else if (wEvent->angleDelta().y() < 0 && zoom > 0.5)
                        zoom /= 1.1f;
                }
                ui->zoomLabel->setText(QString("Zoom: %1%").arg(zoom * 100));
                UpdateView();
                return true;
            }
            break;
        }
        case QEvent::Resize: UpdateView(); break;
        case QEvent::Close:
            if (modified) {
                bool cancelled = false;
                if (MainWindow::ShowCloseWarning(this, &cancelled)) {
                    if (!QFile::exists(animFile.filePath)) {
                        QFileDialog filedialog(this, tr("Save Animation"), "",
                                               tr(typesList[aniType].toStdString().c_str()));
                        filedialog.setAcceptMode(QFileDialog::AcceptSave);
                        if (filedialog.exec() == QDialog::Accepted) {
                            SetStatus("Saving animation...");

                            QString filepath = filedialog.selectedFiles()[0];

                            RotateHitboxes();
                            animFile.write(aniType, filepath);
                            appConfig.addRecentFile(aniType, TOOL_ANIMATIONEDITOR, filepath,
                                                    QList<QString>{});
                            SetStatus("Saved animation to " + QFile(animFile.filePath).fileName());
                            ClearActions();
                            return true;
                        }
                    }
                    else {
                        SetStatus("Saving animation...");

                        QString filepath = animFile.filePath;

                        RotateHitboxes();
                        animFile.write(aniType, filepath);
                        appConfig.addRecentFile(aniType, TOOL_ANIMATIONEDITOR, filepath,
                                                QList<QString>{});
                        SetStatus("Saved animation to " + QFile(animFile.filePath).fileName());
                        ClearActions();
                        return true;
                    }
                }
                else if (cancelled) {
                    event->ignore();
                    return true;
                }
            }
            break;
    }

    return QWidget::event(event);
}

void AnimationEditor::UndoAction()
{
    if (actionIndex > 0) {
        // setStatus("Undid Action: " + actions[actionIndex].name);
        actionIndex--;
        ResetAction();
    }
}
void AnimationEditor::RedoAction()
{
    if (actionIndex + 1 < actions.count()) {
        // setStatus("Redid Action: " + actions[actionIndex].name);
        actionIndex++;
        ResetAction();
    }
}
void AnimationEditor::ResetAction()
{
#if RE_USE_UNSTABLE
    CopyAnimFile(actions[actionIndex].animFile, animFile);
    hitboxVisible.clear();
    for (int h = 0; h < animFile.hitboxTypes.count(); ++h) hitboxVisible.append(false);

    SetupUI(true);
#endif

    UpdateTitle(actionIndex > 0);
}
void AnimationEditor::DoAction(QString name, bool setModified)
{
    ActionState action;
    action.name = name;

#if RE_USE_UNSTABLE
    CopyAnimFile(animFile, action.animFile);

#endif
    // Actions
    for (int i = actions.count() - 1; i > actionIndex; --i) {
        actions.removeAt(i);
    }

    actions.append(action);
    actionIndex = actions.count() - 1;

    UpdateTitle(setModified);

    // setStatus("Did Action: " + name);
}
void AnimationEditor::ClearActions()
{
    actions.clear();
    actionIndex = 0;
    DoAction("Action Setup", false); // first action, cant be undone
}

void AnimationEditor::CopyAnimFile(FormatHelpers::Animation &src, FormatHelpers::Animation &dst)
{
    dst.filePath   = src.filePath;
    dst.playerType = src.playerType;
    dst.unknown2   = src.unknown2;
    for (int i = 0; i < 5; ++i) dst.unknown[i] = src.unknown[i];

    dst.animations.clear();
    for (auto &a : src.animations) {
        FormatHelpers::Animation::AnimationEntry anim;
        anim.name          = a.name;
        anim.loopIndex     = a.loopIndex;
        anim.speed         = a.speed;
        anim.rotationStyle = a.rotationStyle;
        anim.frames.clear();
        for (auto &f : a.frames) {
            FormatHelpers::Animation::Frame frame;
            frame.sheet        = f.sheet;
            frame.collisionBox = f.collisionBox;
            frame.duration     = f.duration;
            frame.id           = f.id;
            frame.sprX         = f.sprX;
            frame.sprY         = f.sprY;
            frame.width        = f.width;
            frame.height       = f.height;
            frame.pivotX       = f.pivotX;
            frame.pivotY       = f.pivotY;

            for (auto &h : f.hitboxes) {
                FormatHelpers::Animation::Hitbox::HitboxInfo hitbox;
                hitbox.left   = h.left;
                hitbox.top    = h.top;
                hitbox.right  = h.right;
                hitbox.bottom = h.bottom;
                frame.hitboxes.append(hitbox);
            }
            anim.frames.append(frame);
        }
        dst.animations.append(anim);
    }

    dst.sheets.clear();
    for (auto &s : src.sheets) {
        dst.sheets.append(s);
    }

    dst.hitboxTypes.clear();
    for (auto &t : src.hitboxTypes) {
        dst.hitboxTypes.append(t);
    }

    dst.hitboxes.clear();
    for (auto &i : src.hitboxes) {
        FormatHelpers::Animation::Hitbox info;
        for (int h = 0; h < 8; ++h) {
            info.hitboxes[h].left   = i.hitboxes[h].left;
            info.hitboxes[h].top    = i.hitboxes[h].top;
            info.hitboxes[h].right  = i.hitboxes[h].right;
            info.hitboxes[h].bottom = i.hitboxes[h].bottom;
        }
        dst.hitboxes.append(info);
    }
}

#include "moc_animationeditor.cpp"
