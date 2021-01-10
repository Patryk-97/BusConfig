#pragma once

#include "ICanMessage.h"
#include "CanSignal.h"

#include <string>
#include <map>

class CanMessage : public CanAttributeOwner, public ICanMessage
{
public:
   CanMessage() : CanAttributeOwner(ICanAttribute::IObjectType_e::MESSAGE) {};
   ~CanMessage();

   void Clear(void);

   uint32_t GetId(void) const;
   void ModifyId(uint32_t id) override;
   void SetId(uint32_t messageId);

   const char* GetName(void) const;
   void ModifyName(const char* name) override;
   void SetName(const char* name);

   uint32_t GetSize(void) const;
   void ModifySize(uint32_t) override;
   void SetSize(uint32_t size);

   const char* GetMainTransmitter(void) const;
   void SetMainTransmitter(const char* mainTransmitter);

   size_t GetSignalsCount(void) const override;
   ICanSignal* GetSignalByName(const char* name) const override;
   ICanSignal* GetSignalByIndex(size_t index) const override;
   std::vector<CanSignal*> GetSignals(void);
   bool RemoveSignalByIndex(size_t index) override;
   bool RemoveSignalByName(const char* name) override;
   void SortSignalsByName(bool caseSensitive = false) override;
   void SortSignalsByStartBit(void) override;
   void AddSignal(CanSignal* signal);

   size_t GetAttributesCount(void) const override;
   ICanAttribute* GetAttributeByIndex(size_t index) const override;
   ICanAttribute* GetAttributeByName(const char* name) const override;

   size_t GetAttributesValuesCount(void) const override;
   ICanAttributeValue* GetAttributeValue(const char* attributeName) const override;

   IdFormat_e GetIdFormat(void) const override;
   void ModifyIdFormat(IdFormat_e idFormat) override;
   void SetIdFormat(IdFormat_e idFormat);

   TxMethod_e GetTxMethod(void) const override;
   void ModifyTxMethod(TxMethod_e txMethod) override;
   void SetTxMethod(TxMethod_e txMethod);

   cycle_time_t GetCycleTime(void) const override;
   void ModifyCycleTime(cycle_time_t cycleTime) override;
   void SetCycleTime(cycle_time_t cycleTime);

   const char* GetComment(void) const override;
   void ModifyComment(const char* comment) override;
   void SetComment(const char* comment);

   const char* ToString(void) override;

   static const inline std::map<std::string_view, IdFormat_e> ID_FORMATS
   {
      { ICanMessage::IdFormat::STANDARD_CAN,    IdFormat_e::STANDARD_CAN },
      { ICanMessage::IdFormat::EXTENDED_CAN,    IdFormat_e::EXTENDED_CAN },
      { ICanMessage::IdFormat::STANDARD_CAN_FD, IdFormat_e::STANDARD_CAN_FD },
      { ICanMessage::IdFormat::EXTENDED_CAN_FD, IdFormat_e::EXTENDED_CAN_FD },
      { ICanMessage::IdFormat::RESERVED,        IdFormat_e::RESERVED }
   };

   static inline std::string_view GetIdFormatKey(IdFormat_e idFormat)
   {
      for (const auto& [key, value] : ID_FORMATS)
      {
         if (value == idFormat)
         {
            return key;
         }
      }
      return std::string_view {};
   }

   static const inline std::map<std::string_view, TxMethod_e> TX_METHODS
   {
      { ICanMessage::TxMethod::NO_MSG_SEND_TYPE, TxMethod_e::NO_MSG_SEND_TYPE },
      { ICanMessage::TxMethod::CYCLIC,           TxMethod_e::CYCLIC },
      { ICanMessage::TxMethod::IF_ACTIVE,        TxMethod_e::IF_ACTIVE },
      { ICanMessage::TxMethod::NOT_USED,         TxMethod_e::NOT_USED },
   };

   static inline std::string_view GetTxMethodKey(TxMethod_e txMethod)
   {
      for (const auto& [key, value] : TX_METHODS)
      {
         if (value == txMethod)
         {
            return key;
         }
      }
      return std::string_view{};
   }

private:
   uint32_t id {};
   std::string name;
   uint32_t size {};
   std::string mainTransmitter;
   std::vector<CanSignal*> signals;
   std::string comment;

   // Main attributes
   IdFormat_e idFormat { ICanMessage::DEFAULT_ID_FORMAT };
   TxMethod_e txMethod { ICanMessage::DEFAULT_TX_METHOD };
   cycle_time_t cycleTime { ICanMessage::DEFAULT_CYCLE_TIME };

   std::string stringRepresentation;
};