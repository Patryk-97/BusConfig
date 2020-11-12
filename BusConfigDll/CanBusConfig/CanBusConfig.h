#pragma once

#include "ICanBusConfig.h"

class CanBusConfig : public ICanBusConfig
{
public:
   CanBusConfig() = default;
   ~CanBusConfig();

   bool Load(const char* filename) override;
};