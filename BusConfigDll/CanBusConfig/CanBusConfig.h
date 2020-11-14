#pragma once

#include "ICanBusConfig.h"
#include "CanMessage.h"
#include <string>
#include <vector>
#include <utility>
#include <string_view>

using LineData_t = std::pair<std::string, size_t>;

class CanBusConfig : public ICanBusConfig
{
public:
   CanBusConfig() = default;
   ~CanBusConfig();

   void Clear(void) override;
   const char* GetLog(void) const override;

   bool Load(const char* filename) override;

   size_t GetMessagesCount(void) const override;
   ICanMessage* GetMessageById(uint32_t id) const override;
   ICanMessage* GetMessageByName(const char* name) const override;
   ICanMessage* GetMessageByIndex(size_t index) const override;
   ICanMessage* GetMessageFront(void) const override;
   ICanMessage* GetMessageBack(void) const override;
   void AddMessage(CanMessage* message);
   CanMessage* CreateAndAddMessage(void);

private:
   bool ParseMessageDefinition(std::ifstream& file, LineData_t& lineData);
   bool ParseSignalDefinition(std::ifstream& file, LineData_t& lineData);


   // member variables
   std::string log;
   std::vector<CanMessage*> messages;

   // static variables
   static constexpr std::string_view MESSAGE_DEFINITION_HEADER = "BO_ ";
   static constexpr std::string_view SIGNAL_DEFINITION_HEADER = "SG_ ";

   // message definition
   static constexpr uint8_t MESSAGE_DEFINITION_ELEMENTS_COUNT = 5;
   static constexpr uint8_t MESSAGE_DEFINITION_HEADER_POS = 0;
   static constexpr uint8_t MESSAGE_ID_POS = 1;
   static constexpr uint8_t MESSAGE_NAME_POS = 2;
   static constexpr uint8_t MESSAGE_SIZE_POS = 3;
   static constexpr uint8_t MESSAGE_TRANSMITTER_POS = 4;

   // signal definition
   static constexpr uint8_t SIGNAL_DEFINITION_ELEMENTS_MIN_COUNT = 11;
   static constexpr uint8_t SIGNAL_DEFINITION_HEADER_POS = 0;
   static constexpr uint8_t SIGNAL_NAME_POS = 1;
   static constexpr uint8_t SIGNAL_MULTIPLEXED_INDICATOR_POS = 2;
   static constexpr uint8_t SIGNAL_START_BIT_POS = 3;
   static constexpr uint8_t SIGNAL_SIZE_POS = 4;
   static constexpr uint8_t SIGNAL_BYTE_ORDER_POS = 5;
   static constexpr uint8_t SIGNAL_VALUE_TYPE_POS = 6;
   static constexpr uint8_t SIGNAL_FACTOR_POS = 7;
   static constexpr uint8_t SIGNAL_OFFSET_POS = 8;
   static constexpr uint8_t SIGNAL_MINIMUM_POS = 9;
   static constexpr uint8_t SIGNAL_MAXIMUM_POS = 10;
   static constexpr uint8_t SIGNAL_UNIT_POS = 11;
   static constexpr uint8_t SIGNAL_RECEIVER_POS = 12;
};