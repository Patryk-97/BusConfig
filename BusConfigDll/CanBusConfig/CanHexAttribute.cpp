#include "CanHexAttribute.h"

CanHexAttribute::~CanHexAttribute()
{
   this->Clear();
}

void CanHexAttribute::Clear(void)
{
   CanAttribute::Clear();
   this->minimum = 0;
   this->maximum = 0;
   this->defaultValue = 0;
}

int32_t CanHexAttribute::GetMinimum(void) const
{
   return this->minimum;
}

void CanHexAttribute::SetMinimum(int32_t minimum)
{
   this->minimum = minimum;
}

int32_t CanHexAttribute::GetMaximum(void) const
{
   return this->maximum;
}

void CanHexAttribute::SetMaximum(int32_t maximum)
{
   this->maximum = maximum;
}

int32_t CanHexAttribute::GetDefaultValue(void) const
{
   return this->defaultValue;
}

void CanHexAttribute::SetDefaultValue(int32_t defaultValue)
{
   this->defaultValue = defaultValue;
}