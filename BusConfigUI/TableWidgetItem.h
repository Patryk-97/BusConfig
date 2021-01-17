#pragma once

#include <qtablewidget.h>

template <typename T = QString, bool editable = true>
class TableWidgetItem : public QTableWidgetItem
{
public:
   TableWidgetItem(const QString& text, Qt::AlignmentFlag textAlignment = Qt::AlignmentFlag::AlignCenter) :
      QTableWidgetItem(text, 0)
   {
      this->setTextAlignment(textAlignment);
      if constexpr (editable)
      {
         this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
      }
      else
      {
         this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
      }
   }

   TableWidgetItem(const QIcon& icon, const QString& text, Qt::AlignmentFlag textAlignment = Qt::AlignmentFlag::AlignLeft) :
      QTableWidgetItem(icon, text, 0)
   {
      this->setTextAlignment(textAlignment);
      if constexpr (editable)
      {
         this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
      }
      else
      {
         this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
      }
   }

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
      else
      {
         QTableWidgetItem::operator<(other);
      }
   }
};