#pragma once

#include <stdint.h>

class ICanValueDescription
{
protected:
   ICanValueDescription() {};
   virtual ~ICanValueDescription() {};
public:
   virtual void Clear(void) = 0;
   virtual uint32_t GetValue(void) const = 0;
   virtual const char * GetDescription(void) const = 0;
};