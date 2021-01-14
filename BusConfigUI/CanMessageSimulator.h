#pragma once

#include "ICanBusConfig.h"
#include <QDialog>
#include <qtablewidget.h>
#include "ICanBusConfig.h"

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
   void on_tableWidget_CanMessage_itemChanged(QTableWidgetItem* item);
   void on_tableWidget_CanSignal_itemChanged(QTableWidgetItem* item);

private:
   void UpdatedCanDataBytesHex(QTableWidgetItem* item);
   void UpdatedCanDataBytesBin(QTableWidgetItem* item);
   void UpdatedCanDataBytesBitDataBytes(QTableWidgetItem* item);
   void CalculateDataHexResult(void);
   void BuildCanMessageTableWidget(const ICanMessage* canMessage);
   void BuildCanSignalTableWidget(const ICanSignal* canSignal);
   void ResetSignalMaskBinAndHexTableWidget(void);

private:
   Ui::CanMessageSimulator* ui;
   bool canDataBytesTableFilled { false };
   bool canMessageTableFilled = { false };
   bool canSignalTableFilled = { false };
   ICanBusConfig* canBusConfig { nullptr };
};
