#pragma once

#include <qstring.h>
#include <sstream>

template <typename T>
inline QString toQString(const T& parameter)
{
   return QStringLiteral("%1").arg(parameter);
}

template <std::integral T>
inline QString toHexQString(const T& hexNumber)
{
   std::stringstream stream;
   stream << "0x" << std::hex << hexNumber;
   return QString::fromStdString(stream.str());
}