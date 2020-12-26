#include "CanBusConfig.h"
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>
#include "helpers.h"
#include "CanAttributeManager.h"
#include "CanIntAttribute.h"
#include "CanHexAttribute.h"
#include "CanFloatAttribute.h"
#include "CanStringAttribute.h"
#include "CanEnumAttribute.h"
#include "ICanAttributeValueFactory.h"
#include <fstream>
#include <iostream>
#include <algorithm>

#ifndef line
#define line lineData.first
#endif

#ifndef lineNr
#define lineNr lineData.second
#endif

using boost_escaped_separator = boost::escaped_list_separator<char>;
using boost_char_separator = boost::char_separator<char>;
using boost_escaped_separator_tokenizer = boost::tokenizer<boost_escaped_separator>;
using boost_char_separator_tokenizer = boost::tokenizer<boost_char_separator>;

namespace ranges = std::ranges;
using namespace std::string_literals;

CanBusConfig::~CanBusConfig()
{
   this->Clear();
}

void CanBusConfig::Clear()
{
   CanAttributeOwner::Clear();
   this->log = "";
   helpers::ClearContainer(this->messages);
   helpers::ClearContainer(this->nodes);
   helpers::ClearContainer(this->signals);
   helpers::ClearContainer(this->envVars);
}

const char* CanBusConfig::GetLog(void) const
{
   return this->log.c_str();
}

bool CanBusConfig::Load(const char* filename)
{
   // locals
   bool rV { false };
   LineData_t lineData;
   std::ifstream file(filename);

   if (file.is_open())
   {
      rV = true;

      while (std::getline(file, line))
      {
         boost::algorithm::trim(line);
         if (line.starts_with(CanBusConfig::MESSAGE_DEFINITION_HEADER))
         {
            this->ParseMessageDefinition(file, lineData);
         }
         else if (line.starts_with(CanBusConfig::SIGNAL_DEFINITION_HEADER))
         {
            this->ParseSignalDefinition(file, lineData);
         }
         else if (line.starts_with(CanBusConfig::NODE_DEFINITION_HEADER))
         {
            this->ParseNodeDefinition(file, lineData);
         }
         else if (line.starts_with(CanBusConfig::VALUE_TABLE_DEFINITION_HEADER))
         {
            this->ParseValueTableDefinition(file, lineData);
         }
         else if (line.starts_with(CanBusConfig::ATTRIBUTE_DEFINITION_HEADER))
         {
            this->ParseAttributeDefinition(file, lineData);
         }
         else if (line.starts_with(CanBusConfig::ATTRIBUTE_DEFAULT_DEFINITION_HEADER))
         {
            this->ParseAttributeDefaultDefinition(file, lineData);
         }
         else if (line.starts_with(CanBusConfig::ATTRIBUTE_VALUE_DEFINITION_HEADER))
         {
            this->ParseAttributeValueDefinition(file, lineData);
         }
         lineNr++;
      }
   }

   return rV;
}

size_t CanBusConfig::GetNodesCount(void) const
{
   return this->nodes.size();
}

ICanNode* CanBusConfig::GetNodeByIndex(size_t index) const
{
   return (index < this->nodes.size() ? this->nodes[index] : nullptr);
}

ICanNode* CanBusConfig::GetNodeByName(const char* name) const
{
   auto it = ranges::find_if(this->nodes, [&name](CanNode* node) { return !std::strcmp(node->GetName(), name); });
   return (it != this->nodes.end() ? *it : nullptr);
}

size_t CanBusConfig::GetNodeIndex(const char* name) const
{
   auto it = ranges::find_if(this->nodes, [&name](CanNode* node) { return !std::strcmp(node->GetName(), name); });
   return (it != this->nodes.end() ? std::distance(this->nodes.begin(), it) :
      ICanBusConfig::INVALID_INDEX);
}

void CanBusConfig::AddNode(CanNode* node)
{
   if (node)
   {
      this->nodes.push_back(node);
   }
}

CanNode* CanBusConfig::CreateAndAddNode(void)
{
   CanNode* node = new CanNode{};
   this->nodes.push_back(node);
   return node;
}

size_t CanBusConfig::GetMessagesCount(void) const
{
   return this->messages.size();
}

ICanMessage* CanBusConfig::GetMessageById(uint32_t id) const
{
   auto it = ranges::find_if(this->messages, [&id] (CanMessage* message) { return message->GetId() == id; });
   return (it != this->messages.end() ? *it : nullptr);
}

ICanMessage* CanBusConfig::GetMessageByName(const char* name) const
{
   auto it = ranges::find_if(this->messages, [&name](CanMessage* message) { return !std::strcmp(message->GetName(), name); });
   return (it != this->messages.end() ? *it : nullptr);
}

ICanMessage* CanBusConfig::GetMessageByIndex(size_t index) const
{
   return (index < this->messages.size() ? this->messages[index] : nullptr);
}

ICanMessage* CanBusConfig::GetMessageFront(void) const
{
   return (this->messages.size() > 0 ? this->messages.front() : nullptr);
}

