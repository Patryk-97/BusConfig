#pragma once

#include "ICanAttributeOwner.h"
#include "ICanAttributeType.h"
#include "ICanAttributeValueType.h"
#include "ICanAttributeCppValueType.h"
#include "helpers.h"
#include <string>
#include <sstream>

class ICanAttributeManager
{
public:
   ICanAttributeManager() = delete;

   template <typename T>
   static void ForEachAttribute(const ICanAttributeOwner* attributeOwner, const T& fun)
   {
      if (attributeOwner)
      {
         for (size_t i = 0; i < attributeOwner->GetAttributesCount(); i++)
         {
            if (const auto attribute = attributeOwner->GetAttributeByIndex(i); attribute)
            {
               fun(attribute);
            }
         }
      }
   }

   template <typename T>
   static void ForAttributeValue(const ICanAttributeValue* attributeValue, const T& fun)
   {
      if (attributeValue)
      {
         helpers::typecase(attributeValue,
            [&fun] (ICanIntAttributeValue* intAttributeValue)
            {
               fun(intAttributeValue->GetValue());
            },
            [&fun](ICanHexAttributeValue* hexAttributeValue)
            {
               fun(hexAttributeValue->GetValue());
            },
            [&fun](ICanFloatAttributeValue* floatAttributeValue)
            {
               fun(floatAttributeValue->GetValue());
            },
            [&fun](ICanStringAttributeValue* stringAttributeValue)
            {
               fun(stringAttributeValue->GetValue());
            },
            [&fun](ICanEnumAttributeValue* enumAttributeValue)
            {
               fun(enumAttributeValue->GetValue());
            });
      }
   }

   template <typename T>
   static void ForAttributeStrValue(const ICanAttributeValue* attributeValue, const T& fun)
   {
      if (attributeValue)
      {
         helpers::typecase(attributeValue,
            [&fun](const ICanIntAttributeValue* intAttributeValue)
            {
               fun(std::to_string(intAttributeValue->GetValue()));
            },
            [&fun](const ICanHexAttributeValue* hexAttributeValue)
            {
               std::stringstream strStream;
               strStream << "0x" << std::hex << hexAttributeValue->GetValue();
               fun(strStream.str());
            },
            [&fun](const ICanFloatAttributeValue* floatAttributeValue)
            {
               fun(std::to_string(floatAttributeValue->GetValue()));
            },
            [&fun](const ICanStringAttributeValue* stringAttributeValue)
            {
               fun(stringAttributeValue->GetValue());
            },
            [&fun](const ICanEnumAttributeValue* enumAttributeValue)
            {
               fun(enumAttributeValue->GetValue());
            });
      }
   }

   template <ICanAttribute::IValueType_e ValueType>
   static typename ICanAttributeType_t<ValueType>
      GetAttribute(const ICanAttribute* attribute)
   {
      if constexpr (ValueType == ICanAttribute::IValueType_e::UNDEFINED)
      {
         return nullptr;
      }
      return dynamic_cast<ICanAttributeType_t<ValueType>>(attribute);
   }

   template <ICanAttribute::IValueType_e ValueType,
      typename Derived = typename ICanAttributeValueType_t<ValueType>,
      typename Base = ICanAttributeValue*>
   static typename ICanAttributeCppValueType_t<ValueType>
      GetAttributeValue(const Base attributeValue)
   {
      auto specificAttributeValue = dynamic_cast<const Derived>(attributeValue);
      return specificAttributeValue->GetValue();
   }
};