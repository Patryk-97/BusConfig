#include "CanBusConfig.h"
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>
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

CanBusConfig::~CanBusConfig()
{
   this->Clear();
}

void CanBusConfig::Clear()
{
   this->log = "";
   for (auto& message : this->messages) { delete message; message = nullptr; }
   this->messages.clear();
   for (auto& node : this->nodes) { delete node; node = nullptr; }
   this->nodes.clear();
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

bool CanBusConfig::ParseMessageDefinition(std::ifstream& file, LineData_t& lineData)
{
   // locals
   bool rV { true };

   if (line.starts_with(CanBusConfig::MESSAGE_DEFINITION_HEADER))
   {
      boost_char_separator sep(" :");
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
               else if (pos == SIGNAL_UNIT_POS && this->GetNodeByName(token.c_str()) != nullptr) // if "" - for unit element
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
      if (ranges::distance(tokens) >= SIGNAL_DEFINITION_ELEMENTS_MIN_COUNT)
      {
         CanSignal* signal = message->CreateAndAddSignal();
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
         this->log += "Invalid signal definition elements count [line: " + line + ", lineNr: " + std::to_string(lineNr) + "].\r\n";
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