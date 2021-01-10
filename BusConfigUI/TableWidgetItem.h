#pragma once

#include <qtablewidget.h>

template <typename T>
class TableWidgetItem : public QTableWidgetItem
{
public:
   TableWidgetItem(const QString& text, int type = 0) : QTableWidgetItem(text, type) {}

   inline bool operator <(const QTableWidgetItem& other) const
   {
      if constexpr (std::is_same_v<T, int32_t>)
      {
         return this->text().toInt() < other.text().toInt();
      }
      else if (std::is_same_v<T, uint32_t>)
      {
         return this->text().toUInt() < other.text().toUInt();
      }
      else if (std::is_same_v<T, double>)
      {
         return this->text().toDouble() < other.text().toDouble();
      }
   }
};