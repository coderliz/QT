/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"

#include "JSHTMLLIElement.h"

#include <wtf/GetPtr.h>

#include "HTMLLIElement.h"
#include "KURL.h"

#include <runtime/JSNumberCell.h>
#include <runtime/JSString.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSHTMLLIElement)

/* Hash table */

static const HashTableValue JSHTMLLIElementTableValues[4] =
{
    { "type", DontDelete, (intptr_t)jsHTMLLIElementType, (intptr_t)setJSHTMLLIElementType },
    { "value", DontDelete, (intptr_t)jsHTMLLIElementValue, (intptr_t)setJSHTMLLIElementValue },
    { "constructor", DontEnum|ReadOnly, (intptr_t)jsHTMLLIElementConstructor, (intptr_t)0 },
    { 0, 0, 0, 0 }
};

static const HashTable JSHTMLLIElementTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 31, JSHTMLLIElementTableValues, 0 };
#else
    { 9, 7, JSHTMLLIElementTableValues, 0 };
#endif

/* Hash table for constructor */

static const HashTableValue JSHTMLLIElementConstructorTableValues[1] =
{
    { 0, 0, 0, 0 }
};

static const HashTable JSHTMLLIElementConstructorTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSHTMLLIElementConstructorTableValues, 0 };
#else
    { 1, 0, JSHTMLLIElementConstructorTableValues, 0 };
#endif

class JSHTMLLIElementConstructor : public DOMObject {
public:
    JSHTMLLIElementConstructor(ExecState* exec)
        : DOMObject(JSHTMLLIElementConstructor::createStructure(exec->lexicalGlobalObject()->objectPrototype()))
    {
        putDirect(exec->propertyNames().prototype, JSHTMLLIElementPrototype::self(exec, exec->lexicalGlobalObject()), None);
    }
    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
    virtual const ClassInfo* classInfo() const { return &s_info; }
    static const ClassInfo s_info;

    static PassRefPtr<Structure> createStructure(JSValuePtr proto) 
    { 
        return Structure::create(proto, TypeInfo(ObjectType, ImplementsHasInstance)); 
    }
};

const ClassInfo JSHTMLLIElementConstructor::s_info = { "HTMLLIElementConstructor", 0, &JSHTMLLIElementConstructorTable, 0 };

bool JSHTMLLIElementConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSHTMLLIElementConstructor, DOMObject>(exec, &JSHTMLLIElementConstructorTable, this, propertyName, slot);
}

/* Hash table for prototype */

static const HashTableValue JSHTMLLIElementPrototypeTableValues[1] =
{
    { 0, 0, 0, 0 }
};

static const HashTable JSHTMLLIElementPrototypeTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSHTMLLIElementPrototypeTableValues, 0 };
#else
    { 1, 0, JSHTMLLIElementPrototypeTableValues, 0 };
#endif

const ClassInfo JSHTMLLIElementPrototype::s_info = { "HTMLLIElementPrototype", 0, &JSHTMLLIElementPrototypeTable, 0 };

JSObject* JSHTMLLIElementPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSHTMLLIElement>(exec, globalObject);
}

const ClassInfo JSHTMLLIElement::s_info = { "HTMLLIElement", &JSHTMLElement::s_info, &JSHTMLLIElementTable, 0 };

JSHTMLLIElement::JSHTMLLIElement(PassRefPtr<Structure> structure, PassRefPtr<HTMLLIElement> impl)
    : JSHTMLElement(structure, impl)
{
}

JSObject* JSHTMLLIElement::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return new (exec) JSHTMLLIElementPrototype(JSHTMLLIElementPrototype::createStructure(JSHTMLElementPrototype::self(exec, globalObject)));
}

bool JSHTMLLIElement::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSHTMLLIElement, Base>(exec, &JSHTMLLIElementTable, this, propertyName, slot);
}

JSValuePtr jsHTMLLIElementType(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    HTMLLIElement* imp = static_cast<HTMLLIElement*>(static_cast<JSHTMLLIElement*>(asObject(slot.slotBase()))->impl());
    return jsString(exec, imp->type());
}

JSValuePtr jsHTMLLIElementValue(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    HTMLLIElement* imp = static_cast<HTMLLIElement*>(static_cast<JSHTMLLIElement*>(asObject(slot.slotBase()))->impl());
    return jsNumber(exec, imp->value());
}

JSValuePtr jsHTMLLIElementConstructor(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    return static_cast<JSHTMLLIElement*>(asObject(slot.slotBase()))->getConstructor(exec);
}
void JSHTMLLIElement::put(ExecState* exec, const Identifier& propertyName, JSValuePtr value, PutPropertySlot& slot)
{
    lookupPut<JSHTMLLIElement, Base>(exec, propertyName, value, &JSHTMLLIElementTable, this, slot);
}

void setJSHTMLLIElementType(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    HTMLLIElement* imp = static_cast<HTMLLIElement*>(static_cast<JSHTMLLIElement*>(thisObject)->impl());
    imp->setType(valueToStringWithNullCheck(exec, value));
}

void setJSHTMLLIElementValue(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    HTMLLIElement* imp = static_cast<HTMLLIElement*>(static_cast<JSHTMLLIElement*>(thisObject)->impl());
    imp->setValue(value->toInt32(exec));
}

JSValuePtr JSHTMLLIElement::getConstructor(ExecState* exec)
{
    return getDOMConstructor<JSHTMLLIElementConstructor>(exec);
}


}
