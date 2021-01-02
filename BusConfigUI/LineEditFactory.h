#pragma once

#include <qstring.h>
#include <qlineedit.h>
#include <qtablewidget.h>

class LineEditFactory
{
public:
   LineEditFactory() = delete;

   static QLineEdit* CreateDoubleLineEdit(const QString& text, QTableWidget* tableWidget,
      int row, int column);
   static QLineEdit* CreateIntLineEdit(const QString& text, QTableWidget* tableWidget,
      int row, int column);
};