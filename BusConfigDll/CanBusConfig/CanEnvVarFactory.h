#pragma once

#include "CanEnvVar.h"

class CanEnvVarFactory
{
public:
   CanEnvVarFactory() = delete;
   static CanEnvVar* CreateEnvVar(ICanEnvVar::Type_e type);
   static CanEnvVar* CreateEnvVar(uint32_t type);
};