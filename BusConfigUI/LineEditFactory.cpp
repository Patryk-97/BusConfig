#include "LineEditFactory.h"

QLineEdit* LineEditFactory::CreateDoubleLineEdit(const QString& text, QTableWidget* tableWidget,
   int row, int column)
{
   QLineEdit* lineEdit = new QLineEdit { tableWidget };
   lineEdit->setFrame(false);
   lineEdit->setText(text);
   //auto doubleValidator = new QDoubleValidator { lineEdit };
   //doubleValidator->setNotation(QDoubleValidator::StandardNotation);
   //lineEdit->setValidator(doubleValidator);
   //QObject::connect(lineEdit, &QLineEdit::returnPressed, tableWidget,
   //   [&tableWidget, &row, &column] () { tableWidget->cellChanged(row, column); });
   return lineEdit;
}

QLineEdit* LineEditFactory::CreateIntLineEdit(const QString& text, QTableWidget* tableWidget,
   int row, int column)
{
   QLineEdit* lineEdit = new QLineEdit { tableWidget };
   lineEdit->setFrame(false);
   lineEdit->setText(text);
   //auto intValidator = new QIntValidator { lineEdit };
   //lineEdit->setValidator(intValidator);
   //QObject::connect(lineEdit, &QLineEdit::returnPressed, tableWidget,
   //   [&tableWidget, &row, &column]() { tableWidget->cellChanged(row, column); });
   return lineEdit;
}