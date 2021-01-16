#include "CanMessageSimulator.h"
#include "ui_CanMessageSimulator.h"
#include "ICanMessage.h"
#include "Conversions.h"
#include "TableWidgetItem.h"
#include "ComboDelegate.h"
#include "CanSignalManager.h"
#include <QtWidgets/qabstractbutton.h>
#include <bitset>

#define MSB_BG_COLOR QColor{ "#00BFFF" }
#define MSB_FG_COLOR Qt::black
#define LSB_BG_COLOR QColor{ "#0000CD" }
#define LSB_FG_COLOR Qt::white
#define CAN_SIGNAL_COLOR QColor { "#B0E0E6" }

CanMessageSimulator::CanMessageSimulator(QWidget* parent) :
   QDialog(parent),
   ui(new Ui::CanMessageSimulator)
{
   this->ui->setupUi(this);
   Qt::WindowFlags flags{ Qt::Window | Qt::WindowContextHelpButtonHint |
      Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint };
   this->setWindowFlags(flags);

   this->ui->tableWidget_CanDataBytes->setStyleSheet("QTableWidget::item { padding: 5px 10px; border: 0; }");
   this->ui->tableWidget_CanDataBytes->horizontalHeader()->setStyleSheet(
      "QHeaderView::section { padding: 5px 10px; border: 0; background: white; }");
   this->ui->tableWidget_CanDataBytes->verticalHeader()->setStyleSheet(
      "QHeaderView::section { padding: 5px 10px; border: 0; background: white; }");
   this->ui->tableWidget_CanDataBytes->setFocusPolicy(Qt::FocusPolicy::NoFocus);

   this->ui->tableWidget_CanDataBytes->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::ResizeToContents);
   this->ui->tableWidget_CanDataBytes->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
   this->ui->tableWidget_CanDataBytes->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
   for (size_t i = 0; i < 8; i++)
   {
      this->ui->tableWidget_CanDataBytes->horizontalHeader()->setSectionResizeMode(i + 2, QHeaderView::ResizeMode::Stretch);
   }

   this->ui->tableWidget_CanDataBytes->setRowCount(8);
   this->ui->tableWidget_CanDataBytes->setColumnCount(10);

   auto horizontalHeaderItem = new QTableWidgetItem{ "Data (Hex)" };
   auto fontBold = QFont{}; fontBold.setBold(true);
   horizontalHeaderItem->setFont(fontBold);
   this->ui->tableWidget_CanDataBytes->setHorizontalHeaderItem(0, horizontalHeaderItem);

   horizontalHeaderItem = new QTableWidgetItem{ "Data (Bin)" };
   horizontalHeaderItem->setFont(fontBold);
   this->ui->tableWidget_CanDataBytes->setHorizontalHeaderItem(1, horizontalHeaderItem);
   for (size_t i = 0; i < 8; i++)
   {
      auto verticalHeaderItem = new QTableWidgetItem{ tr("%1").arg(i) };
      verticalHeaderItem->setFont(fontBold);
      this->ui->tableWidget_CanDataBytes->setVerticalHeaderItem(i, verticalHeaderItem);

      horizontalHeaderItem = new QTableWidgetItem{ tr("%1").arg(7 - i) };
      horizontalHeaderItem->setFont(fontBold);
      this->ui->tableWidget_CanDataBytes->setHorizontalHeaderItem(i + 2, horizontalHeaderItem);
   }

   for (size_t i = 0; i < 8; i++)
   {
      this->ui->tableWidget_CanDataBytes->setItem(i, 0, new TableWidgetItem{ toHexQString(0x00), Qt::AlignCenter });
      std::bitset<8> binaryNumber;
      this->ui->tableWidget_CanDataBytes->setItem(i, 1, new TableWidgetItem{ binaryNumber.to_string().c_str(), Qt::AlignCenter });
      for (size_t j = 0; j < 8; j++)
      {
         this->ui->tableWidget_CanDataBytes->setItem(i, j + 2, new TableWidgetItem{ toQString(0), Qt::AlignCenter });
      }
   }

   this->ui->tableWidget_SignalMaskBin->setStyleSheet("QTableWidget::item { padding: 2px 5px; }");
   this->ui->tableWidget_SignalMaskBin->horizontalHeader()->setStyleSheet(
      "QHeaderView::section { padding: 2px 5px; border: 0; background: white; }");
   this->ui->tableWidget_SignalMaskBin->verticalHeader()->setStyleSheet(
      "QHeaderView::section { padding: 2px 5px; border: 0; background: white; }");
   this->ui->tableWidget_SignalMaskBin->setFocusPolicy(Qt::FocusPolicy::NoFocus);

   this->ui->tableWidget_SignalMaskBin->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
   this->ui->tableWidget_SignalMaskBin->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

   this->ui->tableWidget_SignalMaskBin->setRowCount(8);
   this->ui->tableWidget_SignalMaskBin->setColumnCount(8);
   
   for (size_t i = 0; i < 8; i++)
   {
      horizontalHeaderItem = new QTableWidgetItem { tr("%1").arg(7 - i) };
      horizontalHeaderItem->setFont(fontBold);
      this->ui->tableWidget_SignalMaskBin->setHorizontalHeaderItem(i, horizontalHeaderItem);
   }

   this->ui->tableWidget_SignalMaskBin->verticalHeader()->hide();

   for (size_t i = 0; i < 8; i++)
   {
      for (size_t j = 0; j < 8; j++)
      {
         this->ui->tableWidget_SignalMaskBin->setItem(i, j, new TableWidgetItem<QString, false> { tr("%1").arg(0), Qt::AlignCenter });
      }
   }
   this->ui->tableWidget_SignalMaskBin->updateGeometry();

   this->ui->tableWidget_SignalMaskHex->setStyleSheet("QTableWidget::item { padding: 5px 10px; border: 0; }");
   this->ui->tableWidget_SignalMaskHex->horizontalHeader()->setStyleSheet(
      "QHeaderView::section { padding: 5px 10px; border: 0; background: white; }");
   this->ui->tableWidget_SignalMaskHex->verticalHeader()->setStyleSheet(
      "QHeaderView::section { padding: 5px 10px; border: 0; background: white; }");
   this->ui->tableWidget_SignalMaskHex->setFocusPolicy(Qt::FocusPolicy::NoFocus);

   this->ui->tableWidget_SignalMaskHex->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
   this->ui->tableWidget_SignalMaskHex->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

   this->ui->tableWidget_SignalMaskHex->setRowCount(8);
   this->ui->tableWidget_SignalMaskHex->setColumnCount(1);
   this->ui->tableWidget_SignalMaskHex->setHorizontalHeaderLabels(QStringList { "" });
   this->ui->tableWidget_SignalMaskHex->setMaximumWidth(70);
   this->ui->tableWidget_SignalMaskHex->verticalHeader()->hide();

   for (size_t i = 0; i < 8; i++)
   {
      auto brush = QBrush { QColor { 170, 150, 120, 127 } };
      brush.setStyle(Qt::SolidPattern);
      auto item = new QTableWidgetItem { "0x00" };
      item->setBackground(brush);
      this->ui->tableWidget_SignalMaskHex->setItem(i, 0, item);
   }

   /*this->ui->tableWidget_DataHex->setStyleSheet("QTableWidget::item { padding: 5px 10px; border: 0; }");
   this->ui->tableWidget_DataHex->horizontalHeader()->setStyleSheet(
      "QHeaderView::section { padding: 5px 10px; border: 0; background: white; }");
   this->ui->tableWidget_DataHex->verticalHeader()->setStyleSheet(
      "QHeaderView::section { padding: 5px 10px; border: 0; background: white; }");
   this->ui->tableWidget_DataHex->setFocusPolicy(Qt::FocusPolicy::NoFocus);

   this->ui->tableWidget_DataHex->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
   this->ui->tableWidget_DataHex->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

   this->ui->tableWidget_DataHex->setRowCount(8);
   this->ui->tableWidget_DataHex->setColumnCount(1);
   this->ui->tableWidget_DataHex->setHorizontalHeaderLabels(QStringList { "" });
   this->ui->tableWidget_DataHex->setMaximumWidth(70);
   this->ui->tableWidget_DataHex->verticalHeader()->hide();

   for (size_t i = 0; i < 8; i++)
   {
      this->ui->tableWidget_DataHex->setItem(i, 0, new TableWidgetItem<uint32_t, false> { "", Qt::AlignCenter });
   }*/

   this->ui->tableWidget_CanMessage->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
   this->ui->tableWidget_CanMessage->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

   this->ui->tableWidget_CanMessage->setStyleSheet("QTableWidget::item { padding: 5px 10px; border: 0; }");
   this->ui->tableWidget_CanMessage->setFocusPolicy(Qt::FocusPolicy::NoFocus);

   for (size_t i {}; const auto& canMessageHeaderLabel : { "Name", "Id" })
   {
      auto item = new TableWidgetItem<QString, false> { canMessageHeaderLabel, Qt::AlignCenter };
      item->setFont(fontBold);
      this->ui->tableWidget_CanMessage->setItem(i++, 0, item);
   }
   
   this->ui->tableWidget_CanSignal->setStyleSheet("QTableWidget::item { padding: 2px 10px; border: 0; }");
   this->ui->tableWidget_CanSignal->setFocusPolicy(Qt::FocusPolicy::NoFocus);

   this->ui->tableWidget_CanSignal->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
   this->ui->tableWidget_CanSignal->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

   for (size_t i{}; const auto & canSignalHeaderLabel : { "Name", "Start bit", "Length", "Byte order", "Scale", "Offset" })
   {
      auto item = new TableWidgetItem<QString, false> { canSignalHeaderLabel, Qt::AlignCenter };
      item->setFont(fontBold);
      this->ui->tableWidget_CanSignal->setItem(i++, 0, item);
   }

   for (size_t i = 0; i < this->ui->tableWidget_CanSignal->rowCount(); i++)
   {
      this->ui->tableWidget_CanSignal->item(i, 1)->setTextAlignment(Qt::AlignCenter);
   }

   this->ui->tableWidget_CanSignalRawData->setStyleSheet("QTableWidget::item { padding: 5px 10px; }");
   this->ui->tableWidget_CanSignalRawData->setFocusPolicy(Qt::FocusPolicy::NoFocus);

   this->ui->tableWidget_CanSignalRawData->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
   this->ui->tableWidget_CanSignalRawData->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

   this->ui->tableWidget_CanSignalRawData->setRowCount(8);
   this->ui->tableWidget_CanSignalRawData->setColumnCount(1);
   const auto tableWidgetCanSignalRawDataRowCount = this->ui->tableWidget_CanSignalRawData->rowCount();

   for (size_t i = 0; i < tableWidgetCanSignalRawDataRowCount; i++)
   {
      auto item = new TableWidgetItem<uint32_t, false>{ "0", Qt::AlignCenter };
      item->setFont(fontBold); item->setForeground(Qt::black);
      this->ui->tableWidget_CanSignalRawData->setItem(i, 0, item);
   }
   this->ui->tableWidget_CanSignalRawData->item(0, 0)->setBackground(MSB_BG_COLOR);
   this->ui->tableWidget_CanSignalRawData->item(tableWidgetCanSignalRawDataRowCount - 1, 0)->setBackground(LSB_BG_COLOR);
   this->ui->tableWidget_CanSignalRawData->item(tableWidgetCanSignalRawDataRowCount - 1, 0)->setForeground(LSB_FG_COLOR);

   this->canDataBytesTableFilled = true;
}

