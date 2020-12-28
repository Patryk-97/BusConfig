#pragma once

#include "ICanEnvVar.h"
#include <stdint.h>

class ICanDataEnvVar : public ICanEnvVar
{
protected:
   ICanDataEnvVar() {};
   virtual ~ICanDataEnvVar() {};
public:
   virtual uint32_t GetLength(void) const = 0;
};