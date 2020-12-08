#pragma once

#include "ICanValueTable.h"

class ICanEnvVar
{
protected:
   ICanEnvVar() {};
   virtual ~ICanEnvVar() {};
public:
   virtual void Clear(void) = 0;
   virtual const char * GetName(void) const = 0;

   virtual ICanValueTable* GetValueTable(void) const = 0;
};