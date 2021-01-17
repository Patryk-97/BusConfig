#pragma once

#include <qstyleditemdelegate.h>
#include <qstringlist.h>

class CellComboDelegate : public QStyledItemDelegate
{
public:
   CellComboDelegate(int minimumHeight = 25, int minimumWidth = 100) :
      minimumHeight{ minimumHeight }, minimumWidth{ minimumWidth } {}

   void addItems(const QStringList& items);
   QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
   void setEditorData(QWidget* editor, const QModelIndex& index) const override;
   void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;

private:
   QList<QStringList> items;
   int minimumHeight{};
   int minimumWidth{};
};