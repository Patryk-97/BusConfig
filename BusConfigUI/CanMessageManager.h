#pragma once

#include <qstring.h>
#include "ICanNetwork.h"
#include <qstringlist.h>
#include <string_view>
#include <array>
#include <qmap.h>

class CanMessageManager
{
public:

   constexpr static uint8_t NAME_POS = 0;
   constexpr static uint8_t NETWORK_POS = 1;
   constexpr static uint8_t ID_POS = 2;
   constexpr static uint8_t ID_FORMAT_POS = 3;
   constexpr static uint8_t SIZE_POS = 4;
   constexpr static uint8_t TX_METHOD_POS = 5;
   constexpr static uint8_t CYCLE_TIME_POS = 6;
   constexpr static uint8_t COMMENT_POS = 7;
   constexpr static uint8_t PROPERTIES_COUNT = COMMENT_POS + 1;

   constexpr static std::string_view NAME = "Name";
   constexpr static std::string_view NETWORK = "Network";
   constexpr static std::string_view ID = "ID";
   constexpr static std::string_view ID_FORMAT = "ID-Format";
   constexpr static std::string_view SIZE = "Size (Bytes)";
   constexpr static std::string_view TX_METHOD = "Tx Method";
   constexpr static std::string_view CYCLE_TIME = "Cycle Time";
   constexpr static std::string_view COMMENT = "Comment";

   constexpr static std::array<std::string_view, PROPERTIES_COUNT> PROPERTIES =
   {
      NAME, NETWORK, ID, ID_FORMAT, SIZE, TX_METHOD, CYCLE_TIME, COMMENT
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

      const static inline QMap<std::string_view, ICanMessage::IdFormat_e> MAP
      {
         { STANDARD_CAN, ICanMessage::IdFormat_e::STANDARD_CAN },
         { EXTENDED_CAN, ICanMessage::IdFormat_e::EXTENDED_CAN },
         { STANDARD_CAN_FD, ICanMessage::IdFormat_e::STANDARD_CAN_FD },
         { EXTENDED_CAN_FD, ICanMessage::IdFormat_e::EXTENDED_CAN_FD },
      };
   };

   class TxMethod
   {
   public:
      TxMethod() = delete;

      const static inline QMap<std::string_view, ICanMessage::TxMethod_e> MAP
      {
         { ICanMessage::TxMethod::NO_MSG_SEND_TYPE, ICanMessage::TxMethod_e::NO_MSG_SEND_TYPE },
         { ICanMessage::TxMethod::CYCLIC, ICanMessage::TxMethod_e::CYCLIC },
         { ICanMessage::TxMethod::IF_ACTIVE, ICanMessage::TxMethod_e::IF_ACTIVE },
         { ICanMessage::TxMethod::CYCLIC_AND_SPONTAN_WITH_DELAY, ICanMessage::TxMethod_e::CYCLIC_AND_SPONTAN_WITH_DELAY },
         { ICanMessage::TxMethod::NOT_USED, ICanMessage::TxMethod_e::NOT_USED },
      };
   };

   const static inline QStringList ID_FORMATS { IdFormat::STANDARD_CAN.data(), IdFormat::EXTENDED_CAN.data(),
      IdFormat::STANDARD_CAN_FD.data(), IdFormat::EXTENDED_CAN_FD.data() };

   const static inline QStringList TX_METHODS { ICanMessage::TxMethod::NO_MSG_SEND_TYPE, ICanMessage::TxMethod::CYCLIC,
      ICanMessage::TxMethod::IF_ACTIVE, ICanMessage::TxMethod::CYCLIC_AND_SPONTAN_WITH_DELAY, ICanMessage::TxMethod::NOT_USED };

   static bool Validate(ICanNetwork* canNetwork, const QString& name, const QString& data, uint8_t column, QString& newData);
   static QString GetData(ICanNetwork* canNetwork, const QString& name, uint8_t column);
   static void Modify(ICanNetwork* canNetwork, const QString& name, const QString& data, uint8_t column);
};