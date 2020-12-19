#pragma once

#include "ICanAttribute.h"
#include <stdint.h>

class ICanHexAttribute : public ICanAttribute
{
protected:
   ICanHexAttribute() {};
   virtual ~ICanHexAttribute() {};
public:
   virtual void Clear(void) = 0;
   virtual int32_t GetMinimum(void) const = 0;
   virtual int32_t GetMaximum(void) const = 0;
   virtual int32_t GetDefaultValue(void) const = 0;
};