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


#if ENABLE(SVG) && ENABLE(SVG_FILTERS)

#include "SVGElement.h"
#include "JSSVGFESpotLightElement.h"

#include <wtf/GetPtr.h>

#include "JSSVGAnimatedNumber.h"
#include "SVGFESpotLightElement.h"


using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSSVGFESpotLightElement)

/* Hash table */

static const HashTableValue JSSVGFESpotLightElementTableValues[9] =
{
    { "x", DontDelete|ReadOnly, (intptr_t)jsSVGFESpotLightElementX, (intptr_t)0 },
    { "y", DontDelete|ReadOnly, (intptr_t)jsSVGFESpotLightElementY, (intptr_t)0 },
    { "z", DontDelete|ReadOnly, (intptr_t)jsSVGFESpotLightElementZ, (intptr_t)0 },
    { "pointsAtX", DontDelete|ReadOnly, (intptr_t)jsSVGFESpotLightElementPointsAtX, (intptr_t)0 },
    { "pointsAtY", DontDelete|ReadOnly, (intptr_t)jsSVGFESpotLightElementPointsAtY, (intptr_t)0 },
    { "pointsAtZ", DontDelete|ReadOnly, (intptr_t)jsSVGFESpotLightElementPointsAtZ, (intptr_t)0 },
    { "specularExponent", DontDelete|ReadOnly, (intptr_t)jsSVGFESpotLightElementSpecularExponent, (intptr_t)0 },
    { "limitingConeAngle", DontDelete|ReadOnly, (intptr_t)jsSVGFESpotLightElementLimitingConeAngle, (intptr_t)0 },
    { 0, 0, 0, 0 }
};

static const HashTable JSSVGFESpotLightElementTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 15, JSSVGFESpotLightElementTableValues, 0 };
#else
    { 16, 15, JSSVGFESpotLightElementTableValues, 0 };
#endif

/* Hash table for prototype */

static const HashTableValue JSSVGFESpotLightElementPrototypeTableValues[1] =
{
    { 0, 0, 0, 0 }
};

static const HashTable JSSVGFESpotLightElementPrototypeTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSSVGFESpotLightElementPrototypeTableValues, 0 };
#else
    { 1, 0, JSSVGFESpotLightElementPrototypeTableValues, 0 };
#endif

const ClassInfo JSSVGFESpotLightElementPrototype::s_info = { "SVGFESpotLightElementPrototype", 0, &JSSVGFESpotLightElementPrototypeTable, 0 };

JSObject* JSSVGFESpotLightElementPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSSVGFESpotLightElement>(exec, globalObject);
}

const ClassInfo JSSVGFESpotLightElement::s_info = { "SVGFESpotLightElement", &JSSVGElement::s_info, &JSSVGFESpotLightElementTable, 0 };

JSSVGFESpotLightElement::JSSVGFESpotLightElement(PassRefPtr<Structure> structure, PassRefPtr<SVGFESpotLightElement> impl)
    : JSSVGElement(structure, impl)
{
}

JSObject* JSSVGFESpotLightElement::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return new (exec) JSSVGFESpotLightElementPrototype(JSSVGFESpotLightElementPrototype::createStructure(JSSVGElementPrototype::self(exec, globalObject)));
}

bool JSSVGFESpotLightElement::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGFESpotLightElement, Base>(exec, &JSSVGFESpotLightElementTable, this, propertyName, slot);
}

JSValuePtr jsSVGFESpotLightElementX(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGFESpotLightElement* imp = static_cast<SVGFESpotLightElement*>(static_cast<JSSVGFESpotLightElement*>(asObject(slot.slotBase()))->impl());
    RefPtr<SVGAnimatedNumber> obj = imp->xAnimated();
    return toJS(exec, obj.get(), imp);
}

JSValuePtr jsSVGFESpotLightElementY(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGFESpotLightElement* imp = static_cast<SVGFESpotLightElement*>(static_cast<JSSVGFESpotLightElement*>(asObject(slot.slotBase()))->impl());
    RefPtr<SVGAnimatedNumber> obj = imp->yAnimated();
    return toJS(exec, obj.get(), imp);
}

JSValuePtr jsSVGFESpotLightElementZ(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGFESpotLightElement* imp = static_cast<SVGFESpotLightElement*>(static_cast<JSSVGFESpotLightElement*>(asObject(slot.slotBase()))->impl());
    RefPtr<SVGAnimatedNumber> obj = imp->zAnimated();
    return toJS(exec, obj.get(), imp);
}

JSValuePtr jsSVGFESpotLightElementPointsAtX(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGFESpotLightElement* imp = static_cast<SVGFESpotLightElement*>(static_cast<JSSVGFESpotLightElement*>(asObject(slot.slotBase()))->impl());
    RefPtr<SVGAnimatedNumber> obj = imp->pointsAtXAnimated();
    return toJS(exec, obj.get(), imp);
}

JSValuePtr jsSVGFESpotLightElementPointsAtY(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGFESpotLightElement* imp = static_cast<SVGFESpotLightElement*>(static_cast<JSSVGFESpotLightElement*>(asObject(slot.slotBase()))->impl());
    RefPtr<SVGAnimatedNumber> obj = imp->pointsAtYAnimated();
    return toJS(exec, obj.get(), imp);
}

JSValuePtr jsSVGFESpotLightElementPointsAtZ(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGFESpotLightElement* imp = static_cast<SVGFESpotLightElement*>(static_cast<JSSVGFESpotLightElement*>(asObject(slot.slotBase()))->impl());
    RefPtr<SVGAnimatedNumber> obj = imp->pointsAtZAnimated();
    return toJS(exec, obj.get(), imp);
}

JSValuePtr jsSVGFESpotLightElementSpecularExponent(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGFESpotLightElement* imp = static_cast<SVGFESpotLightElement*>(static_cast<JSSVGFESpotLightElement*>(asObject(slot.slotBase()))->impl());
    RefPtr<SVGAnimatedNumber> obj = imp->specularExponentAnimated();
    return toJS(exec, obj.get(), imp);
}

JSValuePtr jsSVGFESpotLightElementLimitingConeAngle(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGFESpotLightElement* imp = static_cast<SVGFESpotLightElement*>(static_cast<JSSVGFESpotLightElement*>(asObject(slot.slotBase()))->impl());
    RefPtr<SVGAnimatedNumber> obj = imp->limitingConeAngleAnimated();
    return toJS(exec, obj.get(), imp);
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_FILTERS)
