#pragma once

#include <stdint.h>

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
   virtual size_t GetValuesCount(void) const = 0;
   virtual uint32_t GetValue(size_t index) const = 0;
   virtual const char * GetValueDescription(uint32_t value) const = 0;

   static constexpr const char * SIGNAL_PREFIX = "VtSig_";
   static constexpr const char * ENVIRONMENT_VARIABLE_PREFIX = "VtEv_";
};