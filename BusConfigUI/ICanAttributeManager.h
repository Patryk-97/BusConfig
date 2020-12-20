#pragma once

#include "ICanAttributeOwner.h"
#include "ICanIntAttributeValue.h"
#include "ICanHexAttributeValue.h"
#include "ICanFloatAttributeValue.h"
#include "ICanStringAttributeValue.h"
#include "ICanEnumAttributeValue.h"
#include "helpers.h"
#include <string>
#include <sstream>

class ICanAttributeManager
{
public:
   ICanAttributeManager() = delete;

   template <typename T>
   static void ForEachAttribute(ICanAttributeOwner* attributeOwner, T fun)
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
   static void ForAttributeValue(ICanAttributeValue* attributeValue, T fun)
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
   static void ForAttributeStrValue(ICanAttributeValue* attributeValue, T fun)
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
               strStream << std::hex << hexAttributeValue->GetValue();
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