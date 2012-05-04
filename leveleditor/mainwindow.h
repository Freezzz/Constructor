#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

class LevelDef;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

protected:
	LevelDef *ld;
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void open( );
	void itemSelected( QListWidgetItem *item );
	
private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
