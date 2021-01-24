#pragma once

#include "ICanNetwork.h"
#include <QDialog>
#include <qabstractbutton.h>

namespace Ui
{
   class CanSignalCreator;
}

class CanSignalCreator : public QDialog
{
   Q_OBJECT
public:
   explicit CanSignalCreator(QWidget* parent = nullptr);
   ~CanSignalCreator();

   bool Create(ICanNetwork* canNetwork);
   void Reset(void);

private slots:
   void on_buttonBox_clicked(QAbstractButton* button);
   void on_lineEdit_Name_textChanged(const QString& text);
   void on_comboBox_Message_currentTextChanged(const QString& text);
   void on_lineEdit_StartBit_textChanged(const QString& text);
   void on_lineEdit_Size_textChanged(const QString& text);
   void on_comboBox_ByteOrder_textChanged(const QString& text);
   void on_comboBox_ValueType_textChanged(const QString& text);
   void on_lineEdit_Factor_textChanged(const QString& text);
   void on_lineEdit_Offset_textChanged(const QString& text);
   void on_lineEdit_Minimum_textChanged(const QString& text);
   void on_lineEdit_Maximum_textChanged(const QString& text);
   void on_lineEdit_Unit_textChanged(const QString& text);
   void on_lineEdit_Comment_textChanged(const QString& text);

private:
   Ui::CanSignalCreator* ui;
   ICanNetwork* canNetwork { nullptr };
};
