#include "includes.hpp"

Property::Property(QString name, unsigned char *value)
{
    QtIntPropertyManager *m = new QtIntPropertyManager();
    typeManager             = m;
    p                       = m->addProperty(name);
    m->setValue(p, *value);
    m->setRange(p, 0, 255);
    type     = BYTE_MANAGER;
    valuePtr = value;

    connect(m, &QtIntPropertyManager::valueChanged, this, [=](QtProperty *, int x) {
        *value = x;
        emit changed();
    });
}

Property::Property(QString name, signed char *value)
{
    QtIntPropertyManager *m = new QtIntPropertyManager();
    typeManager             = m;
    p                       = m->addProperty(name);
    m->setValue(p, *value);
    m->setRange(p, -128, 127);
    type     = SBYTE_MANAGER;
    valuePtr = value;

    int v = *value;
    connect(m, &QtIntPropertyManager::valueChanged, this, [=](QtProperty *, int x) {
        *value = x;
        emit changed();
    });
}

Property::Property(QString name, int *value)
{
    QtIntPropertyManager *m = new QtIntPropertyManager();
    typeManager             = m;
    p                       = m->addProperty(name);
    m->setValue(p, *value);
    m->setRange(p, -2147483648, 2147483647);
    type     = INT_MANAGER;
    valuePtr = value;
    connect(m, &QtIntPropertyManager::valueChanged, this, [=](QtProperty *, int x) {
        *value = x;
        emit changed();
    });
}

Property::Property(QString name, uint *value)
{
    QtIntPropertyManager *m = new QtIntPropertyManager();
    typeManager             = m;
    p                       = m->addProperty(name);
    m->setValue(p, *value);
    m->setRange(p, 0, 4294967295);
    type     = INT_MANAGER;
    valuePtr = value;
    connect(m, &QtIntPropertyManager::valueChanged, this, [=](QtProperty *, int x) {
        *value = x;
        emit changed();
    });
}

Property::Property(QString name, short *value)
{
    QtIntPropertyManager *m = new QtIntPropertyManager();
    typeManager             = m;
    p                       = m->addProperty(name);
    m->setValue(p, *value);
    m->setRange(p, -32768, 32767);
    type     = SHORT_MANAGER;
    valuePtr = value;
    connect(m, &QtIntPropertyManager::valueChanged, this, [=](QtProperty *, int x) {
        *value = x;
        emit changed();
    });
}

Property::Property(QString name, ushort *value)
{
    QtIntPropertyManager *m = new QtIntPropertyManager();
    typeManager             = m;
    p                       = m->addProperty(name);
    m->setValue(p, *value);
    m->setRange(p, 0, 65535);
    type     = USHORT_MANAGER;
    valuePtr = value;
    connect(m, &QtIntPropertyManager::valueChanged, this, [=](QtProperty *, int x) {
        *value = x;
        emit changed();
    });
}

Property::Property(QString name, float *value)
{
    QtDoublePropertyManager *m = new QtDoublePropertyManager();
    typeManager                = m;
    p                          = m->addProperty(name);
    m->setValue(p, *value);
    m->setRange(p, 3.4 * pow(10, 38), -3.4 * pow(10, 38));
    type     = FLOAT_MANAGER;
    valuePtr = value;
    connect(m, &QtDoublePropertyManager::valueChanged, this, [=](QtProperty *, float x) {
        *value = x;
        emit changed();
    });
}

Property::Property(QString name, bool *value)
{
    QtBoolPropertyManager *m = new QtBoolPropertyManager();
    typeManager              = m;
    p                        = m->addProperty(name);
    m->setValue(p, *value);
    type     = BOOL_MANAGER;
    valuePtr = value;
    connect(m, &QtBoolPropertyManager::valueChanged, this, [=](QtProperty *, bool x) {
        *value = x;
        emit changed();
    });
}

Property::Property(QString name, QString *value)
{
    QtStringPropertyManager *m = new QtStringPropertyManager();
    typeManager                = m;
    p                          = m->addProperty(name);
    m->setValue(p, *value);
    type     = STRING_MANAGER;
    valuePtr = value;
    connect(m, &QtStringPropertyManager::valueChanged, this, [=](QtProperty *, QString x) {
        *value = x;
        emit changed();
    });
}

