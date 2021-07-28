/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Solutions component.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QMetaObject>
#include <QMetaProperty>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QtDebug>
#include "objectcontroller.h"
#include "qtvariantproperty.h"
#include "qtgroupboxpropertybrowser.h"
#include "qttreepropertybrowser.h"
#include "qtpropertybrowser.h"

class ObjectControllerPrivate
{
    ObjectController *q_ptr;
    Q_DECLARE_PUBLIC(ObjectController)
public:
		
		typedef enum PropertyState
		{
			STATE_NONE = 0,
			STATE_SELECTED = 0x01,
			STATE_EXPANDED = 0x02
			
		} PropertyState;

    void addClassProperties(const QMetaObject *metaObject);
    void updateClassProperties(const QMetaObject *metaObject, bool recursive);
		void saveState();
		void saveState(QtProperty *prop, QString prefix = QString());
    void restoreState();
		void restoreState(QtProperty *prop, QString prefix = QString());
    void slotValueChanged(QtProperty *property, const QVariant &value);
    int enumToInt(const QMetaEnum &metaEnum, int enumValue) const;
    int intToEnum(const QMetaEnum &metaEnum, int intValue) const;
    int flagToInt(const QMetaEnum &metaEnum, int flagValue) const;
    int intToFlag(const QMetaEnum &metaEnum, int intValue) const;
    bool isSubValue(int value, int subValue) const;
    bool isPowerOf2(int value) const;

    QObject                  *m_object;

    QMap<const QMetaObject *, QtProperty *> m_classToProperty;
    QMap<QtProperty *, const QMetaObject *> m_propertyToClass;
    QMap<QtProperty *, int>     m_propertyToIndex;
    QMap<const QMetaObject *, QMap<int, QtVariantProperty *> > m_classToIndexToProperty;

		// ideally this should use a QMetaObject,int pair to index a property rather than by name using QString 
    QMap<QString, int /* PropertyState */>    m_propertyToState;

    QList<QtProperty *>         m_topLevelProperties;

    QtAbstractPropertyBrowser    *m_browser;
    QtVariantPropertyManager *m_manager;
    QtVariantPropertyManager *m_readOnlyManager;
};

int ObjectControllerPrivate::enumToInt(const QMetaEnum &metaEnum, int enumValue) const
{
    QMap<int, int> valueMap; // dont show multiple enum values which have the same values
    int pos = 0;
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        int value = metaEnum.value(i);
        if (!valueMap.contains(value)) {
            if (value == enumValue)
                return pos;
            valueMap[value] = pos++;
        }
    }
    return -1;
}

int ObjectControllerPrivate::intToEnum(const QMetaEnum &metaEnum, int intValue) const
{
    QMap<int, bool> valueMap; // dont show multiple enum values which have the same values
    QList<int> values;
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        int value = metaEnum.value(i);
        if (!valueMap.contains(value)) {
            valueMap[value] = true;
            values.append(value);
        }
    }
    if (intValue >= values.count())
        return -1;
    return values.at(intValue);
}

bool ObjectControllerPrivate::isSubValue(int value, int subValue) const
{
    if (value == subValue)
        return true;
    int i = 0;
    while (subValue) {
        if (!(value & (1 << i))) {
            if (subValue & 1)
                return false;
        }
        i++;
        subValue = subValue >> 1;
    }
    return true;
}

bool ObjectControllerPrivate::isPowerOf2(int value) const
{
    while (value) {
        if (value & 1) {
            return value == 1;
        }
        value = value >> 1;
    }
    return false;
}

int ObjectControllerPrivate::flagToInt(const QMetaEnum &metaEnum, int flagValue) const
{
    if (!flagValue)
        return 0;
    int intValue = 0;
    QMap<int, int> valueMap; // dont show multiple enum values which have the same values
    int pos = 0;
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        int value = metaEnum.value(i);
        if (!valueMap.contains(value) && isPowerOf2(value)) {
            if (isSubValue(flagValue, value))
                intValue |= (1 << pos);
            valueMap[value] = pos++;
        }
    }
    return intValue;
}

int ObjectControllerPrivate::intToFlag(const QMetaEnum &metaEnum, int intValue) const
{
    QMap<int, bool> valueMap; // dont show multiple enum values which have the same values
    QList<int> values;
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        int value = metaEnum.value(i);
        if (!valueMap.contains(value) && isPowerOf2(value)) {
            valueMap[value] = true;
            values.append(value);
        }
    }
    int flagValue = 0;
    int temp = intValue;
    int i = 0;
    while (temp) {
        if (i >= values.count())
            return -1;
        if (temp & 1)
            flagValue |= values.at(i);
        i++;
        temp = temp >> 1;
    }
    return flagValue;
}

