#pragma once

// Classes
class ICanBusConfig;

// Dll functions
ICanBusConfig* CanBusConfigInstanceCreate(void);
bool CanBusConfigInstanceDelete(ICanBusConfig* canBusConfig);

class ICanBusConfig
{
   virtual bool Load(const char* filename) = 0;
};