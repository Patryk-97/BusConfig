#pragma once

#include "ICanMessage.h"

#include <string>

class CanMessage : public ICanMessage
{
public:
   CanMessage() = default;
   CanMessage(uint32_t id, const char* name, uint32_t size, const char* mainTransmitter) :
      id(id), name(name), size(size), mainTransmitter(mainTransmitter) {}
   ~CanMessage();

   void Clear(void);

   uint32_t GetId(void) const;
   void SetId(uint32_t messageId);

   const char* GetName(void) const;
   void SetName(const char* messageName);

   uint32_t GetSize(void) const;
   void SetSize(uint32_t messageSize);

   const char* GetMainTransmitter(void) const;
   void SetMainTransmitter(const char* mainTransmitter);

   size_t GetCanSignalsCount(void) const;
   //CanSignal* GetCanSignalByIndex(size_t index) const;
   //CanSignal* GetCanSignalByName(const char* signalName) const;
   //void AddCanSignal(CanSignal* dbcSignal);

   const char* ToString(void) override;

private:
   uint32_t id {};
   std::string name;
   uint32_t size {};
   std::string mainTransmitter;
   //std::vector<std::unique_ptr<CanSignal>> signals;
   std::string stringRepresentation;
};