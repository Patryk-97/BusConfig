#include "CanAttributeOwner.h"
#include "CanIntAttribute.h"
#include "helpers.h"
#include <algorithm>

namespace ranges = std::ranges;

CanAttributeOwner::~CanAttributeOwner()
{
   this->Clear();
}

void CanAttributeOwner::Clear(void)
{
   if (this->objectType == ICanAttribute::IObjectType_e::NETWORK)
   {
      helpers::ClearContainer(this->attributes);
   }
   else
   {
      this->attributes.clear();
   }
   this->attributesValuesCount = 0;
   for (auto& [key, value] : this->attributesValues) { delete value; value = nullptr; };
   this->attributesValues.clear();
   helpers::ClearContainer(this->attributes);
   this->objectType = ICanAttribute::IObjectType_e::UNDEFINED;
}

size_t CanAttributeOwner::GetAttributesCount(void) const
{
   return this->attributes.size();
}

ICanAttribute* CanAttributeOwner::GetAttributeByIndex(size_t index) const
{
   return (index < this->attributes.size() ? this->attributes[index] : nullptr);
}

ICanAttribute* CanAttributeOwner::GetAttributeByName(const char* name) const
{
   auto it = ranges::find_if(this->attributes, [&name](CanAttribute* attribute) { return !std::strcmp(attribute->GetName(), name); });
   return (it != this->attributes.end() ? *it : nullptr);
}

std::vector<CanAttribute*> CanAttributeOwner::GetAttributes(void) const
{
   return this->attributes;
}

void CanAttributeOwner::AddAttribute(CanAttribute* attribute)
{
   if (attribute)
   {
      if (this->objectType != ICanAttribute::IObjectType_e::NETWORK)
      {
         if (this->objectType == attribute->GetObjectType())
         {
            this->attributes.push_back(attribute);
         }
      }
      else
      {
         this->attributes.push_back(attribute);
      }
   }
}

size_t CanAttributeOwner::GetAttributesValuesCount(void) const
{
   return this->attributesValuesCount;
}

ICanAttributeValue* CanAttributeOwner::GetAttributeValue(const char* attributeName) const
{
   auto it = this->attributesValues.find(std::string{ attributeName });
   return (it != this->attributesValues.end() ? it->second : nullptr);
}

void CanAttributeOwner::AddAttributeValue(const std::string& attributeName, CanAttributeValue* attributeValue)
{
   if (this->attributesValues.find(attributeName) == this->attributesValues.end())
   {
      ++this->attributesValuesCount;
   }

   this->attributesValues[attributeName] = attributeValue;
}