#pragma once

#include "ICanNodeBuilder.h"
#include "CanNetwork.h"

class CanNodeBuilder : public ICanNodeBuilder
{
public:
   CanNodeBuilder(CanNetwork* network) : network{ network } {}
   ~CanNodeBuilder();

   void Clear(void);

   ICanNodeBuilder* WithName(const char* name) override;
   ICanNodeBuilder* WithAddress(int32_t address) override;
   ICanNodeBuilder* WithComment(const char* comment) override;
   ICanNode* Build(void) override;

private:
   CanNetwork* network{ nullptr };
   std::string name;
   int32_t address { 0 };
   std::string comment;
};