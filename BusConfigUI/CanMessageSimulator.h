#pragma once

#include "ICanBusConfig.h"
#include <QDialog>
#include <qtablewidget.h>

namespace Ui
{
   class CanMessageSimulator;
}

class CanMessageSimulator : public QDialog
{
   Q_OBJECT
public:
   explicit CanMessageSimulator(QWidget* parent = nullptr);
   ~CanMessageSimulator();

   bool Create(ICanBusConfig* canBusConfig, ICanMessage* canMessage = nullptr);

private slots:
   void on_tableWidget_CanDataBytes_itemChanged(QTableWidgetItem* item);

private:
   Ui::CanMessageSimulator* ui;
   bool canDataBytesTableFilled = false;
};
