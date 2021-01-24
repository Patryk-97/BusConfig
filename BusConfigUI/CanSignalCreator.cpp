#include "CanSignalCreator.h"
#include "ui_CanSignalCreator.h"
#include "ICanSignalBuilder.h"
#include "CanSignalManager.h"
#include "Conversions.h"
#include "TableWidgetItem.h"
#include "ComboDelegate.h"
#include <qmessagebox.h>
#include <qpushbutton.h>

CanSignalCreator::CanSignalCreator(QWidget* parent) :
   QDialog(parent),
   ui(new Ui::CanSignalCreator)
{
   ui->setupUi(this);
   Qt::WindowFlags flags{ Qt::Window | Qt::WindowCloseButtonHint };
   this->setWindowFlags(flags);
   this->ui->tableWidget_ValueTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

   this->ui->tableWidget_ValueTable->setStyleSheet("QTableWidget::item { padding: 5 10px; border: 0; }");
   this->ui->tableWidget_ValueTable->horizontalHeader()->setStyleSheet(
      "QHeaderView::section { padding: 5 10px; border: 0; }");
   this->ui->tableWidget_ValueTable->setFocusPolicy(Qt::FocusPolicy::NoFocus);

   this->ui->tableWidget_Receivers->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

   this->ui->tableWidget_Receivers->setStyleSheet("QTableWidget::item { padding: 5 10px; border: 0; }");
   this->ui->tableWidget_Receivers->horizontalHeader()->setStyleSheet(
      "QHeaderView::section { padding: 5 10px; border: 0; }");
   this->ui->tableWidget_Receivers->setFocusPolicy(Qt::FocusPolicy::NoFocus);
}

CanSignalCreator::~CanSignalCreator()
{
   delete ui;
}

bool CanSignalCreator::Create(ICanNetwork* canNetwork)
{
   // locals
   bool rV{ false };

   if (canNetwork)
   {
      this->Clear();
      this->canNetwork = canNetwork;
      for (size_t i = 0; i < canNetwork->GetMessagesCount(); i++)
      {
         if (const auto canMessage = canNetwork->GetMessageByIndex(i); canMessage)
         {
            this->ui->comboBox_Message->addItem(canMessage->GetName());
         }
      }
   }

   return rV;
}

void CanSignalCreator::Clear(void)
{
   this->ui->lineEdit_Name->setText("");
   this->ui->comboBox_Message->setCurrentIndex(0);
   this->ui->lineEdit_StartBit->setText("0");
   this->ui->lineEdit_Size->setText("1");
   this->ui->comboBox_ByteOrder->setCurrentIndex(0);
   this->ui->comboBox_ValueType->setCurrentIndex(0);
   this->ui->lineEdit_Factor->setText("1");
   this->ui->lineEdit_Offset->setText("0");
   this->ui->lineEdit_Minimum->setText("0");
   this->ui->lineEdit_Maximum->setText("0");
   this->ui->lineEdit_Unit->setText("");
   this->ui->lineEdit_Comment->setText("");
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);

   this->ClearTableWidget();
}

void CanSignalCreator::ClearTableWidget(void)
{
   this->ui->tableWidget_ValueTable->setRowCount(0);

   for (size_t i = 0; i < this->ui->tableWidget_Receivers->rowCount(); i++)
   {
      this->ui->tableWidget_Receivers->setItemDelegateForRow(i, nullptr);
   }
   this->ui->tableWidget_Receivers->setRowCount(0);
}