CanMessageSimulator::~CanMessageSimulator()
{
   delete ui;
}

bool CanMessageSimulator::Create(ICanNetwork* canNetwork, ICanMessage* canMessage)
{
   if (canNetwork)
   {
      this->canNetwork = canNetwork;
      if (const auto message = this->canNetwork->GetMessageByIndex(0); message)
      {
         this->canMessageTableFilled = false; this->canSignalTableFilled = false;
         this->BuildCanMessageTableWidget(message);
         QStringList messagesNames;
         QStringList signalsNames;
         for (size_t i = 0; i < canNetwork->GetMessagesCount(); i++)
         {
            if (const auto message = canNetwork->GetMessageByIndex(i); message)
            {
               messagesNames.append(message->GetName());
            }
         }
         ComboDelegate* messageDelegate = new ComboDelegate(messagesNames);
         this->ui->tableWidget_CanMessage->setItemDelegateForRow(0, messageDelegate);

         this->canMessageTableFilled = true; this->canSignalTableFilled = true;
      }
   }

   return true;
}

void CanMessageSimulator::on_tableWidget_CanDataBytes_itemChanged(QTableWidgetItem* item)
{
   if (this->canDataBytesTableFilled)
   {
      this->canDataBytesTableFilled = false;
      const int column = item->column();

      if (column == 0)
      {
         this->UpdatedCanDataBytesHex(item);
      }
      else if (column == 1)
      {
         this->UpdatedCanDataBytesBin(item);
      }
      else
      {
         this->UpdatedCanDataBytesBitDataBytes(item);
      }
      this->canDataBytesTableFilled = true;
   }
}

