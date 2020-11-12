#include "ICanBusConfig.h"
#include "CanBusConfig.h"

CanBusConfig canBusConfig;

ICanBusConfig* CanBusConfigInstanceCreate(void)
{
   return &canBusConfig;
}