void ObjectControllerPrivate::updateClassProperties(const QMetaObject *metaObject, bool recursive)
{
    if (!metaObject)
        return;

    if (recursive)
        updateClassProperties(metaObject->superClass(), recursive);

    QtProperty *classProperty = m_classToProperty.value(metaObject);
    if (!classProperty)
        return;

    for (int idx = metaObject->propertyOffset(); idx < metaObject->propertyCount(); idx++) {
        QMetaProperty metaProperty = metaObject->property(idx);
				
        if (metaProperty.isReadable()) {
            if (m_classToIndexToProperty.contains(metaObject) && m_classToIndexToProperty[metaObject].contains(idx)) {
                QtVariantProperty *subProperty = m_classToIndexToProperty[metaObject][idx];
                if (metaProperty.isEnumType()) {
                    if (metaProperty.isFlagType())
                        subProperty->setValue(flagToInt(metaProperty.enumerator(), metaProperty.read(m_object).toInt()));
                    else
                        subProperty->setValue(enumToInt(metaProperty.enumerator(), metaProperty.read(m_object).toInt()));
                } else {
                    subProperty->setValue(metaProperty.read(m_object));
                }
            }
        }
    }
}

void ObjectControllerPrivate::addClassProperties(const QMetaObject *metaObject)
{
    if (!metaObject)
        return;

    addClassProperties(metaObject->superClass());

    QtProperty *classProperty = m_classToProperty.value(metaObject);
	bool skipClass = false;
	QtBrowserItem *item;

	if (!classProperty)
	{
        QString className = QLatin1String(metaObject->className());

		// XXX bit of a lazy hack
		if (q_ptr->_filterList.contains(className) && q_ptr->_filterList.value(className))
			skipClass = true;

		// XXX bit of a lazy hack
		if (q_ptr->_displayList.contains(className))
			className = q_ptr->_displayList.value(className);

        classProperty = m_manager->addProperty(QtVariantPropertyManager::groupTypeId(), className);
        m_classToProperty[metaObject] = classProperty;
        m_propertyToClass[classProperty] = metaObject;

        for (int idx = metaObject->propertyOffset(); idx < metaObject->propertyCount(); idx++) {
            QMetaProperty metaProperty = metaObject->property(idx);
			QLatin1String propertyName = QLatin1String(metaProperty.name());
			QString displayName = propertyName;
            int type = metaProperty.userType();
						
			if (q_ptr->_filterList.contains(propertyName) && q_ptr->_filterList.value(propertyName))
				continue;

			if (q_ptr->_displayList.contains(propertyName))
				displayName = q_ptr->_displayList.value(propertyName);

            QtVariantProperty *subProperty = 0;
            if (!metaProperty.isReadable()) {
				subProperty = m_readOnlyManager->addProperty(QVariant::String, displayName);
                subProperty->setValue(QLatin1String("< Non Readable >"));
            } else if (metaProperty.isEnumType()) {
                if (metaProperty.isFlagType()) {
					subProperty = m_manager->addProperty(QtVariantPropertyManager::flagTypeId(), displayName);
                    QMetaEnum metaEnum = metaProperty.enumerator();
                    QMap<int, bool> valueMap;
                    QStringList flagNames;
                    for (int i = 0; i < metaEnum.keyCount(); i++) {
                        int value = metaEnum.value(i);
                        if (!valueMap.contains(value) && isPowerOf2(value)) {
                            valueMap[value] = true;
                            flagNames.append(QLatin1String(metaEnum.key(i)));
                        }
                    subProperty->setAttribute(QLatin1String("flagNames"), flagNames);
                    subProperty->setValue(flagToInt(metaEnum, metaProperty.read(m_object).toInt()));
                    }
                } else {
					subProperty = m_manager->addProperty(QtVariantPropertyManager::enumTypeId(), displayName);
                    QMetaEnum metaEnum = metaProperty.enumerator();
                    QMap<int, bool> valueMap; // dont show multiple enum values which have the same values
                    QStringList enumNames;
                    for (int i = 0; i < metaEnum.keyCount(); i++) {
                        int value = metaEnum.value(i);
                        if (!valueMap.contains(value)) {
                            valueMap[value] = true;
                            enumNames.append(QLatin1String(metaEnum.key(i)));
                        }
                    }
                    subProperty->setAttribute(QLatin1String("enumNames"), enumNames);
                    subProperty->setValue(enumToInt(metaEnum, metaProperty.read(m_object).toInt()));
                }
            } else if (m_manager->isPropertyTypeSupported(type)) {
                if (!metaProperty.isWritable())
					subProperty = m_readOnlyManager->addProperty(type, displayName + QLatin1String(" (Non Writable)"));
                if (!metaProperty.isDesignable())
					subProperty = m_readOnlyManager->addProperty(type, displayName + QLatin1String(" (Non Designable)"));
                else
					subProperty = m_manager->addProperty(type, displayName);
                subProperty->setValue(metaProperty.read(m_object));
            } else {
				subProperty = m_readOnlyManager->addProperty(QVariant::String, displayName);
                subProperty->setValue(QLatin1String("< Unknown Type >"));
                subProperty->setEnabled(false);
            }
				
				if (subProperty != NULL)
				{
					QList<QtProperty *> subList = subProperty->subProperties();
					
					for (int l = 0; l < subList.count(); l++)
					{
						QtProperty *subProp = subList.at(l);
						QString subName = propertyName + "." + subProp->propertyName();
						
						if (q_ptr->_displayList.contains(subName))
						{
							subProp->setPropertyName(q_ptr->_displayList.value(subName));
						}
					}
				}

			if (skipClass)
			{
				m_topLevelProperties.append(subProperty);
				item = m_browser->addProperty(subProperty);

				QtTreePropertyBrowser *treeBrowser = qobject_cast<QtTreePropertyBrowser*>(m_browser);

				if (treeBrowser != NULL)
					treeBrowser->setExpanded(item, false);
			}
			else
            classProperty->addSubProperty(subProperty);

            m_propertyToIndex[subProperty] = idx;
            m_classToIndexToProperty[metaObject][idx] = subProperty;
        }
    } else {
        updateClassProperties(metaObject, false);
    }

    m_topLevelProperties.append(classProperty);
	item = m_browser->addProperty(classProperty);

	QtTreePropertyBrowser *treeBrowser = qobject_cast<QtTreePropertyBrowser*>(m_browser);

	if (treeBrowser != NULL)
		treeBrowser->setExpanded(item, true);

	if (skipClass)
		treeBrowser->setItemVisible(item, false);
}

