#pragma once

#include "ICanSignal.h"

#include <stdint.h>
#include <iostream>

class ICanMessage : public ICanAttributeOwner
{
protected:
   ICanMessage() {};
   virtual ~ICanMessage() {};
public:

   enum class IdFormat_e
   {
      STANDARD_CAN = 0, // default
      EXTENDED_CAN = 1,
      STANDARD_CAN_FD = 2,
      EXTENDED_CAN_FD = 3,
      RESERVED = 4
   };

   class IdFormat
   {
   public:
      IdFormat() = delete;

      static constexpr const char * STANDARD_CAN = "StandardCAN";
      static constexpr const char * EXTENDED_CAN = "ExtendedCAN";
      static constexpr const char * STANDARD_CAN_FD = "StandardCAN_FD";
      static constexpr const char * EXTENDED_CAN_FD = "ExtendedCAN_FD";
      static constexpr const char * RESERVED = "reserved";

      static constexpr const char * DEFAULT = STANDARD_CAN;

      static constexpr ICanAttribute::IValueType_e VALUE_TYPE = ICanAttribute::IValueType_e::ENUM;
   };

   enum class TxMethod_e
   {
      NO_MSG_SEND_TYPE = 0, // default
      CYCLIC = 1,
      IF_ACTIVE = 2,
      NOT_USED = 3
   };

   class TxMethod
   {
   public:
      TxMethod() = delete;

      static constexpr const char * NO_MSG_SEND_TYPE = "NoMsgSendType";
      static constexpr const char * CYCLIC = "Cyclic";
      static constexpr const char * IF_ACTIVE = "IfActive";
      static constexpr const char * NOT_USED = "not_used";

      static constexpr const char * DEFAULT = NO_MSG_SEND_TYPE;

      static constexpr ICanAttribute::IValueType_e VALUE_TYPE = ICanAttribute::IValueType_e::ENUM;
   };

   using cycle_time_t = uint16_t;

   class CycleTime
   {
   public:
      CycleTime() = delete;

      static constexpr ICanAttribute::IValueType_e VALUE_TYPE = ICanAttribute::IValueType_e::INT;
   };

   static constexpr IdFormat_e DEFAULT_ID_FORMAT { IdFormat_e::STANDARD_CAN };
   static constexpr TxMethod_e DEFAULT_TX_METHOD { TxMethod_e::NO_MSG_SEND_TYPE };
   static constexpr cycle_time_t DEFAULT_CYCLE_TIME { 0 };

   virtual void Clear(void) = 0;

   virtual uint32_t GetId(void) const = 0;
   virtual void ModifyId(uint32_t id) = 0;

   virtual const char* GetName(void) const = 0;
   virtual void ModifyName(const char* name) = 0;

   virtual uint32_t GetSize(void) const = 0;
   virtual void ModifySize(uint32_t) = 0;

   virtual const char* GetMainTransmitter(void) const = 0;

   virtual size_t GetSignalsCount(void) const = 0;
   virtual ICanSignal* GetSignalByName(const char* name) const = 0;
   virtual ICanSignal* GetSignalByIndex(size_t index) const = 0;

   virtual size_t GetAttributesCount(void) const = 0;
   virtual ICanAttribute* GetAttributeByIndex(size_t index) const = 0;
   virtual ICanAttribute* GetAttributeByName(const char* name) const = 0;

   virtual ICanAttributeValue* GetAttributeValue(const char* attributeName) const = 0;

   virtual IdFormat_e GetIdFormat(void) const = 0;
   virtual void ModifyIdFormat(IdFormat_e idFormat) = 0;

   virtual TxMethod_e GetTxMethod(void) const = 0;
   virtual void ModifyTxMethod(TxMethod_e txMethod) = 0;

   virtual cycle_time_t GetCycleTime(void) const = 0;
   virtual void ModifyCycleTime(cycle_time_t cycleTime) = 0;

   virtual const char* GetComment(void) const = 0;
   virtual void ModifyComment(const char* comment) = 0;

   virtual const char* ToString(void) = 0;

   static constexpr const char * ID_FORMAT = "VFrameFormat";
   static constexpr const char * TX_METHOD = "GenMsgSendType";
   static constexpr const char * CYCLE_TIME = "GenMsgCycleTime";
};