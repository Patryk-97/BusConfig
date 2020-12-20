#include "CanAttributeManager.h"
#include "CanIntAttribute.h"
#include "CanHexAttribute.h"
#include "CanFloatAttribute.h"
#include "CanStringAttribute.h"
#include "CanEnumAttribute.h"
#include "CanIntAttributeValue.h"
#include "CanHexAttributeValue.h"
#include "CanFloatAttributeValue.h"
#include "CanStringAttributeValue.h"
#include "CanEnumAttributeValue.h"
#include "helpers.h"

void CanAttributeManager::SetDefaultValue(ICanAttribute* attribute, const std::string& defaultValue)
{
   helpers::typecase(attribute,
      [&defaultValue](CanIntAttribute* intAttribute)
      {
         intAttribute->SetDefaultValue(std::stoi(defaultValue));
      },
      [&defaultValue](CanHexAttribute* hexAttribute)
      {
         hexAttribute->SetDefaultValue(std::stoi(defaultValue));
      },
      [&defaultValue](CanFloatAttribute* floatAttribute)
      {
         floatAttribute->SetDefaultValue(std::stod(defaultValue));
      },
      [&defaultValue](CanStringAttribute* stringAttribute)
      {
         stringAttribute->SetDefaultValue(defaultValue.c_str());
      },
      [&defaultValue](CanEnumAttribute* enumAttribute)
      {
         enumAttribute->SetDefaultValue(defaultValue.c_str());
      });
}

void CanAttributeManager::SetValue(ICanAttributeValue* attributeValue, const std::string& value)
{
   helpers::typecase(attributeValue,
      [&value](CanIntAttributeValue* intAttributeValue)
      {
         intAttributeValue->SetValue(std::stoi(value));
      },
      [&value](CanHexAttributeValue* hexAttributeValue)
      {
         hexAttributeValue->SetValue(std::stoi(value));
      },
      [&value](CanFloatAttributeValue* floatAttributeValue)
      {
         floatAttributeValue->SetValue(std::stod(value));
      },
      [&value](CanStringAttributeValue* stringAttributeValue)
      {
         stringAttributeValue->SetValue(value.c_str());
      },
      [&value](CanEnumAttributeValue* enumAttributeValue)
      {
         enumAttributeValue->SetValue(value.c_str());
      });
}