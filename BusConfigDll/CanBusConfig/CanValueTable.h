#pragma once

#include "ICanValueTable.h"
#include <vector>
#include <string>
#include <map>

class CanValueTable : public ICanValueTable
{
public:
   CanValueTable() = default;
   ~CanValueTable();

   void Clear(void) override;

   const char* GetName(void) const override;
   void SetName(const char * name);

   uint32_t GetMinValue(void) const override;
   void SetMinValue(uint32_t minValue);

   uint32_t GetMaxValue(void) const override;
   void SetMaxValue(uint32_t maxValue);

   size_t GetValuesCount(void) const override;
   uint32_t GetValue(size_t index) const override;
   const char * GetValueDescription(uint32_t value) const override;
   void AddValue(uint32_t value, std::string_view valueDescription);

private:
   std::string name;
   uint32_t minValue {};
   uint32_t maxValue {};
   std::map<uint32_t, std::string_view> values;
};