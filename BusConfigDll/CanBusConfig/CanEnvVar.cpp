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
   this->type = Type_e::UNDEFINED;
   this->unit = "";
   this->id = 0;
   this->accessType = AccessType_e::UNDEFINED;
   this->accessNode = nullptr;
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

ICanEnvVar::Type_e CanEnvVar::GetType(void) const
{
   return this->type;
}

void CanEnvVar::SetType(Type_e type)
{
   this->type = type;
}

const char* CanEnvVar::GetUnit(void) const
{
   return this->unit.c_str();
}

void CanEnvVar::SetUnit(const char* unit)
{
   this->unit = unit;
}

uint32_t CanEnvVar::GetId(void) const
{
   return this->id;
}

void CanEnvVar::SetId(uint32_t id)
{
   this->id = id;
}

ICanEnvVar::AccessType_e CanEnvVar::GetAccessType(void) const
{
   return this->accessType;
}

void CanEnvVar::SetAccessType(AccessType_e accessType)
{
   this->accessType = accessType;
}

ICanNode* CanEnvVar::GetAccessNode(void) const
{
   return this->accessNode;
}

void CanEnvVar::SetAccessNode(CanNode* accessNode)
{
   this->accessNode = accessNode;
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