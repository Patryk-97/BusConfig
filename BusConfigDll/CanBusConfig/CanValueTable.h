#pragma once

#include "ICanValueTable.h"
#include "CanValueDescription.h"
#include <vector>
#include <string>

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

   size_t GetValueDescriptionsCount(void) const override;
   ICanValueDescription* GetValueDescription(size_t index) const override;
   void AddValueDescription(CanValueDescription* valueDescription);
   CanValueDescription* CreateAndAddValueDescription(void);

private:
   std::string name;
   uint32_t minValue {};
   uint32_t maxValue {};
   std::vector<CanValueDescription*> valueDescriptions;
};