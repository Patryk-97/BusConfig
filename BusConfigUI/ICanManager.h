#pragma once

#include "ICanBusConfig.h"

class ICanManager
{
public:
   ICanManager() = delete;

   template <typename Predicate>
   static void ForEachNetworkNode(const ICanBusConfig* canBusConfig, const Predicate& predicate)
   {
      if (canBusConfig)
      {
         for (size_t i = 0; i < canBusConfig->GetNodesCount(); i++)
         {
            if (const auto& canNetworkNode = canBusConfig->GetNodeByIndex(i); canNetworkNode)
            {
               predicate(canNetworkNode);
            }
         }
      }
   }

   template <typename Predicate>
   static void ForEachSignal(const ICanBusConfig* canBusConfig, const Predicate& predicate)
   {
      if (canBusConfig)
      {
         for (size_t i = 0; i < canBusConfig->GetSignalsCount(); i++)
         {
            if (const auto& canSignal = canBusConfig->GetSignalByIndex(i); canSignal)
            {
               predicate(canSignal);
            }
         }
      }
   }
};