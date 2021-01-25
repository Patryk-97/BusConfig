#pragma once

#include "ICanNetwork.h"
#include <QDialog>
#include <qabstractbutton.h>

namespace Ui
{
   class CanNodeCreator;
}

class CanNodeCreator : public QDialog
{
   Q_OBJECT
public:
   explicit CanNodeCreator(QWidget* parent = nullptr);
   ~CanNodeCreator();

   bool Create(ICanNetwork* canNetwork);
   void Clear(void);

private slots:
   void on_buttonBox_clicked(QAbstractButton* button);

   void on_lineEdit_Name_textChanged(const QString& text);
   void on_lineEdit_Address_textChanged(const QString& text);
   void on_lineEdit_Comment_textChanged(const QString& text);

private:
   Ui::CanNodeCreator* ui;
   ICanNetwork* canNetwork{ nullptr };
};
