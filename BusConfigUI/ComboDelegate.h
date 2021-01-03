#pragma once

#include <qstyleditemdelegate.h>
#include <qstringlist.h>
#include <qlistwidget.h>

class ComboDelegate : public QStyledItemDelegate
{
public:
   ComboDelegate(const QStringList& items, int minimumHeight = 25, int minimumWidth = 100) : items { items },
      minimumHeight { minimumHeight }, minimumWidth { minimumWidth } {}

   void SetItems(const QStringList& items);
   QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
   void setEditorData(QWidget* editor, const QModelIndex& index) const override;
   void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
   void currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);

private:
   QStringList items;
   mutable QListWidget* editor { nullptr };
   int minimumHeight {};
   int minimumWidth {};
};