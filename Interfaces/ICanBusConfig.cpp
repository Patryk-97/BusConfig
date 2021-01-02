#include "ICanBusConfig.h"
#include "CanBusConfig.h"

ICanBusConfig* CanBusConfigInstanceCreate(void)
{
   return new CanBusConfig {};
}

bool CanBusConfigInstanceDelete(ICanBusConfig* canBusConfig)
{
   if (const auto instance = dynamic_cast<CanBusConfig*>(canBusConfig); instance)
   {
      delete instance;
      return true;
   }
   return false;
}