void ObjectControllerPrivate::saveState()
{
	QtTreePropertyBrowser *treeBrowser = qobject_cast<QtTreePropertyBrowser*>(m_browser);
	
	if (treeBrowser != NULL)
	{
		for (int i = 0; i < m_topLevelProperties.count(); i++)
		{
			QtProperty *prop = m_topLevelProperties.at(i);
			
			saveState(prop);
		}
	}
}

void ObjectControllerPrivate::saveState(QtProperty *prop, QString prefix)
{
	QtTreePropertyBrowser *treeBrowser = qobject_cast<QtTreePropertyBrowser*>(m_browser);
	
	if (treeBrowser != NULL)
	{
		QList<QtBrowserItem *> items = treeBrowser->items(prop);
		
		if (items.count() > 0)
		{
			QtBrowserItem *item = items.first();
			QString name = prop->propertyName();
			
			if (!prefix.isEmpty())
				name = prefix + "." + name;
			
			int state = STATE_NONE;
			
			if (treeBrowser->isExpanded(item))
				state |= STATE_EXPANDED;
			
			if (treeBrowser->isSelected(item))
				state |= STATE_SELECTED;
			
			//qDebug() << "saveState name: " << prop->propertyName() << " count: " << items.count() << " subs: " << item->children().count() << " state: " << state;
			m_propertyToState[name] = state;
			
			QList<QtBrowserItem *> children = item->children();
			
			for (int i = 0; i < children.count(); i++)
			{
				saveState(children.at(i)->property(), name);
			}
		}
	}
}

void ObjectControllerPrivate::restoreState()
{
	QtTreePropertyBrowser *treeBrowser = qobject_cast<QtTreePropertyBrowser*>(m_browser);
	
	if (treeBrowser != NULL)
	{
		for (int i = 0; i < m_topLevelProperties.count(); i++)
		{
			QtProperty *prop = m_topLevelProperties.at(i);
			
			restoreState(prop);
		}
	}
}

