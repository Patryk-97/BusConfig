#include "CanIntEnvVar.h"

CanIntEnvVar::~CanIntEnvVar()
{
   this->Clear();
}

void CanIntEnvVar::Clear(void)
{
   CanEnvVar::Clear();
   this->minimum = 0;
   this->maximum = 0;
   this->initialValue = 0;
}

const char* CanIntEnvVar::GetName(void) const
{
   return CanEnvVar::GetName();
}

void CanIntEnvVar::ModifyName(const char* name)
{
   CanEnvVar::ModifyName(name);
}

ICanEnvVar::Type_e CanIntEnvVar::GetType(void) const
{
   return CanEnvVar::GetType();
}

void CanIntEnvVar::ModifyType(Type_e type)
{
   CanEnvVar::ModifyType(type);
}

const char* CanIntEnvVar::GetUnit(void) const
{
   return CanEnvVar::GetUnit();
}

void CanIntEnvVar::ModifyUnit(const char* unit)
{
   CanEnvVar::ModifyUnit(unit);
}

uint32_t CanIntEnvVar::GetId(void) const
{
   return CanEnvVar::GetId();
}

void CanIntEnvVar::ModifyId(uint32_t id)
{
   CanEnvVar::ModifyId(id);
}

ICanEnvVar::AccessType_e CanIntEnvVar::GetAccessType(void) const
{
   return CanEnvVar::GetAccessType();
}

void CanIntEnvVar::ModifyAccessType(AccessType_e accessType)
{
   CanEnvVar::ModifyAccessType(accessType);
}

size_t CanIntEnvVar::GetAccessNodesCount(void) const
{
   return CanEnvVar::GetAccessNodesCount();
}

ICanNode* CanIntEnvVar::GetAccessNodeByIndex(size_t index) const
{
   return CanEnvVar::GetAccessNodeByIndex(index);
}

ICanNode* CanIntEnvVar::GetAccessNodeByName(const char* name) const
{
   return CanEnvVar::GetAccessNodeByName(name);
}

ICanValueTable* CanIntEnvVar::GetValueTable(void) const
{
   return CanEnvVar::GetValueTable();
}

int32_t CanIntEnvVar::GetMinimum(void) const
{
   return this->minimum;
}

void CanIntEnvVar::SetMinimum(int32_t minimum)
{
   this->minimum = minimum;
}

int32_t CanIntEnvVar::GetMaximum(void) const
{
   return this->maximum;
}

void CanIntEnvVar::SetMaximum(int32_t maximum)
{
   this->maximum = maximum;
}

int32_t CanIntEnvVar::GetInitialValue(void) const
{
   return this->initialValue;
}

void CanIntEnvVar::SetInitialValue(int32_t initialValue)
{
   this->initialValue = initialValue;
}

size_t CanIntEnvVar::GetAttributesCount(void) const
{
   return CanEnvVar::GetAttributesCount();
}

ICanAttribute* CanIntEnvVar::GetAttributeByIndex(size_t index) const
{
   return CanEnvVar::GetAttributeByIndex(index);
}

ICanAttribute* CanIntEnvVar::GetAttributeByName(const char* name) const
{
   return CanEnvVar::GetAttributeByName(name);
}

size_t CanIntEnvVar::GetAttributesValuesCount(void) const
{
   return CanAttributeOwner::GetAttributesValuesCount();
}

ICanAttributeValue* CanIntEnvVar::GetAttributeValue(const char* attributeName) const
{
   return CanEnvVar::GetAttributeValue(attributeName);
}

const char* CanIntEnvVar::GetComment(void) const
{
   return CanEnvVar::GetComment();
}

void CanIntEnvVar::ModifyComment(const char* comment)
{
   CanEnvVar::ModifyComment(comment);
}

ICanNetwork* CanIntEnvVar::GetNetwork(void) const
{
   return CanEnvVar::GetNetwork();
}