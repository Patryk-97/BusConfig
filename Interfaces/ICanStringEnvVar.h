#pragma once

#include "ICanEnvVar.h"
#include <stdint.h>

class ICanStringEnvVar : public ICanEnvVar
{
protected:
   ICanStringEnvVar() {};
   virtual ~ICanStringEnvVar() {};
};