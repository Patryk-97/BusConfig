#pragma once

#include <stdint.h>
#include <string>

class CanMessage
{
   CanMessage();
   ~CanMessage();

   void Clear(void);

   uint32_t GetId(void) const;
   void SetId(uint32_t messageId);

   const char* GetName(void) const;
   void SetName(const char* messageName);

   uint8_t GetSize(void) const;
   void SetSize(uint8_t messageSize);

   const char* GetMainTransmitter(void) const;
   void SetMainTransmitter(const char* mainTransmitter);

   size_t GetCanSignalsCount(void) const;
   //CanSignal* GetCanSignalByIndex(size_t index) const;
   //CanSignal* GetCanSignalByName(const char* signalName) const;
   //void AddCanSignal(CanSignal* dbcSignal);

private:
   uint32_t id;
   std::string name;
   uint8_t size;
   std::string mainTransmitter;
   //std::vector<std::unique_ptr<CanSignal>> signals;
};