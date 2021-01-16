#include "CanBusConfig.h"
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>
#include "helpers.h"
#include "CanAttributeManager.h"
#include "ICanAttributeManager.h"
#include "CanIntAttribute.h"
#include "CanHexAttribute.h"
#include "CanFloatAttribute.h"
#include "CanStringAttribute.h"
#include "CanEnumAttribute.h"
#include "CanAttributeValueFactory.h"
#include "CanEnvVarFactory.h"
#include "CanIntEnvVar.h"
#include "CanFloatEnvVar.h"
#include "CanStringEnvVar.h"
#include "CanDataEnvVar.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include "CanIntAttributeValue.h";
#include "CanHexAttributeValue.h";
#include "CanFloatAttributeValue.h";
#include "CanStringAttributeValue.h";
#include "CanEnumAttributeValue.h";
#include <variant>

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
   this->fileName.clear();
   this->log.clear();
   helpers::ClearContainer(this->networks);
}

const char* CanBusConfig::GetLog(void) const
{
   return this->log.c_str();
}

bool CanBusConfig::Load(const char* fileName)
{
   // locals
   bool rV { false };
   LineData_t lineData;
   std::ifstream file { fileName };
   this->fileName = fileName;

   if (file.is_open())
   {
      rV = true;
      const auto networkName = helpers::RemovePhrases(fileName, ".dbc");
      auto network = this->CreateAndAddNetwork();
      network->SetName(networkName.c_str());

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
         else if (line.starts_with(CanBusConfig::ENVIRONMENT_VARIABLE_DEFINITION_HEADER))
         {
            this->ParseEnvironmentVariableDefinition(file, lineData);
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
         else if (line.starts_with(CanBusConfig::ENVIRONMENT_VARIABLE_DATA_DEFINITION_HEADER))
         {
            this->ParseEnvironmentVariableDataDefinition(file, lineData);
         }
         else if (line.starts_with(CanBusConfig::COMMENT_DEFINITION_HEADER))
         {
            this->ParseCommentDefinition(file, lineData);
         }
         lineNr++;
      }
   }

   if (rV)
   {
      this->SetMainAttributes();
   }

   return rV;
}

bool CanBusConfig::Save(void) const
{
   return this->Export(this->fileName.c_str());
}

bool CanBusConfig::Export(const char* fileName) const
{
   // locals
   bool rV { false };
   std::string lineStr;
   std::ofstream file { fileName };

   if (file.is_open())
   {
      rV = true;

      this->WriteFileHeader(lineStr); file << lineStr; lineStr.clear();
      this->WriteNodeDefinition(lineStr); file << lineStr; lineStr.clear();
      this->WriteMessageDefinition(lineStr); file << lineStr; lineStr.clear();
      this->WriteEnvironmentVariableDefinition(lineStr); file << lineStr; lineStr.clear();
      this->WriteEnvironmentVariableDataDefinition(lineStr); file << lineStr; lineStr.clear();
      this->WriteCommentDefinition(lineStr); file << lineStr; lineStr.clear();
      this->WriteAttributeDefinition(lineStr); file << lineStr; lineStr.clear();
      this->WriteAttributeDefaultDefinition(lineStr); file << lineStr; lineStr.clear();
      this->WriteAttributeValueDefinition(lineStr); file << lineStr; lineStr.clear();
      this->WriteValueTableDefinition(lineStr); file << lineStr; lineStr.clear();
   }

   return rV;
}

size_t CanBusConfig::GetNetworksCount(void) const
{
   return this->networks.size();
}

ICanNetwork* CanBusConfig::GetNetworkByIndex(size_t index) const
{
   return (index < this->networks.size() ? this->networks[index] : nullptr);
}

ICanNetwork* CanBusConfig::GetNetworkByName(const char* name) const
{
   auto it = ranges::find_if(this->networks, [&name](CanNetwork* network) { return !std::strcmp(network->GetName(), name); });
   return (it != this->networks.end() ? *it : nullptr);
}

ICanNetwork* CanBusConfig::GetNetworkFront(void) const
{
   return (this->networks.size() > 0 ? this->networks[0] : nullptr);
}

ICanNetwork* CanBusConfig::GetNetworkBack(void) const
{
   return (this->networks.size() > 0 ? this->networks[this->networks.size() - 1] : nullptr);
}

void CanBusConfig::AddNetwork(CanNetwork* network)
{
   if (network)
   {
      this->networks.push_back(network);
   }
}

CanNetwork* CanBusConfig::CreateAndAddNetwork(void)
{
   auto network = new CanNetwork {};
   this->networks.push_back(network);
   return network;
}

