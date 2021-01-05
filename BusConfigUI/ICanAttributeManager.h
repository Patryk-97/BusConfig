#pragma once

#include "ICanAttributeOwner.h"
#include "ICanIntAttributeValue.h"
#include "ICanHexAttributeValue.h"
#include "ICanFloatAttributeValue.h"
#include "ICanStringAttributeValue.h"
#include "ICanEnumAttributeValue.h"
#include "ICanIntAttribute.h"
#include "ICanHexAttribute.h"
#include "ICanFloatAttribute.h"
#include "ICanStringAttribute.h"
#include "ICanEnumAttribute.h"
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
   static void ForAttributeDefaultValueStr(const ICanAttribute* attribute, const T& fun)
   {
      helpers::typecase(attribute,
         [&fun] (const ICanIntAttribute* intAttribute)
         {
            fun(std::to_string(intAttribute->GetDefaultValue()));
         },
         [&fun] (const ICanHexAttribute* hexAttribute)
         {
            std::stringstream strStream;
            strStream << "0x" << std::hex << hexAttribute->GetDefaultValue();
            fun(strStream.str());
         },
         [&fun] (const ICanFloatAttribute* floatAttribute)
         {
            fun(std::to_string(floatAttribute->GetDefaultValue()));
         },
         [&fun] (const ICanStringAttribute* stringAttribute)
         {
            fun(stringAttribute->GetDefaultValue());
         },
         [&fun] (const ICanEnumAttribute* enumAttribute)
         {
            fun(enumAttribute->GetDefaultValue());
         });
   }

   template <typename T>
   static void ForAttributeValue(const ICanAttributeValue* attributeValue, const T& fun)
   {
      if (attributeValue)
      {
         helpers::typecase(attributeValue,
            [&fun] (const ICanIntAttributeValue* intAttributeValue)
            {
               fun(intAttributeValue->GetValue());
            },
            [&fun](const ICanHexAttributeValue* hexAttributeValue)
            {
               fun(hexAttributeValue->GetValue());
            },
            [&fun](const ICanFloatAttributeValue* floatAttributeValue)
            {
               fun(floatAttributeValue->GetValue());
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

   template <typename T>
   static void ForAttributeStrValue(ICanAttributeValue* attributeValue, const T& fun)
   {
      if (attributeValue)
      {
         helpers::typecase(attributeValue,
            [&fun](ICanIntAttributeValue* intAttributeValue)
            {
               fun(std::to_string(intAttributeValue->GetValue()));
            },
            [&fun](ICanHexAttributeValue* hexAttributeValue)
            {
               std::stringstream strStream;
               strStream << "0x" << std::hex << hexAttributeValue->GetValue();
               fun(strStream.str());
            },
            [&fun](ICanFloatAttributeValue* floatAttributeValue)
            {
               fun(std::to_string(floatAttributeValue->GetValue()));
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
};