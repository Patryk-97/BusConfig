#pragma once

#include "ICanAttribute.h"
#include "CanAttributeValue.h"

class CanAttributeValueFactory
{
public:
   CanAttributeValueFactory() = delete;
   static CanAttributeValue* CreateAttributeValue(ICanAttribute::IValueType_e valueType);
};