#pragma once

#include "ICanEnvVar.h"
#include <stdint.h>

class ICanIntEnvVar : public ICanEnvVar
{
protected:
   ICanIntEnvVar() {};
   virtual ~ICanIntEnvVar() {};
public:
   virtual int32_t GetMinimum(void) const = 0;
   virtual int32_t GetMaximum(void) const = 0;
   virtual int32_t GetInitialValue(void) const = 0;
};