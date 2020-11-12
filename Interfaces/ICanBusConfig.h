#pragma once

// Classes
class ICanBusConfig;

// Dll functions
ICanBusConfig* CanBusConfigInstanceCreate(void);

class ICanBusConfig
{
   virtual bool Load(const char* filename) = 0;
};