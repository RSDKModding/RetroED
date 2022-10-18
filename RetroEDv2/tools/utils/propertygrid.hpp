#pragma once

// Crap for PropertyGrid
#include "dependencies/QtPropertyBrowser/src/qttreepropertybrowser.h"
#include "dependencies/QtPropertyBrowser/src/qtpropertybrowser.h"
#include "dependencies/QtPropertyBrowser/src/qtpropertymanager.h"
#include "dependencies/QtPropertyBrowser/src/qteditorfactory.h"

struct PropertyValue {
    QString name;
    QVariant value;

    PropertyValue() {}
    PropertyValue(QString n, QVariant v)
    {
        name  = n;
        value = v;
    }
};

class Property : public QObject
{
    Q_OBJECT
public:
    enum managerType {
        BYTE_MANAGER,
        SBYTE_MANAGER,
        SHORT_MANAGER,
        USHORT_MANAGER,
        INT_MANAGER,
        UINT_MANAGER,
        BOOL_MANAGER,
        STRING_MANAGER,
        ENUM_MANAGER,
        GROUP_MANAGER,
        FLOAT_MANAGER,
        COLOR_MANAGER,
    };

    Property(QString name, unsigned char *value);
    Property(QString name, signed char *value);
    Property(QString name, int *value);
    Property(QString name, uint *value);
    Property(QString name, short *value);
    Property(QString name, ushort *value);
    Property(QString name, float *value);
    Property(QString name, bool *value);
    Property(QString name, QString *value);
    Property(QString name, QColor *value);
    // Property(QString name, Color *value);
    Property(QString name, QStringList names, void *value, byte type);
    Property(QString name, QList<PropertyValue> valueList, void *value, byte type);
    Property(QString name);
    QtAbstractPropertyManager *typeManager;
    QtProperty *p;
    unsigned char type = 0;
    byte varType       = 0;
    QList<Property *> subProperties;

    void *valuePtr;

    void addSubProperty(Property *property);
    void removeSubProperty(Property *property);
    void setSubProperties(QList<Property *> properties);

    void updateValue();

public slots:
    void setValuePtr(void *newPtr);
    void setRange(long min, long max);

signals:
    void changed();

private:
    QList<PropertyValue> *enumValues;
};

class PropertyBrowser : public QtTreePropertyBrowser
{
    Q_OBJECT
public:
    PropertyBrowser(QWidget *parent = nullptr);
    void setPropertySet(QList<Property *> properties);
    QList<Property *> propertySet;

public slots:
    void updateDelegates();

private:
    void assignDelegate(Property *property);
};


