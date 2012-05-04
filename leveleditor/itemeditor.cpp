#include "itemeditor.h"
#include "ui_itemeditor.h"

ItemEditor::ItemEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemEditor)
{
	ui->setupUi(this);
}

ItemEditor::~ItemEditor()
{
	delete ui;
}

void ItemEditor::setInventoryItem( InventoryItem *item )
{
	m_item = item;

	ui->itemIcon->setPixmap( QPixmap( QString( item->itemSpritePath.c_str() ) ) );
	ui->name->setText( item->name.c_str() );
	// ui->typeBox // TODO
	ui->availableBox->setChecked( item->available );
	ui->quantityBox->setValue( item->maxQuantity );
	ui->staticBox->setChecked( item->isStatic );
	ui->staticBox->setChecked( item->isRotatable );

	// TODO: set objectSpritePath and hte shape defined in fixtureDef
	//       to the central widget: it should be a custom widget...
}
