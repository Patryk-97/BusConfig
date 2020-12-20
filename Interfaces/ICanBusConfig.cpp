#include "ICanBusConfig.h"
#include "CanBusConfig.h"

ICanBusConfig* CanBusConfigInstanceCreate(void)
{
   auto canBusConfig = new CanBusConfig {};
   return canBusConfig;
}

bool CanBusConfigInstanceDelete(ICanBusConfig* canBusConfig)
{
   if (const auto instance = dynamic_cast<CanBusConfig*>(canBusConfig); instance != nullptr)
   {
      delete instance;
      return true;
   }
   return false;
}