#include "CanDataEnvVar.h"

CanDataEnvVar::~CanDataEnvVar()
{
   this->Clear();
}

void CanDataEnvVar::Clear(void)
{
   CanEnvVar::Clear();
   this->length = 0;
}

const char* CanDataEnvVar::GetName(void) const
{
   return CanEnvVar::GetName();
}

void CanDataEnvVar::ModifyName(const char* name)
{
   CanEnvVar::ModifyName(name);
}

ICanEnvVar::Type_e CanDataEnvVar::GetType(void) const
{
   return CanEnvVar::GetType();
}

void CanDataEnvVar::ModifyType(Type_e type)
{
   CanEnvVar::ModifyType(type);
}

const char* CanDataEnvVar::GetUnit(void) const
{
   return CanEnvVar::GetUnit();
}

void CanDataEnvVar::ModifyUnit(const char* unit)
{
   CanEnvVar::ModifyUnit(unit);
}

uint32_t CanDataEnvVar::GetId(void) const
{
   return CanEnvVar::GetId();
}

void CanDataEnvVar::ModifyId(uint32_t id)
{
   CanEnvVar::ModifyId(id);
}

ICanEnvVar::AccessType_e CanDataEnvVar::GetAccessType(void) const
{
   return CanEnvVar::GetAccessType();
}

void CanDataEnvVar::ModifyAccessType(AccessType_e accessType)
{
   CanEnvVar::ModifyAccessType(accessType);
}

size_t CanDataEnvVar::GetAccessNodesCount(void) const
{
   return CanEnvVar::GetAccessNodesCount();
}

ICanNode* CanDataEnvVar::GetAccessNodeByIndex(size_t index) const
{
   return CanEnvVar::GetAccessNodeByIndex(index);
}

ICanNode* CanDataEnvVar::GetAccessNodeByName(const char* name) const
{
   return CanEnvVar::GetAccessNodeByName(name);
}

ICanValueTable* CanDataEnvVar::GetValueTable(void) const
{
   return CanEnvVar::GetValueTable();
}

uint32_t CanDataEnvVar::GetLength(void) const
{
   return this->length;
}

void CanDataEnvVar::SetLength(uint32_t length)
{
   this->length = length;
}

size_t CanDataEnvVar::GetAttributesCount(void) const
{
   return CanAttributeOwner::GetAttributesCount();
}

ICanAttribute* CanDataEnvVar::GetAttributeByIndex(size_t index) const
{
   return CanAttributeOwner::GetAttributeByIndex(index);
}

ICanAttribute* CanDataEnvVar::GetAttributeByName(const char* name) const
{
   return CanAttributeOwner::GetAttributeByName(name);
}

size_t CanDataEnvVar::GetAttributesValuesCount(void) const
{
   return CanAttributeOwner::GetAttributesValuesCount();
}

ICanAttributeValue* CanDataEnvVar::GetAttributeValue(const char* attributeName) const
{
   return CanAttributeOwner::GetAttributeValue(attributeName);
}

const char* CanDataEnvVar::GetComment(void) const
{
   return CanEnvVar::GetComment();
}

void CanDataEnvVar::ModifyComment(const char* comment)
{
   CanEnvVar::ModifyComment(comment);
}