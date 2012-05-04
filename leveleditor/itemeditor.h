#ifndef ITEMEDITOR_H
#define ITEMEDITOR_H

#include "LevelDef.h"
#include "fixtureeditor.h"

#include <QWidget>

namespace Ui {
class ItemEditor;
}

class ItemEditor : public QWidget
{
	Q_OBJECT

protected:
	InventoryItem *m_item;
	
public:
	explicit ItemEditor(QWidget *parent = 0);
	~ItemEditor();

	void setInventoryItem( InventoryItem *item );
	
private:
	Ui::ItemEditor *ui;
};

#endif // ITEMEDITOR_H