ICanMessage* CanBusConfig::GetMessageBack(void) const
{
   return (this->messages.size() > 0 ? this->messages.back() : nullptr);
}

size_t CanBusConfig::GetSignalsCount(void) const
{
   return this->signals.size();
}

ICanSignal* CanBusConfig::GetSignalByIndex(size_t index) const
{
   return (index < this->signals.size() ? this->signals[index] : nullptr);
}

ICanSignal* CanBusConfig::GetSignalByName(const char* name) const
{
   auto it = ranges::find_if(this->signals, [&name](CanSignal* signal) { return !std::strcmp(signal->GetName(), name); });
   return (it != this->signals.end() ? *it : nullptr);
}

size_t CanBusConfig::GetSignalIndex(const char* name) const
{
   auto it = ranges::find_if(this->signals, [&name](CanSignal* signal) { return !std::strcmp(signal->GetName(), name); });
   return (it != this->signals.end() ? std::distance(this->signals.begin(), it) :
      ICanBusConfig::INVALID_INDEX);
}

void CanBusConfig::AddSignal(CanSignal* signal)
{
   if (signal)
   {
      this->signals.push_back(signal);
   }
}

CanSignal* CanBusConfig::CreateAndAddSignal(void)
{
   CanSignal* signal = new CanSignal {};
   this->signals.push_back(signal);
   return signal;
}

void CanBusConfig::AddMessage(CanMessage* message)
{
   if (message)
   {
      this->messages.push_back(message);
   }
}

CanMessage* CanBusConfig::CreateAndAddMessage(void)
{
   CanMessage* message = new CanMessage {};
   this->messages.push_back(message);
   return message;
}

size_t CanBusConfig::GetEnvVarsCount(void) const
{
   return this->envVars.size();
}

ICanEnvVar* CanBusConfig::GetEnvVarByIndex(size_t index) const
{
   return (index < this->envVars.size() ? this->envVars[index] : nullptr);
}

ICanEnvVar* CanBusConfig::GetEnvVarByName(const char* name) const
{
   auto it = ranges::find_if(this->envVars, [&name](CanEnvVar* envVar) { return !std::strcmp(envVar->GetName(), name); });
   return (it != this->envVars.end() ? *it : nullptr);
}

void CanBusConfig::AddEnvVar(CanEnvVar* envVar)
{
   if (envVar)
   {
      this->envVars.push_back(envVar);
   }
}

CanEnvVar* CanBusConfig::CreateAndAddEnvVar(void)
{
   CanEnvVar* envVar = new CanEnvVar {};
   this->envVars.push_back(envVar);
   return envVar;
}

size_t CanBusConfig::GetAttributesCount(void) const
{
   return CanAttributeOwner::GetAttributesCount();
}

ICanAttribute* CanBusConfig::GetAttributeByIndex(size_t index) const
{
   return CanAttributeOwner::GetAttributeByIndex(index);
}

ICanAttribute* CanBusConfig::GetAttributeByName(const char* name) const
{
   return CanAttributeOwner::GetAttributeByName(name);
}

ICanAttributeValue* CanBusConfig::GetAttributeValue(const char* attributeName) const
{
   return CanAttributeOwner::GetAttributeValue(attributeName);
}

