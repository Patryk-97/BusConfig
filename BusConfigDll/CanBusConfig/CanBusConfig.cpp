#include "CanBusConfig.h"
#include <boost/tokenizer.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>

#ifndef line
#define line lineData.first
#endif

#ifndef lineNr
#define lineNr lineData.second
#endif

namespace ranges = std::ranges;

CanBusConfig::~CanBusConfig()
{
   this->Clear();
}

void CanBusConfig::Clear()
{
   this->log = "";
   for (const auto& message : this->messages) { delete message; }
   this->messages.clear();
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
         if (line.starts_with(CanBusConfig::MESSAGE_DEFINITION_HEADER))
         {
            this->ParseMessageDefinition(file, lineData);
         }
         lineNr++;
      }
   }

   return rV;
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
      boost::char_separator<char> sep(" :");
      boost::tokenizer<boost::char_separator<char>> tokenizer { line, sep };
      
      // If everything's okay
      if (ranges::distance(tokenizer) == MESSAGE_DEFINITION_ELEMENTS_COUNT)
      {
         uint8_t pos {};
         CanMessage* message = this->CreateAndAddMessage();
         for (const auto& token : tokenizer)
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