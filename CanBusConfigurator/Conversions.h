#pragma once

#include <qstring.h>

template <typename T>
inline QString toQString(const T& parameter)
{
   return QStringLiteral("%1").arg(parameter);
}

template <std::integral T>
inline QString toIntQString(const T& number, bool hex = false)
{
   if (!hex) return QString::number(number);
   QString rV = "0x";

   if (number <= 0xF || (number > 0xFF && number <= 0xFFF))
   {
      rV += "0";
   }

   return rV + QString::number(number, 16);
}