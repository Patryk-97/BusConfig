#pragma once

#include "ICanMessage.h"

class ICanMessageBuilder
{
protected:
   ICanMessageBuilder() {};
   virtual ~ICanMessageBuilder() {};
public:
   virtual ICanMessageBuilder* WithName(const char* name) = 0;
   virtual ICanMessageBuilder* WithId(uint32_t id) = 0;
   virtual ICanMessageBuilder* WithIdFormat(ICanMessage::IdFormat_e idFormat) = 0;
   virtual ICanMessageBuilder* WithSize(uint32_t size) = 0;
   virtual ICanMessageBuilder* WithTxMethod(ICanMessage::TxMethod_e txMethod) = 0;
   virtual ICanMessageBuilder* WithCycleTime(ICanMessage::cycle_time_t cycleTime) = 0;
   virtual ICanMessageBuilder* WithMainTransmitter(ICanNode* mainTransmitter) = 0;
   virtual ICanMessageBuilder* WithComment(const char* comment) = 0;
   virtual ICanMessage* Build(void) = 0;
};