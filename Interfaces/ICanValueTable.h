#pragma once

#include "ICanValueDescription.h"

class ICanValueTable
{
protected:
   ICanValueTable() {};
   virtual ~ICanValueTable() {};
public:
   virtual void Clear(void) = 0;
   virtual const char * GetName(void) const = 0;
   virtual uint32_t GetMinValue(void) const = 0;
   virtual uint32_t GetMaxValue(void) const = 0;
   virtual size_t GetValueDescriptionsCount(void) const = 0;
   virtual ICanValueDescription* GetValueDescription(size_t index) const = 0;
};