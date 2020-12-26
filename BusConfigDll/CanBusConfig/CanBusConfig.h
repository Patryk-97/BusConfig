#pragma once

#include "ICanBusConfig.h"
#include "CanMessage.h"
#include "CanNode.h"
#include "CanEnvVar.h"
#include "CanAttributeOwner.h"
#include <string>
#include <vector>
#include <utility>
#include <string_view>
#include <span>

using LineData_t = std::pair<std::string, size_t>;

class CanBusConfig : public CanAttributeOwner, public ICanBusConfig
{
public:
   CanBusConfig() : CanAttributeOwner(ICanAttribute::IObjectType_e::NETWORK) {}
   ~CanBusConfig();

   void Clear(void) override;
   const char* GetLog(void) const override;

   bool Load(const char* filename) override;

   size_t GetNodesCount(void) const override;
   ICanNode* GetNodeByIndex(size_t index) const override;
   ICanNode* GetNodeByName(const char* name) const override;
   size_t GetNodeIndex(const char* name) const override;
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
   size_t GetSignalIndex(const char* name) const override;
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

   ICanAttributeValue* GetAttributeValue(const char* attributeName) const override;

private:
   bool ParseMessageDefinition(std::ifstream& file, LineData_t& lineData);
   bool ParseSignalDefinition(std::ifstream& file, LineData_t& lineData);
   bool ParseNodeDefinition(std::ifstream& file, LineData_t& lineData);
   bool ParseValueTableDefinition(std::ifstream& file, LineData_t& lineData);
   bool ParseAttributeDefinition(std::ifstream& file, LineData_t& lineData);
   bool ParseAttributeDefaultDefinition(std::ifstream& file, LineData_t& lineData);
   bool ParseAttributeValueDefinition(std::ifstream& file, LineData_t& lineData);

   // helpers for ParseAttributeDefinition() function
   bool ParseAttributeIntParams(std::span<std::string> paramTokens, CanAttribute*& attribute, LineData_t& lineData);
   bool ParseAttributeHexParams(std::span<std::string> paramTokens, CanAttribute*& attribute, LineData_t& lineData);
   bool ParseAttributeFloatParams(std::span<std::string> paramTokens, CanAttribute*& attribute, LineData_t& lineData);
   bool ParseAttributeStringParams(CanAttribute*& attribute, LineData_t& lineData);
   bool ParseAttributeEnumParams(std::span<std::string> paramTokens, CanAttribute*& attribute, LineData_t& lineData);


   // member variables
   std::string log;
   std::vector<CanMessage*> messages;
   std::vector<CanNode*> nodes;
   std::vector<CanSignal*> signals;
   std::vector<CanEnvVar*> envVars;

   // static variables
   static constexpr std::string_view MESSAGE_DEFINITION_HEADER = "BO_ ";
   static constexpr std::string_view SIGNAL_DEFINITION_HEADER = "SG_ ";
   static constexpr std::string_view NODE_DEFINITION_HEADER = "BU_: ";
   static constexpr std::string_view VALUE_TABLE_DEFINITION_HEADER = "VAL_ ";
   static constexpr std::string_view ATTRIBUTE_DEFINITION_HEADER = "BA_DEF_ ";
   static constexpr std::string_view ATTRIBUTE_DEFAULT_DEFINITION_HEADER = "BA_DEF_DEF_ ";
   static constexpr std::string_view ATTRIBUTE_VALUE_DEFINITION_HEADER = "BA_ ";

   static constexpr std::string_view DBC_KEYWORD_NETWORK_NODE = "BU_";
   static constexpr std::string_view DBC_KEYWORD_MESSAGE = "BO_";
   static constexpr std::string_view DBC_KEYWORD_SIGNAL = "SG_";
   static constexpr std::string_view DBC_KEYWORD_ENVIRONMENT_VARIABLE = "EV_";

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

