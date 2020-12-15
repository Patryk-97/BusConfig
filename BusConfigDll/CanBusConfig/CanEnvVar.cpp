#include "CanEnvVar.h"
#include "helpers.h"

namespace ranges = std::ranges;

CanEnvVar::~CanEnvVar()
{
   this->Clear();
}

void CanEnvVar::Clear(void)
{
   this->name = "";
   helpers::ClearPtr(this->valueTable);
   this->attributes.clear();
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
   return this->attributes.size();
}

ICanAttribute* CanEnvVar::GetAttributeByIndex(size_t index) const
{
   return (index < this->attributes.size() ? this->attributes[index] : nullptr);
}

ICanAttribute* CanEnvVar::GetAttributeByName(const char* name) const
{
   auto it = ranges::find_if(this->attributes, [&name](CanAttribute* attribute) { return !std::strcmp(attribute->GetName(), name); });
   return (it != this->attributes.end() ? *it : nullptr);
}

void CanEnvVar::AddAttribute(CanAttribute* attribute)
{
   if (attribute && attribute->GetObjectType() == ICanAttribute::IObjectType_e::ENVIRONMENT_VARIABLE)
   {
      this->attributes.push_back(attribute);
   }
}