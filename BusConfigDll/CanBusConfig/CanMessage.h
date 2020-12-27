#pragma once

#include "ICanMessage.h"
#include "CanSignal.h"

#include <string>

class CanMessage : public CanAttributeOwner, public ICanMessage
{
public:
   CanMessage() : CanAttributeOwner(ICanAttribute::IObjectType_e::MESSAGE) {};
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

   ICanAttributeValue* GetAttributeValue(const char* attributeName) const override;

   IdFormat_e GetIdFormat(void) const override;
   void SetIdFormat(IdFormat_e idFormat);

   TxMethod_e GetTxMethod(void) const override;
   void SetTxMethod(TxMethod_e txMethod);

   cycle_time_t GetCycleTime(void) const override;
   void SetCycleTime(cycle_time_t cycleTime);

   const char* ToString(void) override;

private:
   uint32_t id {};
   std::string name;
   uint32_t size {};
   std::string mainTransmitter;
   std::vector<CanSignal*> signals;

   // Main attributes
   IdFormat_e idFormat { ICanMessage::DEFAULT_ID_FORMAT };
   TxMethod_e txMethod { ICanMessage::DEFAULT_TX_METHOD };
   cycle_time_t cycleTime { ICanMessage::DEFAULT_CYCLE_TIME };

   std::string stringRepresentation;
};