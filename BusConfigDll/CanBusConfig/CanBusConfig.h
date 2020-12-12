#pragma once

#include "ICanBusConfig.h"
#include "CanMessage.h"
#include "CanNode.h"
#include "CanEnvVar.h"
#include "CanAttribute.h"
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

   size_t GetNodesCount(void) const override;
   ICanNode* GetNodeByIndex(size_t index) const override;
   ICanNode* GetNodeByName(const char* name) const override;
   void AddNode(CanNode* node);
   CanNode* CreateAndAddNode(void);

   size_t GetMessagesCount(void) const override;
   ICanMessage* GetMessageById(uint32_t id) const override;
   ICanMessage* GetMessageByName(const char* name) const override;
   ICanMessage* GetMessageByIndex(size_t index) const override;
   ICanMessage* GetMessageFront(void) const override;
   ICanMessage* GetMessageBack(void) const override;
   void AddMessage(CanMessage* message);
   CanMessage* CreateAndAddMessage(void);

   size_t GetSignalsCount(void) const override;
   ICanSignal* GetSignalByIndex(size_t index) const override;
   ICanSignal* GetSignalByName(const char* name) const override;
   void AddSignal(CanSignal* signal);
   CanSignal* CreateAndAddSignal(void);

   size_t GetEnvVarsCount(void) const override;
   ICanEnvVar* GetEnvVarByIndex(size_t index) const override;
   ICanEnvVar* GetEnvVarByName(const char* name) const override;
   void AddEnvVar(CanEnvVar* envVar);
   CanEnvVar* CreateAndAddEnvVar(void);

   size_t GetAttributesCount(void) const override;
   ICanAttribute* GetAttributeByIndex(size_t index) const override;
   ICanAttribute* GetAttributeByName(const char* name) const override;
   void AddAttribute(CanAttribute* attribute);
   CanAttribute* CreateAndAddAttribute(void);

private:
   bool ParseMessageDefinition(std::ifstream& file, LineData_t& lineData);
   bool ParseSignalDefinition(std::ifstream& file, LineData_t& lineData);
   bool ParseNodeDefinition(std::ifstream& file, LineData_t& lineData);
   bool ParseValueTableDefinition(std::ifstream& file, LineData_t& lineData);


   // member variables
   std::string log;
   std::vector<CanMessage*> messages;
   std::vector<CanNode*> nodes;
   std::vector<CanSignal*> signals;
   std::vector<CanEnvVar*> envVars;
   std::vector<CanAttribute*> attributes;

   // static variables
   static constexpr std::string_view MESSAGE_DEFINITION_HEADER = "BO_ ";
   static constexpr std::string_view SIGNAL_DEFINITION_HEADER = "SG_ ";
   static constexpr std::string_view NODE_DEFINITION_HEADER = "BU_: ";
   static constexpr std::string_view VALUE_TABLE_DEFINITION_HEADER = "VAL_ ";

   // message definition
   static constexpr uint8_t MESSAGE_DEFINITION_ELEMENTS_COUNT = 5;
   static constexpr uint8_t MESSAGE_DEFINITION_HEADER_POS = 0;
   static constexpr uint8_t MESSAGE_ID_POS = 1;
   static constexpr uint8_t MESSAGE_NAME_POS = 2;
   static constexpr uint8_t MESSAGE_SIZE_POS = 3;
   static constexpr uint8_t MESSAGE_TRANSMITTER_POS = 4;

   // signal definition
   static constexpr uint8_t SIGNAL_DEFINITION_ELEMENTS_MIN_COUNT = 13;
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

   // node definition
   static constexpr uint8_t NODE_DEFINITION_ELEMENTS_MIN_COUNT = 2;

   // value table definition
   static constexpr uint8_t VALUE_TABLE_DEFINITION_ELEMENTS_MIN_COUNT = 4;
   static constexpr uint8_t VALUE_TABLE_DEFINITION_HEADER_POS = 0;
   static constexpr uint8_t VALUE_TABLE_ENV_VAR_NAME_POS = 1;
   static constexpr uint8_t VALUE_TABLE_MESSAGE_ID_POS = 1;
   static constexpr uint8_t VALUE_TABLE_SIGNAL_NAME_POS = 2;
};