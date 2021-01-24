#pragma once

#include "ICanValueTable.h"

class ICanValueTableBuilder
{
protected:
   ICanValueTableBuilder() {};
   virtual ~ICanValueTableBuilder() {};
public:
   virtual ICanValueTableBuilder* WithValue(uint32_t value, const char* description) = 0;
   virtual ICanValueTable* Build(void) = 0;
};