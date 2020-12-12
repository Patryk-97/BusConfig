#pragma once

#include "ICanMessage.h"
#include "CanSignal.h"
#include "CanAttribute.h"

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

   size_t GetSignalsCount(void) const override;
   ICanSignal* GetSignalByName(const char* name) const override;
   ICanSignal* GetSignalByIndex(size_t index) const override;
   void AddSignal(CanSignal* signal);

   size_t GetAttributesCount(void) const override;
   ICanAttribute* GetAttributeByIndex(size_t index) const override;
   ICanAttribute* GetAttributeByName(const char* name) const override;
   void AddAttribute(CanAttribute* attribute);

   const char* ToString(void) override;

private:
   uint32_t id {};
   std::string name;
   uint32_t size {};
   std::string mainTransmitter;
   std::vector<CanSignal*> signals;
   std::vector<CanAttribute*> attributes;
   std::string stringRepresentation;
};