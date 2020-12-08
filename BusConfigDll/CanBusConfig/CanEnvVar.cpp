#include "CanEnvVar.h"
#include "helpers.h"

CanEnvVar::~CanEnvVar()
{
   this->Clear();
}

void CanEnvVar::Clear(void)
{
   this->name = "";
   helpers::ClearPtr(this->valueTable);
}

const char* CanEnvVar::GetName(void) const
{
   return this->name.c_str();
}

void CanEnvVar::SetName(const char* name)
{
   this->name = name;
}

ICanValueTable* CanEnvVar::GetValueTable(void) const
{
   return this->valueTable;
}

void CanEnvVar::SetValueTable(CanValueTable* valueTable)
{
   this->valueTable = valueTable;
}