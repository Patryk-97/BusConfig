#include "CanEnvVar.h"
#include "helpers.h"

namespace ranges = std::ranges;

CanEnvVar::~CanEnvVar()
{
   this->Clear();
}

void CanEnvVar::Clear(void)
{
   CanAttributeOwner::Clear();
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

size_t CanEnvVar::GetAttributesCount(void) const
{
   return CanAttributeOwner::GetAttributesCount();
}

ICanAttribute* CanEnvVar::GetAttributeByIndex(size_t index) const
{
   return CanAttributeOwner::GetAttributeByIndex(index);
}

ICanAttribute* CanEnvVar::GetAttributeByName(const char* name) const
{
   return CanAttributeOwner::GetAttributeByName(name);
}

ICanAttributeValue* CanEnvVar::GetAttributeValue(const char* attributeName) const
{
   return CanAttributeOwner::GetAttributeValue(attributeName);
}