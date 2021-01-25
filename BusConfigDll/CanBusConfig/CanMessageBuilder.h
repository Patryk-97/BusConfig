#pragma once

#include "ICanMessageBuilder.h"
#include <string>

class CanNetwork; // circular dependency
class CanNode;

class CanMessageBuilder : public ICanMessageBuilder
{
public:
   CanMessageBuilder(CanNetwork* network) : network { network } {}
   ~CanMessageBuilder();

   void Clear(void);

   ICanMessageBuilder* WithName(const char* name) override;
   ICanMessageBuilder* WithId(uint32_t id) override;
   ICanMessageBuilder* WithIdFormat(ICanMessage::IdFormat_e idFormat) override;
   ICanMessageBuilder* WithSize(uint32_t size) override;
   ICanMessageBuilder* WithTxMethod(ICanMessage::TxMethod_e txMethod) override;
   ICanMessageBuilder* WithCycleTime(ICanMessage::cycle_time_t cycleTime) override;
   ICanMessageBuilder* WithMainTransmitter(ICanNode* mainTransmitter) override;
   ICanMessageBuilder* WithComment(const char* comment) override;
   ICanMessage* Build(void) override;

private:
   CanNetwork* network { nullptr };
   std::string name;
   uint32_t id { ICanMessage::INVALID_ID };
   ICanMessage::IdFormat_e idFormat { ICanMessage::DEFAULT_ID_FORMAT };
   uint32_t size { ICanMessage::INVALID_SIZE };
   ICanMessage::TxMethod_e txMethod { ICanMessage::DEFAULT_TX_METHOD };
   ICanMessage::cycle_time_t cycleTime { ICanMessage::DEFAULT_CYCLE_TIME };
   CanNode* mainTransmitter { nullptr };
   std::string comment;
};