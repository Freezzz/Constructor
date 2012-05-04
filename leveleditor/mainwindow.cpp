#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <LevelDef.h>

#include <QFileDialog>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect( ui->openAction, SIGNAL(triggered()), this, SLOT(open()) );
	connect( ui->quitAction, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()) );

	connect( ui->itemList, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(itemSelected(QListWidgetItem*)) );
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::open()
{
	QString file = QFileDialog::getOpenFileName( this, "Select file", "", "Constructor levels (*.clz)" );
	if( file.isNull() ) {
		return ;
	}

	ld = LevelDef::loadFromFile( file.toStdString() );
	for( unsigned int i = 0; i < ld->inventoryItems.size(); ++i ) {
		InventoryItem *invItem = ld->inventoryItems.at( i );
		std::cout << "Adding " << invItem->name << std::endl;

		QListWidgetItem *item = new QListWidgetItem();
		item->setText( invItem->name.c_str() );
		item->setData( Qt::UserRole, QVariant::fromValue<void*>(invItem) );
		ui->itemList->addItem( item );
	}
}
void MainWindow::itemSelected( QListWidgetItem *item )
{
	InventoryItem *invItem = static_cast<InventoryItem*>( item->data( Qt::UserRole ).value<void*>() );

	ui->itemEditor->setInventoryItem( invItem );
}
