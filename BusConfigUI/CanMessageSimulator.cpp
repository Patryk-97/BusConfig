#include "CanMessageSimulator.h"
#include "ui_CanMessageSimulator.h"
#include "ICanMessage.h"
#include "Conversions.h"
#include "TableWidgetItem.h"
#include "ComboDelegate.h"
#include <QtWidgets/qabstractbutton.h>
#include <bitset>

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

   this->ui->tableWidget_SignalMaskBin->setStyleSheet("QTableWidget::item { padding: 2px 5px; border: 0; }");
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
         this->ui->tableWidget_SignalMaskBin->setItem(i, j, new TableWidgetItem{ tr("%1").arg(0), Qt::AlignCenter });
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
      this->ui->tableWidget_SignalMaskHex->setItem(i, 0, new TableWidgetItem<uint32_t, false> { "0x00", Qt::AlignCenter });
   }

   this->ui->tableWidget_DataHex->setStyleSheet("QTableWidget::item { padding: 5px 10px; border: 0; }");
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
   }

   this->ui->tableWidget_CanMessage->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
   this->ui->tableWidget_CanMessage->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

   this->ui->tableWidget_CanMessage->setStyleSheet("QTableWidget::item { padding: 5px 10px; border: 0; }");
   this->ui->tableWidget_CanMessage->setFocusPolicy(Qt::FocusPolicy::NoFocus);

   for (size_t i {}; const auto& canMessageHeaderLabel : { "Name", "Id" })
   {
      auto item = new TableWidgetItem{ canMessageHeaderLabel, Qt::AlignCenter };
      item->setFont(fontBold);
      this->ui->tableWidget_CanMessage->setItem(i++, 0, item);
   }
   
   this->ui->tableWidget_CanSignal->setStyleSheet("QTableWidget::item { padding: 2px 10px; border: 0; }");
   this->ui->tableWidget_CanSignal->setFocusPolicy(Qt::FocusPolicy::NoFocus);

   this->ui->tableWidget_CanSignal->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
   this->ui->tableWidget_CanSignal->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

   for (size_t i{}; const auto & canSignalHeaderLabel : { "Name", "Start bit", "Length", "Byte order", "Scale", "Offset" })
   {
      auto item = new TableWidgetItem{ canSignalHeaderLabel, Qt::AlignCenter };
      item->setFont(fontBold);
      this->ui->tableWidget_CanSignal->setItem(i++, 0, item);
   }

   this->canDataBytesTableFilled = true;
}

CanMessageSimulator::~CanMessageSimulator()
{
   delete ui;
}

bool CanMessageSimulator::Create(ICanBusConfig* canBusConfig, ICanMessage* canMessage)
{
   if (canBusConfig)
   {
      if (const auto message = canBusConfig->GetMessageByIndex(0); message)
      {
         this->ui->tableWidget_CanMessage->setItem(0, 1, new QTableWidgetItem{ message->GetName() });
         this->ui->tableWidget_CanMessage->setItem(1, 1, new QTableWidgetItem{ toHexQString(message->GetId()) });

         for (size_t i = 0; i < this->ui->tableWidget_CanMessage->rowCount(); i++)
         {
            this->ui->tableWidget_CanMessage->item(i, 1)->setTextAlignment(Qt::AlignCenter);
         }

         if (const auto signal = message->GetSignalByIndex(1); signal)
         {
            this->ui->tableWidget_CanSignal->setItem(0, 1, new QTableWidgetItem{ signal->GetName() });
            this->ui->tableWidget_CanSignal->setItem(1, 1, new QTableWidgetItem{ toQString(signal->GetStartBit()) });
            this->ui->tableWidget_CanSignal->setItem(2, 1, new QTableWidgetItem{ toQString(signal->GetSize()) });
            this->ui->tableWidget_CanSignal->setItem(3, 1, new QTableWidgetItem{ "Little endian" });
            this->ui->tableWidget_CanSignal->setItem(4, 1, new QTableWidgetItem{ toQString(signal->GetFactor()) });
            this->ui->tableWidget_CanSignal->setItem(5, 1, new QTableWidgetItem{ toQString(signal->GetOffset()) });

            for (size_t i = 0; i < signal->GetSize(); i++)
            {
               auto item = this->ui->tableWidget_SignalMaskBin->item((i + signal->GetStartBit()) / 8, 7 - (signal->GetStartBit() + i) % 8);
               item->setText("1");
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

               std::bitset<8> rawBinaryNumber { this->ui->tableWidget_CanDataBytes->item(i, 1)->text().toUtf8().constData() };
               rawBinaryNumber &= binaryNumber;
               this->ui->tableWidget_DataHex->item(i, 0)->setText(toHexQString(rawBinaryNumber.to_ulong()));
            }
         }

         for (size_t i = 0; i < this->ui->tableWidget_CanSignal->rowCount(); i++)
         {
            this->ui->tableWidget_CanSignal->item(i, 1)->setTextAlignment(Qt::AlignCenter);
         }
      }
   }

   return true;
}

void CanMessageSimulator::on_tableWidget_CanDataBytes_itemChanged(QTableWidgetItem* item)
{
   if (this->canDataBytesTableFilled)
   {
      this->canDataBytesTableFilled = false;
      const int row = item->row();
      const int column = item->column();

      if (column == 0)
      {
         const uint8_t value = item->text().toUInt(nullptr, 16);
         std::bitset<8> binaryNumber{ value };
         this->ui->tableWidget_CanDataBytes->item(row, 1)->setText(binaryNumber.to_string().c_str());

         for (size_t i = 0; i < 8; i++)
         {
            this->ui->tableWidget_CanDataBytes->item(row, i + 2)->setText(std::to_string(binaryNumber[7 - i]).c_str());
         }
      }
      else if (column == 1)
      {
         std::bitset<8> binaryNumber{ item->text().toUtf8().constData() };
         const uint8_t value = static_cast<uint8_t>(binaryNumber.to_ulong());
         this->ui->tableWidget_CanDataBytes->item(row, 0)->setText(toHexQString(value));

         for (size_t i = 0; i < 8; i++)
         {
            this->ui->tableWidget_CanDataBytes->item(row, i + 2)->setText(std::to_string(binaryNumber[7 - i]).c_str());
         }
      }
      else
      {
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
      }
      this->canDataBytesTableFilled = true;
   }
}