void CanMessageSimulator::on_tableWidget_CanMessage_itemChanged(QTableWidgetItem* item)
{
   if (this->canNetwork && this->canMessageTableFilled)
   {
      this->canMessageTableFilled = false;
      if (const auto canMessage = this->canNetwork->GetMessageByName(item->text().toUtf8()); canMessage)
      {
         this->BuildCanMessageTableWidget(canMessage);
      }
      this->canMessageTableFilled = true;
   }
}

void CanMessageSimulator::on_tableWidget_CanSignal_itemChanged(QTableWidgetItem* item)
{
   if (this->canNetwork && this->canSignalTableFilled)
   {
      this->canSignalTableFilled = false;

      const auto canMessage = this->canNetwork->GetMessageByName(this->ui->tableWidget_CanMessage->item(0, 1)->text().toUtf8());
      if (canMessage)
      {
         if (const auto canSignal = canMessage->GetSignalByName(item->text().toUtf8()); canSignal)
         {
            this->BuildCanSignalTableWidget(canSignal);
         }
      }

      this->canSignalTableFilled = true;
   }
}

void CanMessageSimulator::UpdatedCanDataBytesHex(QTableWidgetItem* item)
{
   const int row = item->row();
   const uint8_t value = item->text().toUInt(nullptr, 16);
   std::bitset<8> binaryNumber{ value };
   this->ui->tableWidget_CanDataBytes->item(row, 1)->setText(binaryNumber.to_string().c_str());

   for (size_t i = 0; i < 8; i++)
   {
      this->ui->tableWidget_CanDataBytes->item(row, i + 2)->setText(std::to_string(binaryNumber[7 - i]).c_str());
   }

   const auto canSignalBitStartItem = this->ui->tableWidget_CanSignal->item(1, 1);
   const auto canSignalSizeItem = this->ui->tableWidget_CanSignal->item(2, 1);

   if (!canSignalBitStartItem->text().isEmpty() && !canSignalSizeItem->text().isEmpty())
   {
      this->CalculateDataHexResult();
   }
}

