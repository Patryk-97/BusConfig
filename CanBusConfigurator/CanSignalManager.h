#pragma once

#include <qstring.h>
#include "ICanNetwork.h"
#include <string_view>
#include <qtableview.h>
#include <qmap.h>
#include <array>

class CanSignalManager
{
public:
   
   constexpr static uint8_t NAME_POS = 0;
   constexpr static uint8_t NETWORK_POS = 1;
   constexpr static uint8_t MESSAGE_POS = 2;
   constexpr static uint8_t START_BIT_POS = 3;
   constexpr static uint8_t SIZE_POS = 4;
   constexpr static uint8_t BYTE_ORDER_POS = 5;
   constexpr static uint8_t VALUE_TYPE_POS = 6;
   constexpr static uint8_t INITIAL_VALUE_POS = 7;
   constexpr static uint8_t FACTOR_POS = 8;
   constexpr static uint8_t OFFSET_POS = 9;
   constexpr static uint8_t MINIMUM_POS = 10;
   constexpr static uint8_t MAXIMUM_POS = 11;
   constexpr static uint8_t UNIT_POS = 12;
   constexpr static uint8_t VALUE_TABLE_POS = 13;
   constexpr static uint8_t COMMENT_POS = 14;
   constexpr static uint8_t PROPERTIES_COUNT = COMMENT_POS + 1;

   constexpr static std::string_view NAME = "Name";
   constexpr static std::string_view NETWORK = "Network";
   constexpr static std::string_view MESSAGE = "Message";
   //constexpr static std::string_view MULTIPLEXING = "Multiplexing";
   constexpr static std::string_view START_BIT = "Start bit";
   constexpr static std::string_view SIZE = "Size";
   constexpr static std::string_view BYTE_ORDER = "Byte order";
   constexpr static std::string_view VALUE_TYPE = "Value type";
   constexpr static std::string_view INITIAL_VALUE = "Initial value";
   constexpr static std::string_view FACTOR = "Factor";
   constexpr static std::string_view OFFSET = "Offset";
   constexpr static std::string_view MINIMUM = "Minimum";
   constexpr static std::string_view MAXIMUM = "Maximum";
   constexpr static std::string_view UNIT = "Unit";
   constexpr static std::string_view VALUE_TABLE = "Value table";
   constexpr static std::string_view COMMENT = "Comment";

   constexpr static std::array<std::string_view, PROPERTIES_COUNT> PROPERTIES = 
   {
      NAME, NETWORK, MESSAGE, START_BIT, SIZE, BYTE_ORDER, VALUE_TYPE, INITIAL_VALUE,
      FACTOR, OFFSET, MINIMUM, MAXIMUM, UNIT, VALUE_TABLE, COMMENT
   };

   class ByteOrder
   {
   public:
      ByteOrder() = delete;

      constexpr static std::string_view BIG_ENDIAN = "Big endian";
      constexpr static std::string_view LITTLE_ENDIAN = "Little endian";

      constexpr static std::string_view DEFAULT = BIG_ENDIAN;

      const static inline QMap<QString, ICanSignal::IByteOrder_e> MAP
      {
         { BIG_ENDIAN.data(), ICanSignal::IByteOrder_e::BIG_ENDIAN },
         { LITTLE_ENDIAN.data(), ICanSignal::IByteOrder_e::LITTLE_ENDIAN }
      };
   };

   class ValueType
   {
   public:
      ValueType() = delete;

      constexpr static std::string_view UNSIGNED = "Unsigned";
      constexpr static std::string_view SIGNED = "Signed";

      constexpr static std::string_view DEFAULT = UNSIGNED;

      const static inline QMap<QString, ICanSignal::IValueType_e> MAP
      {
         { UNSIGNED.data(), ICanSignal::IValueType_e::UNSIGNED_TYPE },
         { SIGNED.data(), ICanSignal::IValueType_e::SIGNED_TYPE }
      };
   };

   const static inline QStringList BYTE_ORDERS { ByteOrder::BIG_ENDIAN.data(), ByteOrder::LITTLE_ENDIAN.data() };

   const static inline QStringList VALUE_TYPES { ValueType::UNSIGNED.data(), ValueType::SIGNED.data() };

   static bool Validate(ICanNetwork* canNetwork, const QString& name, const QString& data, uint8_t column, QString& newData);
   static QString GetData(ICanNetwork* canNetwork, const QString& name, uint8_t column);
   static void Modify(ICanNetwork* canNetwork, const QString& name, const QString& data, uint8_t column);
};