bool CanBusConfig::ParseMessageDefinition(std::ifstream& file, LineData_t& lineData)
{
   // locals
   bool rV { true };

   if (line.starts_with(CanBusConfig::MESSAGE_DEFINITION_HEADER))
   {
      boost_char_separator sep { " :" };
      boost_char_separator_tokenizer tokenizer { line, sep };
      
      // If everything's okay
      if (ranges::distance(tokenizer) == MESSAGE_DEFINITION_ELEMENTS_COUNT)
      {
         CanMessage* message = this->CreateAndAddMessage();
         for (uint8_t pos{}; const auto& token : tokenizer)
         {
            switch (pos)
            {
               case MESSAGE_DEFINITION_HEADER_POS:
               {
                  // do nothing
                  break;
               }
               case MESSAGE_ID_POS:
               {
                  try
                  {
                     message->SetId(std::stoul(token));
                  }
                  catch (...)
                  {
                     rV = false;
                     this->log += "Message id conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                  }
                  break;
               }
               case MESSAGE_NAME_POS:
               {
                  message->SetName(token.c_str());
                  break;
               }
               case MESSAGE_SIZE_POS:
               {
                  try
                  {
                     message->SetSize(std::stoul(token));
                  }
                  catch (...)
                  {
                     rV = false;
                     this->log += "Message size conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                  }
                  break;
               }
               case MESSAGE_TRANSMITTER_POS:
               {
                  message->SetMainTransmitter(token.c_str());
                  if (CanNode* node = dynamic_cast<CanNode*>(this->GetNodeByName(token.c_str())); node != nullptr)
                  {
                     node->AddTxMessage(message);
                  }
                  break;
               }
            }

            // If something went wrong
            if (!rV)
            {
               break;
            }
            ++pos;
         }
      }
      else
      {
         rV = false;
         this->log += "Invalid message definition elements count [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      }
   }
   else
   {
      this->log += "Message definition header invalid [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      rV = false;
   }

   return rV;
}

bool CanBusConfig::ParseSignalDefinition(std::ifstream& file, LineData_t& lineData)
{
   CanMessage* message = this->messages.back();
   if (message == nullptr) { return false; }

   // locals
   bool rV { true };

   if (line.starts_with(CanBusConfig::SIGNAL_DEFINITION_HEADER))
   {
      boost_escaped_separator sep("\\", " :|@(,)[]", "\"");
      boost_escaped_separator_tokenizer tokenizer { line, sep }; // remains empty tokens
      std::vector<std::string> tokens;

      // prepare signal definition tokens
      std::invoke([&tokenizer, &tokens, this]
      {
         for (size_t pos{}; const auto& token : tokenizer)
         {
            if (token != "")
            {
               if (pos == SIGNAL_MULTIPLEXED_INDICATOR_POS && !token.starts_with("m") && !token.starts_with("M"))
               {
                  tokens.push_back("");
                  tokens.push_back(token);
                  pos += 2;
               }
               else if (pos == SIGNAL_UNIT_POS &&
                  (this->GetNodeByName(token.c_str()) != nullptr || token == ICanNode::PSEUDO_NODE_NAME)) // if "" - for unit element
               {
                  tokens.push_back("");
                  tokens.push_back(token);
                  pos += 2;
               }
               else if (token.size() == 2 && token.ends_with("+")) // split byte order and value type
               {
                  tokens.push_back(std::string{ 1, token[0] });
                  tokens.push_back(std::string{ 1, token[1] });
                  pos += 2;
               }
               else
               {
                  tokens.push_back(token);
                  pos++;
               }
            }
         }  
      });

      // If everything's okay
      if (const auto elementsCount = ranges::distance(tokens); elementsCount >= SIGNAL_DEFINITION_ELEMENTS_MIN_COUNT)
      {
         CanSignal* signal = this->CreateAndAddSignal();
         message->AddSignal(signal);
         signal->SetMessage(message);
         if (CanNode* transmitterNode = dynamic_cast<CanNode*>(this->GetNodeByName(message->GetMainTransmitter())); transmitterNode != nullptr)
         {
            transmitterNode->AddMappedTxSignal(signal);
         }

         for (uint8_t pos{}; const auto& token : tokens)
         {
            switch (pos)
            {
               case SIGNAL_DEFINITION_HEADER_POS:
               {
                  // do nothing
                  break;
               }
               case SIGNAL_NAME_POS:
               {
                  signal->SetName(token.c_str());
                  break;
               }
               case SIGNAL_MULTIPLEXED_INDICATOR_POS:
               {
                  if (token != "")
                  {
                     signal->SetMuxIndicator(token.c_str());
                  }
                  break;
               }
               case SIGNAL_START_BIT_POS:
               {
                  try
                  {
                     signal->SetStartBit(std::stoul(token));
                  }
                  catch (...)
                  {
                     rV = false;
                     this->log += "Signal start bit conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                  }
                  break;
               }
               case SIGNAL_SIZE_POS:
               {
                  try
                  {
                     signal->SetSize(std::stoul(token));
                  }
                  catch (...)
                  {
                     rV = false;
                     this->log += "Signal size conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                  }
                  break;
               }
               case SIGNAL_BYTE_ORDER_POS:
               {
                  signal->SetByteOrderSymbol(token[0]);
                  break;
               }
               case SIGNAL_VALUE_TYPE_POS:
               {
                  signal->SetValueTypeSymbol(token[0]);
                  break;
               }
               case SIGNAL_FACTOR_POS:
               {
                  try
                  {
                     signal->SetFactor(std::stod(token));
                  }
                  catch (...)
                  {
                     rV = false;
                     this->log += "Signal factor conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                  }
                  break;
               }
               case SIGNAL_OFFSET_POS:
               {
                  try
                  {
                     signal->SetOffset(std::stod(token));
                  }
                  catch (...)
                  {
                     rV = false;
                     this->log += "Signal offset conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                  }
                  break;
               }
               case SIGNAL_MINIMUM_POS:
               {
                  try
                  {
                     signal->SetMinimum(std::stod(token));
                  }
                  catch (...)
                  {
                     rV = false;
                     this->log += "Signal minimum conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                  }
                  break;
               }
               case SIGNAL_MAXIMUM_POS:
               {
                  try
                  {
                     signal->SetMaximum(std::stod(token));
                  }
                  catch (...)
                  {
                     rV = false;
                     this->log += "Signal maximum conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                  }
                  break;
               }
               case SIGNAL_UNIT_POS:
               {
                  signal->SetUnit(token.c_str());
                  break;
               }
               default:
               {
                  signal->AddReceiver(token.c_str());
                  if (CanNode* node = dynamic_cast<CanNode*>(this->GetNodeByName(token.c_str())); node != nullptr)
                  {
                     node->AddMappedRxSignal(signal);
                     node->AddRxMessage(dynamic_cast<CanMessage*>(signal->GetMessage()));
                  }
                  break;
               }
            }

            // If something went wrong
            if (!rV)
            {
               break;
            }
            ++pos;
         }
      }
      else
      {
         rV = false;
         this->log += "Invalid signal definition elements count: " + std::to_string(elementsCount);
         this->log += " [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      }
   }
   else
   {
      this->log += "Signal definition header invalid [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      rV = false;
   }

   return rV;
}

bool CanBusConfig::ParseNodeDefinition(std::ifstream& file, LineData_t& lineData)
{
   // locals
   bool rV{ true };

   if (line.starts_with(CanBusConfig::NODE_DEFINITION_HEADER))
   {
      boost_char_separator sep(" ");
      boost_char_separator_tokenizer tokenizer{ line, sep };

      // If everything's okay
      if (ranges::distance(tokenizer) >= NODE_DEFINITION_ELEMENTS_MIN_COUNT)
      {
         for (uint8_t pos{}; const auto& token : tokenizer)
         {
            if (pos == 0)
            {
               ++pos;
               continue;
            }
            CanNode* node = this->CreateAndAddNode();
            node->SetName(token.c_str());
         }
      }
      else
      {
         rV = false;
         this->log += "Invalid node definition elements count [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      }
   }
   else
   {
      this->log += "Node definition header invalid [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      rV = false;
   }

   return rV;
}

bool CanBusConfig::ParseValueTableDefinition(std::ifstream& file, LineData_t& lineData)
{
   // locals
   bool rV{ true };

   ICanMessage* message { nullptr }; // only if signal is value owner
   CanValueTable* valueTable { nullptr };
   uint32_t value { (uint32_t)-1 };

   if (line.starts_with(CanBusConfig::VALUE_TABLE_DEFINITION_HEADER))
   {
      boost_escaped_separator sep("\\", " ;", "\"");
      boost_escaped_separator_tokenizer tokenizer{ line, sep }; // remains empty tokens
      std::vector<std::string> tokens;

      // prepare value table definition tokens
      std::invoke([&tokenizer, &tokens, this]
      {
         for (const auto& token : tokenizer)
         {
            if (token != "")
            {
               tokens.push_back(token);
            }
         }
      });

      // If everything's okay
      if (ranges::distance(tokens) >= VALUE_TABLE_DEFINITION_ELEMENTS_MIN_COUNT)
      {
         for (uint8_t pos{}; const auto& token : tokens)
         {
            switch (pos)
            {
               case VALUE_TABLE_DEFINITION_HEADER_POS:
               {
                  // do nothing
                  break;
               }
               case VALUE_TABLE_ENV_VAR_NAME_POS:
               {
                  try
                  {
                     message = this->GetMessageById(std::stoul(token));
                     if (message == nullptr)
                     {
                        rV = false;
                        this->log += "Not found message [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                     }
                  }
                  catch (...)
                  {
                     if (CanEnvVar* envVar = dynamic_cast<CanEnvVar*>(this->GetEnvVarByName(token.c_str())); envVar != nullptr)
                     {
                        valueTable = new CanValueTable {};
                        std::string valueTableName = "VtEv_"s + envVar->GetName();
                        valueTable->SetName(valueTableName.c_str());
                        envVar->SetValueTable(valueTable);
                        pos++;
                     }
                     else
                     {
                        rV = false;
                        this->log += "Not found value owner [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                     }
                  }
                  break;
               }
               case VALUE_TABLE_SIGNAL_NAME_POS:
               {
                  if (message != nullptr)
                  {
                     if (CanSignal* signal = dynamic_cast<CanSignal*>(message->GetSignalByName(token.c_str())); signal != nullptr)
                     {
                        valueTable = new CanValueTable {};
                        std::string valueTableName = "VtSig_"s + signal->GetName();
                        valueTable->SetName(valueTableName.c_str());
                        signal->SetValueTable(valueTable);
                     }
                     else
                     {
                        rV = false;
                        this->log += "Not found value owner [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                     }
                  }
                  break;
               }
               default:
               {
                  if (valueTable)
                  {
                     // value
                     if (pos % 2 == 1)
                     {
                        try
                        {
                           value = std::stoul(token);
                        }
                        catch (...)
                        {
                           rV = false;
                           this->log += "Value conversion failed, value: " + token;
                           this->log += " [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                        }
                     }
                     // description
                     else
                     {
                        if (value != (uint32_t)-1 && valueTable != nullptr)
                        {
                           valueTable->AddValue(value, token);
                        }
                     }
                  }
                  break;
               }
            }

            // If something went wrong
            if (!rV)
            {
               break;
            }
            ++pos;
         }
      }
      else
      {
         rV = false;
         this->log += "Invalid value table definition elements count [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      }
   }
   else
   {
      this->log += "Value table definition header invalid [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      rV = false;
   }

   return rV;
}

bool CanBusConfig::ParseAttributeDefinition(std::ifstream& file, LineData_t& lineData)
{
   // locals
   bool rV{ true };

   if (line.starts_with(CanBusConfig::ATTRIBUTE_DEFINITION_HEADER))
   {
      boost_char_separator sep(" \",;");
      boost_char_separator_tokenizer tokenizer{ line, sep };
      std::vector<std::string> tokens;
      size_t elementsMinCount {};
      ICanAttribute::IObjectType_e objectType = ICanAttribute::IObjectType_e::UNDEFINED;
      CanAttribute* attribute { nullptr };
      std::string attributeName;

      // If everything's okay
      if (ranges::distance(tokenizer) >= ATTRIBUTE_DEFINITION_ELEMENTS_MIN_COUNT)
      {
         for (auto& token : tokenizer)
         {
            tokens.push_back(token);
         }
         if (const std::string objectTypeToken = tokens[ATTRIBUTE_OBJECT_TYPE_POS]; objectTypeToken == DBC_KEYWORD_NETWORK_NODE)
         {
            elementsMinCount = NODE_ATTRIBUTE_DEFINITION_ELEMENTS_MIN_COUNT;
            objectType = ICanAttribute::IObjectType_e::NODE;
         }
         else if (objectTypeToken == DBC_KEYWORD_MESSAGE)
         {
            elementsMinCount = MESSAGE_ATTRIBUTE_DEFINITION_ELEMENTS_MIN_COUNT;
            objectType = ICanAttribute::IObjectType_e::MESSAGE;
         }
         else if (objectTypeToken == DBC_KEYWORD_SIGNAL)
         {
            elementsMinCount = SIGNAL_ATTRIBUTE_DEFINITION_ELEMENTS_MIN_COUNT;
            objectType = ICanAttribute::IObjectType_e::SIGNAL;
         }
         else if (objectTypeToken == DBC_KEYWORD_ENVIRONMENT_VARIABLE)
         {
            elementsMinCount = ENVIRONMENT_VARIABLE_ATTRIBUTE_DEFINITION_ELEMENTS_MIN_COUNT;
            objectType = ICanAttribute::IObjectType_e::ENVIRONMENT_VARIABLE;
         }
         else
         {
            elementsMinCount = NETWORK_ATTRIBUTE_DEFINITION_ELEMENTS_MIN_COUNT;
            objectType = ICanAttribute::IObjectType_e::NETWORK;
         }

         if (elementsMinCount >= elementsMinCount)
         {
            for (size_t pos{}; const auto& token : tokens)
            {
               // if network object type
               if (pos == ATTRIBUTE_OBJECT_TYPE_POS && objectType == ICanAttribute::IObjectType_e::NETWORK)
               {
                  ++pos;
               }

               switch (pos)
               {
                  case ATTRIBUTE_DEFINITION_HEADER_POS:
                  {
                     break;
                  }
                  case ATTRIBUTE_OBJECT_TYPE_POS:
                  {
                     break;
                  }
                  case ATTRIBUTE_NAME_POS:
                  {
                     attributeName = token;
                     break;
                  }
                  case ATTRIBUTE_VALUE_TYPE_POS:
                  {
                     const uint8_t valueTypePos = std::invoke([&objectType]() -> uint8_t
                     {
                        if (objectType == ICanAttribute::IObjectType_e::NETWORK)
                        {
                           return ATTRIBUTE_VALUE_TYPE_POS - 1;
                        }
                        return ATTRIBUTE_VALUE_TYPE_POS;
                     });
                     if (token == ATTRIBUTE_INTEGER)
                     {
                        rV = this->ParseAttributeIntParams(helpers::make_span(tokens.begin() + valueTypePos + 1, tokens.end()), attribute, lineData);
                     }
                     else if (token == ATTRIBUTE_HEXADECIMAL)
                     {
                        rV = this->ParseAttributeHexParams(helpers::make_span(tokens.begin() + valueTypePos + 1, tokens.end()), attribute, lineData);
                     }
                     else if (token == ATTRIBUTE_FLOAT)
                     {
                        rV = this->ParseAttributeFloatParams(helpers::make_span(tokens.begin() + valueTypePos + 1, tokens.end()), attribute, lineData);
                     }
                     else if (token == ATTRIBUTE_STRING)
                     {
                        if (ranges::distance(tokens) == elementsMinCount)
                        {
                           rV = this->ParseAttributeStringParams(attribute, lineData);
                        }
                        else
                        {
                           this->log += "Invalid string attribute params count [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                           rV = false;
                        }
                     }
                     else if (token == ATTRIBUTE_ENUM)
                     {
                        rV = this->ParseAttributeEnumParams(helpers::make_span(tokens.begin() + valueTypePos + 1, tokens.end()), attribute, lineData);
                     }
                     else
                     {
                        this->log += "Invalid attribute definition value type: " + token;
                        this->log += " [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                        rV = false;
                     }

                     if (rV && attribute != nullptr)
                     {
                        attribute->SetObjectType(objectType);
                        attribute->SetName(attributeName.c_str());
                        this->AddAttribute(attribute);
                     }
                     break;
                  }
                  default:
                  {
                     break;
                  }
               }

               // If something went wrong
               if (!rV)
               {
                  break;
               }
               ++pos;
            }
         }
         else
         {
            rV = false;
            this->log += "Invalid attribute definition elements count [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
         }
      }
      else
      {
         rV = false;
         this->log += "Invalid attribute definition elements count [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      }
   }
   else
   {
      this->log += "Attribute definition header invalid [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      rV = false;
   }

   return rV;
}

bool CanBusConfig::ParseAttributeDefaultDefinition(std::ifstream& file, LineData_t& lineData)
{
   bool rV { true };

   if (line.starts_with(CanBusConfig::ATTRIBUTE_DEFAULT_DEFINITION_HEADER))
   {
      boost_char_separator sep { " \";" };
      boost_char_separator_tokenizer tokenizer { line, sep };
      CanAttribute* attribute { nullptr };
      std::vector<std::string> tokens;
      
      // prepare attribute default definition tokens
      for (const auto& token : tokenizer)
      {
         tokens.push_back(token);
      }
      if (tokens.size() == 2)
      {
         tokens.push_back(std::string{""});
      }

      // If everything's okay
      if (ranges::distance(tokens) == ATTRIBUTE_DEFAULT_DEFINITION_ELEMENTS_COUNT)
      {
         for (size_t pos{}; const auto& token : tokens)
         {
            switch (pos)
            {
               case ATTRIBUTE_DEFAULT_DEFINITION_HEADER_POS:
               {
                  break;
               }
               case ATTRIBUTE_DEFAULT_NAME_POS:
               {
                  if (attribute = dynamic_cast<CanAttribute*>(this->GetAttributeByName(token.c_str())); attribute == nullptr)
                  {
                     rV = false;
                     this->log += "Not found attribute with this name [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                  }
                  break;
               }
               case ATTRIBUTE_DEFAULT_VALUE_POS:
               {
                  if (attribute)
                  {
                     CanAttributeManager::SetDefaultValue(attribute, token);
                  }
                  break;
               }
               default:
               {
                  break;
               }
            }

            // If something went wrong
            if (!rV)
            {
               break;
            }
            ++pos;
         }
      }
      else
      {
         rV = false;
         this->log += "Invalid attribute default definition elements count [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      }
   }
   else
   {
      this->log += "Attribute default definition header invalid [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      rV = false;
   }

   return rV;
}

bool CanBusConfig::ParseAttributeValueDefinition(std::ifstream& file, LineData_t& lineData)
{
   // locals
   bool rV { true };

   if (line.starts_with(CanBusConfig::ATTRIBUTE_VALUE_DEFINITION_HEADER))
   {
      boost_escaped_separator sep("\\", " ;", "\"");
      boost_escaped_separator_tokenizer tokenizer{ line, sep }; // remains empty tokens
      std::vector<std::string> tokens;
      size_t elementsMinCount {};
      CanAttribute* attribute { nullptr };
      ICanAttribute::IObjectType_e objectType { ICanAttribute::IObjectType_e::UNDEFINED };
      CanAttributeOwner* attributeOwner { nullptr };
      uint32_t messageId {};

      // prepare attribute value definition tokens
      std::invoke([&tokenizer, &tokens, this]
      {
         for (const auto& token : tokenizer)
         {
            if (token != "")
            {
               tokens.push_back(token);
            }
         }
      });

      // If everything's okay
      if (ranges::distance(tokens) >= ATTRIBUTE_VALUE_DEFINITION_ELEMENTS_MIN_COUNT)
      {
         attribute = dynamic_cast<CanAttribute*>(this->GetAttributeByName(tokens[ATTRIBUTE_VALUE_NAME_POS].c_str()));
         if (attribute)
         {
            if (objectType = attribute->GetObjectType(); objectType == ICanAttribute::IObjectType_e::NETWORK)
            {
               elementsMinCount = NETWORK_ATTRIBUTE_VALUE_DEFINITION_ELEMENTS_MIN_COUNT;
            }
            else if (objectType == ICanAttribute::IObjectType_e::NODE)
            {
               elementsMinCount = NODE_ATTRIBUTE_VALUE_DEFINITION_ELEMENTS_MIN_COUNT;
            }
            else if (objectType == ICanAttribute::IObjectType_e::MESSAGE)
            {
               elementsMinCount = MESSAGE_ATTRIBUTE_VALUE_DEFINITION_ELEMENTS_MIN_COUNT;
            }
            else if (objectType == ICanAttribute::IObjectType_e::SIGNAL)
            {
               elementsMinCount = SIGNAL_ATTRIBUTE_VALUE_DEFINITION_ELEMENTS_MIN_COUNT;
            }
            else if (objectType == ICanAttribute::IObjectType_e::ENVIRONMENT_VARIABLE)
            {
               elementsMinCount = ENVIRONMENT_VARIABLE_ATTRIBUTE_VALUE_DEFINITION_ELEMENTS_MIN_COUNT;
            }
            else
            {
               rV = false;
               this->log += "Wrong attribute's object type [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
            }

            if (rV && ranges::distance(tokens) == elementsMinCount - 1)
            {
               tokens.push_back(std::string{""});
            }

            if (rV && ranges::distance(tokens) == elementsMinCount)
            {
               for (size_t pos{}; const auto & token : tokens)
               {
                  if (pos == ATTRIBUTE_VALUE_OBJECT_TYPE_POS && objectType == ICanAttribute::IObjectType_e::NETWORK)
                  {
                     pos = ATTRIBUTE_VALUE_POS;
                     attributeOwner = this;
                  }

                  switch (pos)
                  {
                     case ATTRIBUTE_VALUE_DEFINITION_HEADER_POS:
                     {
                        break;
                     }
                     case ATTRIBUTE_VALUE_NAME_POS:
                     {
                        break;
                     }
                     case ATTRIBUTE_VALUE_OBJECT_TYPE_POS:
                     {
                        if (token != DBC_KEYWORD_NETWORK_NODE && objectType == ICanAttribute::IObjectType_e::NODE)
                        {
                           rV = false;
                           this->log += "Invalid attribute object type [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                        }
                        else if (token != DBC_KEYWORD_MESSAGE && objectType == ICanAttribute::IObjectType_e::MESSAGE)
                        {
                           rV = false;
                           this->log += "Invalid attribute object type [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                        }
                        else if (token != DBC_KEYWORD_SIGNAL && objectType == ICanAttribute::IObjectType_e::SIGNAL)
                        {
                           rV = false;
                           this->log += "Invalid attribute object type [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                        }
                        else if (token != DBC_KEYWORD_ENVIRONMENT_VARIABLE && objectType == ICanAttribute::IObjectType_e::ENVIRONMENT_VARIABLE)
                        {
                           rV = false;
                           this->log += "Invalid attribute object type [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                        }
                        break;
                     }
                     case ATTRIBUTE_VALUE_MESSAGE_ID_POS:
                     {
                        if (objectType == ICanAttribute::IObjectType_e::MESSAGE)
                        {
                           try
                           {
                              messageId = std::stoul(token);
                              if (auto message = dynamic_cast<CanMessage*>(this->GetMessageById(messageId)); message != nullptr)
                              {
                                 attributeOwner = message;
                                 ++pos;
                              }
                              else
                              {
                                 rV = false;
                                 this->log += "Message not found [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                              }
                           }
                           catch (...)
                           {
                              rV = false;
                              this->log += "Message id conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                           }
                        }
                        else if (objectType == ICanAttribute::IObjectType_e::SIGNAL)
                        {
                           try
                           {
                              messageId = std::stoul(token);
                           }
                           catch (...)
                           {
                              rV = false;
                              this->log += "Message id conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                           }
                        }
                        else if (objectType == ICanAttribute::IObjectType_e::NODE)
                        {
                           if (auto node = dynamic_cast<CanNode*>(this->GetNodeByName(token.c_str())); node != nullptr)
                           {
                              attributeOwner = node;
                              ++pos;
                           }
                           else
                           {
                              rV = false;
                              this->log += "Node not found [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                           }
                        }
                        else if (objectType == ICanAttribute::IObjectType_e::ENVIRONMENT_VARIABLE)
                        {
                           if (auto envVar = dynamic_cast<CanEnvVar*>(this->GetEnvVarByName(token.c_str())); envVar != nullptr)
                           {
                              attributeOwner = envVar;
                              ++pos;
                           }
                           else
                           {
                              rV = false;
                              this->log += "Node not found [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                           }
                        }
                        break;
                     }
                     case ATTRIBUTE_VALUE_SIGNAL_NAME_POS:
                     {
                        if (objectType == ICanAttribute::IObjectType_e::SIGNAL)
                        {
                           if (auto message = dynamic_cast<CanMessage*>(this->GetMessageById(messageId)); message != nullptr)
                           {
                              if (auto signal = dynamic_cast<CanSignal*>(message->GetSignalByName(token.c_str())); signal != nullptr)
                              {
                                 attributeOwner = signal;
                              }
                              else
                              {
                                 rV = false;
                                 this->log += "Signal not found [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                              }
                           }
                           else
                           {
                              rV = false;
                              this->log += "Message not found [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                           }
                        }
                        else
                        {
                           rV = false;
                           this->log += "Wrong algorithm. Check it! [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                        }
                        break;
                     }
                     case ATTRIBUTE_VALUE_POS:
                     {
                        // because for network object type assigning is in BA_DEF_DEF_ section
                        if (objectType != ICanAttribute::IObjectType_e::NETWORK)
                        {
                           attributeOwner->AddAttribute(attribute);
                        }
                        ICanAttributeValue* attributeValue = 
                           ICanAttributeValueFactory::CreateAttributeValue(attribute->GetValueType());
                        if (attributeValue)
                        {
                           CanAttributeManager::SetValue(attributeValue, token);
                           attributeOwner->AddAttributeValue(attribute->GetName(), attributeValue);
                        }
                        break;
                     }
                     default:
                     {
                        break;
                     }
                  }

                  // If something went wrong
                  if (!rV)
                  {
                     break;
                  }
                  ++pos;
               }
            }
            else
            {
               rV = false;
               this->log += "Invalid attribute value definition elements count [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
            }
         }
         else
         {
            rV = false;
            this->log += "Attribute has not found [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
         }
      }
      else
      {
         rV = false;
         this->log += "Invalid attribute value definition elements count [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      }
   }
   else
   {
      this->log += "Attribute value definition header invalid [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      rV = false;
   }

   return rV;
}

bool CanBusConfig::ParseAttributeIntParams(std::span<std::string> paramTokens, CanAttribute*& attribute, LineData_t& lineData)
{
   // locals
   bool rV { true };

   if (ranges::distance(paramTokens) == ATTRIBUTE_INT_PARAMS_COUNT)
   {
      attribute = new CanIntAttribute{};
      attribute->SetValueType(ICanAttribute::IValueType_e::INT);

      if (CanIntAttribute* intAttribute = dynamic_cast<CanIntAttribute*>(attribute); intAttribute != nullptr)
      {
         try
         {
            intAttribute->SetMinimum(std::stoi(paramTokens[0]));
            intAttribute->SetMaximum(std::stoi(paramTokens[1]));
         }
         catch (...)
         {
            rV = false;
            this->log += "Invalid int attribute params [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
         }
      }
   }
   else
   {
      this->log += "Invalid int attribute params count [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      rV = false;
   }

   return rV;
}

bool CanBusConfig::ParseAttributeHexParams(std::span<std::string> paramTokens, CanAttribute*& attribute, LineData_t& lineData)
{
   // locals
   bool rV{ true };

   if (ranges::distance(paramTokens) == ATTRIBUTE_HEX_PARAMS_COUNT)
   {
      attribute = new CanHexAttribute{};
      attribute->SetValueType(ICanAttribute::IValueType_e::HEX);

      if (CanHexAttribute* hexAttribute = dynamic_cast<CanHexAttribute*>(attribute); hexAttribute != nullptr)
      {
         try
         {
            hexAttribute->SetMinimum(std::stoi(paramTokens[0]));
            hexAttribute->SetMaximum(std::stoi(paramTokens[1]));
         }
         catch (...)
         {
            rV = false;
            this->log += "Invalid hex attribute params [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
         }
      }
   }
   else
   {
      this->log += "Invalid hex attribute params count [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      rV = false;
   }

   return rV;
}

bool CanBusConfig::ParseAttributeFloatParams(std::span<std::string> paramTokens, CanAttribute*& attribute, LineData_t& lineData)
{
   // locals
   bool rV{ true };

   if (ranges::distance(paramTokens) == ATTRIBUTE_FLOAT_PARAMS_COUNT)
   {
      attribute = new CanFloatAttribute {};
      attribute->SetValueType(ICanAttribute::IValueType_e::FLOAT);

      if (CanFloatAttribute* floatAttribute = dynamic_cast<CanFloatAttribute*>(attribute); floatAttribute != nullptr)
      {
         try
         {
            floatAttribute->SetMinimum(std::stod(paramTokens[0]));
            floatAttribute->SetMaximum(std::stod(paramTokens[1]));
         }
         catch (...)
         {
            rV = false;
            this->log += "Invalid float attribute params [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
         }
      }
   }
   else
   {
      this->log += "Invalid float attribute params count [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      rV = false;
   }

   return rV;
}

bool CanBusConfig::ParseAttributeStringParams(CanAttribute*& attribute, LineData_t& lineData)
{
   // locals
   bool rV{ true };

   attribute = new CanStringAttribute{};
   attribute->SetValueType(ICanAttribute::IValueType_e::STRING);

   return rV;
}

bool CanBusConfig::ParseAttributeEnumParams(std::span<std::string> paramTokens, CanAttribute*& attribute, LineData_t& lineData)
{
   // locals
   bool rV{ true };

   if (ranges::distance(paramTokens) >= ATTRIBUTE_ENUM_PARAMS_MIN_COUNT)
   {
      attribute = new CanEnumAttribute{};
      attribute->SetValueType(ICanAttribute::IValueType_e::ENUM);


      if (CanEnumAttribute* enumAttribute = dynamic_cast<CanEnumAttribute*>(attribute); enumAttribute != nullptr)
      {
         for (const auto& enumarator : paramTokens)
         {
            enumAttribute->AddEnumarator(enumarator.c_str());
         }
      }
   }
   else
   {
      this->log += "Invalid enum attribute enumarators count [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      rV = false;
   }

   return rV;
}