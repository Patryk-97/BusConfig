#include "DllLoader.h"
#include "ICanBusConfig.h"

#include <iostream>

DllLoader<ICanBusConfig, bool> dllLoader { "BusConfigDll", "CanBusConfigInstanceCreate", "CanBusConfigInstanceDelete" };

bool LoadBusConfigDll(void)
{
   // locals
   bool rV {};
   ICanBusConfig* busConfig { nullptr };

   if (dllLoader.Load())
   {
      busConfig = dllLoader.pfCreate();

      if (busConfig != nullptr)
      {
         rV = true;
      }
   }

   return rV;
}

int main()
{
   bool rV = LoadBusConfigDll();
   if (rV)
   {
      std::cout << "Successfully loaded BusConfigDll.dll\n";
      auto canBusConfig = dllLoader.pfCreate();

      canBusConfig->Load("sample.dbc");

      std::cout << "Nodes count: " << canBusConfig->GetNodesCount() << "\n";
      for (size_t i = 0; i < canBusConfig->GetNodesCount(); i++)
      {
         auto node = canBusConfig->GetNodeByIndex(i);
         std::cout << node->GetName() << ", ";
      }
      std::cout << "\n\n";
      std::cout << "Messages count: " << canBusConfig->GetMessagesCount() << "\n";
      for (size_t i = 0; i < canBusConfig->GetMessagesCount(); i++)
      {
         auto message = canBusConfig->GetMessageByIndex(i);
         std::cout << message->ToString() << "\n";
         for (size_t j = 0; j < message->GetSignalsCount(); j++)
         {
            auto signal = message->GetSignalByIndex(j);
            std::cout << '\t' << signal->ToString() << "\n";
         }
      }
      std::cout << canBusConfig->GetLog() << "\n";
   }
   else
   {
      std::cout << "Loading BusConfigDll.dll failed\n";
   }
}