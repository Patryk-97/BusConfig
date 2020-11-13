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
   void AddMessage(CanMessage* message);
   CanMessage* CreateAndAddMessage(void);

private:
   bool ParseMessageDefinition(std::ifstream& file, LineData_t& lineData);


   // member variables
   std::string log;
   std::vector<CanMessage*> messages;

   // static variables
   static constexpr std::string_view MESSAGE_DEFINITION_HEADER = "BO_ ";

   static constexpr uint8_t MESSAGE_DEFINITION_ELEMENTS_COUNT = 5;
   static constexpr uint8_t MESSAGE_DEFINITION_HEADER_POS = 0;
   static constexpr uint8_t MESSAGE_ID_POS = 1;
   static constexpr uint8_t MESSAGE_NAME_POS = 2;
   static constexpr uint8_t MESSAGE_SIZE_POS = 3;
   static constexpr uint8_t MESSAGE_TRANSMITTER_POS = 4;
};