#pragma once

#include "ICanAttributeOwner.h"
#include "CanAttribute.h"
#include "CanAttributeValue.h"
#include <string>
#include <vector>
#include <map>

class CanAttributeOwner : public ICanAttributeOwner
{
public:
   CanAttributeOwner(ICanAttribute::IObjectType_e objectType) : objectType(objectType) {}
   ~CanAttributeOwner();

   void Clear(void) override;

   size_t GetAttributesCount(void) const override;
   ICanAttribute* GetAttributeByIndex(size_t index) const override;
   ICanAttribute* GetAttributeByName(const char* name) const override;
   std::vector<CanAttribute*> GetAttributes(void) const;
   void AddAttribute(CanAttribute* attribute);

   size_t GetAttributesValuesCount(void) const override;
   ICanAttributeValue* GetAttributeValue(const char* attributeName) const override;
   void AddAttributeValue(const std::string& attributeName, CanAttributeValue* attributeValue);

private:
   ICanAttribute::IObjectType_e objectType{ ICanAttribute::IObjectType_e::UNDEFINED };
   std::vector<CanAttribute*> attributes;
   size_t attributesValuesCount {};
   std::map<std::string, CanAttributeValue*> attributesValues;
};