Property::Property(QString name, QStringList names, void *value, byte type)
{
    QtEnumPropertyManager *m = new QtEnumPropertyManager();
    typeManager              = m;
    p                        = m->addProperty(name);
    m->setEnumNames(p, names);
    switch (type) {
        default: break;
        case BYTE_MANAGER: m->setValue(p, *(byte *)value); break;
        case SBYTE_MANAGER: m->setValue(p, *(sbyte *)value); break;
        case SHORT_MANAGER: m->setValue(p, *(short *)value); break;
        case USHORT_MANAGER: m->setValue(p, *(ushort *)value); break;
        case INT_MANAGER: m->setValue(p, *(int *)value); break;
        case UINT_MANAGER:
            m->setValue(p, *(uint *)value);
            break;
            // case BOOL_MANAGER: m->setValue(p, *(bool *)value); break;
            // case STRING_MANAGER: m->setValue(p, *(QString *)value); break;
            // case FLOAT_MANAGER: m->setValue(p, *(float *)value); break;
            // case COLOR_MANAGER: m->setValue(p, ((Colour *)value)->toUInt()); break;
    }
    this->type    = ENUM_MANAGER;
    this->varType = type;
    valuePtr      = value;
    connect(m, &QtEnumPropertyManager::valueChanged, this, [=](QtProperty *, int x) {
        switch (this->varType) {
            default: break;
            case BYTE_MANAGER: *reinterpret_cast<byte *>(value) = x; break;
            case SBYTE_MANAGER: *reinterpret_cast<sbyte *>(value) = x; break;
            case SHORT_MANAGER: *reinterpret_cast<short *>(value) = x; break;
            case USHORT_MANAGER: *reinterpret_cast<ushort *>(value) = x; break;
            case INT_MANAGER: *reinterpret_cast<int *>(value) = x; break;
            case UINT_MANAGER:
                *reinterpret_cast<uint *>(value) = x;
                break;
                // case BOOL_MANAGER: *reinterpret_cast<bool *>(value) = x; break;
                // case STRING_MANAGER: *reinterpret_cast<QString *>(value) = x; break;
                // case FLOAT_MANAGER: *reinterpret_cast<float *>(value) = x; break;
                // case COLOR_MANAGER: *reinterpret_cast<Colour *>(value) = Colour((uint)x);
                // break;
        }
        emit changed();
    });
}

Property::Property(QString name, QList<PropertyValue> valueList, void *value, byte type)
{
    QStringList names;
    for (PropertyValue &val : valueList) {
        names.append(val.name);
    }

    QtEnumPropertyManager *m = new QtEnumPropertyManager();
    typeManager              = m;
    p                        = m->addProperty(name);
    m->setEnumNames(p, names);
    switch (type) {
        default: break;
        case BYTE_MANAGER: m->setValue(p, *(byte *)value); break;
        case SBYTE_MANAGER: m->setValue(p, *(sbyte *)value); break;
        case SHORT_MANAGER: m->setValue(p, *(short *)value); break;
        case USHORT_MANAGER: m->setValue(p, *(ushort *)value); break;
        case INT_MANAGER: m->setValue(p, *(int *)value); break;
        case UINT_MANAGER:
            m->setValue(p, *(uint *)value);
            break;
            // case BOOL_MANAGER: m->setValue(p, *(bool *)value); break;
            // case STRING_MANAGER: m->setValue(p, *(QString *)value); break;
            // case FLOAT_MANAGER: m->setValue(p, *(float *)value); break;
            // case COLOR_MANAGER: m->setValue(p, ((Colour *)value)->toUInt()); break;
    }
    this->type    = ENUM_MANAGER;
    this->varType = type;
    valuePtr      = value;
    connect(m, &QtEnumPropertyManager::valueChanged, this, [=](QtProperty *, int x) {
        switch (this->varType) {
            default: break;
            case BYTE_MANAGER: *reinterpret_cast<byte *>(value) = valueList[x].value.toUInt(); break;
            case SBYTE_MANAGER: *reinterpret_cast<sbyte *>(value) = valueList[x].value.toInt(); break;
            case SHORT_MANAGER: *reinterpret_cast<short *>(value) = valueList[x].value.toInt(); break;
            case USHORT_MANAGER:
                *reinterpret_cast<ushort *>(value) = valueList[x].value.toUInt();
                break;
            case INT_MANAGER: *reinterpret_cast<int *>(value) = valueList[x].value.toInt(); break;
            case UINT_MANAGER: *reinterpret_cast<uint *>(value) = valueList[x].value.toUInt(); break;
            case BOOL_MANAGER: *reinterpret_cast<bool *>(value) = valueList[x].value.toBool(); break;
            case STRING_MANAGER:
                *reinterpret_cast<QString *>(value) = valueList[x].value.toString();
                break;
            case FLOAT_MANAGER:
                *reinterpret_cast<float *>(value) = valueList[x].value.toFloat();
                break;
                // case COLOR_MANAGER:
                //    *reinterpret_cast<Colour *>(value) = Colour(valueList[x].value.toUInt());
                //    break;
        }
        emit changed();
    });
}

