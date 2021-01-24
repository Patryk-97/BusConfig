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

private slots:
   void on_buttonBox_clicked(QAbstractButton* button);

private:
   Ui::CanSignalCreator* ui;
   ICanNetwork* canNetwork { nullptr };
};
