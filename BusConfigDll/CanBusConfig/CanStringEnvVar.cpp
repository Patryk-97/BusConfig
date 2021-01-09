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

void CanStringEnvVar::ModifyName(const char* name)
{
   CanEnvVar::ModifyName(name);
}

ICanEnvVar::Type_e CanStringEnvVar::GetType(void) const
{
   return CanEnvVar::GetType();
}

void CanStringEnvVar::ModifyType(Type_e type)
{
   CanEnvVar::ModifyType(type);
}

const char* CanStringEnvVar::GetUnit(void) const
{
   return CanEnvVar::GetUnit();
}

void CanStringEnvVar::ModifyUnit(const char* unit)
{
   CanEnvVar::ModifyUnit(unit);
}

uint32_t CanStringEnvVar::GetId(void) const
{
   return CanEnvVar::GetId();
}

void CanStringEnvVar::ModifyId(uint32_t id)
{
   CanEnvVar::ModifyId(id);
}

ICanEnvVar::AccessType_e CanStringEnvVar::GetAccessType(void) const
{
   return CanEnvVar::GetAccessType();
}

void CanStringEnvVar::ModifyAccessType(AccessType_e accessType)
{
   CanEnvVar::ModifyAccessType(accessType);
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

size_t CanStringEnvVar::GetAttributesValuesCount(void) const
{
   return CanAttributeOwner::GetAttributesValuesCount();
}

ICanAttributeValue* CanStringEnvVar::GetAttributeValue(const char* attributeName) const
{
   return CanAttributeOwner::GetAttributeValue(attributeName);
}

const char* CanStringEnvVar::GetComment(void) const
{
   return CanEnvVar::GetComment();
}

void CanStringEnvVar::ModifyComment(const char* comment)
{
   CanEnvVar::ModifyComment(comment);
}