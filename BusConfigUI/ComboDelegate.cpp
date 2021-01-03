#include "ComboDelegate.h"
#include <qlistwidget.h>
#include <qdebug.h>

void ComboDelegate::SetItems(const QStringList& items)
{
   this->items = items;
}

QWidget* ComboDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   editor = new QListWidget { parent };
   editor->setMinimumHeight(minimumHeight * this->items.size());
   editor->setMinimumWidth(minimumWidth);

   QObject::connect(editor, &QListWidget::currentItemChanged, this, &ComboDelegate::currentItemChanged);
   
   return editor;
}

void ComboDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
   for (size_t i {}; const auto& item : items)
   {
      auto listItem = new QListWidgetItem { item };
      qobject_cast<QListWidget*>(editor)->addItem(listItem);
      if (item == index.data())
      {
         qobject_cast<QListWidget*>(editor)->setCurrentItem(qobject_cast<QListWidget*>(editor)->item(i));
      }
      ++i;
   }
}

void ComboDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
   auto editorIndex = qobject_cast<QListWidget*>(editor)->currentIndex();
   auto text = qobject_cast<QListWidget*>(editor)->currentItem()->text();
   model->setData(index, text);
}

void ComboDelegate::currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
   this->commitData(this->editor);
   this->closeEditor(this->editor);
}