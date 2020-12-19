#pragma once

#include "ICanAttribute.h"
#include <stdint.h>

class ICanIntAttribute : public ICanAttribute
{
protected:
   ICanIntAttribute() {};
   virtual ~ICanIntAttribute() {};
public:
   virtual void Clear(void) = 0;
   virtual int32_t GetMinimum(void) const = 0;
   virtual int32_t GetMaximum(void) const = 0;
   virtual int32_t GetDefaultValue(void) const = 0;
};