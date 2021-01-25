#pragma once

#include "ICanNode.h"

class ICanNodeBuilder
{
protected:
   ICanNodeBuilder() {};
   virtual ~ICanNodeBuilder() {};
public:
   virtual ICanNodeBuilder* WithName(const char* name) = 0;
   virtual ICanNodeBuilder* WithAddress(int32_t address) = 0;
   virtual ICanNodeBuilder* WithComment(const char* comment) = 0;
   virtual ICanNode* Build(void) = 0;
};