#include "CanValueDescription.h"

CanValueDescription::~CanValueDescription()
{
   this->Clear();
}

void CanValueDescription::Clear(void)
{
   this->value = 0;
   this->description = "";
}

uint32_t CanValueDescription::GetValue(void) const
{
   return this->value;
}

void CanValueDescription::SetValue(uint32_t value)
{
   this->value = value;
}

const char* CanValueDescription::GetDescription(void) const
{
   return this->description.c_str();
}

void CanValueDescription::SetDescription(const char* description)
{
   this->description = description;
}