bool CanBusConfig::ParseMessageDefinition(std::ifstream& file, LineData_t& lineData)
{
   auto network = this->networks.back();
   if (!network) { return false; }

   // locals
   bool rV { true };

   if (line.starts_with(CanBusConfig::MESSAGE_DEFINITION_HEADER))
   {
      boost_char_separator sep { " :" };
      boost_char_separator_tokenizer tokenizer { line, sep };
      
      // If everything's okay
      if (ranges::distance(tokenizer) == MESSAGE_DEFINITION_ELEMENTS_COUNT)
      {
         CanMessage* message = network->CreateAndAddMessage();
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
                  if (CanNode* node = dynamic_cast<CanNode*>(network->GetNodeByName(token.c_str())); node != nullptr)
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
   auto network = this->networks.back();
   if (!network) { return false; }
   CanMessage* message = dynamic_cast<CanMessage*>(network->GetMessageBack());
   if (!message) { return false; }

   // locals
   bool rV { true };

   if (line.starts_with(CanBusConfig::SIGNAL_DEFINITION_HEADER))
   {
      boost_escaped_separator sep("\\", " :|@(,)[]", "\"");
      boost_escaped_separator_tokenizer tokenizer { line, sep }; // remains empty tokens
      std::vector<std::string> tokens;

      // prepare signal definition tokens
      std::invoke([&tokenizer, &tokens, network]
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
                  (network->GetNodeByName(token.c_str()) != nullptr || token == ICanNode::PSEUDO_NODE_NAME)) // if "" - for unit element
               {
                  tokens.push_back("");
                  tokens.push_back(token);
                  pos += 2;
               }
               else if (token.size() == 2 && (token.ends_with("+") || token.ends_with("-"))) // split byte order and value type
               {
                  tokens.push_back(std::string{ token[0] });
                  tokens.push_back(std::string{ token[1] });
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
         CanSignal* signal = network->CreateAndAddSignal();
         message->AddSignal(signal);
         signal->SetMessage(message);
         if (CanNode* transmitterNode = dynamic_cast<CanNode*>(network->GetNodeByName(message->GetMainTransmitter())); transmitterNode)
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
                  if (CanNode* node = dynamic_cast<CanNode*>(network->GetNodeByName(token.c_str())); node != nullptr)
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
   auto network = this->networks.back();
   if (!network) { return false; }

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
            CanNode* node = network->CreateAndAddNode();
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

bool CanBusConfig::ParseEnvironmentVariableDefinition(std::ifstream& file, LineData_t& lineData)
{
   auto network = this->networks.back();
   if (!network) { return false; }

   // locals
   bool rV { true };

   if (line.starts_with(CanBusConfig::ENVIRONMENT_VARIABLE_DEFINITION_HEADER))
   {
      boost_escaped_separator sep("\\", " :[|];", "\"");
      boost_escaped_separator_tokenizer tokenizer{ line, sep }; // remains empty tokens
      std::vector<std::string> tokens;
      CanEnvVar* envVar { nullptr };
      std::string envVarName;

      // prepare environment variable definition tokens
      std::invoke([&tokenizer, &tokens, this]
      {
         for (size_t pos{}; const auto& token : tokenizer)
         {
            if (pos == ENVIRONMENT_VARIABLE_UNIT_POS)
            {
               try
               {
                  std::stod(token);
                  tokens.push_back(std::string{ "" });
                  ++pos;
               }
               catch (...)
               {
                  ;
               }
            }
            if (token != "")
            {
               tokens.push_back(token);
               ++pos;
            }
         }
      });

      // If everything's okay
      const auto elementsCount = ranges::distance(tokens);
      if (elementsCount == ENVIRONMENT_VARIABLE_DEFINITION_ELEMENTS_COUNT)
      {
         for (uint8_t pos{}; const auto& token : tokens)
         {
            switch (pos)
            {
               case ENVIRONMENT_VARIABLE_DEFINITION_HEADER_POS:
               {
                  // do nothing
                  break;
               }
               case ENVIRONMENT_VARIABLE_NAME_POS:
               {
                  envVarName = token;
                  break;
               }
               case ENVIRONMENT_VARIABLE_TYPE_POS:
               {
                  try
                  {
                     uint32_t type = std::stoul(token);
                     if (envVar = CanEnvVarFactory::CreateEnvVar(type); !envVar)
                     {
                        rV = false;
                        this->log += "Invalid environment variable type [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                     }
                     else
                     {
                        envVar->SetName(envVarName.c_str());
                        network->AddEnvVar(envVar);
                     }
                  }
                  catch (...)
                  {
                     rV = false;
                     this->log += "Environment variable type conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                  }
                  break;
               }
               case ENVIRONMENT_VARIABLE_MINIMUM_POS:
               {
                  helpers::typecase(envVar,
                     [&token, &rV, this, &lineData](CanIntEnvVar* intEnvVar)
                     {
                        try
                        {
                           uint32_t minimum = std::stoul(token);
                           intEnvVar->SetMinimum(minimum);
                        }
                        catch (...)
                        {
                           rV = false;
                           this->log += "Environment variable minimum conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                        }
                     },
                     [&token, &rV, this, &lineData](CanFloatEnvVar* floatEnvVar)
                     {
                        try
                        {
                           double minimum = std::stod(token);
                           floatEnvVar->SetMinimum(minimum);
                        }
                        catch (...)
                        {
                           rV = false;
                           this->log += "Environment variable minimum conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                        }
                     });
                  break;
               }
               case ENVIRONMENT_VARIABLE_MAXIMUM_POS:
               {
                  helpers::typecase(envVar,
                     [&token, &rV, this, &lineData] (CanIntEnvVar* intEnvVar)
                     {
                        try
                        {
                           uint32_t maximum = std::stoul(token);
                           intEnvVar->SetMaximum(maximum);
                        }
                        catch (...)
                        {
                           rV = false;
                           this->log += "Environment variable maximum conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                        }
                     },
                     [&token, &rV, this, &lineData] (CanFloatEnvVar* floatEnvVar)
                     {
                        try
                        {
                           double maximum = std::stod(token);
                           floatEnvVar->SetMaximum(maximum);
                        }
                        catch (...)
                        {
                           rV = false;
                           this->log += "Environment variable maximum conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                        }
                     });
                  break;
               }
               case ENVIRONMENT_VARIABLE_UNIT_POS:
               {
                  envVar->SetUnit(token.c_str());
                  break;
               }
               case ENVIRONMENT_VARIABLE_INITIAL_VALUE_POS:
               {
                  helpers::typecase(envVar,
                     [&token, &rV, this, &lineData](CanIntEnvVar* intEnvVar)
                     {
                        try
                        {
                           uint32_t initialValue = std::stoul(token);
                           intEnvVar->SetInitialValue(initialValue);
                        }
                        catch (...)
                        {
                           rV = false;
                           this->log += "Environment variable initial value conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                        }
                     },
                     [&token, &rV, this, &lineData](CanFloatEnvVar* floatEnvVar)
                     {
                        try
                        {
                           double initialValue = std::stod(token);
                           floatEnvVar->SetInitialValue(initialValue);
                        }
                        catch (...)
                        {
                           rV = false;
                           this->log += "Environment variable initial value conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                        }
                     });
                  break;
               }
               case ENVIRONMENT_VARIABLE_ID_POS:
               {
                  try
                  {
                     uint32_t id = std::stoul(token);
                     envVar->SetId(id);
                  }
                  catch (...)
                  {
                     rV = false;
                     this->log += "Environment variable id conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                  }
                  break;
               }
               case ENVIRONMENT_VARIABLE_ACCESS_TYPE_POS:
               {
                  const auto accessTypeStr = helpers::RemovePhrases(token, ICanEnvVar::ACCESS_TYPE_PREFIX);
                  try
                  {
                     uint32_t accessTypeValue = std::stoul(accessTypeStr, nullptr, 16);
                     envVar->SetAccessTypeValue(accessTypeValue);
                  }
                  catch (...)
                  {
                     this->log += "Environment variable access type conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                  }
                  break;
               }
               default:
               {
                  if (ICanNode* accessNode = network->GetNodeByName(token.c_str()); accessNode)
                  {
                     envVar->AddAccessNode(dynamic_cast<CanNode*>(accessNode));
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
         this->log += "Invalid environment variable definition elements count: " + std::to_string(elementsCount);
         this->log += " [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      }
   }
   else
   {
      this->log += "Environment variable definition header invalid [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      rV = false;
   }

   return rV;
}

bool CanBusConfig::ParseValueTableDefinition(std::ifstream& file, LineData_t& lineData)
{
   auto network = this->networks.back();
   if (!network) { return false; }

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
                     message = network->GetMessageById(std::stoul(token));
                     if (message == nullptr)
                     {
                        rV = false;
                        this->log += "Not found message [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                     }
                  }
                  catch (...)
                  {
                     if (CanEnvVar* envVar = dynamic_cast<CanEnvVar*>(network->GetEnvVarByName(token.c_str())); envVar != nullptr)
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
   auto network = this->networks.back();
   if (!network) { return false; }

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
                        network->AddAttribute(attribute);
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
   auto network = this->networks.back();
   if (!network) { return false; }

   // locals
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
                  if (attribute = dynamic_cast<CanAttribute*>(network->GetAttributeByName(token.c_str())); attribute == nullptr)
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
   auto network = this->networks.back();
   if (!network) { return false; }

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
         attribute = dynamic_cast<CanAttribute*>(network->GetAttributeByName(tokens[ATTRIBUTE_VALUE_NAME_POS].c_str()));
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
                     attributeOwner = network;
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
                              if (auto message = dynamic_cast<CanMessage*>(network->GetMessageById(messageId)); message != nullptr)
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
                           if (auto node = dynamic_cast<CanNode*>(network->GetNodeByName(token.c_str())); node != nullptr)
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
                           if (auto envVar = dynamic_cast<CanEnvVar*>(network->GetEnvVarByName(token.c_str())); envVar != nullptr)
                           {
                              attributeOwner = envVar;
                              ++pos;
                           }
                           else
                           {
                              rV = false;
                              this->log += "Environment variable not found [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                           }
                        }
                        break;
                     }
                     case ATTRIBUTE_VALUE_SIGNAL_NAME_POS:
                     {
                        if (objectType == ICanAttribute::IObjectType_e::SIGNAL)
                        {
                           if (auto message = dynamic_cast<CanMessage*>(network->GetMessageById(messageId)); message != nullptr)
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
                        CanAttributeValue* attributeValue = 
                           CanAttributeValueFactory::CreateAttributeValue(attribute->GetValueType());
                        if (attributeValue)
                        {
                           std::string value = token;

                           // exception to the rule, so handle it
                           if (attribute->GetValueType() == ICanAttribute::IValueType_e::ENUM)
                           {
                              uint32_t potentialEnumaratorIndex {};
                              try
                              {
                                 potentialEnumaratorIndex = std::stoul(token);
                                 const auto enumAttribute = dynamic_cast<CanEnumAttribute*>(attribute);
                                 if (enumAttribute)
                                 {
                                    const char* enumarator = enumAttribute->GetEnumarator(potentialEnumaratorIndex);
                                    if (enumarator != nullptr)
                                    {
                                       value = enumarator;
                                    }
                                 }
                              }
                              catch (...)
                              {
                                 // token is not enumarator index
                              }
                           }
                           // end of exception to the rule

                           CanAttributeManager::SetValue(attributeValue, value);
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

bool CanBusConfig::ParseEnvironmentVariableDataDefinition(std::ifstream& file, LineData_t& lineData)
{
   auto network = this->networks.back();
   if (!network) { return false; }

   // locals
   bool rV { true };

   if (line.starts_with(CanBusConfig::ENVIRONMENT_VARIABLE_DATA_DEFINITION_HEADER))
   {
      boost_char_separator sep(" :;");
      boost_char_separator_tokenizer tokenizer { line, sep };
      CanDataEnvVar* dataEnvVar { nullptr };

      // If everything's okay
      if (ranges::distance(tokenizer) == ENVIRONMENT_VARIABLE_DATA_DEFINITION_ELEMENTS_COUNT)
      {
         for (size_t pos{}; const auto& token : tokenizer)
         {
            switch (pos)
            {
               case ENVIRONMENT_VARIABLE_DATA_DEFINITION_HEADER_POS:
               {
                  break;
               }
               case ENVIRONMENT_VARIABLE_NAME_POS:
               {
                  network->RemoveEnvVarByName(token.c_str());
                  dataEnvVar = new CanDataEnvVar {};
                  dataEnvVar->SetName(token.c_str());
                  network->AddEnvVar(dataEnvVar);
                  break;
               }
               case ENVIRONMENT_VARIABLE_DATA_SIZE_POS:
               {
                  if (dataEnvVar)
                  {
                     try
                     {
                        uint32_t dataSize = std::stoul(token);
                        dataEnvVar->SetLength(dataSize);
                     }
                     catch (...)
                     {
                        this->log += "Environment variable data size conversion failed [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                     }
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
         this->log += "Invalid environment variable data definition elements count [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      }
   }
   else
   {
      this->log += "Environment variable data definition header invalid [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      rV = false;
   }

   return rV;
}

bool CanBusConfig::ParseCommentDefinition(std::ifstream& file, LineData_t& lineData)
{
   auto network = this->networks.back();
   if (!network) { return false; }

   // locals
   bool rV { true };

   if (line.starts_with(CanBusConfig::COMMENT_DEFINITION_HEADER))
   {
      boost_char_separator sep(" ");
      boost_char_separator_tokenizer tokenizer { line, sep };

      CanAttributeOwner* commentOwner { nullptr };
      uint32_t messageId {};
      size_t elementsMinCount {};
      ICanAttribute::IObjectType_e objectType { ICanAttribute::IObjectType_e::UNDEFINED };
      bool endingQuote { false };
      std::string comment;
      std::vector<std::string> tokens;

      // prepare comment definition tokens
      std::invoke([&tokenizer, &tokens, this]
      {
         bool commentStart { false };

         for (const auto& token : tokenizer)
         {
            if (token != "")
            {
               if (token.starts_with("\"") && !commentStart)
               {
                  tokens.push_back(token);
                  commentStart = true;
               }
               else
               {
                  if (commentStart)
                  {
                     tokens[tokens.size() - 1] = tokens[tokens.size() - 1] + token;
                  }
                  else
                  {
                     tokens.push_back(token);
                  }
               }
            }
         }
      });

      // If everything's okay
      if (ranges::distance(tokens) >= COMMENT_DEFINITION_ELEMENTS_MIN_COUNT)
      {
         const auto commentObjectType = *std::next(tokens.begin(), COMMENT_OBJECT_TYPE_POS);
         if (commentObjectType == DBC_KEYWORD_NETWORK_NODE)
         {
            elementsMinCount = NODE_COMMENT_DEFINITION_ELEMENTS_MIN_COUNT;
            objectType = ICanAttribute::IObjectType_e::NODE;
         }
         else if (commentObjectType == DBC_KEYWORD_MESSAGE)
         {
            elementsMinCount = MESSAGE_COMMENT_DEFINITION_ELEMENTS_MIN_COUNT;
            objectType = ICanAttribute::IObjectType_e::MESSAGE;
         }
         else if (commentObjectType == DBC_KEYWORD_SIGNAL)
         {
            elementsMinCount = SIGNAL_COMMENT_DEFINITION_ELEMENTS_MIN_COUNT;
            objectType = ICanAttribute::IObjectType_e::SIGNAL;
         }
         else if (commentObjectType == DBC_KEYWORD_ENVIRONMENT_VARIABLE)
         {
            elementsMinCount = ENVIRONMENT_VARIABLE_COMMENT_DEFINITION_ELEMENTS_MIN_COUNT;
            objectType = ICanAttribute::IObjectType_e::ENVIRONMENT_VARIABLE;
         }
         else if (commentObjectType.starts_with("\""))
         {
            elementsMinCount = NETWORK_COMMENT_DEFINITION_ELEMENTS_MIN_COUNT;
            objectType = ICanAttribute::IObjectType_e::NETWORK;
         }
         else
         {
            rV = false;
            this->log += "Wrong comment object type [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
         }

         if (rV && ranges::distance(tokens) == elementsMinCount)
         {
            for (size_t pos{}; const auto& token : tokens)
            {
               if (pos == COMMENT_OBJECT_TYPE_POS && objectType == ICanAttribute::IObjectType_e::NETWORK)
               {
                  pos = COMMENT_DATA_POS;
                  commentOwner = network;
               }

               switch (pos)
               {
                  case COMMENT_DEFINITION_HEADER_POS:
                  {
                     break;
                  }
                  case COMMENT_OBJECT_TYPE_POS:
                  {
                     break;
                  }
                  case COMMENT_OBJECT_TYPE_NAME_POS:
                  {
                     switch (objectType)
                     {
                        case ICanAttribute::IObjectType_e::NODE:
                        {
                           ICanNode* node = network->GetNodeByName(token.c_str());
                           if (node)
                           {
                              commentOwner = dynamic_cast<CanNode*>(node);
                              ++pos;
                           }
                           else
                           {
                              rV = false;
                              this->log += "Node not found [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                           }
                           break;
                        }
                        case ICanAttribute::IObjectType_e::MESSAGE:
                        {
                           try
                           {
                              messageId = std::stoul(token);
                              if (auto message = dynamic_cast<CanMessage*>(network->GetMessageById(messageId)); message != nullptr)
                              {
                                 commentOwner = message;
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
                           break;
                        }
                        case ICanAttribute::IObjectType_e::SIGNAL:
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
                           break;
                        }
                        case ICanAttribute::IObjectType_e::ENVIRONMENT_VARIABLE:
                        {
                           ICanEnvVar* envVar = network->GetEnvVarByName(token.c_str());
                           if (envVar)
                           {
                              commentOwner = dynamic_cast<CanEnvVar*>(envVar);
                              ++pos;
                           }
                           else
                           {
                              rV = false;
                              this->log += "Environment variable not found [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
                           }
                           break;
                        }
                     }
                     break;
                  }
                  case COMMENT_SIGNAL_NAME_POS:
                  {
                     if (objectType == ICanAttribute::IObjectType_e::SIGNAL)
                     {
                        if (auto message = dynamic_cast<CanMessage*>(network->GetMessageById(messageId)); message != nullptr)
                        {
                           if (auto signal = dynamic_cast<CanSignal*>(message->GetSignalByName(token.c_str())); signal != nullptr)
                           {
                              commentOwner = signal;
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
                  default:
                  {
                     if (token.ends_with(";"))
                     {
                        endingQuote = true;
                     }
                     comment += token;
                  }
               }

               // If something went wrong
               if (!rV)
               {
                  break;
               }
               ++pos;
            }

            while (!endingQuote)
            {
               std::getline(file, line);
               boost::algorithm::trim(line);
               if (line.ends_with(";"))
               {
                  endingQuote = true;
               }
               comment += "\n" + line;
            }

            if (comment.size() > 3)
            {
               comment = comment.substr(1, comment.size() - 3);
            }

            helpers::typecase(commentOwner,
               [&comment] (CanNetwork* network)
               {
                  network->SetComment(comment.c_str());
               },
               [&comment] (CanNode* networkNode)
               {
                  networkNode->SetComment(comment.c_str());
               },
               [&comment] (CanMessage* message)
               {
                  message->SetComment(comment.c_str());
               },
               [&comment] (CanSignal* signal)
               {
                  signal->SetComment(comment.c_str());
               },
               [&comment] (CanEnvVar* envVar)
               {
                  envVar->SetComment(comment.c_str());
               });
         }
         else
         {
            rV = false;
            this->log += "Invalid comment definition elements count [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
         }
      }
      else
      {
         rV = false;
         this->log += "Invalid comment definition elements count [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
      }
   }
   else
   {
      this->log += "Comment definition header invalid [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
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
      attribute = new CanIntAttribute {};

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
      attribute = new CanHexAttribute {};

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

   attribute = new CanStringAttribute {};

   return rV;
}

bool CanBusConfig::ParseAttributeEnumParams(std::span<std::string> paramTokens, CanAttribute*& attribute, LineData_t& lineData)
{
   // locals
   bool rV{ true };

   if (ranges::distance(paramTokens) >= ATTRIBUTE_ENUM_PARAMS_MIN_COUNT)
   {
      attribute = new CanEnumAttribute {};


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

void CanBusConfig::SetMainAttributes(void)
{
   for (auto& network : this->networks)
   {
      if (network)
      {
         for (auto& message : network->GetMessages())
         {
            if (message)
            {
               for (auto& attribute : message->GetAttributes())
               {
                  if (attribute)
                  {
                     std::string_view attributeName = attribute->GetName();
                     if (attributeName == ICanMessage::ID_FORMAT)
                     {
                        auto attributeValue = message->GetAttributeValue(attributeName.data());
                        if (attributeValue)
                        {
                           auto value = ICanAttributeManager::GetAttributeValue<ICanMessage::IdFormat::VALUE_TYPE>
                              (attributeValue);
                           const auto idFormat = CanMessage::ID_FORMATS.at(value);
                           message->SetIdFormat(idFormat);
                        }
                     }
                     else if (attributeName == ICanMessage::TX_METHOD)
                     {
                        auto attributeValue = message->GetAttributeValue(attributeName.data());
                        if (attributeValue)
                        {
                           auto value = ICanAttributeManager::GetAttributeValue<ICanMessage::TxMethod::VALUE_TYPE>
                              (attributeValue);
                           const auto txMethod = CanMessage::TX_METHODS.at(value);
                           message->SetTxMethod(txMethod);
                        }
                     }
                     else if (attributeName == ICanMessage::CYCLE_TIME)
                     {
                        auto attributeValue = message->GetAttributeValue(attributeName.data());
                        if (attributeValue)
                        {
                           auto value = ICanAttributeManager::GetAttributeValue<ICanMessage::CycleTime::VALUE_TYPE>
                              (attributeValue);
                           message->SetCycleTime(value);
                        }
                     }
                  }
               }
            }
         }
         for (auto& signal : network->GetSignals())
         {
            if (signal)
            {
               for (auto& attribute : signal->GetAttributes())
               {
                  if (attribute)
                  {
                     std::string_view attributeName = attribute->GetName();
                     if (attributeName == ICanSignal::RAW_INITIAL_VALUE)
                     {
                        auto attributeValue = signal->GetAttributeValue(attributeName.data());
                        if (attributeValue)
                        {
                           auto value = ICanAttributeManager::GetAttributeValue<ICanAttribute::IValueType_e::INT>
                              (attributeValue);
                           signal->SetRawInitialValue(value);
                        }
                     }
                  }
               }
            }
         }
      }
   }
}

bool CanBusConfig::WriteFileHeader(std::string& lineStr) const
{
   lineStr += R"(VERSION "HNNBNNNYNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN/4/%%%/4/'%**4NNN///")";
   lineStr += "\n\n\n";
   lineStr += "NS_ :\n";
   lineStr += "\tNS_DESC_\n";
   lineStr += "\tCM_\n";
   lineStr += "\tBA_DEF_\n";
   lineStr += "\tBA_\n";
   lineStr += "\tVAL_\n";
   lineStr += "\tCAT_DEF_\n";
   lineStr += "\tCAT_\n";
   lineStr += "\tFILTER\n";
   lineStr += "\tBA_DEF_DEF_\n";
   lineStr += "\tEV_DATA_\n";
   lineStr += "\tENVVAR_DATA_\n";
   lineStr += "\tSGTYPE_\n";
   lineStr += "\tSGTYPE_VAL_\n";
   lineStr += "\tBA_DEF_SGTYPE_\n";
   lineStr += "\tBA_SGTYPE_\n";
   lineStr += "\tSIG_TYPE_REF_\n";
   lineStr += "\tVAL_TABLE_\n";
   lineStr += "\tSIG_GROUP_\n";
   lineStr += "\tSIG_VALTYPE_\n";
   lineStr += "\tSIGTYPE_VALTYPE_\n";
   lineStr += "\tBO_TX_BU_\n";
   lineStr += "\tBA_DEF_REL_\n";
   lineStr += "\tBA_REL_\n";
   lineStr += "\tBA_DEF_DEF_REL_\n";
   lineStr += "\tBU_SG_REL_\n";
   lineStr += "\tBU_EV_REL_\n";
   lineStr += "\tBU_BO_REL_\n";
   lineStr += "\n";
   lineStr += "BS_ :\n";
   lineStr += "\n";

   return false;
}

bool CanBusConfig::WriteMessageDefinition(std::string& lineStr) const
{
   auto network = this->networks.back();
   if (!network) { return false; }

   for (const auto& message : network->GetMessages())
   {
      if (message)
      {
         lineStr += DBC_KEYWORD_MESSAGE.data() + " "s;
         lineStr += std::to_string(message->GetId()) + " ";
         lineStr += message->GetName() + ": "s;
         lineStr += std::to_string(message->GetSize()) + " ";
         lineStr += message->GetMainTransmitter() + "\n"s;

         WriteSignalDefinition(message, lineStr);
      }
   }

   return false;
}

bool CanBusConfig::WriteSignalDefinition(CanMessage* message, std::string& lineStr) const
{
   if (message)
   {
      for (const auto& signal : message->GetSignals())
      {
         lineStr += " "s + DBC_KEYWORD_SIGNAL.data() + " ";
         lineStr += signal->GetName() + " "s;
         lineStr += signal->GetMuxIndicator() + " : "s;
         lineStr += std::to_string(signal->GetStartBit()) + "|"s;
         lineStr += std::to_string(signal->GetSize()) + "@"s;
         lineStr += (char)signal->GetByteOrderSymbol();
         lineStr += (char)signal->GetValueTypeSymbol();
         lineStr += " (" + std::to_string(signal->GetFactor()) + ",";
         lineStr += std::to_string(signal->GetOffset()) + ") ";
         lineStr += "[" + std::to_string(signal->GetMinimum()) + "|";
         lineStr += std::to_string(signal->GetMaximum()) + "] ";
         lineStr += "\""s + signal->GetUnit() + "\" ";
         for (size_t i = 0; i < signal->GetReceiversCount(); i++)
         {
            lineStr += signal->GetReceiver(i) + " "s;
         }
         lineStr += "\n";
      }
      lineStr += "\n";
   }
   
   return false;
}

bool CanBusConfig::WriteNodeDefinition(std::string& lineStr) const
{
   auto network = this->networks.back();
   if (!network) { return false; }

   lineStr += NODE_DEFINITION_HEADER.data();
   for (const auto& node : network->GetNodes())
   {
      if (node)
      {
         lineStr += " "s + node->GetName();
      }
   }
   lineStr += "\n\n";

   return false;
}

bool CanBusConfig::WriteEnvironmentVariableDefinition(std::string& lineStr) const
{
   auto network = this->networks.back();
   if (!network) { return false; }

   for (const auto& envVar : network->GetEnvVars())
   {
      lineStr += ENVIRONMENT_VARIABLE_DEFINITION_HEADER.data();
      lineStr += envVar->GetName() + ": "s;
      auto type = envVar->GetType();
      if (type == ICanEnvVar::Type_e::DATA)
      {
         lineStr += "0";
      }
      else
      {
         lineStr += std::to_string(static_cast<int>(type));
      }

      lineStr += " ";

      std::variant<CanEnvVar*, CanIntEnvVar*, CanFloatEnvVar*, CanStringEnvVar*, CanDataEnvVar*> vEnvVar = envVar;

      std::invoke([&type, &vEnvVar, &envVar]
      {
         switch (type)
         {
            case ICanEnvVar::Type_e::INTEGER:
            {
               vEnvVar = dynamic_cast<CanIntEnvVar*>(envVar);
               break;
            }
            case ICanEnvVar::Type_e::FLOAT:
            {
               vEnvVar = dynamic_cast<CanFloatEnvVar*>(envVar);
               break;
            }
            case ICanEnvVar::Type_e::STRING:
            {
               vEnvVar = dynamic_cast<CanStringEnvVar*>(envVar);
               break;
            }
            case ICanEnvVar::Type_e::DATA:
            {
               vEnvVar = dynamic_cast<CanDataEnvVar*>(envVar);
               break;
            }
         }
      });

      lineStr += "[";

      std::visit([this, &lineStr](auto&& arg)
         {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, CanIntEnvVar*> || std::is_same_v<T, CanFloatEnvVar*>)
            {
               lineStr += std::to_string(arg->GetMinimum()) + "|";
               lineStr += std::to_string(arg->GetMaximum()) + "] ";
               lineStr += "\""s + arg->GetUnit() + "\" ";
               lineStr += std::to_string(arg->GetInitialValue()) + " ";
            }
            else
            {
               lineStr += "0|0] \""s + arg->GetUnit() + "\" 0 ";
            }
         }, vEnvVar);

      lineStr += std::to_string(envVar->GetId()) + " ";

      lineStr += ICanEnvVar::ACCESS_TYPE_PREFIX;

      const auto accessType = static_cast<int>(envVar->GetAccessType());
      if (type == ICanEnvVar::Type_e::STRING)
      {
         lineStr += std::to_string(8000 + accessType);
      }
      else
      {
         lineStr += std::to_string(accessType);
      }

      lineStr += " ";
      size_t accessNodesCount = envVar->GetAccessNodesCount();
      if (accessNodesCount == 0)
      {
         lineStr += ICanNode::PSEUDO_NODE_NAME;
      }

      for (size_t i{}; const auto & accessNode : envVar->GetAccessNodes())
      {
         if (accessNode)
         {
            lineStr += accessNode->GetName();
         }
         if (i < accessNodesCount - 1)
         {
            lineStr += ", ";
         }
      }

      lineStr += ";\n";
   }

   return false;
}

bool CanBusConfig::WriteEnvironmentVariableDataDefinition(std::string& lineStr) const
{
   auto network = this->networks.back();
   if (!network) { return false; }

   for (const auto& envVar : network->GetEnvVars())
   {
      if (envVar)
      {
         if (envVar->GetType() == ICanEnvVar::Type_e::DATA)
         {
            lineStr += ENVIRONMENT_VARIABLE_DATA_DEFINITION_HEADER.data();
            lineStr += envVar->GetName() + ": "s;

            if (const auto dataEnvVar = dynamic_cast<CanDataEnvVar*>(envVar); dataEnvVar)
            {
               lineStr += std::to_string(dataEnvVar->GetLength()) + ";\n";
            }
         }
      }
   }

   return false;
}

bool CanBusConfig::WriteCommentDefinition(std::string& lineStr) const
{
   auto network = this->networks.back();
   if (!network) { return false; }

   const std::string commentHeader = COMMENT_DEFINITION_HEADER.data();
   std::string comment = network->GetComment();
   if (comment != "")
   {
      lineStr += commentHeader + "\"" + comment + "\";\n";
   }

   for (const auto& node : network->GetNodes())
   {
      if (node)
      {
         comment = node->GetComment();
         if (comment != "")
         {
            lineStr += commentHeader;
            lineStr += DBC_KEYWORD_NETWORK_NODE.data() + " "s;
            lineStr += node->GetName() + " "s;
            lineStr += "\"" + comment + "\";\n";
         }
      }
   }

   for (const auto& message : network->GetMessages())
   {
      if (message)
      {
         comment = message->GetComment();
         if (comment != "")
         {
            lineStr += commentHeader;
            lineStr += DBC_KEYWORD_MESSAGE.data() + " "s;
            lineStr += std::to_string(message->GetId()) + " "s;
            lineStr += "\"" + comment + "\";\n";
         }
      }
   }

   for (const auto& signal : network->GetSignals())
   {
      if (signal)
      {
         comment = signal->GetComment();
         const auto message = signal->GetMessage();
         if (comment != "" && message)
         {
            lineStr += commentHeader;
            lineStr += DBC_KEYWORD_SIGNAL.data() + " "s;
            lineStr += std::to_string(message->GetId()) + " "s;
            lineStr += signal->GetName() + " "s;
            lineStr += "\"" + comment + "\";\n";
         }
      }
   }

   for (const auto& envVar : network->GetEnvVars())
   {
      if (envVar)
      {
         comment = envVar->GetComment();
         if (comment != "")
         {
            lineStr += commentHeader;
            lineStr += DBC_KEYWORD_ENVIRONMENT_VARIABLE.data() + " "s;
            lineStr += envVar->GetName() + " "s;
            lineStr += "\"" + comment + "\";\n";
         }
      }
   }

   return false;
}

bool CanBusConfig::WriteAttributeDefinition(std::string& lineStr) const
{
   auto network = this->networks.back();
   if (!network) { return false; }

   for (const auto& attribute : network->GetAttributes())
   {
      if (attribute)
      {
         lineStr += ATTRIBUTE_DEFINITION_HEADER.data();

         switch (attribute->GetObjectType())
         {
            case ICanAttribute::IObjectType_e::NODE:
            {
               lineStr += DBC_KEYWORD_NETWORK_NODE.data();
               break;
            }
            case ICanAttribute::IObjectType_e::MESSAGE:
            {
               lineStr += DBC_KEYWORD_MESSAGE.data();
               break;
            }
            case ICanAttribute::IObjectType_e::SIGNAL:
            {
               lineStr += DBC_KEYWORD_SIGNAL.data();
               break;
            }
            case ICanAttribute::IObjectType_e::ENVIRONMENT_VARIABLE:
            {
               lineStr += DBC_KEYWORD_ENVIRONMENT_VARIABLE.data();
               break;
            }
            default:
            {
               break;
            }
         }

         lineStr += " \""s + attribute->GetName() + "\" ";

         helpers::typecase(attribute,
            [&lineStr] (CanIntAttribute* intAttribute)
            {
               lineStr += ATTRIBUTE_INTEGER.data() + " "s;
               lineStr += std::to_string(intAttribute->GetMinimum()) + " ";
               lineStr += std::to_string(intAttribute->GetMaximum());
            },
            [&lineStr] (CanHexAttribute* hexAttribute)
            {
               lineStr += ATTRIBUTE_HEXADECIMAL.data() + " "s;
               lineStr += std::to_string(hexAttribute->GetMinimum()) + " ";
               lineStr += std::to_string(hexAttribute->GetMaximum());
            },
            [&lineStr] (CanFloatAttribute* floatAttribute)
            {
               lineStr += ATTRIBUTE_FLOAT.data() + " "s;
               lineStr += std::to_string(floatAttribute->GetMinimum()) + " ";
               lineStr += std::to_string(floatAttribute->GetMaximum());
            },
            [&lineStr] (CanStringAttribute* stringAttribute)
            {
               lineStr += ATTRIBUTE_STRING.data();
            },
            [&lineStr](CanEnumAttribute* enumAttribute)
            {
               lineStr += ATTRIBUTE_ENUM.data() + " "s;
            
               const size_t enumaratorsCount = enumAttribute->GetEnumaratorsCount();
               for (size_t i = 0; i < enumaratorsCount; i++)
               {
                  const auto enumarator = enumAttribute->GetEnumarator(i);
                  if (enumarator)
                  {
                     lineStr += "\""s + enumarator + "\"";
                  }
                  if (i < enumaratorsCount - 1)
                  {
                     lineStr += ", ";
                  }
               }
            });

         lineStr += ";\n";
      }
   }

   return false;
}

bool CanBusConfig::WriteAttributeDefaultDefinition(std::string& lineStr) const
{
   auto network = this->networks.back();
   if (!network) { return false; }

   for (const auto& attribute : network->GetAttributes())
   {
      if (attribute)
      {
         lineStr += ATTRIBUTE_DEFAULT_DEFINITION_HEADER.data();

         lineStr += " \""s + attribute->GetName() + "\" ";

         helpers::typecase(attribute,
            [&lineStr] (CanIntAttribute* intAttribute)
            {
               lineStr += std::to_string(intAttribute->GetDefaultValue());
            },
            [&lineStr] (CanHexAttribute* hexAttribute)
            {
               lineStr += std::to_string(hexAttribute->GetDefaultValue());
            },
            [&lineStr] (CanFloatAttribute* floatAttribute)
            {
               lineStr += std::to_string(floatAttribute->GetDefaultValue());
            },
            [&lineStr] (CanStringAttribute* stringAttribute)
            {
               lineStr += "\""s + stringAttribute->GetDefaultValue() + "\"";
            },
            [&lineStr] (CanEnumAttribute* enumAttribute)
            {
               lineStr += "\""s + enumAttribute->GetDefaultValue() + "\"";
            });

         lineStr += ";\n";
      }
   }

   return false;
}

bool CanBusConfig::WriteAttributeValueDefinition(std::string& lineStr) const
{
   auto network = this->networks.back();
   if (!network) { return false; }

   auto WriteAttributeValue = [&lineStr, this] (const CanAttributeOwner* attributeOwner)
   {
      for (const auto& attribute : attributeOwner->GetAttributes())
      {
         if (attribute)
         {
            lineStr += ATTRIBUTE_VALUE_DEFINITION_HEADER.data();

            lineStr += "\""s + attribute->GetName() + "\" ";

            switch (attribute->GetObjectType())
            {
               case ICanAttribute::IObjectType_e::NODE:
               {
                  lineStr += DBC_KEYWORD_NETWORK_NODE.data() + " "s;
                  break;
               }
               case ICanAttribute::IObjectType_e::MESSAGE:
               {
                  lineStr += DBC_KEYWORD_MESSAGE.data() + " "s;
                  break;
               }
               case ICanAttribute::IObjectType_e::SIGNAL:
               {
                  lineStr += DBC_KEYWORD_SIGNAL.data() + " "s;
                  break;
               }
               case ICanAttribute::IObjectType_e::ENVIRONMENT_VARIABLE:
               {
                  lineStr += DBC_KEYWORD_ENVIRONMENT_VARIABLE.data() + " "s;
                  break;
               }
               default:
               {
                  break;
               }
            }

            helpers::typecase(attributeOwner,
               [&lineStr] (const CanBusConfig* canBusConfig)
               {
                  ;
               },
               [&lineStr] (const CanNode* node)
               {
                  lineStr += node->GetName() + " "s;
               },
               [&lineStr] (const CanMessage* message)
               {
                  lineStr += std::to_string(message->GetId()) + " "s;
               },
               [&lineStr] (const CanSignal* signal)
               {
                  if (const auto message = signal->GetMessage(); message)
                  {
                     lineStr += std::to_string(message->GetId()) + " ";
                  }
                  lineStr += signal->GetName() + " "s;
               },
               [&lineStr] (const CanEnvVar* envVar)
               {
                  lineStr += envVar->GetName() + " "s;
               });

            const auto attributeValue = attributeOwner->GetAttributeValue(attribute->GetName());
            if (attributeValue == nullptr)
            {
               lineStr = "";
               continue;
            }

            helpers::typecase(attributeValue,
               [&lineStr] (const CanIntAttributeValue* intAttributeValue)
               {
                  lineStr += std::to_string(intAttributeValue->GetValue());
               },
               [&lineStr] (const CanHexAttributeValue* hexAttributeValue)
               {
                  lineStr += std::to_string(hexAttributeValue->GetValue());
               },
               [&lineStr] (const CanFloatAttributeValue* floatAttributeValue)
               {
                  lineStr += std::to_string(floatAttributeValue->GetValue());
               },
               [&lineStr] (const CanStringAttributeValue* stringAttributeValue)
               {
                  lineStr += "\""s + stringAttributeValue->GetValue() + "\"";
               },
               [&lineStr] (const CanEnumAttributeValue* enumAttributeValue)
               {
                  lineStr += "\""s + enumAttributeValue->GetValue() + "\"";
               });

            lineStr += ";\n";
         }
      }
   };

   WriteAttributeValue(network);

   for (const auto& node : network->GetNodes())
   {
      WriteAttributeValue(node);
   }

   for (const auto& message : network->GetMessages())
   {
      WriteAttributeValue(message);
   }

   for (const auto& signal : network->GetMessages())
   {
      WriteAttributeValue(signal);
   }

   for (const auto& envVar : network->GetEnvVars())
   {
      WriteAttributeValue(envVar);
   }

   return false;
}

bool CanBusConfig::WriteValueTableDefinition(std::string& lineStr) const
{
   auto network = this->networks.back();
   if (!network) { return false; }

   for (const auto& signal : network->GetSignals())
   {
      if (signal)
      {
         const auto valueTable = signal->GetValueTable();
         if (const auto message = signal->GetMessage(); message && valueTable)
         {
            lineStr += VALUE_TABLE_DEFINITION_HEADER.data();
            lineStr += std::to_string(message->GetId()) + " ";
            lineStr += signal->GetName();

            for (size_t i = 0; i < valueTable->GetValuesCount(); i++)
            {
               auto value = valueTable->GetValue(i);
               if (const auto description = valueTable->GetValueDescription(value); description)
               {
                  lineStr += " " + std::to_string(value) + " ";
                  lineStr += "\""s + description + "\"";
               }
               else
               {
                  lineStr = "";
                  break;
               }
            }

            lineStr += ";\n";
         }
      }
   }

   for (const auto& envVar : network->GetEnvVars())
   {
      if (envVar)
      {
         if (const auto valueTable = envVar->GetValueTable();  valueTable)
         {
            lineStr += VALUE_TABLE_DEFINITION_HEADER.data();
            lineStr += envVar->GetName();

            for (size_t i = 0; i < valueTable->GetValuesCount(); i++)
            {
               auto value = valueTable->GetValue(i);
               if (const auto description = valueTable->GetValueDescription(value); description)
               {
                  lineStr += " " + std::to_string(value) + " ";
                  lineStr += "\""s + description + "\"";
               }
               else
               {
                  lineStr = "";
                  break;
               }
            }

            lineStr += ";\n";
         }
      }
   }

   return false;
}