Property::Property(QString name, QColor *value)
{
    QtColorPropertyManager *m = new QtColorPropertyManager();
    typeManager               = m;
    p                         = m->addProperty(name);
    m->setValue(p, *value);
    type     = COLOR_MANAGER;
    valuePtr = value;
    connect(m, &QtColorPropertyManager::valueChanged, this, [=](QtProperty *, QColor x) {
        *value = x;
        emit changed();
    });
}

Property::Property(QString name)
{
    QtStringPropertyManager *m = new QtStringPropertyManager();
    typeManager                = m;
    p                          = m->addProperty(name);
    type                       = GROUP_MANAGER;
}

void Property::setValuePtr(void *newPtr)
{
    valuePtr = newPtr;
    switch (type) {
        case BYTE_MANAGER: {
            QtIntPropertyManager *m = static_cast<QtIntPropertyManager *>(typeManager);
            disconnect(m, &QtIntPropertyManager::valueChanged, nullptr, nullptr);
            m->setValue(p, *static_cast<unsigned char *>(valuePtr));
            connect(m, &QtIntPropertyManager::valueChanged, this, [=](QtProperty *, int x) {
                *(static_cast<unsigned char *>(valuePtr)) = x;
                emit changed();
            });
        } break;
        case SBYTE_MANAGER: {
            QtIntPropertyManager *m = static_cast<QtIntPropertyManager *>(typeManager);
            disconnect(m, &QtIntPropertyManager::valueChanged, nullptr, nullptr);
            m->setValue(p, *static_cast<signed char *>(valuePtr));
            connect(m, &QtIntPropertyManager::valueChanged, this, [=](QtProperty *, int x) {
                *(static_cast<signed char *>(valuePtr)) = x;
                emit changed();
            });
        } break;
        case INT_MANAGER: {
            QtIntPropertyManager *m = static_cast<QtIntPropertyManager *>(typeManager);
            disconnect(m, &QtIntPropertyManager::valueChanged, nullptr, nullptr);
            m->setValue(p, *static_cast<int *>(valuePtr));
            connect(m, &QtIntPropertyManager::valueChanged, this, [=](QtProperty *, int x) {
                *(static_cast<int *>(valuePtr)) = x;
                emit changed();
            });
        } break;
        case UINT_MANAGER: {
            QtIntPropertyManager *m = static_cast<QtIntPropertyManager *>(typeManager);
            disconnect(m, &QtIntPropertyManager::valueChanged, nullptr, nullptr);
            m->setValue(p, *static_cast<uint *>(valuePtr));
            connect(m, &QtIntPropertyManager::valueChanged, this, [=](QtProperty *, int x) {
                *(static_cast<uint *>(valuePtr)) = x;
                emit changed();
            });
        } break;
        case SHORT_MANAGER: {
            QtIntPropertyManager *m = static_cast<QtIntPropertyManager *>(typeManager);
            disconnect(m, &QtIntPropertyManager::valueChanged, nullptr, nullptr);
            m->setValue(p, *static_cast<short *>(valuePtr));
            connect(m, &QtIntPropertyManager::valueChanged, this, [=](QtProperty *, int x) {
                *(static_cast<short *>(valuePtr)) = x;
                emit changed();
            });
        } break;
        case FLOAT_MANAGER: {
            QtDoublePropertyManager *m = static_cast<QtDoublePropertyManager *>(typeManager);
            disconnect(m, &QtDoublePropertyManager::valueChanged, nullptr, nullptr);
            m->setValue(p, *static_cast<float *>(valuePtr));
            connect(m, &QtDoublePropertyManager::valueChanged, this, [=](QtProperty *, float x) {
                *(static_cast<float *>(valuePtr)) = x;
                emit changed();
            });
        } break;
        case USHORT_MANAGER: {
            QtIntPropertyManager *m = static_cast<QtIntPropertyManager *>(typeManager);
            disconnect(m, &QtIntPropertyManager::valueChanged, nullptr, nullptr);
            m->setValue(p, *static_cast<ushort *>(valuePtr));
            connect(m, &QtIntPropertyManager::valueChanged, this, [=](QtProperty *, int x) {
                *(static_cast<ushort *>(valuePtr)) = x;
                emit changed();
            });
        } break;
        case BOOL_MANAGER: {
            QtBoolPropertyManager *m = static_cast<QtBoolPropertyManager *>(typeManager);
            disconnect(m, &QtBoolPropertyManager::valueChanged, nullptr, nullptr);
            m->setValue(p, *static_cast<bool *>(valuePtr));
            connect(m, &QtBoolPropertyManager::valueChanged, this, [=](QtProperty *, bool x) {
                *static_cast<bool *>(valuePtr) = x;
                emit changed();
            });
        } break;
        case STRING_MANAGER: {
            QtStringPropertyManager *m = static_cast<QtStringPropertyManager *>(typeManager);
            disconnect(m, &QtStringPropertyManager::valueChanged, nullptr, nullptr);
            m->setValue(p, *static_cast<QString *>(valuePtr));
            connect(m, &QtStringPropertyManager::valueChanged, this, [=](QtProperty *, QString x) {
                *static_cast<QString *>(valuePtr) = x;
                emit changed();
            });
        } break;
        case ENUM_MANAGER: {
            QtEnumPropertyManager *m = static_cast<QtEnumPropertyManager *>(typeManager);
            disconnect(m, &QtEnumPropertyManager::valueChanged, nullptr, nullptr);
            m->setValue(p, *static_cast<unsigned char *>(valuePtr));
            connect(m, &QtEnumPropertyManager::valueChanged, this, [=](QtProperty *, int x) {
                *static_cast<int *>(valuePtr) = x;
                emit changed();
            });
        } break;
        case COLOR_MANAGER: {
            QtColorPropertyManager *m = static_cast<QtColorPropertyManager *>(typeManager);
            disconnect(m, &QtColorPropertyManager::valueChanged, nullptr, nullptr);
            m->setValue(p, *static_cast<QColor *>(valuePtr));
            connect(m, &QtColorPropertyManager::valueChanged, this, [=](QtProperty *, QColor x) {
                *static_cast<QColor *>(valuePtr) = x;
                emit changed();
            });
            break;
        }
    }
}

