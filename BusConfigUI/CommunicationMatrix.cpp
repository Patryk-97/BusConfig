#include "CommunicationMatrix.h"
#include "ui_CommunicationMatrix.h"

CommunicationMatrix::CommunicationMatrix(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::CommunicationMatrix)
{
   ui->setupUi(this);
}

CommunicationMatrix::~CommunicationMatrix()
{
   delete ui;
}
