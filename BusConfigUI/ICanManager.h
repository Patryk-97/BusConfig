#pragma once

#include "ICanNetwork.h"

class ICanManager
{
public:
   ICanManager() = delete;

   template <typename Predicate>
   static void ForEachNetworkNode(const ICanNetwork* canBusConfig, const Predicate& predicate)
   {
      if (canNetwork)
      {
         for (size_t i = 0; i < canNetwork->GetNodesCount(); i++)
         {
            if (const auto& canNetworkNode = canBusConfig->GetNodeByIndex(i); canNetworkNode)
            {
               predicate(canNetworkNode);
            }
         }
      }
   }

   template <typename Predicate>
   static void ForEachSignal(const ICanNetwork* canNetwork, const Predicate& predicate)
   {
      if (canNetwork)
      {
         for (size_t i = 0; i < canNetwork->GetSignalsCount(); i++)
         {
            if (const auto& canSignal = canBusConfig->GetSignalByIndex(i); canSignal)
            {
               predicate(canSignal);
            }
         }
      }
   }
};