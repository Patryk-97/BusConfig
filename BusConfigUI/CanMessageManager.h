#pragma once

#include <qstring.h>
#include "ICanBusConfig.h"
#include <string_view>
#include <array>

class CanMessageManager
{
public:

   constexpr static uint8_t NAME_POS = 0;
   constexpr static uint8_t ID_POS = 1;
   constexpr static uint8_t ID_FORMAT_POS = 2;
   constexpr static uint8_t SIZE_POS = 3;
   constexpr static uint8_t TX_METHOD_POS = 4;
   constexpr static uint8_t CYCLE_TIME_POS = 5;
   constexpr static uint8_t COMMENT_POS = 6;
   constexpr static uint8_t PROPERTIES_COUNT = COMMENT_POS + 1;

   constexpr static std::string_view NAME = "Name";
   constexpr static std::string_view ID = "ID";
   constexpr static std::string_view ID_FORMAT = "ID-Format";
   constexpr static std::string_view SIZE = "Size (Bytes)";
   constexpr static std::string_view TX_METHOD = "Tx Method";
   constexpr static std::string_view CYCLE_TIME = "Cycle Time";
   constexpr static std::string_view COMMENT = "Comment";

   constexpr static std::array<std::string_view, PROPERTIES_COUNT> PROPERTIES =
   {
      NAME, ID, ID_FORMAT, SIZE, TX_METHOD, CYCLE_TIME, COMMENT
   };

   class IdFormat
   {
   public:
      IdFormat() = delete;

      constexpr static std::string_view STANDARD_CAN = "CAN Standard";
      constexpr static std::string_view EXTENDED_CAN = "CAN Extended";
      constexpr static std::string_view STANDARD_CAN_FD = "CAN FD Standard";
      constexpr static std::string_view EXTENDED_CAN_FD = "CAN FD Extended";

      constexpr static std::string_view DEFAULT = STANDARD_CAN;
   };

   static void Modify(ICanBusConfig* canBusConfig, size_t index, const QString& data,
      uint8_t column);
};