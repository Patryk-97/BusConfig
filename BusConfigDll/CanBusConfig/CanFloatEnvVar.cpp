#include "CanFloatEnvVar.h"

CanFloatEnvVar::~CanFloatEnvVar()
{
   this->Clear();
}

void CanFloatEnvVar::Clear(void)
{
   CanEnvVar::Clear();
   this->minimum = 0.0;
   this->maximum = 0.0;
   this->initialValue = 0.0;
}

const char* CanFloatEnvVar::GetName(void) const
{
   return CanEnvVar::GetName();
}

void CanFloatEnvVar::ModifyName(const char* name)
{
   CanEnvVar::ModifyName(name);
}

ICanEnvVar::Type_e CanFloatEnvVar::GetType(void) const
{
   return CanEnvVar::GetType();
}

void CanFloatEnvVar::ModifyType(Type_e type)
{
   CanEnvVar::ModifyType(type);
}

const char* CanFloatEnvVar::GetUnit(void) const
{
   return CanEnvVar::GetUnit();
}

void CanFloatEnvVar::ModifyUnit(const char* unit)
{
   CanEnvVar::ModifyUnit(unit);
}

uint32_t CanFloatEnvVar::GetId(void) const
{
   return CanEnvVar::GetId();
}

void CanFloatEnvVar::ModifyId(uint32_t id)
{
   CanEnvVar::ModifyId(id);
}

ICanEnvVar::AccessType_e CanFloatEnvVar::GetAccessType(void) const
{
   return CanEnvVar::GetAccessType();
}

void CanFloatEnvVar::ModifyAccessType(AccessType_e accessType)
{
   CanEnvVar::ModifyAccessType(accessType);
}

size_t CanFloatEnvVar::GetAccessNodesCount(void) const
{
   return CanEnvVar::GetAccessNodesCount();
}

ICanNode* CanFloatEnvVar::GetAccessNodeByIndex(size_t index) const
{
   return CanEnvVar::GetAccessNodeByIndex(index);
}

ICanNode* CanFloatEnvVar::GetAccessNodeByName(const char* name) const
{
   return CanEnvVar::GetAccessNodeByName(name);
}

ICanValueTable* CanFloatEnvVar::GetValueTable(void) const
{
   return CanEnvVar::GetValueTable();
}

double CanFloatEnvVar::GetMinimum(void) const
{
   return this->minimum;
}

void CanFloatEnvVar::SetMinimum(double minimum)
{
   this->minimum = minimum;
}

double CanFloatEnvVar::GetMaximum(void) const
{
   return this->maximum;
}

void CanFloatEnvVar::SetMaximum(double maximum)
{
   this->maximum = maximum;
}

double CanFloatEnvVar::GetInitialValue(void) const
{
   return this->initialValue;
}

void CanFloatEnvVar::SetInitialValue(double initialValue)
{
   this->initialValue = initialValue;
}

size_t CanFloatEnvVar::GetAttributesCount(void) const
{
   return CanAttributeOwner::GetAttributesCount();
}

ICanAttribute* CanFloatEnvVar::GetAttributeByIndex(size_t index) const
{
   return CanAttributeOwner::GetAttributeByIndex(index);
}

ICanAttribute* CanFloatEnvVar::GetAttributeByName(const char* name) const
{
   return CanAttributeOwner::GetAttributeByName(name);
}

size_t CanFloatEnvVar::GetAttributesValuesCount(void) const
{
   return CanAttributeOwner::GetAttributesValuesCount();
}

ICanAttributeValue* CanFloatEnvVar::GetAttributeValue(const char* attributeName) const
{
   return CanAttributeOwner::GetAttributeValue(attributeName);
}

const char* CanFloatEnvVar::GetComment(void) const
{
   return CanEnvVar::GetComment();
}

void CanFloatEnvVar::ModifyComment(const char* comment)
{
   CanEnvVar::ModifyComment(comment);
}

ICanNetwork* CanFloatEnvVar::GetNetwork(void) const
{
   return CanEnvVar::GetNetwork();
}