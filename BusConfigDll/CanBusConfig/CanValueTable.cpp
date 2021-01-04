#include "CanValueTable.h"
#include "helpers.h"
#include <algorithm>

CanValueTable::~CanValueTable()
{
   this->Clear();
}

void CanValueTable::Clear(void)
{
   this->name = "";
   this->minValue = 0;
   this->maxValue = 0;
   this->values.clear();
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

size_t CanValueTable::GetValuesCount(void) const
{
   return this->values.size();
}

uint32_t CanValueTable::GetValue(size_t index) const
{
   return (index < this->values.size() ? (*std::next(this->values.begin(), index)).first : -1);
}

const char * CanValueTable::GetValueDescription(uint32_t value) const
{
   auto it = this->values.find(value);
   return (it != this->values.end() ? it->second.c_str() : nullptr);
}

void CanValueTable::AddValue(uint32_t value, std::string valueDescription)
{
   this->values[value] = std::move(valueDescription);
}