void ObjectControllerPrivate::restoreState(QtProperty *prop, QString prefix)
{
	QtTreePropertyBrowser *treeBrowser = qobject_cast<QtTreePropertyBrowser*>(m_browser);
	
	if (treeBrowser != NULL)
	{
		QList<QtBrowserItem *> items = treeBrowser->items(prop);
		
		if (items.count() > 0)
		{
			QtBrowserItem *item = items.first();
			QString name = prop->propertyName();
			
			if (!prefix.isEmpty())
				name = prefix + "." + name;
			
			if (m_propertyToState.contains(name))
			{
				int state = m_propertyToState[name];
				
				treeBrowser->setExpanded(item, state & STATE_EXPANDED);
				treeBrowser->setSelected(item, state & STATE_SELECTED);
				
				QList<QtBrowserItem *> children = item->children();
				
				for (int i = 0; i < children.count(); i++)
				{
					restoreState(children.at(i)->property(), name);
				}
			}
		}
	}
}

void ObjectControllerPrivate::slotValueChanged(QtProperty *property, const QVariant &value)
{
    if (!m_propertyToIndex.contains(property))
        return;

    int idx = m_propertyToIndex.value(property);

    const QMetaObject *metaObject = m_object->metaObject();
    QMetaProperty metaProperty = metaObject->property(idx);
    if (metaProperty.isEnumType()) {
        if (metaProperty.isFlagType())
            metaProperty.write(m_object, intToFlag(metaProperty.enumerator(), value.toInt()));
        else
            metaProperty.write(m_object, intToEnum(metaProperty.enumerator(), value.toInt()));
    } else {
        metaProperty.write(m_object, value);
    }

    updateClassProperties(metaObject, true);
}

///////////////////

ObjectController::ObjectController(QWidget *parent)
    : QWidget(parent)
{
    d_ptr = new ObjectControllerPrivate;
    d_ptr->q_ptr = this;

    d_ptr->m_object = 0;
/*
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);

    d_ptr->m_browser = new QtGroupBoxPropertyBrowser(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(scroll);
    scroll->setWidget(d_ptr->m_browser);
*/
    QtTreePropertyBrowser *browser = new QtTreePropertyBrowser(this);
	//browser->setRootIsDecorated(false);
    d_ptr->m_browser = browser;
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(d_ptr->m_browser);

    d_ptr->m_readOnlyManager = new QtVariantPropertyManager(this);
    d_ptr->m_manager = new QtVariantPropertyManager(this);
    QtVariantEditorFactory *factory = new QtVariantEditorFactory(this);
    d_ptr->m_browser->setFactoryForManager(d_ptr->m_manager, factory);

    connect(d_ptr->m_manager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                this, SLOT(slotValueChanged(QtProperty *, const QVariant &)));
}

ObjectController::~ObjectController()
{
    delete d_ptr;
}

void ObjectController::setObject(QObject *object)
{
    if (d_ptr->m_object == object)
        return;

    if (d_ptr->m_object) {
        d_ptr->saveState();
        QListIterator<QtProperty *> it(d_ptr->m_topLevelProperties);
        while (it.hasNext()) {
            d_ptr->m_browser->removeProperty(it.next());
        }
        d_ptr->m_topLevelProperties.clear();
		d_ptr->m_classToProperty.clear();
    }

    d_ptr->m_object = object;

    if (!d_ptr->m_object)
        return;

    d_ptr->addClassProperties(d_ptr->m_object->metaObject());

    d_ptr->restoreState();
}

QObject *ObjectController::object() const
{
	return d_ptr->m_object;
}

void ObjectController::updateObject()
{
	QObject *ob = d_ptr->m_object;
	
	if (ob != NULL)
		d_ptr->updateClassProperties(ob->metaObject(), true);	
}

const QString ObjectController::getDisplayName(const QString &propertyName) const
{
	if (_displayList.contains(propertyName))
		return _displayList[propertyName];

    return QString();
}

void ObjectController::setDisplayName(const QString &propertyName, const QString &displayName)
{
	_displayList[propertyName] = displayName;
}

bool ObjectController::getFilter(const QString &propertyName) const
{
	if (_filterList.contains(propertyName))
		return _filterList.value(propertyName);

	return false;
}

void ObjectController::setFilter(const QString &propertyName, bool filter)
{
	_filterList[propertyName] = filter;
}


#include "moc_objectcontroller.cpp"
