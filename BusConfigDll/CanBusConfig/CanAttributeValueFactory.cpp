#include "CanAttributeValueFactory.h"
#include "CanIntAttributeValue.h"
#include "CanHexAttributeValue.h"
#include "CanFloatAttributeValue.h"
#include "CanStringAttributeValue.h"
#include "CanEnumAttributeValue.h"

CanAttributeValue* CanAttributeValueFactory::CreateAttributeValue(ICanAttribute::IValueType_e valueType)
{
   switch (valueType)
   {
      case ICanAttribute::IValueType_e::INT:
      {
         return new CanIntAttributeValue {};
      }
      case ICanAttribute::IValueType_e::HEX:
      {
         return new CanHexAttributeValue {};
      }
      case ICanAttribute::IValueType_e::FLOAT:
      {
         return new CanFloatAttributeValue {};
      }
      case ICanAttribute::IValueType_e::STRING:
      {
         return new CanStringAttributeValue {};
      }
      case ICanAttribute::IValueType_e::ENUM:
      {
         return new CanEnumAttributeValue {};
      }
   }
   return nullptr;
}