#pragma once

#include "ICanBusConfig.h"
#include "CanNetwork.h"
#include "CanAttributeOwner.h"
#include <string>
#include <vector>
#include <utility>
#include <string_view>
#include <span>

using LineData_t = std::pair<std::string, size_t>;

class CanBusConfig : public ICanBusConfig
{
public:
   CanBusConfig() = default;
   ~CanBusConfig();

   void Clear(void) override;

   const char* GetLog(void) const override;

   bool Load(const char* fileName) override;
   bool Save(void) const override;
   bool Export(const char* fileName) const override;

   size_t GetNetworksCount(void) const override;
   ICanNetwork* GetNetworkByIndex(size_t index) const override;
   ICanNetwork* GetNetworkByName(const char* name) const override;
   ICanNetwork* GetNetworkFront(void) const override;
   ICanNetwork* GetNetworkBack(void) const override;
   void AddNetwork(CanNetwork* network);
   CanNetwork* CreateAndAddNetwork(void);

private:
   bool ParseMessageDefinition(CanNetwork* network, LineData_t& lineData);
   bool ParseSignalDefinition(CanNetwork* network, LineData_t& lineData);
   bool ParseNodeDefinition(CanNetwork* network, LineData_t& lineData);
   bool ParseEnvironmentVariableDefinition(CanNetwork* network, LineData_t& lineData);
   bool ParseValueTableDefinition(CanNetwork* network, LineData_t& lineData);
   bool ParseAttributeDefinition(CanNetwork* network, LineData_t& lineData);
   bool ParseAttributeDefaultDefinition(CanNetwork* network, LineData_t& lineData);
   bool ParseAttributeValueDefinition(CanNetwork* network, LineData_t& lineData);
   bool ParseEnvironmentVariableDataDefinition(CanNetwork* network, LineData_t& lineData);
   bool ParseCommentDefinition(std::ifstream& file, CanNetwork* network, LineData_t& lineData);

   // helpers for ParseAttributeDefinition() function
   bool ParseAttributeIntParams(std::span<std::string> paramTokens, CanAttribute*& attribute, LineData_t& lineData);
   bool ParseAttributeHexParams(std::span<std::string> paramTokens, CanAttribute*& attribute, LineData_t& lineData);
   bool ParseAttributeFloatParams(std::span<std::string> paramTokens, CanAttribute*& attribute, LineData_t& lineData);
   bool ParseAttributeStringParams(CanAttribute*& attribute, LineData_t& lineData);
   bool ParseAttributeEnumParams(std::span<std::string> paramTokens, CanAttribute*& attribute, LineData_t& lineData);

   void SetMainAttributes(void);

   bool WriteFileHeader(std::string& lineStr) const;
   bool WriteMessageDefinition(std::string& lineStr) const;
   bool WriteSignalDefinition(CanMessage* message, std::string& lineStr) const;
   bool WriteNodeDefinition(std::string& lineStr) const;
   bool WriteEnvironmentVariableDefinition(std::string& lineStr) const;
   bool WriteEnvironmentVariableDataDefinition(std::string& lineStr) const;
   bool WriteCommentDefinition(std::string& lineStr) const;
   bool WriteAttributeDefinition(std::string& lineStr) const;
   bool WriteAttributeDefaultDefinition(std::string& lineStr) const;
   bool WriteAttributeValueDefinition(std::string& lineStr) const;
   bool WriteValueTableDefinition(std::string& lineStr) const;

   // member variables
   std::string fileName;
   std::string log;
   std::vector<CanNetwork*> networks;

   // static variables
   static constexpr std::string_view MESSAGE_DEFINITION_HEADER = "BO_ ";
   static constexpr std::string_view SIGNAL_DEFINITION_HEADER = "SG_ ";
   static constexpr std::string_view NODE_DEFINITION_HEADER = "BU_: ";
   static constexpr std::string_view ENVIRONMENT_VARIABLE_DEFINITION_HEADER = "EV_ ";
   static constexpr std::string_view VALUE_TABLE_DEFINITION_HEADER = "VAL_ ";
   static constexpr std::string_view ATTRIBUTE_DEFINITION_HEADER = "BA_DEF_ ";
   static constexpr std::string_view ATTRIBUTE_DEFAULT_DEFINITION_HEADER = "BA_DEF_DEF_ ";
   static constexpr std::string_view ATTRIBUTE_VALUE_DEFINITION_HEADER = "BA_ ";
   static constexpr std::string_view ENVIRONMENT_VARIABLE_DATA_DEFINITION_HEADER = "ENVVAR_DATA_ ";
   static constexpr std::string_view COMMENT_DEFINITION_HEADER = "CM_ ";

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

   // environment variable definition
   static constexpr uint8_t ENVIRONMENT_VARIABLE_DEFINITION_ELEMENTS_COUNT = 10;
   static constexpr uint8_t ENVIRONMENT_VARIABLE_DEFINITION_HEADER_POS = 0;
   static constexpr uint8_t ENVIRONMENT_VARIABLE_NAME_POS = 1;
   static constexpr uint8_t ENVIRONMENT_VARIABLE_TYPE_POS = 2;
   static constexpr uint8_t ENVIRONMENT_VARIABLE_MINIMUM_POS = 3;
   static constexpr uint8_t ENVIRONMENT_VARIABLE_MAXIMUM_POS = 4;
   static constexpr uint8_t ENVIRONMENT_VARIABLE_UNIT_POS = 5;
   static constexpr uint8_t ENVIRONMENT_VARIABLE_INITIAL_VALUE_POS = 6;
   static constexpr uint8_t ENVIRONMENT_VARIABLE_ID_POS = 7;
   static constexpr uint8_t ENVIRONMENT_VARIABLE_ACCESS_TYPE_POS = 8;
   static constexpr uint8_t ENVIRONMENT_VARIABLE_ACCESS_NODE_POS = 9;

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

   // environment variable data definition
   static constexpr uint8_t ENVIRONMENT_VARIABLE_DATA_DEFINITION_ELEMENTS_COUNT = 3;
   static constexpr uint8_t ENVIRONMENT_VARIABLE_DATA_DEFINITION_HEADER_POS = 0;
   //static constexpr uint8_t ENVIRONMENT_VARIABLE_NAME_POS = 1;
   static constexpr uint8_t ENVIRONMENT_VARIABLE_DATA_SIZE_POS = 2;

   // comment definition
   static constexpr uint8_t COMMENT_DEFINITION_ELEMENTS_MIN_COUNT = 2;
   static constexpr uint8_t NETWORK_COMMENT_DEFINITION_ELEMENTS_MIN_COUNT = 2;
   static constexpr uint8_t NODE_COMMENT_DEFINITION_ELEMENTS_MIN_COUNT = 4;
   static constexpr uint8_t MESSAGE_COMMENT_DEFINITION_ELEMENTS_MIN_COUNT = 4;
   static constexpr uint8_t SIGNAL_COMMENT_DEFINITION_ELEMENTS_MIN_COUNT = 5;
   static constexpr uint8_t ENVIRONMENT_VARIABLE_COMMENT_DEFINITION_ELEMENTS_MIN_COUNT = 4;
   static constexpr uint8_t COMMENT_DEFINITION_HEADER_POS = 0;
   static constexpr uint8_t COMMENT_OBJECT_TYPE_POS = 1;
   static constexpr uint8_t COMMENT_OBJECT_TYPE_NAME_POS = 2;
   static constexpr uint8_t COMMENT_SIGNAL_NAME_POS = 3;
   static constexpr uint8_t COMMENT_DATA_POS = 4;
};