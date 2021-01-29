#include "ComboDelegate.h"
#include <qcombobox.h>

void ComboDelegate::SetItems(const QStringList& items)
{
   this->items = items;
}

QWidget* ComboDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   auto editor = new QComboBox { parent };
   editor->setMinimumHeight(minimumHeight);
   editor->setMinimumWidth(minimumWidth);

   QObject::connect(editor, &QComboBox::currentTextChanged, this, [this, editor] (const QString& text) mutable
   {
      const_cast<ComboDelegate*>(this)->commitData(editor);
      const_cast<ComboDelegate*>(this)->closeEditor(editor);
   });
   
   return editor;
}

void ComboDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
   for (size_t i {}; const auto& item : items)
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

void ComboDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
   auto editorIndex = qobject_cast<QComboBox*>(editor)->currentIndex();
   auto text = qobject_cast<QComboBox*>(editor)->currentText();
   model->setData(index, text);
}