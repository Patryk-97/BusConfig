#pragma once

#include "ICanNetwork.h"
#include <QDialog>
#include <qabstractbutton.h>
#include <qtablewidget.h>

namespace Ui
{
   class CanMessageCreator;
}

class CanMessageCreator : public QDialog
{
   Q_OBJECT
public:
   explicit CanMessageCreator(QWidget* parent = nullptr);
   ~CanMessageCreator();

   bool Create(ICanNetwork* canNetwork);
   void Clear(void);

private slots:
   void on_buttonBox_clicked(QAbstractButton* button);

   void on_lineEdit_Name_textChanged(const QString& text);
   void on_lineEdit_Id_textChanged(const QString& text);
   void on_comboBox_IdFormat_currentTextChanged(const QString& text);
   void on_lineEdit_Size_textChanged(const QString& text);
   void on_comboBox_TxMethod_currentTextChanged(const QString& text);
   void on_lineEdit_CycleTime_textChanged(const QString& text);
   void on_comboBox_MainTransmitter_currentTextChanged(const QString& text);
   void on_lineEdit_Comment_textChanged(const QString& text);

private:
   Ui::CanMessageCreator* ui;
   ICanNetwork* canNetwork{ nullptr };
};
