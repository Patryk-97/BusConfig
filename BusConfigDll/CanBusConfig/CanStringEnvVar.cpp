#include "CanStringEnvVar.h"

CanStringEnvVar::~CanStringEnvVar()
{
   this->Clear();
}

void CanStringEnvVar::Clear(void)
{
   CanEnvVar::Clear();
}

const char* CanStringEnvVar::GetName(void) const
{
   return CanEnvVar::GetName();
}

ICanEnvVar::Type_e CanStringEnvVar::GetType(void) const
{
   return CanEnvVar::GetType();
}
const char* CanStringEnvVar::GetUnit(void) const
{
   return CanEnvVar::GetUnit();
}

uint32_t CanStringEnvVar::GetId(void) const
{
   return CanEnvVar::GetId();
}

ICanEnvVar::AccessType_e CanStringEnvVar::GetAccessType(void) const
{
   return CanEnvVar::GetAccessType();
}

size_t CanStringEnvVar::GetAccessNodesCount(void) const
{
   return CanEnvVar::GetAccessNodesCount();
}

ICanNode* CanStringEnvVar::GetAccessNodeByIndex(size_t index) const
{
   return CanEnvVar::GetAccessNodeByIndex(index);
}

ICanNode* CanStringEnvVar::GetAccessNodeByName(const char* name) const
{
   return CanEnvVar::GetAccessNodeByName(name);
}

ICanValueTable* CanStringEnvVar::GetValueTable(void) const
{
   return CanEnvVar::GetValueTable();
}

size_t CanStringEnvVar::GetAttributesCount(void) const
{
   return CanAttributeOwner::GetAttributesCount();
}

ICanAttribute* CanStringEnvVar::GetAttributeByIndex(size_t index) const
{
   return CanAttributeOwner::GetAttributeByIndex(index);
}

ICanAttribute* CanStringEnvVar::GetAttributeByName(const char* name) const
{
   return CanAttributeOwner::GetAttributeByName(name);
}

ICanAttributeValue* CanStringEnvVar::GetAttributeValue(const char* attributeName) const
{
   return CanAttributeOwner::GetAttributeValue(attributeName);
}