void CanSignalCreator::on_buttonBox_clicked(QAbstractButton* button)
{
   // locals
   bool ok;

   if (qobject_cast<QPushButton*>(button) == this->ui->buttonBox->button(QDialogButtonBox::Apply))
   {
      if (this->canNetwork)
      {
         QString canMessageName = this->ui->comboBox_Message->currentText();
         const auto canMessage = this->canNetwork->GetMessageByName(canMessageName.toUtf8());

         QString name = this->ui->lineEdit_Name->text();

         if (name.isEmpty())
         {
            QMessageBox::warning(this, "CanSignalCreator", "Signal name can not be empty");
            return;
         }

         if (this->canNetwork->SignalExists(name.toUtf8()))
         {
            QMessageBox::warning(this, "CanSignalCreator", "Signal already exists");
            return;
         }

         uint32_t startBit = this->ui->lineEdit_StartBit->text().toUInt(&ok);
         if (canMessage->IsCanStandard())
         {
            if (!ok || startBit > 63)
            {
               QMessageBox::warning(this, "CanSignalCreator", "Start bit should be in range [0, 63]");
               return;
            }
         }
         else if (canMessage->IsCanFdStandard())
         {
            if (!ok || startBit > 255)
            {
               QMessageBox::warning(this, "CanSignalCreator", "Start bit should be in range [0, 255]");
               return;
            }
         }

         uint32_t size = this->ui->lineEdit_Size->text().toUInt(&ok);
         if (canMessage->IsCanStandard())
         {
            if (!ok || size > 63)
            {
               QMessageBox::warning(this, "CanSignalCreator", "Size should be in range [0, 63]");
               return;
            }
         }
         else if (canMessage->IsCanFdStandard())
         {
            if (!ok || size > 255)
            {
               QMessageBox::warning(this, "CanSignalCreator", "Size should be in range [0, 255]");
               return;
            }
         }

         auto byteOrder = CanSignalManager::ByteOrder::MAP[this->ui->comboBox_ByteOrder->currentText()];
         auto valueType = CanSignalManager::ValueType::MAP[this->ui->comboBox_ValueType->currentText()];

         double factor = this->ui->lineEdit_Factor->text().toDouble(&ok);
         if (!ok)
         {
            QMessageBox::warning(this, "CanSignalCreator", "Factor must be floating point number");
            return;
         }

         double offset = this->ui->lineEdit_Offset->text().toDouble(&ok);
         if (!ok)
         {
            QMessageBox::warning(this, "CanSignalCreator", "Offset must be floating point number");
            return;
         }

         double minimum = this->ui->lineEdit_Minimum->text().toDouble(&ok);
         if (!ok)
         {
            QMessageBox::warning(this, "CanSignalCreator", "Minimum must be floating point number");
            return;
         }

         double maximum = this->ui->lineEdit_Maximum->text().toDouble(&ok);
         if (!ok)
         {
            QMessageBox::warning(this, "CanSignalCreator", "Maximum must be floating point number");
            return;
         }

         QString unit = this->ui->lineEdit_Unit->text();
         QString comment = this->ui->lineEdit_Comment->text();

         this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);

         auto canValueTableBuilder = this->canNetwork->ValueTableBuilder();
         for (size_t i = 0; i < this->ui->tableWidget_ValueTable->rowCount(); i++)
         {
            auto value = this->ui->tableWidget_ValueTable->item(i, 0)->text().toUInt(nullptr, 16);
            auto description = this->ui->tableWidget_ValueTable->item(i, 1)->text();
            canValueTableBuilder->WithValue(value, description.toUtf8());
         }
         auto canValueTable = canValueTableBuilder->Build();

         auto canSignalBuilder = this->canNetwork->SignalBuilder();

         for (size_t i = 0; i < this->ui->tableWidget_Receivers->rowCount(); i++)
         {
            auto receiver = this->ui->tableWidget_Receivers->item(i, 0)->text();
            canSignalBuilder->WithReceiver(this->canNetwork->GetNodeByName(receiver.toUtf8()));
         }

         auto canSignal = canSignalBuilder->WithName(name.toUtf8())->AddToMessage(canMessage)->WithStartBit(startBit)
            ->WithSize(size)->WithByteOrder(byteOrder)->WithValueType(valueType)->WithFactor(factor)->WithOffset(offset)
            ->WithMinimum(minimum)->WithMaximum(maximum)->WithValueTable(canValueTable)->WithUnit(unit.toUtf8())
            ->WithComment(comment.toUtf8())->Build();
         if (canSignal)
         {
            QMessageBox::information(this, "CanSignalCreator", "Successfully created signal");
            /* todo update tree and table */
         }
      }
   }
}