void Property::updateValue()
{
    switch (type) {
        case BYTE_MANAGER: {
            QtIntPropertyManager *m = static_cast<QtIntPropertyManager *>(typeManager);
            m->blockSignals(true);
            m->setValue(p, *static_cast<unsigned char *>(valuePtr));
            m->blockSignals(false);
            break;
        }
        case SBYTE_MANAGER: {
            QtIntPropertyManager *m = static_cast<QtIntPropertyManager *>(typeManager);
            m->blockSignals(true);
            m->setValue(p, *static_cast<signed char *>(valuePtr));
            m->blockSignals(false);
            break;
        }
        case INT_MANAGER: {
            QtIntPropertyManager *m = static_cast<QtIntPropertyManager *>(typeManager);
            m->blockSignals(true);
            m->setValue(p, *static_cast<int *>(valuePtr));
            m->blockSignals(false);
            break;
        }
        case UINT_MANAGER: {
            QtIntPropertyManager *m = static_cast<QtIntPropertyManager *>(typeManager);
            m->blockSignals(true);
            m->setValue(p, *static_cast<uint *>(valuePtr));
            m->blockSignals(false);
            break;
        }
        case SHORT_MANAGER: {
            QtIntPropertyManager *m = static_cast<QtIntPropertyManager *>(typeManager);
            m->blockSignals(true);
            m->setValue(p, *static_cast<short *>(valuePtr));
            m->blockSignals(false);
            break;
        }
        case FLOAT_MANAGER: {
            QtDoublePropertyManager *m = static_cast<QtDoublePropertyManager *>(typeManager);
            m->blockSignals(true);
            m->setValue(p, *static_cast<float *>(valuePtr));
            m->blockSignals(false);
            break;
        }
        case USHORT_MANAGER: {
            QtIntPropertyManager *m = static_cast<QtIntPropertyManager *>(typeManager);
            m->blockSignals(true);
            m->setValue(p, *static_cast<ushort *>(valuePtr));
            m->blockSignals(false);
            break;
        }
        case BOOL_MANAGER: {
            QtBoolPropertyManager *m = static_cast<QtBoolPropertyManager *>(typeManager);
            m->blockSignals(true);
            m->setValue(p, *static_cast<bool *>(valuePtr));
            m->blockSignals(false);
            break;
        }
        case STRING_MANAGER: {
            QtStringPropertyManager *m = static_cast<QtStringPropertyManager *>(typeManager);
            m->blockSignals(true);
            m->setValue(p, *static_cast<QString *>(valuePtr));
            m->blockSignals(false);
            break;
        }
        case ENUM_MANAGER: {
            QtEnumPropertyManager *m = static_cast<QtEnumPropertyManager *>(typeManager);
            m->blockSignals(true);
            m->setValue(p, *static_cast<int *>(valuePtr));
            m->blockSignals(false);
            break;
        }
        case COLOR_MANAGER: {
            QtColorPropertyManager *m = static_cast<QtColorPropertyManager *>(typeManager);
            m->blockSignals(true);
            m->setValue(p, *static_cast<QColor *>(valuePtr));
            m->blockSignals(false);
        } break;
    }
}

