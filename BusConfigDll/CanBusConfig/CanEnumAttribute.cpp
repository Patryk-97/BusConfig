#include "CanEnumAttribute.h"

CanEnumAttribute::~CanEnumAttribute()
{
   this->Clear();
}

void CanEnumAttribute::Clear(void)
{
   CanAttribute::Clear();
   this->enumerators.clear();
   this->defaultValue = "";
}

size_t CanEnumAttribute::GetEnumaratorsCount(void) const
{
   return this->enumerators.size();
}

const char* CanEnumAttribute::GetEnumarator(size_t index) const
{
   return (index < this->enumerators.size() ? this->enumerators[index].c_str() : nullptr);
}

void CanEnumAttribute::AddEnumarator(const char* enumarator)
{
   if (enumarator)
   {
      this->enumerators.push_back(enumarator);
   }
}

const char* CanEnumAttribute::GetDefaultValue(void) const
{
   return this->defaultValue.c_str();
}

void CanEnumAttribute::SetDefaultValue(const char* defaultValue)
{
   this->defaultValue = defaultValue;
}