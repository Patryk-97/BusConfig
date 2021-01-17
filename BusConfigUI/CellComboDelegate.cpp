#include "CellComboDelegate.h"
#include <qcombobox.h>

void CellComboDelegate::addItems(const QStringList& items)
{
   this->items.push_back(items);
}

QWidget* CellComboDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   auto editor = new QComboBox { parent };
   editor->setMinimumHeight(minimumHeight);
   editor->setMinimumWidth(minimumWidth);

   QObject::connect(editor, &QComboBox::currentTextChanged, this, [this, editor] (const QString& text) mutable
   {
      const_cast<CellComboDelegate*>(this)->commitData(editor);
      const_cast<CellComboDelegate*>(this)->closeEditor(editor);
   });

   return editor;
}

void CellComboDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
   for (size_t i{}; const auto& item : items[index.row()])
   {
      qobject_cast<QComboBox*>(editor)->addItem(item);
      if (item == index.data())
      {
         qobject_cast<QComboBox*>(editor)->setCurrentText(qobject_cast<QComboBox*>(editor)->itemText(i));
      }
      ++i;
   }
   //qobject_cast<QComboBox*>(editor)->setSizeAdjustPolicy(QComboBox::AdjustToContents);
}

void CellComboDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
   auto editorIndex = qobject_cast<QComboBox*>(editor)->currentIndex();
   auto text = qobject_cast<QComboBox*>(editor)->currentText();
   model->setData(index, text);
}