#pragma once

#include "ICanAttribute.h"
#include <stdint.h>

class ICanEnumAttribute : public ICanAttribute
{
protected:
   ICanEnumAttribute() {};
   virtual ~ICanEnumAttribute() {};
public:
   virtual void Clear(void) = 0;
   virtual size_t GetEnumaratorsCount(void) const = 0;
   virtual const char * GetEnumarator(size_t index) const = 0;
   virtual const char * GetDefaultValue(void) const = 0;
};