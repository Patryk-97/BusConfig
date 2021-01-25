#include "CanMessageCreator.h"
#include "ui_CanMessageCreator.h"
#include "ICanMessageBuilder.h"
#include "CanMessageManager.h"
#include "Conversions.h"
#include "TableWidgetItem.h"
#include "ComboDelegate.h"
#include <qmessagebox.h>
#include <qpushbutton.h>

CanMessageCreator::CanMessageCreator(QWidget* parent) :
   QDialog(parent),
   ui(new Ui::CanMessageCreator)
{
   ui->setupUi(this);
   Qt::WindowFlags flags{ Qt::Window | Qt::WindowCloseButtonHint };
   this->setWindowFlags(flags);
}

CanMessageCreator::~CanMessageCreator()
{
   delete ui;
}

bool CanMessageCreator::Create(ICanNetwork* canNetwork)
{
   // locals
   bool rV { false };

   if (canNetwork)
   {
      this->Clear();
      this->canNetwork = canNetwork;
      for (size_t i = 0; i < canNetwork->GetMessagesCount(); i++)
      {
         if (const auto canNode = canNetwork->GetNodeByIndex(i); canNode)
         {
            this->ui->comboBox_MainTransmitter->addItem(canNode->GetName());
         }
      }
   }

   return rV;
}

void CanMessageCreator::Clear(void)
{
   this->ui->lineEdit_Name->setText("");
   this->ui->lineEdit_Id->setText(toHexQString(0x00));
   this->ui->comboBox_IdFormat->setCurrentIndex(0);
   this->ui->lineEdit_Size->setText("1");
   this->ui->comboBox_TxMethod->setCurrentIndex(0);
   this->ui->lineEdit_CycleTime->setText("0");
   this->ui->comboBox_MainTransmitter->setCurrentIndex(0);
   this->ui->lineEdit_Comment->setText("");
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanMessageCreator::on_buttonBox_clicked(QAbstractButton* button)
{
   // locals
   bool ok;

   if (qobject_cast<QPushButton*>(button) == this->ui->buttonBox->button(QDialogButtonBox::Apply))
   {
      if (this->canNetwork)
      {
         QString mainTransmitterName = this->ui->comboBox_MainTransmitter->currentText();
         const auto mainTransmitter = this->canNetwork->GetNodeByName(mainTransmitterName.toUtf8());

         QString name = this->ui->lineEdit_Name->text();

         if (name.isEmpty())
         {
            QMessageBox::warning(this, "CanMessageCreator", "Message name can not be empty");
            return;
         }

         if (this->canNetwork->MessageExists(name.toUtf8()))
         {
            QMessageBox::warning(this, "CanMessageCreator", "Message already exists");
            return;
         }

         uint32_t id = this->ui->lineEdit_Id->text().toUInt(&ok);
         if (!ok)
         {
            QMessageBox::warning(this, "CanMessageCreator", "Id should be unsigned integer");
            return;
         }

         if (this->canNetwork->MessageExists(id))
         {
            QMessageBox::warning(this, "CanMessageCreator", "Message already exists");
            return;
         }

         auto idFormat = CanMessageManager::IdFormat::MAP[this->ui->comboBox_IdFormat->currentText()];

         uint32_t size = this->ui->lineEdit_Size->text().toUInt(&ok);
         if (!ok || size > 63)
         {
            QMessageBox::warning(this, "CanMessageCreator", "Size should be in range [0, 63]");
            return;
         }

         auto txMethod = CanMessageManager::TxMethod::MAP[this->ui->comboBox_TxMethod->currentText()];

         ICanMessage::cycle_time_t cycleTime = this->ui->lineEdit_CycleTime->text().toUInt(&ok);
         if (!ok)
         {
            QMessageBox::warning(this, "CanMessageCreator", "Cycle time must be unsigned integer");
            return;
         }

         QString comment = this->ui->lineEdit_Comment->text();

         this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);

         auto canMessageBuilder = this->canNetwork->MessageBuilder();

         auto canMessage = canMessageBuilder->WithName(name.toUtf8())->WithId(id)->WithIdFormat(idFormat)->WithSize(size)
            ->WithTxMethod(txMethod)->WithCycleTime(cycleTime)->WithMainTransmitter(mainTransmitter)
            ->WithComment(comment.toUtf8())->Build();
         if (canMessage)
         {
            QMessageBox::information(this, "CanMessageCreator", "Successfully created message");
            /* todo update tree and table */
         }
      }
   }
}

void CanMessageCreator::on_lineEdit_Name_textChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanMessageCreator::on_lineEdit_Id_textChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanMessageCreator::on_comboBox_IdFormat_currentTextChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanMessageCreator::on_lineEdit_Size_textChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanMessageCreator::on_comboBox_TxMethod_currentTextChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanMessageCreator::on_lineEdit_CycleTime_textChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanMessageCreator::on_comboBox_MainTransmitter_currentTextChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanMessageCreator::on_lineEdit_Comment_textChanged(const QString& text)
{

}