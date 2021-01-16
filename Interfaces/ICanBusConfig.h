#pragma once

#include "ICanNetwork.h"

// Classes
class ICanBusConfig;

// Dll functions
ICanBusConfig* CanBusConfigInstanceCreate(void);
bool CanBusConfigInstanceDelete(ICanBusConfig* canBusConfig);

class ICanBusConfig
{
protected:
   ICanBusConfig() {};
   virtual ~ICanBusConfig() {};
public:
   virtual void Clear(void) = 0;
   virtual const char* GetLog(void) const = 0;

   virtual bool Load(const char* fileName) = 0;
   virtual bool Save(void) const = 0;
   virtual bool Export(const char* fileName) const = 0;

   virtual size_t GetNetworksCount(void) const = 0;
   virtual ICanNetwork* GetNetworkByIndex(size_t index) const = 0;
   virtual ICanNetwork* GetNetworkByName(const char * name) const = 0;
   virtual ICanNetwork* GetNetworkFront(void) const = 0;
   virtual ICanNetwork* GetNetworkBack(void) const = 0;
};