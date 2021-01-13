#include "ComboDelegate.h"
#include <qcombobox.h>
#include <qdebug.h>
#include <qscrollbar.h>
#include <qlistwidget.h>

void ComboDelegate::SetItems(const QStringList& items)
{
   this->items = items;
}

QWidget* ComboDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   editor = new QComboBox { parent };
   editor->setMinimumHeight(minimumHeight);
   editor->setMinimumWidth(minimumWidth);

   QObject::connect(editor, &QComboBox::currentTextChanged, this, &ComboDelegate::currentTextChanged);
   
   return editor;
}

void ComboDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
   for (size_t i {}; const auto& item : items)
   {
      //auto listItem = new QListWidgetItem { item };
      qobject_cast<QComboBox*>(editor)->addItem(item);
      if (item == index.data())
      {
         qobject_cast<QComboBox*>(editor)->setCurrentText(qobject_cast<QComboBox*>(editor)->itemText(i));
      }
      ++i;
   }
}

void ComboDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
   auto editorIndex = qobject_cast<QComboBox*>(editor)->currentIndex();
   auto text = qobject_cast<QComboBox*>(editor)->currentText();
   model->setData(index, text);
}

void ComboDelegate::currentTextChanged(const QString& text)
{
   this->commitData(this->editor);
   this->closeEditor(this->editor);
}