void CanMessageSimulator::UpdatedCanDataBytesBin(QTableWidgetItem* item)
{
   const int row = item->row();
   std::bitset<8> binaryNumber{ item->text().toUtf8().constData() };
   const uint8_t value = static_cast<uint8_t>(binaryNumber.to_ulong());
   this->ui->tableWidget_CanDataBytes->item(row, 0)->setText(toHexQString(value));

   for (size_t i = 0; i < 8; i++)
   {
      this->ui->tableWidget_CanDataBytes->item(row, i + 2)->setText(std::to_string(binaryNumber[7 - i]).c_str());
   }

   const auto canSignalBitStartItem = this->ui->tableWidget_CanSignal->item(1, 1);
   const auto canSignalSizeItem = this->ui->tableWidget_CanSignal->item(2, 1);

   if (!canSignalBitStartItem->text().isEmpty() && !canSignalSizeItem->text().isEmpty())
   {
      this->CalculateDataHexResult();
   }
}

void CanMessageSimulator::UpdatedCanDataBytesBitDataBytes(QTableWidgetItem* item)
{
   const int row = item->row();
   const int column = item->column();
   auto bit = item->text();
   std::bitset<8> binaryNumber{ this->ui->tableWidget_CanDataBytes->item(row, 1)->text().toUtf8().constData() };

   if (bit == "1")
   {
      binaryNumber.set(static_cast<size_t>(9) - column);
   }
   else if (bit == "0")
   {
      binaryNumber.reset(static_cast<size_t>(9) - column);
   }

   this->ui->tableWidget_CanDataBytes->item(row, 0)->setText(toHexQString(binaryNumber.to_ulong()));
   this->ui->tableWidget_CanDataBytes->item(row, 1)->setText(binaryNumber.to_string().c_str());

   const auto canSignalBitStartItem = this->ui->tableWidget_CanSignal->item(1, 1);
   const auto canSignalSizeItem = this->ui->tableWidget_CanSignal->item(2, 1);

   if (!canSignalBitStartItem->text().isEmpty() && !canSignalSizeItem->text().isEmpty())
   {
      this->CalculateDataHexResult();
   }
}

