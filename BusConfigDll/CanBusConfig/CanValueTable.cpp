#include "CanValueTable.h"
#include "helpers.h"

CanValueTable::~CanValueTable()
{
   this->Clear();
}

void CanValueTable::Clear(void)
{
   this->name = "";
   this->minValue = 0;
   this->maxValue = 0;
   helpers::ClearContainer(this->valueDescriptions);
}

const char* CanValueTable::GetName(void) const
{
   return this->name.c_str();
}

void CanValueTable::SetName(const char* name)
{
   this->name = name;
}

uint32_t CanValueTable::GetMinValue(void) const
{
   return this->minValue;
}

void CanValueTable::SetMinValue(uint32_t minValue)
{
   this->minValue = minValue;
}

uint32_t CanValueTable::GetMaxValue(void) const
{
   return this->maxValue;
}

void CanValueTable::SetMaxValue(uint32_t maxValue)
{
   this->maxValue = maxValue;
}

size_t CanValueTable::GetValueDescriptionsCount(void) const
{
   return this->valueDescriptions.size();
}

ICanValueDescription* CanValueTable::GetValueDescription(size_t index) const
{
   return (index < this->valueDescriptions.size() ? this->valueDescriptions[index] : nullptr);
}

void CanValueTable::AddValueDescription(CanValueDescription* valueDescription)
{
   if (valueDescription)
   {
      this->valueDescriptions.push_back(valueDescription);
   }
}

CanValueDescription* CanValueTable::CreateAndAddValueDescription(void)
{
   CanValueDescription* valueDescription = new CanValueDescription();
   this->valueDescriptions.push_back(valueDescription);
   return valueDescription;
}