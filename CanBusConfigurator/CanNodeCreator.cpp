#include "CanNodeCreator.h"
#include "ui_CanNodeCreator.h"
#include "ICanNodeBuilder.h"
#include "Conversions.h"
#include <qmessagebox.h>
#include <qpushbutton.h>

CanNodeCreator::CanNodeCreator(QWidget* parent, std::function<void()> OnUpdate) :
   QDialog(parent), OnUpdate { std::move(OnUpdate) },
   ui(new Ui::CanNodeCreator)
{
   ui->setupUi(this);
   Qt::WindowFlags flags{ Qt::Window | Qt::WindowCloseButtonHint };
   this->setWindowFlags(flags);
}

CanNodeCreator::~CanNodeCreator()
{
   delete ui;
}

bool CanNodeCreator::Create(ICanNetwork* canNetwork)
{
   // locals
   bool rV { false };

   if (canNetwork)
   {
      this->Clear();
      this->canNetwork = canNetwork;
   }

   return rV;
}

void CanNodeCreator::Clear(void)
{
   this->ui->lineEdit_Name->setText("");
   this->ui->lineEdit_Address->setText(toIntQString(0x00, true));
   this->ui->lineEdit_Comment->setText("");
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanNodeCreator::on_buttonBox_clicked(QAbstractButton* button)
{
   // locals
   bool ok;

   if (qobject_cast<QPushButton*>(button) == this->ui->buttonBox->button(QDialogButtonBox::Apply))
   {
      if (this->canNetwork)
      {
         QString name = this->ui->lineEdit_Name->text();

         if (name.isEmpty())
         {
            QMessageBox::warning(this, "CanNodeCreator", "Node name can not be empty");
            return;
         }

         if (this->canNetwork->NodeExists(name.toUtf8()))
         {
            QMessageBox::warning(this, "CanNodeCreator", "Node already exists");
            return;
         }

         int32_t address = this->ui->lineEdit_Address->text().toInt(&ok);
         if (!ok)
         {
            QMessageBox::warning(this, "CanNodeCreator", "Address must be signed integer");
            return;
         }

         QString comment = this->ui->lineEdit_Comment->text();

         this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);

         auto canNodeBuilder = this->canNetwork->NodeBuilder();

         auto canMessage = canNodeBuilder->WithName(name.toUtf8())->WithAddress(address)->WithComment(comment.toUtf8())->Build();
         if (canMessage)
         {
            QMessageBox::information(this, "CanNodeCreator", "Successfully created node");
            if (this->OnUpdate)
            {
               this->OnUpdate();
            }
         }
      }
   }
}

void CanNodeCreator::on_lineEdit_Name_textChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanNodeCreator::on_lineEdit_Address_textChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void CanNodeCreator::on_lineEdit_Comment_textChanged(const QString& text)
{
   this->ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}