void CanMessageSimulator::CalculateDataHexResult(void)
{
   QString binStr;

   const auto canSignal = this->canNetwork->GetSignalByName(this->ui->tableWidget_CanSignal->item(0, 1)->text().toUtf8());
   if (canSignal)
   {
      const auto canSignalByteOrder = canSignal->GetByteOrder();

      for (size_t i = 0; i < this->ui->tableWidget_SignalMaskBin->rowCount(); i++)
      {
         QString rowBinStr;
         for (size_t j = 0; j < this->ui->tableWidget_SignalMaskBin->columnCount(); j++)
         {
            const auto itemText = this->ui->tableWidget_SignalMaskBin->item(i, j)->text();
            if (itemText == "1")
            {
               const auto rawBit = this->ui->tableWidget_CanDataBytes->item(i, j + 2)->text();
               rowBinStr += rawBit;
            }
         }

         if (!rowBinStr.isEmpty())
         {
            if (canSignalByteOrder == ICanSignal::IByteOrder_e::BIG_ENDIAN)
            {
               binStr.append(rowBinStr);
            }
            else // (canSignalByteOrder == ICanSignal::IByteOrder_e::LITTLE_ENDIAN)
            {
               binStr = rowBinStr + binStr;
            }
         }
      }

      if (!binStr.isEmpty())
      {
         std::bitset<64> binaryNumber{ binStr.toUtf8().constData() };
         const auto value = binaryNumber.to_ullong();

         this->ui->label_DataDec->setText(toQString(value));
         this->ui->label_DataHex->setText(toHexQString(value));

         const auto canSignalFactor = this->ui->tableWidget_CanSignal->item(4, 1)->text().toDouble();
         const auto canSignalOffset = this->ui->tableWidget_CanSignal->item(5, 1)->text().toDouble();
         this->ui->label_ValueFormula->setText(toQString(value) + " * " + toQString(canSignalFactor) + " + " + toQString(canSignalOffset));
         this->ui->label_Value->setText(toQString(value * canSignalFactor + canSignalOffset) + " " + canSignal->GetUnit());

         const auto canSignalSize = this->ui->tableWidget_CanSignal->item(2, 1)->text().toUInt();
         this->ui->tableWidget_CanSignalRawData->setRowCount(canSignalSize);
         this->ui->tableWidget_CanSignalRawData->setColumnCount(1);

         for (size_t i = 0; i < canSignalSize; i++)
         {
            auto item = new TableWidgetItem<uint32_t, false>{ binStr.at(i), Qt::AlignCenter };
            auto boldFont = QFont{}; boldFont.setBold(true);
            item->setFont(boldFont);
            item->setForeground(Qt::black); item->setBackground(CAN_SIGNAL_COLOR);
            this->ui->tableWidget_CanSignalRawData->setItem(i, 0, item);
         }
         this->ui->tableWidget_CanSignalRawData->item(0, 0)->setBackground(MSB_BG_COLOR);

         if (canSignalSize != 1)
         {
            this->ui->tableWidget_CanSignalRawData->item(canSignalSize - 1, 0)->setBackground(LSB_BG_COLOR);
            this->ui->tableWidget_CanSignalRawData->item(canSignalSize - 1, 0)->setForeground(LSB_FG_COLOR);
         }
      }
   }
}

void CanMessageSimulator::BuildCanMessageTableWidget(const ICanMessage* canMessage)
{
   if (canMessage)
   {
      this->ui->tableWidget_CanMessage->item(0, 1)->setText(canMessage->GetName());
      this->ui->tableWidget_CanMessage->item(1, 1)->setText(toHexQString(canMessage->GetId()));

      for (size_t i = 0; i < this->ui->tableWidget_CanMessage->rowCount(); i++)
      {
         this->ui->tableWidget_CanMessage->item(i, 1)->setTextAlignment(Qt::AlignCenter);
      }

      if (const auto canSignal = canMessage->GetSignalByIndex(0); canSignal)
      {
         this->BuildCanSignalTableWidget(canSignal);
      }

      QStringList canSignalsNames;
      for (size_t i = 0; i < canMessage->GetSignalsCount(); i++)
      {
         if (const auto canSignal = canMessage->GetSignalByIndex(i); canSignal)
         {
            canSignalsNames << canSignal->GetName();
         }
      }

      ComboDelegate* signalDelegate = new ComboDelegate(canSignalsNames);
      this->ui->tableWidget_CanSignal->setItemDelegateForRow(0, signalDelegate);
   }
}

