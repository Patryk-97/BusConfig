#pragma once

#include "ICanAttribute.h"
#include "ICanAttributeValue.h"
#include <string>

class CanAttributeManager
{
public:
   CanAttributeManager() = delete;
   static void SetDefaultValue(ICanAttribute* attribute, const std::string& defaultValue);
   static void SetValue(ICanAttributeValue* attributeValue, const std::string& value);
};