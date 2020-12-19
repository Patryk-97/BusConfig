#pragma once

#include "ICanAttributeOwner.h"
#include "CanAttribute.h"
#include <string>
#include <vector>
#include <map>

class CanAttributeOwner : public ICanAttributeOwner
{
public:
   CanAttributeOwner() = default;
   CanAttributeOwner(ICanAttribute::IObjectType_e objectType) : objectType(objectType) {}
   ~CanAttributeOwner();

   void Clear(void) override;

   size_t GetAttributesCount(void) const override;
   ICanAttribute* GetAttributeByIndex(size_t index) const override;
   ICanAttribute* GetAttributeByName(const char* name) const override;
   void AddAttribute(CanAttribute* attribute);

   ICanAttributeValue* GetAttributeValue(const char* attributeName) const override;
   void AddAttributeValue(const std::string& attributeName, ICanAttributeValue* attributeValue);

private:
   ICanAttribute::IObjectType_e objectType { ICanAttribute::IObjectType_e::UNDEFINED };
   std::vector<CanAttribute*> attributes;
   std::map<std::string, ICanAttributeValue*> attributesValues;
};