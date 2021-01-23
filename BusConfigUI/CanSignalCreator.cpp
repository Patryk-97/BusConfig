#include "CanSignalCreator.h"
#include "ui_CanSignalCreator.h"
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
   
}