#pragma once

#include "ICanEnvVar.h"
#include "CanValueTable.h"
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

private:
   std::string name;
   CanValueTable* valueTable { nullptr };
};