void CanSignalCreator::on_lineEdit_Name_textChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanSignalCreator::on_comboBox_Message_currentTextChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanSignalCreator::on_lineEdit_StartBit_textChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanSignalCreator::on_lineEdit_Size_textChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanSignalCreator::on_comboBox_ByteOrder_textChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanSignalCreator::on_comboBox_ValueType_textChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanSignalCreator::on_lineEdit_Factor_textChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanSignalCreator::on_lineEdit_Offset_textChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanSignalCreator::on_lineEdit_Minimum_textChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanSignalCreator::on_lineEdit_Maximum_textChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanSignalCreator::on_lineEdit_Unit_textChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanSignalCreator::on_lineEdit_Comment_textChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanSignalCreator::on_tableWidget_Receivers_itemChanged(QTableWidgetItem* item)
{
   auto row = item->row();
   auto receiver = item->text();
   if (!receiver.isEmpty())
   {
      if (const auto canNode = this->canNetwork->GetNodeByName(receiver.toUtf8()); canNode)
      {
         this->ui->tableWidget_Receivers->item(row, 1)->setText(toHexQString(canNode->GetAddress()));
      }
   }
}

void CanSignalCreator::on_pushButton_ValueTable_Add_clicked()
{
   auto rowCount = this->ui->tableWidget_ValueTable->rowCount();
   this->ui->tableWidget_ValueTable->insertRow(rowCount);
   auto newValue = std::invoke([this, &rowCount]
   {
      if (rowCount > 0)
      {
         return this->ui->tableWidget_ValueTable->item(rowCount - 1, 0)->text().toUInt(nullptr, 16) + 1;
      }
      else
      {
         return static_cast<uint32_t>(0);
      }
   });
   this->ui->tableWidget_ValueTable->setItem(rowCount, 0, new TableWidgetItem<uint32_t> { toHexQString(newValue) });
   this->ui->tableWidget_ValueTable->setItem(rowCount, 1, new TableWidgetItem<QString> { "Description for value: " + toHexQString(newValue) });

}

void CanSignalCreator::on_pushButton_ValueTable_Remove_clicked()
{
   this->ui->tableWidget_ValueTable->removeRow(this->ui->tableWidget_ValueTable->currentRow());
}

void CanSignalCreator::on_pushButton_Receivers_Add_clicked()
{
   QStringList receivers { "" };
   for (size_t i = 0; i < this->canNetwork->GetNodesCount(); i++)
   {
      if (const auto canNode = this->canNetwork->GetNodeByIndex(i); canNode)
      {
         receivers.push_back(canNode->GetName());
      }
   }
   auto rowCount = this->ui->tableWidget_Receivers->rowCount();
   for (size_t i = 0; i < rowCount; i++)
   {
      receivers.removeAll(this->ui->tableWidget_Receivers->item(i, 0)->text());
   }
   this->ui->tableWidget_Receivers->insertRow(rowCount);

   this->ui->tableWidget_Receivers->setItem(rowCount, 0, new TableWidgetItem<QString>{ "" });
   this->ui->tableWidget_Receivers->setItem(rowCount, 1, new TableWidgetItem<QString, false>{ "" });

   ComboDelegate* receiversDelegate = new ComboDelegate{ receivers };
   this->ui->tableWidget_Receivers->setItemDelegateForRow(rowCount, receiversDelegate);
}

void CanSignalCreator::on_pushButton_Receivers_Remove_clicked()
{
   this->ui->tableWidget_Receivers->removeRow(this->ui->tableWidget_Receivers->currentRow());
}