void Property::setRange(long min, long max)
{
    if (type == STRING_MANAGER || type == ENUM_MANAGER || type == BOOL_MANAGER)
        return;
    QtIntPropertyManager *m = static_cast<QtIntPropertyManager *>(typeManager);
    m->setRange(p, min, max);
    // TODO: ADD CHECKS TO ENSURE THE VALUEPTR ACTUALLY SUPPORTS THIS RANGE
}

void Property::addSubProperty(Property *property)
{
    subProperties << property;
    p->addSubProperty(property->p);
}

void Property::removeSubProperty(Property *property)
{
    p->removeSubProperty(property->p);
    subProperties.removeOne(property);
    delete property;
}

void Property::setSubProperties(QList<Property *> properties)
{
    subProperties = properties;
    QListIterator<Property *> i(properties);
    while (i.hasNext()) p->addSubProperty(i.next()->p);
}

PropertyBrowser::PropertyBrowser(QWidget *parent) : QtTreePropertyBrowser(parent)
{
    setResizeMode(QtTreePropertyBrowser::Interactive);
}

void PropertyBrowser::setPropertySet(QList<Property *> properties)
{
    QList<QList<Property *>> groups{ properties };
    for (int i = 0; i < groups.count(); i++) {
        for (int j = 0; j < groups[i].count(); j++) {
            assignDelegate(groups[i][j]);
            if (groups[i][j]->type == Property::GROUP_MANAGER)
                groups.append(groups[i][j]->subProperties);
        }
    }
    for (int i = 0; i < properties.count(); i++) addProperty(properties[i]->p);
    propertySet = properties;
}

void PropertyBrowser::updateDelegates()
{
    QList<QList<Property *>> groups{ propertySet };
    for (int i = 0; i < groups.count(); i++) {
        for (int j = 0; j < groups[i].count(); j++) {
            assignDelegate(groups[i][j]);
            if (groups[i][j]->type == Property::GROUP_MANAGER)
                groups.append(groups[i][j]->subProperties);
        }
    }
}

void PropertyBrowser::assignDelegate(Property *property)
{
    switch (property->type) {
        case Property::BYTE_MANAGER:
        case Property::SBYTE_MANAGER:
        case Property::SHORT_MANAGER:
        case Property::USHORT_MANAGER:
        case Property::INT_MANAGER:
        case Property::UINT_MANAGER:
            this->setFactoryForManager(static_cast<QtIntPropertyManager *>(property->typeManager),
                                       new QtSpinBoxFactory);
            break;
        case Property::FLOAT_MANAGER:
            this->setFactoryForManager(static_cast<QtDoublePropertyManager *>(property->typeManager),
                                       new QtDoubleSpinBoxFactory);
            break;
        case Property::BOOL_MANAGER:
            this->setFactoryForManager(static_cast<QtBoolPropertyManager *>(property->typeManager),
                                       new QtCheckBoxFactory);
            break;
        case Property::STRING_MANAGER:
            this->setFactoryForManager(static_cast<QtStringPropertyManager *>(property->typeManager),
                                       new QtLineEditFactory);
            break;
        case Property::ENUM_MANAGER:
            this->setFactoryForManager(static_cast<QtEnumPropertyManager *>(property->typeManager),
                                       new QtEnumEditorFactory);
            break;
        case Property::COLOR_MANAGER: break;
        case Property::GROUP_MANAGER: break;
    }
}

#include "moc_propertygrid.cpp"