   // attribute definition
   static constexpr uint8_t ATTRIBUTE_DEFINITION_ELEMENTS_MIN_COUNT = 3;
   static constexpr uint8_t NETWORK_ATTRIBUTE_DEFINITION_ELEMENTS_MIN_COUNT = 3;
   static constexpr uint8_t NODE_ATTRIBUTE_DEFINITION_ELEMENTS_MIN_COUNT = 4;
   static constexpr uint8_t MESSAGE_ATTRIBUTE_DEFINITION_ELEMENTS_MIN_COUNT = 4;
   static constexpr uint8_t SIGNAL_ATTRIBUTE_DEFINITION_ELEMENTS_MIN_COUNT = 4;
   static constexpr uint8_t ENVIRONMENT_VARIABLE_ATTRIBUTE_DEFINITION_ELEMENTS_MIN_COUNT = 4;
   static constexpr uint8_t ATTRIBUTE_INT_PARAMS_COUNT = 2;
   static constexpr uint8_t ATTRIBUTE_HEX_PARAMS_COUNT = 2;
   static constexpr uint8_t ATTRIBUTE_FLOAT_PARAMS_COUNT = 2;
   static constexpr uint8_t ATTRIBUTE_STRING_PARAMS_COUNT = 0;
   static constexpr uint8_t ATTRIBUTE_ENUM_PARAMS_MIN_COUNT = 1;
   static constexpr uint8_t ATTRIBUTE_DEFINITION_HEADER_POS = 0;
   static constexpr uint8_t ATTRIBUTE_OBJECT_TYPE_POS = 1;
   static constexpr uint8_t ATTRIBUTE_NAME_POS = 2;
   static constexpr uint8_t ATTRIBUTE_VALUE_TYPE_POS = 3;
   static constexpr std::string_view ATTRIBUTE_INTEGER = "INT";
   static constexpr std::string_view ATTRIBUTE_HEXADECIMAL = "HEX";
   static constexpr std::string_view ATTRIBUTE_FLOAT = "FLOAT";
   static constexpr std::string_view ATTRIBUTE_STRING = "STRING";
   static constexpr std::string_view ATTRIBUTE_ENUM = "ENUM";

   // attribute default definition
   static constexpr uint8_t ATTRIBUTE_DEFAULT_DEFINITION_ELEMENTS_COUNT = 3;
   static constexpr uint8_t ATTRIBUTE_DEFAULT_DEFINITION_HEADER_POS = 0;
   static constexpr uint8_t ATTRIBUTE_DEFAULT_NAME_POS = 1;
   static constexpr uint8_t ATTRIBUTE_DEFAULT_VALUE_POS = 2;

   // attribute value definition
   static constexpr uint8_t ATTRIBUTE_VALUE_DEFINITION_ELEMENTS_MIN_COUNT = 3;
   static constexpr uint8_t NETWORK_ATTRIBUTE_VALUE_DEFINITION_ELEMENTS_MIN_COUNT = 3;
   static constexpr uint8_t NODE_ATTRIBUTE_VALUE_DEFINITION_ELEMENTS_MIN_COUNT = 5;
   static constexpr uint8_t MESSAGE_ATTRIBUTE_VALUE_DEFINITION_ELEMENTS_MIN_COUNT = 5;
   static constexpr uint8_t SIGNAL_ATTRIBUTE_VALUE_DEFINITION_ELEMENTS_MIN_COUNT = 6;
   static constexpr uint8_t ENVIRONMENT_VARIABLE_ATTRIBUTE_VALUE_DEFINITION_ELEMENTS_MIN_COUNT = 5;
   static constexpr uint8_t ATTRIBUTE_VALUE_DEFINITION_HEADER_POS = 0;
   static constexpr uint8_t ATTRIBUTE_VALUE_NAME_POS = 1;
   static constexpr uint8_t ATTRIBUTE_VALUE_OBJECT_TYPE_POS = 2;
   static constexpr uint8_t ATTRIBUTE_VALUE_MESSAGE_ID_POS = 3;
   static constexpr uint8_t ATTRIBUTE_VALUE_NODE_NAME_POS = 3;
   static constexpr uint8_t ATTRIBUTE_VALUE_ENVIRONMENT_VARIABLE_NAME_POS = 3;
   static constexpr uint8_t ATTRIBUTE_VALUE_SIGNAL_NAME_POS = 4;
   static constexpr uint8_t ATTRIBUTE_VALUE_POS = 5;
};