#pragma once

#include "ICanEnvVar.h"
#include "CanValueTable.h"
#include "CanAttributeOwner.h"
#include <string>

class CanEnvVar : public CanAttributeOwner, public ICanEnvVar
{
public:
   CanEnvVar() : CanAttributeOwner(ICanAttribute::IObjectType_e::ENVIRONMENT_VARIABLE) {}
   ~CanEnvVar();

   void Clear(void) override;

   const char* GetName(void) const override;
   void SetName(const char * name);

   ICanValueTable* GetValueTable(void) const override;
   void SetValueTable(CanValueTable* valueTable);

   size_t GetAttributesCount(void) const override;
   ICanAttribute* GetAttributeByIndex(size_t index) const override;
   ICanAttribute* GetAttributeByName(const char* name) const override;

   ICanAttributeValue* GetAttributeValue(const char* attributeName) const override;

private:
   std::string name;
   CanValueTable* valueTable { nullptr };
};