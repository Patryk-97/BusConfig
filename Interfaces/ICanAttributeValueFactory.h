#pragma once

#include "ICanAttribute.h"
#include "ICanAttributeValue.h"

class ICanAttributeValueFactory
{
public:
   ICanAttributeValueFactory() = delete;
   static ICanAttributeValue* CreateAttributeValue(ICanAttribute::IValueType_e valueType);
};