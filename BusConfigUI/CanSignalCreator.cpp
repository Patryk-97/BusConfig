#include "CanSignalCreator.h"
#include "ui_CanSignalCreator.h"
#include "ICanSignalBuilder.h"
#include "CanSignalManager.h"
#include <qmessagebox.h>
#include <qpushbutton.h>

CanSignalCreator::CanSignalCreator(QWidget* parent) :
   QDialog(parent),
   ui(new Ui::CanSignalCreator)
{
   ui->setupUi(this);
   Qt::WindowFlags flags{ Qt::Window | Qt::WindowCloseButtonHint };
   this->setWindowFlags(flags);
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

         this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);

         auto canSignalBuilder = this->canNetwork->SignalBuilder();
         auto canSignal = canSignalBuilder->WithName(name.toUtf8())->AddToMessage(canMessage)->WithStartBit(startBit)
            ->WithSize(size)->WithByteOrder(byteOrder)->WithValueType(valueType)->WithFactor(factor)->WithOffset(offset)
            ->WithMinimum(minimum)->WithMaximum(maximum)->WithUnit(unit.toUtf8())->Build();
      }
   }
}