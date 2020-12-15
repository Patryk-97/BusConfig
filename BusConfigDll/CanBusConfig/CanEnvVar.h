#pragma once

#include "ICanEnvVar.h"
#include "CanValueTable.h"
#include "CanAttribute.h"
#include <string>

class CanEnvVar : public ICanEnvVar
{
public:
   CanEnvVar() = default;
   ~CanEnvVar();

   void Clear(void) override;

   const char* GetName(void) const override;
   void SetName(const char * name);

   ICanValueTable* GetValueTable(void) const override;
   void SetValueTable(CanValueTable* valueTable);

   size_t GetAttributesCount(void) const override;
   ICanAttribute* GetAttributeByIndex(size_t index) const override;
   ICanAttribute* GetAttributeByName(const char* name) const override;
   void AddAttribute(CanAttribute* attribute);

private:
   std::string name;
   CanValueTable* valueTable { nullptr };
   std::vector<CanAttribute*> attributes;
};