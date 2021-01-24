#pragma once

#include "ICanValueTableBuilder.h"
#include "CanValueTable.h"
#include <unordered_map>

class CanValueTableBuilder : public ICanValueTableBuilder
{
public:
   CanValueTableBuilder() = default;
   ~CanValueTableBuilder();

   void Clear(void);

   ICanValueTableBuilder* WithValue(uint32_t value, const char* description) override;
   ICanValueTable* Build(void) override;

private:
   std::unordered_map<uint32_t, std::string> values;
};