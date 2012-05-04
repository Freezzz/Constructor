#include "itemeditor.h"
#include "fixtureeditor.h"
#include "ui_itemeditor.h"

#include <QMessageBox>

ItemEditor::ItemEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemEditor)
{
	ui->setupUi(this);

	ui->typeBox->addItem( "?", QVariant::fromValue<int>(Undefined) );
	ui->typeBox->addItem( "Normal object", QVariant::fromValue<int>(SimpleBox) );
	ui->typeBox->addItem( "Area", QVariant::fromValue<int>(Area) );
	ui->typeBox->addItem( "Spring", QVariant::fromValue<int>(Spring) );
	ui->typeBox->addItem( "Pin", QVariant::fromValue<int>(Pin) );
	ui->typeBox->addItem( "Glue", QVariant::fromValue<int>(Glue) );
}

ItemEditor::~ItemEditor()
{
	delete ui;
}

void ItemEditor::setInventoryItem( InventoryItem *item )
{
	m_item = item;

	ui->itemIcon->setPixmap( QPixmap( QString( item->itemSpritePath.c_str() ) ) );
	ui->fixtureEditor->setSprite( item->objectSpritePath.c_str() );
	ui->fixtureEditor->setFixtureDef( item->fixtureDef );

	ui->nameEdit->setText( item->name.c_str() );
	ui->availableBox->setChecked( item->available );
	ui->quantityBox->setValue( item->maxQuantity );
	ui->staticBox->setChecked( item->isStatic );
	ui->rotableBox->setChecked( item->isRotatable );

	int typeIndex = ui->typeBox->findData( QVariant::fromValue<int>(item->type) );
	if( typeIndex != -1 ) {
		ui->typeBox->setCurrentIndex( typeIndex );
	}
	else {
		QMessageBox::warning( this, "Invalid type", QString("Item %1 has an invalid type (%2)").arg(item->name.c_str()).arg(item->type) );
		ui->typeBox->setCurrentIndex( 0 );
	}
}
