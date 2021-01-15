#pragma once

#include <qstring.h>

template <typename T>
inline QString toQString(const T& parameter)
{
   return QStringLiteral("%1").arg(parameter);
}

template <std::integral T>
inline QString toHexQString(const T& hexNumber)
{
   QString rV = "0x";

   if (hexNumber <= 0xF || (hexNumber > 0xFF && hexNumber <= 0xFFF))
   {
      rV += "0";
   }

   return rV + QString::number(hexNumber, 16);
}