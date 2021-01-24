#include "CanValueTableBuilder.h"
#include <algorithm>

namespace ranges = std::ranges;

CanValueTableBuilder::~CanValueTableBuilder()
{
   this->Clear();
}

void CanValueTableBuilder::Clear(void)
{
   this->values.clear();
}

ICanValueTableBuilder* CanValueTableBuilder::WithValue(uint32_t value, const char* description)
{
   this->values[value] = description;
   return this;
}

ICanValueTable* CanValueTableBuilder::Build(void)
{
   auto valueTable = new CanValueTable {};

   for (const auto& [value, description] : this->values)
   {
      valueTable->AddValue(value, description);
   }

   return valueTable;
}