void CanMessageSimulator::BuildCanSignalTableWidget(const ICanSignal* canSignal)
{
   if (canSignal)
   {
      const auto byteOrder = canSignal->GetByteOrder();
      const auto canSignalSize = canSignal->GetSize();

      this->ui->tableWidget_CanSignal->item(0, 1)->setText(canSignal->GetName());
      this->ui->tableWidget_CanSignal->item(1, 1)->setText(toQString(canSignal->GetStartBit()));
      this->ui->tableWidget_CanSignal->item(2, 1)->setText(toQString(canSignalSize));
      this->ui->tableWidget_CanSignal->item(3, 1)->setText(CanSignalManager::BYTE_ORDERS[static_cast<int>(byteOrder)]);
      this->ui->tableWidget_CanSignal->item(4, 1)->setText(toQString(canSignal->GetFactor()));
      this->ui->tableWidget_CanSignal->item(5, 1)->setText(toQString(canSignal->GetOffset()));

      this->ResetSignalMaskBinAndHexTableWidget();

      uint8_t bitIndex = canSignal->GetStartBit();
      auto moveBitIndex = [&byteOrder] (uint8_t& bitIndex)
      {
         if (byteOrder == ICanSignal::IByteOrder_e::LITTLE_ENDIAN)
         {
            ++bitIndex;
         }
         else if (byteOrder == ICanSignal::IByteOrder_e::BIG_ENDIAN)
         {
            if (bitIndex % 8 == 0)
            {
               bitIndex += 15;
            }
            else
            {
               --bitIndex;
            }
         }
      };
      for (size_t i = 0; i < canSignalSize; i++)
      {
         int row = bitIndex / 8;
         int column = 7 - (bitIndex % 8);
         auto item = this->ui->tableWidget_SignalMaskBin->item(row, column);
         item->setText("1");

         if (i == 0)
         {
            item->setBackground(byteOrder == ICanSignal::IByteOrder_e::LITTLE_ENDIAN ? LSB_BG_COLOR : MSB_BG_COLOR);
            item->setForeground(byteOrder == ICanSignal::IByteOrder_e::LITTLE_ENDIAN ? LSB_FG_COLOR : MSB_FG_COLOR);
         }
         else if (i == canSignal->GetSize() - 1)
         {
            item->setBackground(byteOrder == ICanSignal::IByteOrder_e::LITTLE_ENDIAN ? MSB_BG_COLOR : LSB_BG_COLOR);
            item->setForeground(byteOrder == ICanSignal::IByteOrder_e::LITTLE_ENDIAN ? MSB_FG_COLOR : LSB_FG_COLOR);
         }
         else
         {
            item->setBackground(CAN_SIGNAL_COLOR);
         }

         moveBitIndex(bitIndex);
      }

      for (size_t i = 0; i < 8; i++)
      {
         QString row;
         for (size_t j = 0; j < 8; j++)
         {
            row += this->ui->tableWidget_SignalMaskBin->item(i, j)->text();
         }

         std::bitset<8> binaryNumber { row.toUtf8().constData() };
         this->ui->tableWidget_SignalMaskHex->item(i, 0)->setText(toHexQString(binaryNumber.to_ulong()));
      }

      this->CalculateDataHexResult();
   }
}

void CanMessageSimulator::ResetSignalMaskBinAndHexTableWidget(void)
{
   for (size_t i = 0; i < this->ui->tableWidget_SignalMaskBin->rowCount(); i++)
   {
      for (size_t j = 0; j < this->ui->tableWidget_SignalMaskBin->columnCount(); j++)
      {
         this->ui->tableWidget_SignalMaskBin->item(i, j)->setText("0");
         this->ui->tableWidget_SignalMaskBin->item(i, j)->setBackground(Qt::white);
         this->ui->tableWidget_SignalMaskBin->item(i, j)->setForeground(Qt::black);
      }
      //this->ui->tableWidget_DataHex->item(i, 0)->setText("");
   }

   for (size_t i = 0; i < this->ui->tableWidget_CanSignalRawData->rowCount(); i++)
   {
      this->ui->tableWidget_CanSignalRawData->item(i, 0)->setText("");
   }
}