#pragma once

#include "ICanNetwork.h"
#include <array>
#include <string_view>
#include <qstring.h>

class CanNodeManager
{
public:

   constexpr static uint8_t NAME_POS = 0;
   constexpr static uint8_t NETWORK_POS = 1;
   constexpr static uint8_t ADDRESS_POS = 2;
   constexpr static uint8_t COMMENT_POS = 3;
   constexpr static uint8_t PROPERTIES_COUNT = COMMENT_POS + 1;

   constexpr static std::string_view NAME = "Name";
   constexpr static std::string_view NETWORK = "Network";
   constexpr static std::string_view ADDRESS = "Address";
   constexpr static std::string_view COMMENT = "Comment";

   constexpr static std::array<std::string_view, PROPERTIES_COUNT> PROPERTIES =
   {
      NAME, NETWORK, ADDRESS, COMMENT
   };

   static bool Validate(ICanNetwork* canNetwork, const QString& name, const QString& data, uint8_t column, QString& newData);
   static QString GetData(ICanNetwork* canNetwork, const QString& name, uint8_t column);
   static void Modify(ICanNetwork* canNetwork, const QString& name, const QString& data, uint8_t column);
};